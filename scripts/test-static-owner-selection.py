#!/usr/bin/env python3
"""Target-independent, adversarial tests of explicit linked static ownership.

Builds tiny synthetic PE32 and MSF7/PDB evidence in a temporary directory.
No native compiler, target binary, source edits, or ledger writes are used.
"""
from __future__ import annotations

import hashlib
import importlib.util
import io
import json
from pathlib import Path
import struct
import tempfile
import unittest
from types import SimpleNamespace
from contextlib import ExitStack
from unittest.mock import patch

import linked_image as linked

CODE = 0x60000020
SYMBOL = '?LocalOwner@@YAXXZ'
GUID = bytes(range(16))
STAMP = 0x12345678


def row(symbol=SYMBOL, offset=0x20, obj='source.ltcg.obj', flags='f', va=None):
    if va is None:
        va = 0x401000 + offset
    return f' 0001:{offset:08x} {symbol} {va:08x} {flags} {obj}'


def write_msf(path, streams):
    """Serialize a genuine one-directory-block MSF7 container."""
    blocks = [bytearray(512) for _ in range(3)]
    directory = bytearray(struct.pack('<I', len(streams)))
    directory.extend(struct.pack('<' + 'I' * len(streams), *(len(s) for s in streams)))
    for stream in streams:
        numbers = []
        for start in range(0, len(stream), 512):
            numbers.append(len(blocks))
            blocks.append(bytearray(stream[start:start + 512].ljust(512, b'\0')))
        if numbers:
            directory.extend(struct.pack('<' + 'I' * len(numbers), *numbers))
    assert len(directory) <= 512
    blocks[0][:32] = linked.MSF7_SIGNATURE
    struct.pack_into('<6I', blocks[0], 32, 512, 0, len(blocks), len(directory), 0, 1)
    struct.pack_into('<I', blocks[1], 0, 2)
    blocks[2][:len(directory)] = directory
    path.write_bytes(b''.join(blocks))


class Evidence:
    def __init__(self, directory):
        self.image = Path(directory) / 'fixture.exe'
        self.map = Path(directory) / 'fixture.map'
        self.pdb = Path(directory) / 'fixture.pdb'
        self.publics = [row('_Public', 0, 'public.obj')]
        self.statics = [row()]
        self.modules = ['public.obj', str(Path(directory).resolve() / 'source.ltcg.obj')]
        self.contributions = [(0, 8, CODE, 0), (0x20, 8, CODE, 1)]
        self.pe_characteristics = CODE
        self.map_stamp = STAMP
        self.map_base = 0x400000
        self.pdb_guid = GUID
        self.pdb_age = 1
        self.dbi_age = 1
        self.suffix = []

    def write(self):
        data = bytearray(0x600)
        data[:2] = b'MZ'
        struct.pack_into('<I', data, 0x3c, 0x80)
        data[0x80:0x84] = b'PE\0\0'
        struct.pack_into('<HHIIIHH', data, 0x84, 0x14c, 1, STAMP, 0, 0, 224, 0)
        optional = 0x98
        struct.pack_into('<H', data, optional, 0x10b)
        struct.pack_into('<I', data, optional + 28, 0x400000)
        struct.pack_into('<I', data, optional + 92, 16)
        struct.pack_into('<II', data, optional + 96 + 6 * 8, 0x1100, 28)
        section = optional + 224
        struct.pack_into('<8sIIII', data, section, b'.text\0\0\0', 0x400, 0x1000, 0x400, 0x200)
        struct.pack_into('<I', data, section + 36, self.pe_characteristics)
        data[0x200:0x280] = b'\x90' * 0x80
        cv = b'RSDS' + GUID + struct.pack('<I', 1) + b'fixture.pdb\0'
        struct.pack_into('<IIHHIIII', data, 0x300, 0, STAMP, 0, 0, 2, len(cv), 0x1140, 0x340)
        data[0x340:0x340 + len(cv)] = cv
        self.image.write_bytes(data)
        self.map.write_text('\n'.join([
            f' Timestamp is {self.map_stamp:08x}',
            f' Preferred load address is {self.map_base:08x}',
            ' Address Publics by Value Rva+Base Lib:Object', *self.publics,
            ' entry point at 0001:00000000', '', ' Static symbols', '',
            *self.statics, *self.suffix, '',
        ]), encoding='cp1252')
        modules = bytearray()
        for obj in self.modules:
            record = bytearray(64)
            struct.pack_into('<H', record, 34, 0xffff)
            record.extend(obj.encode('cp1252') + b'\0' + obj.encode('cp1252') + b'\0')
            record.extend(b'\0' * (-len(record) % 4))
            modules.extend(record)
        contributions = bytearray(struct.pack('<I', linked.DBI_SECTION_CONTRIB_V60))
        for offset, size, chars, module in self.contributions:
            contributions.extend(struct.pack('<H2xiiIH2xII', 1, offset, size, chars, module, 0, 0))
        dbi = bytearray(64)
        struct.pack_into('<iII', dbi, 0, -1, 19990903, self.dbi_age)
        struct.pack_into('<ii', dbi, 24, len(modules), len(contributions))
        info = struct.pack('<III', 20000404, 0, self.pdb_age) + self.pdb_guid
        write_msf(self.pdb, [b'', info, b'', bytes(dbi + modules + contributions)])

    def read(self, selected=True, selector=None):
        self.write()
        kwargs = {'static_owner': selector or {'symbol': SYMBOL, 'object': 'source.ltcg.obj', 'size': 8}} if selected else {}
        return linked.linked_functions(self.image, self.map, self.pdb, **kwargs)


class StaticEvidenceTests(unittest.TestCase):
    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory(prefix='static-owner-test-')
        self.addCleanup(self.temporary.cleanup)
        self.e = Evidence(self.temporary.name)

    def rejected(self):
        with self.assertRaises(linked.LinkedImageError):
            self.e.read()

    def test_public_default_unchanged_and_static_opt_in(self):
        public = self.e.read(selected=False)
        self.assertEqual([f['symbol'] for f in public['functions']], ['_Public'])
        self.assertEqual(public['unowned_code_contribution_count'], 1)
        result = self.e.read()
        self.assertEqual(result['function_count'], 2)
        self.assertEqual([f for f in result['functions'] if f['symbol'] == '_Public'], public['functions'])
        owner = next(f for f in result['functions'] if f['symbol'] == SYMBOL)
        self.assertEqual(owner['size'], 8)
        self.assertEqual(owner['address'], '0x00401020')
        self.assertEqual(owner['sha256'], hashlib.sha256(b'\x90' * 8).hexdigest())

    def test_windows_object_normalization(self):
        obj = 'Z:' + str(self.e.image.parent / 'source.ltcg.obj').replace('/', '\\').upper()
        self.e.statics = [row(obj=obj)]
        self.e.modules[1] = obj
        self.e.read()

    def test_qualified_same_spelling_other_object(self):
        self.e.modules.append('other.obj')
        self.e.contributions.append((0x40, 8, CODE, 2))
        self.e.statics.append(row(offset=0x40, obj='other.obj'))
        result = self.e.read()
        owners = [f for f in result['functions'] if f['symbol'] == SYMBOL]
        self.assertEqual(len(owners), 1)
        self.assertEqual(owners[0]['section_offset'], 0x20)

    def test_absent_static(self):
        self.e.statics = []
        self.rejected()

    def test_duplicate_static(self):
        self.e.statics *= 2
        self.rejected()

    def test_duplicate_qualified_static_different_address(self):
        self.e.statics.append(row(offset=0x40))
        self.e.contributions.append((0x40, 8, CODE, 1))
        self.rejected()

    def test_wrong_map_object(self):
        self.e.statics = [row(obj='other.obj')]
        self.rejected()

    def test_wrong_pdb_object(self):
        self.e.modules[1] = 'other.obj'
        self.rejected()

    def test_duplicate_module_basename(self):
        self.e.modules.append(r'D:\different\SOURCE.LTCG.OBJ')
        self.rejected()

    def test_wrong_directory_same_object_basename(self):
        self.e.modules[1] = '/unrelated/source.ltcg.obj'
        self.rejected()

    def test_wrong_map_directory_same_object_basename(self):
        self.e.statics = [row(obj='/unrelated/source.ltcg.obj')]
        self.rejected()

    def test_relative_or_unknown_drive_pdb_object(self):
        for path in ('source.ltcg.obj', r'C:\build\source.ltcg.obj'):
            with self.subTest(path=path):
                self.e.modules[1] = path
                self.rejected()

    def test_overlapping_contributions(self):
        for start, size in ((0x18, 16), (0x24, 8), (0x21, 1), (0x18, 32)):
            for flags in (CODE, 0x40000040):
                with self.subTest(start=start, size=size, flags=flags):
                    self.e.contributions = [(0, 8, CODE, 0), (0x20, 8, CODE, 1), (start, size, flags, 0)]
                    self.rejected()

    def test_adjacent_contributions_permitted(self):
        self.e.contributions.extend([(0x18, 8, CODE, 0), (0x28, 8, CODE, 0)])
        self.e.read()

    def test_duplicate_or_malformed_static_section(self):
        for suffix in ([' Static symbols'], [' 0001:nothex broken'], ['arbitrary garbage']):
            with self.subTest(suffix=suffix):
                self.e.suffix = suffix
                self.rejected()

    def test_wrong_va(self):
        self.e.statics = [row(va=0x401021)]
        self.rejected()

    def test_mid_contribution(self):
        self.e.statics = [row(offset=0x21)]
        self.rejected()

    def test_missing_function_flag(self):
        self.e.statics = [row(flags='i')]
        self.rejected()

    def test_wrong_symbol_case(self):
        self.e.statics = [row(symbol=SYMBOL.lower())]
        self.rejected()

    def test_extents_and_contribution_flags(self):
        for size, flags in [(0, CODE), (7, CODE), (9, CODE), (8, 0x40000040), (8, 0x40000020), (8, 0x60000000)]:
            with self.subTest(size=size, flags=hex(flags)):
                self.e.contributions[1] = (0x20, size, flags, 1)
                self.rejected()

    def test_pe_flags_must_be_code_and_execute(self):
        for flags in (0x40000020, 0x60000000, 0x40000040):
            with self.subTest(flags=hex(flags)):
                self.e.pe_characteristics = flags
                self.rejected()

    def test_duplicate_contributions_even_noncode(self):
        for flags in (CODE, 0x40000040):
            with self.subTest(flags=hex(flags)):
                self.e.contributions = [(0, 8, CODE, 0), (0x20, 8, CODE, 1), (0x20, 8, flags, 1)]
                self.rejected()

    def test_colliding_public(self):
        self.e.publics.append(row(symbol='_Alias', obj='public.obj'))
        self.rejected()

    def test_colliding_static(self):
        self.e.statics.append(row(symbol='?Alias@@YAXXZ', obj='other.obj'))
        self.rejected()

    def test_no_unselected_static_leaks(self):
        self.e.modules.append('other.obj')
        self.e.contributions.append((0x40, 8, CODE, 2))
        self.e.statics.append(row(symbol='_Unselected', offset=0x40, obj='other.obj'))
        self.assertNotIn('_Unselected', [f['symbol'] for f in self.e.read()['functions']])

    def test_map_parser_ignores_public_and_presection_rows(self):
        self.e.write()
        self.e.map.write_text(row(symbol='_Before') + '\n' + self.e.map.read_text())
        self.assertEqual([s['symbol'] for s in linked.map_statics(self.e.map)], [SYMBOL])

    def test_direct_selector_validation(self):
        valid = {'symbol': SYMBOL, 'object': 'source.ltcg.obj', 'size': 8}
        invalid = [{}, {'symbol': SYMBOL}, dict(valid, unexpected=True),
                   dict(valid, object='/other/source.ltcg.obj'), dict(valid, size=True),
                   dict(valid, size=8.0), dict(valid, size=0)]
        self.e.write()
        for selector in invalid:
            with self.subTest(selector=selector), self.assertRaises(linked.LinkedImageError):
                linked.linked_functions(self.e.image, self.e.map, self.e.pdb, static_owner=selector)

    def test_static_section_does_not_turn_public_into_static(self):
        self.e.publics.append(row())
        self.e.statics = []
        self.rejected()

    def test_map_parser_without_static_section(self):
        self.e.write()
        self.e.map.write_text(self.e.map.read_text().replace(' Static symbols', ' Other symbols'))
        self.assertEqual(linked.map_statics(self.e.map), [])

    def test_existing_identity_checks(self):
        for attribute, bad in [('map_stamp', STAMP + 1), ('map_base', 0x500000), ('pdb_guid', b'x' * 16), ('pdb_age', 2), ('dbi_age', 2)]:
            with self.subTest(attribute=attribute):
                old = getattr(self.e, attribute)
                setattr(self.e, attribute, bad)
                self.rejected()
                setattr(self.e, attribute, old)


class CapturedMetadataTests(unittest.TestCase):
    def test_four_real_map_rows_join_captured_pdb_metadata(self):
        """Metadata compatibility only: fake PE bytes cannot establish exactness."""
        fixture = json.loads((Path(__file__).parent / 'fixtures' / 'static-owner-metadata.json').read_text())
        with tempfile.TemporaryDirectory(prefix='static-owner-metadata-') as directory:
            path = Path(directory) / 'captured.map'
            path.write_text('\n'.join(fixture['map_lines']) + '\n', encoding='cp1252')
            statics = linked.map_statics(path)
        self.assertEqual(len(statics), 4)
        section = linked.PESection(1, '.text', 0x40000, 0x1000, 0x40000, 0x400, CODE)
        image = SimpleNamespace(path=Path(fixture['image_path']), image_base=0x400000,
                                section=lambda number: section,
                                read_section=lambda number, offset, size: b'\x00' * size)
        pdb = {'modules': [fixture['module']], 'contributions': fixture['contributions']}
        for row_data, contribution in zip(statics, fixture['contributions']):
            selector = {'symbol': row_data['symbol'], 'object': row_data['object'], 'size': contribution['size']}
            with self.subTest(symbol=row_data['symbol']):
                selected = linked._static_function(image, [], statics, pdb, selector)
                self.assertEqual(selected['section_offset'], contribution['section_offset'])
                self.assertEqual(selected['size'], contribution['size'])
                self.assertEqual(int(selected['address'], 16), row_data['address'])
                self.assertEqual(selected['pdb_module_index'], 8)
                self.assertEqual(selected['owner_visibility'], 'static')


class StaticSchemaTests(unittest.TestCase):
    def setUp(self):
        self.unit = dict(artifact_kind='linked-pe', owner_visibility='static', source='src/Main.cpp',
                         pdb_source='src/Main.cpp', symbol=SYMBOL, size=8, compare_size=8)

    def test_default_and_explicit_public(self):
        for unit in ({}, {'artifact_kind': 'linked-pe'}, {'artifact_kind': 'linked-pe', 'owner_visibility': 'public'}):
            self.assertIsNone(linked.static_owner_selector(unit))

    def test_primary_object(self):
        self.assertEqual(linked.static_owner_selector(self.unit), dict(symbol=SYMBOL, object='source.ltcg.obj', size=8))

    def test_support_objects(self):
        self.unit.update(support_sources=['src/A.cpp', 'src/Helper.cpp'], pdb_source='src/Helper.cpp')
        self.assertEqual(linked.static_owner_selector(self.unit)['object'], 'support-01-Helper.ltcg.obj')
        self.unit['normal_support_sources'] = ['src/Helper.cpp']
        self.assertEqual(linked.static_owner_selector(self.unit)['object'], 'support-01-Helper.normal.obj')

    def test_visibility_invalid_values(self):
        for value in (None, '', 'STATIC', 'private', True, 1, [], {}):
            with self.subTest(value=value), self.assertRaises(ValueError):
                linked.static_owner_selector(dict(self.unit, owner_visibility=value))

    def test_visibility_on_nonlinked(self):
        for kind in ('coff', 'coff-archive', None, ''):
            for visibility in ('static', 'public'):
                with self.subTest(kind=kind, visibility=visibility), self.assertRaises(ValueError):
                    linked.static_owner_selector(dict(self.unit, artifact_kind=kind, owner_visibility=visibility))

    def test_explicit_pdb_source_required(self):
        del self.unit['pdb_source']
        with self.assertRaises(ValueError):
            linked.static_owner_selector(self.unit)

    def test_noncanonical_paths(self):
        for path in ('/src/Main.cpp', 'C:/src/Main.cpp', 'src\\Main.cpp', './src/Main.cpp', 'src/../src/Main.cpp', 'src//Main.cpp', '../Main.cpp', '', None, 7):
            with self.subTest(path=path), self.assertRaises(ValueError):
                linked.static_owner_selector(dict(self.unit, pdb_source=path))

    def test_foreign_source_rejected(self):
        with self.assertRaises(ValueError):
            linked.static_owner_selector(dict(self.unit, pdb_source='src/Other.cpp'))

    def test_invalid_producer_graph(self):
        changes = [
            {'source': './src/Main.cpp'},
            {'support_sources': 'src/Helper.cpp'},
            {'support_sources': ['src/Helper.cpp', 'src/Helper.cpp']},
            {'support_sources': ['src/Main.cpp']},
            {'support_sources': ['src/../src/Helper.cpp']},
            {'normal_support_sources': ['src/Helper.cpp']},
            {'support_sources': ['src/Helper.cpp'], 'normal_support_sources': ['src/Helper.cpp', 'src/Helper.cpp']},
        ]
        for change in changes:
            with self.subTest(change=change), self.assertRaises(ValueError):
                linked.static_owner_selector(dict(self.unit, **change))

    def test_invalid_symbol_and_size_types(self):
        for value in ('', 'two symbols', None, 7):
            with self.subTest(symbol=value), self.assertRaises(ValueError):
                linked.static_owner_selector(dict(self.unit, symbol=value))
        for value in (None, True, '8', 8.0, -1):
            with self.subTest(size=value), self.assertRaises(ValueError):
                linked.static_owner_selector(dict(self.unit, size=value))

    def test_compare_size_requires_integer(self):
        for value in (8.0, True, '8', None):
            with self.subTest(compare_size=value), self.assertRaises(ValueError):
                linked.static_owner_selector(dict(self.unit, compare_size=value))

    def test_partial_extent_and_data_spans_rejected(self):
        for change in ({'compare_size': 7}, {'compare_size': 9}, {'size': 0, 'compare_size': 0}, {'data_spans': [{'offset': 0, 'size': 4}]}):
            with self.subTest(change=change), self.assertRaises(ValueError):
                linked.static_owner_selector(dict(self.unit, **change))


def import_script(filename):
    spec = importlib.util.spec_from_file_location(filename.replace('-', '_'), Path(__file__).parent / filename)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


class EntryPointTests(unittest.TestCase):
    """Invalid selectors stop entry points before decoding, builds, or replay."""
    def setUp(self):
        self.unit = dict(artifact_kind='linked-pe', owner_visibility='static', source='src/Main.cpp',
                         symbol=SYMBOL, size=8, functions=['FUN_00401020'], target_address=0x401020)
        self.target = {'target': {'sha256': 'fixture'}, 'pe': {'text_start': '0x401000', 'text_end': '0x402000'}}
        self.manifest = {'schema_version': 1, 'target_sha256': 'fixture', 'units': {'owner': self.unit}}
        opened = patch.object(Path, 'open', side_effect=lambda *args, **kwargs: io.BytesIO(b''))
        opened.start()
        self.addCleanup(opened.stop)

    def test_build_schema_rejects_missing_owner_before_native_tools(self):
        module = import_script('build-match-unit.py')
        with patch.object(module.tomllib, 'load', side_effect=[self.target, self.manifest]), \
             patch.object(module, 'tool_environment') as native:
            with self.assertRaisesRegex(ValueError, 'canonical|pdb_source'):
                module.load()
            native.assert_not_called()

    def test_compare_rejects_missing_owner_before_decoder(self):
        module = import_script('compare-linked-function.py')
        with patch.object(module.tomllib, 'load', side_effect=[self.manifest, self.target]), \
             patch.object(module, 'verify_capstone') as decoder, \
             patch.object(module, 'linked_functions') as evidence:
            with self.assertRaisesRegex(ValueError, 'canonical|pdb_source'):
                module.compare_unit('owner')
            decoder.assert_not_called()
            evidence.assert_not_called()

    def test_replay_rejects_missing_owner_before_subprocess(self):
        module = import_script('replay-exact-units.py')
        with patch.object(module, 'run') as runner:
            with self.assertRaisesRegex(ValueError, 'canonical|pdb_source'):
                module.replay({'owner': self.unit}, ['owner'])
            runner.assert_not_called()

    def test_replay_load_rejects_invalid_selector(self):
        module = import_script('replay-exact-units.py')
        with patch.object(module.tomllib, 'load', return_value=self.manifest), \
             patch.object(module, 'run') as runner:
            with self.assertRaisesRegex(ValueError, 'canonical|pdb_source'):
                module.load_units()
            runner.assert_not_called()

    def test_replay_groups_static_and_public_producer_once(self):
        module = import_script('replay-exact-units.py')
        self.unit.update(pdb_source='src/Main.cpp', output_dir='build/synthetic-owner',
                         profile=['/GL'], entry_symbol='_Entry', link_profile=['/LTCG'], harness='fixture')
        public = dict(self.unit, owner_visibility='public', symbol='_Public')
        def result(command):
            if command[1] == 'scripts/build-match-unit.py':
                return SimpleNamespace(returncode=0, stdout='', stderr='')
            return SimpleNamespace(returncode=0, stdout=json.dumps({'result': 'exact', 'size': 8}), stderr='')
        with patch.object(module, 'run', side_effect=result) as runner:
            report = module.replay({'static': self.unit, 'public': public}, ['static', 'public'])
        self.assertEqual(report['artifact_count'], 1)
        self.assertEqual(report['unit_count'], 2)
        self.assertEqual(report['matched_bytes'], 16)
        commands = [call.args[0][1:] for call in runner.call_args_list]
        self.assertEqual(commands, [
            ['scripts/build-match-unit.py', '--unit', 'static'],
            ['scripts/compare-linked-function.py', '--unit', 'static', '--json'],
            ['scripts/compare-linked-function.py', '--unit', 'public', '--json'],
        ])

    def test_compare_passes_selector_and_preserves_public_call(self):
        module = import_script('compare-linked-function.py')
        self.unit.update(pdb_source='src/Main.cpp', output_dir='build/synthetic-owner')
        for visibility in ('static', 'public'):
            self.unit['owner_visibility'] = visibility
            with self.subTest(visibility=visibility), \
                 patch.object(module.tomllib, 'load', side_effect=[self.manifest, self.target, {'capstone': {}}]), \
                 patch.object(module, 'verify_capstone', return_value={}), \
                 patch.object(module, 'linked_functions', side_effect=RuntimeError('evidence boundary')) as evidence:
                with self.assertRaisesRegex(RuntimeError, 'evidence boundary'):
                    module.compare_unit('owner')
                self.assertEqual(evidence.call_args.kwargs, {'static_owner': {'symbol': SYMBOL, 'object': 'source.ltcg.obj', 'size': 8}} if visibility == 'static' else {})


class MaintainedComparisonTests(unittest.TestCase):
    def test_successful_static_comparison_and_unchanged_public_report(self):
        """Real candidate parsers and comparator, fake decoder/target evidence."""
        module = import_script('compare-linked-function.py')
        with tempfile.TemporaryDirectory(prefix='static-owner-compare-') as directory:
            root = Path(directory)
            output = root / 'build' / 'fixture'
            output.mkdir(parents=True)
            evidence = Evidence(output)
            evidence.image = output / 'source.exe'
            evidence.map = output / 'source.map'
            evidence.pdb = output / 'source.pdb'
            unit = dict(artifact_kind='linked-pe', owner_visibility='static', source='src/Main.cpp',
                        pdb_source='src/Main.cpp', symbol=SYMBOL, size=8, target_address=0x401020,
                        output_dir='build/fixture', linkages=[])
            manifest = {'target_sha256': 'fixture', 'units': {'owner': unit}}
            target = {'target': {'sha256': 'fixture'}}
            config = root / 'fake-config.toml'
            config.write_text('')
            reports = []
            with ExitStack() as stack:
                stack.enter_context(patch.object(module, 'ROOT', root))
                for name in ('UNITS_MANIFEST', 'TARGET_MANIFEST', 'TOOLS_LOCK'):
                    stack.enter_context(patch.object(module, name, config))
                stack.enter_context(patch.object(module, 'verify_capstone', return_value={'version': 'fake-test-decoder'}))
                stack.enter_context(patch.object(module, 'linked_code_fields', return_value={
                    'fields': [], 'decoded_bytes': 8, 'normalization_complete': True}))
                stack.enter_context(patch.object(module, 'verified_target', return_value=SimpleNamespace(
                    read_address=lambda address, size: b'\x90' * size)))
                for visibility in ('static', 'public', None):
                    if visibility is None:
                        unit.pop('owner_visibility')
                    else:
                        unit['owner_visibility'] = visibility
                    if visibility != 'static':
                        evidence.publics = [row('_Public', 0, 'public.obj'), row()]
                        evidence.statics = []
                    evidence.write()
                    with patch.object(module.tomllib, 'load', side_effect=[manifest, target, {'capstone': {}}]):
                        reports.append(module.compare_unit('owner'))
            static, public, default = reports
            self.assertEqual(static['result'], 'exact')
            self.assertEqual(static['matched_bytes'], 8)
            self.assertTrue(static['raw_equal'])
            self.assertEqual(static['owner_selection']['visibility'], 'static')
            self.assertEqual(static['owner_selection']['expected_object'], 'source.ltcg.obj')
            self.assertEqual(static['owner_selection']['pdb_source'], 'src/Main.cpp')
            self.assertEqual(static['owner_selection']['section_offset'], 0x20)
            self.assertNotIn('owner_selection', public)
            self.assertEqual(public, default)
            self.assertEqual({key: value for key, value in static.items() if key != 'owner_selection'}, public)


if __name__ == '__main__':
    unittest.main(verbosity=2)
