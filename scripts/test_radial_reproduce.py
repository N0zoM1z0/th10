"""Portable source/synthetic tests. No compiler, native parser or decoder runs."""
import ast
import contextlib
import copy
import io
import json
from pathlib import Path
import tempfile
from types import SimpleNamespace
import unittest
from unittest.mock import patch

import radial_reproduce as r

HERE = Path(__file__).resolve().parent


def unit_text(name, unit):
    lines = ['[units.' + name + ']']
    for key, value in unit.items():
        if key != 'linkages' and value is not None:
            lines.append(key + ' = ' + json.dumps(value))
    for field in unit.get('linkages', []):
        lines += ['[[units.' + name + '.linkages]]']
        lines += [key + ' = ' + json.dumps(value) for key, value in field.items()]
    return ('\n'.join(lines) + '\n').encode()


def synthetic_tree(root):
    m = copy.deepcopy(r.load_manifest(HERE))
    units = {}
    for phase in m['recipes']:
        for name in phase['units']:
            spec = m['units'].get(name, {'size': 803})
            units[name] = {k: v for k, v in phase['recipe'].items() if v is not None}
            units[name]['size'] = spec['size']
    newname = m['new_unit']['name']
    newunit = units.pop(newname)
    newunit['linkages'] = [{'offset': i * 4, 'type': 'DIR32', 'symbol': 'synthetic',
                            'target': 0x1000} for i in range(43)]
    baseline = b'version = 1\n' + b'\n'.join(unit_text(k, v) for k, v in sorted(units.items()))
    unit = unit_text(newname, newunit)
    m['baseline_config_sha256'] = r.digest(baseline)
    m['new_unit'].update(serialized_sha256=r.digest(unit), serialized_bytes=len(unit))
    for name, value in units.items():
        m['units'][name]['sha256'] = r.digest(r.canonical(value))
    for key in ('source_pins', 'tool_pins', 'helper_pins'):
        for name in m[key]:
            data = ('synthetic source ' + name + '\n').encode()
            path = root / name
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_bytes(data)
            m[key][name] = r.digest(data)
    raw = baseline + b'\n' + unit
    (root / 'config/match-units.toml').write_bytes(raw)
    return m, raw


def receipt_for(m, verified):
    rows = []
    for i, phase in enumerate(m['recipes']):
        coff = phase['recipe']['artifact_kind'] == 'coff'
        artifacts = {k: r.digest((str(i) + k).encode())
                     for k in (['object'] if coff else ['image', 'map', 'pdb'])}
        comparisons = []
        for name in phase['units']:
            spec = m['units'].get(name, {'size': 803, 'compared_size': 803})
            report = dict(unit=name, result='exact', artifact_kind='coff' if coff else 'linked-pe',
                          size=spec['size'], matched_bytes=spec['size'],
                          compared_size=spec['compared_size'], matched_compared_bytes=spec['compared_size'],
                          artifact_sha256=artifacts['object' if coff else 'image'])
            if not coff:
                report.update(normalization_complete=True, decoded_bytes=spec['size'])
            comparisons.append(report)
        rows.append(dict(id=phase['id'], claim_sha256=r.digest(str(i).encode()), build_calls=1,
                         exit_code=0, limits=copy.deepcopy(m['native_limits']), artifacts_before=artifacts,
                         artifacts_after=copy.deepcopy(artifacts), comparisons=comparisons))
    return dict(schema='radial-native-summary-v1', plan_sha256=r.digest(r.canonical(r.native_plan(m, verified))),
                target_sha256=m['target_sha256'], phases=rows)


class ReproductionTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        self.m, self.raw = synthetic_tree(self.root)
        self.verified = r.verify_source(self.root, self.m, self.root)

    def test_frozen_manifest_and_helper_identity(self):
        m = r.load_manifest(HERE)
        for name, expected in m['helper_pins'].items():
            self.assertEqual(r.digest((HERE / name).read_bytes()), expected)

    def test_full_synthetic_source_and_exact_unit(self):
        self.assertEqual(self.verified['source_count'], 83)
        self.assertEqual(self.verified['guard_count'], 29)
        self.assertFalse(self.verified['target_read'])
        self.assertEqual(self.verified['factory_acceptance'], 'pending')

    def test_source_tool_and_helper_mutations(self):
        for table in ('source_pins', 'tool_pins', 'helper_pins'):
            path = self.root / next(iter(self.m[table]))
            original = path.read_bytes()
            path.write_bytes(original + b'drift')
            with self.assertRaises(r.VerificationError):
                r.verify_source(self.root, self.m, self.root)
            path.write_bytes(original)

    def test_config_raw_framing_and_tail_mutations(self):
        for raw in (self.raw + b'\n', self.raw.replace(b'\n\n', b'\n', 1), self.raw[:-1],
                    b'# changed\n' + self.raw, self.raw[:-20] + b'X' + self.raw[-19:]):
            with self.assertRaises((r.VerificationError, ValueError)):
                r.verify_config(raw, self.m)

    def test_graph_and_guard_omissions(self):
        for alter in (lambda m: m['recipes'][1]['recipe']['support_sources'].pop(),
                      lambda m: m['recipes'][0]['units'].pop(),
                      lambda m: m['units'].pop(next(iter(m['units'])))):
            m = copy.deepcopy(self.m)
            alter(m)
            with self.assertRaises(r.VerificationError):
                r.verify_config(self.raw, m)

    def test_nonoverlap_even_with_synthetic_authority_hashes(self):
        raw = self.raw.replace(b'offset = 4\n', b'offset = 0\n', 1)
        m = copy.deepcopy(self.m)
        m['new_unit']['serialized_sha256'] = r.digest(raw[-m['new_unit']['serialized_bytes']:])
        with self.assertRaises(r.VerificationError):
            r.verify_config(raw, m)

    def test_no_target_or_build_reads(self):
        called = []
        original = r.read_file
        def read(root, relative, *args):
            called.append(relative)
            return original(root, relative, *args)
        with patch.object(r, 'read_file', read):
            r.verify_source(self.root, self.m, self.root)
        self.assertFalse(any(p.startswith(('build/', 'resources/', '.tools/')) for p in called))

    def test_non_regular_and_escaping_files_rejected(self):
        (self.root / 'link').symlink_to(self.root / 'config/match-units.toml')
        for name in ('link', '../outside', '/etc/passwd', 'config', 'x\\y'):
            with self.assertRaises(r.VerificationError):
                r.read_file(self.root, name)
        with self.assertRaises(r.VerificationError):
            r.read_file(self.root, 'config/match-units.toml', 8)

    def test_plan_exact_12_builds_30_compares_no_execution(self):
        plan = r.native_plan(self.m, self.verified)
        self.assertEqual(len(plan['phases']), 12)
        self.assertEqual(sum(len(p['compare_argv']) for p in plan['phases']), 30)
        self.assertFalse(plan['executor_implemented'])
        self.assertFalse(plan['execution_authorized'])
        self.assertEqual(sum(p['build_argv'][0] == 'bash' for p in plan['phases']), 1)
        for phase in plan['phases'][1:]:
            self.assertEqual(phase['build_argv'][3].count('l.cold_link('), 1)
        self.assertNotIn('.analysis/', json.dumps(plan))

    def test_native_mode_refuses_before_source_reads(self):
        with patch.object(r, 'load_manifest', side_effect=AssertionError('must not read')):
            with contextlib.redirect_stdout(io.StringIO()) as output:
                self.assertEqual(r.main(['reproduce-native']), 2)
        self.assertEqual(json.loads(output.getvalue())['error'], 'native-executor-not-implemented')

    def test_runtime_entry_has_no_execution_imports_or_calls(self):
        tree = ast.parse((HERE / 'radial_reproduce.py').read_text())
        imports = {n.name.split('.')[0] for node in ast.walk(tree) if isinstance(node, ast.Import) for n in node.names}
        self.assertFalse(imports & {'subprocess', 'os', 'capstone', 'ltcg_link', 'ctypes'})
        calls = {n.func.id for n in ast.walk(tree) if isinstance(n, ast.Call) and isinstance(n.func, ast.Name)}
        self.assertFalse(calls & {'exec', 'eval', '__import__'})

    def test_receipt_consistent_is_not_execution_authentication(self):
        report = r.verify_receipt(receipt_for(self.m, self.verified), self.m, self.verified)
        self.assertTrue(report['receipt_consistent'])
        self.assertFalse(report['execution_authenticated'])
        self.assertFalse(report['native_executed_by_verifier'])

    def test_receipt_fail_closed(self):
        mutations = [lambda x: x['phases'].reverse(), lambda x: x['phases'].pop(),
                     lambda x: x['phases'][0].update(build_calls=True),
                     lambda x: x['phases'][0].update(exit_code=False),
                     lambda x: x['phases'][1].update(claim_sha256=x['phases'][0]['claim_sha256']),
                     lambda x: x['phases'][0]['artifacts_after'].update(object='0' * 64),
                     lambda x: x['phases'][0]['comparisons'][0].update(matched_bytes=1),
                     lambda x: x['phases'][1]['comparisons'][0].update(normalization_complete=1),
                     lambda x: x['phases'][1]['comparisons'][0].update(decoded_bytes=802),
                     lambda x: x['phases'][0]['limits'].update(sweeps=True),
                     lambda x: x.update(plan_sha256='0' * 64),
                     lambda x: x['phases'][0]['comparisons'][0].update(body_bytes='forbidden')]
        for mutation in mutations:
            receipt = receipt_for(self.m, self.verified)
            mutation(receipt)
            with self.assertRaises(r.VerificationError):
                r.verify_receipt(receipt, self.m, self.verified)

    def test_duplicate_json_keys_rejected(self):
        with self.assertRaises(r.VerificationError):
            json.loads('{"x":1,"x":2}', object_pairs_hook=r.unique_object)

    def test_cli_errors_are_opaque(self):
        with contextlib.redirect_stdout(io.StringIO()) as output:
            self.assertEqual(r.main(['verify-source', '--repo', str(self.root)]), 1)
        self.assertEqual(json.loads(output.getvalue()), {'ok': False, 'error': 'verification-failed'})


class PureHelperTests(unittest.TestCase):
    def test_reviewed_mov_form_does_not_grant_authority(self):
        import radial_rng_binding_core as core
        field = dict(type='DIR32', width=4, operand_kind='absolute_memory',
                     field_encoding='displacement', operand_width=4, instruction_id=core.RNG_MOV_ID,
                     instruction_mnemonic='mov', constant_read_eligible=False, constant_value_type=None,
                     operand_structure=((1, 4, 19), (3, 4, 0, 0, 0, 1)))
        self.assertTrue(core.rng_object_form(field, 0))
        self.assertFalse(core.rng_object_form(field, 2))
        for shape in (((1, 2, 19), (3, 4, 0, 0, 0, 1)),
                      ((1, 4, 1), (3, 4, 0, 0, 0, 1)),
                      ((1, 4, 19), (3, 4, 0, 19, 0, 1))):
            self.assertFalse(core.rng_object_form(dict(field, operand_structure=shape), 0))
        field['operand_structure'] = tuple(reversed(field['operand_structure']))
        self.assertFalse(core.rng_object_form(field, 0))
        self.assertTrue(core.rng_object_form(field, 4))
        self.assertFalse(core._rng_policy_allows(None, None, field, None, 4, 'candidate_addend_collision'))

    def test_scalar_budget_and_readonly_gate(self):
        import radial_rng_typed_fields as typed
        section = SimpleNamespace(rva=0, virtual_size=400, raw_size=400, raw_offset=0,
                                  characteristics=typed.READ | typed.INITIALIZED)
        image = SimpleNamespace(data=bytes(400), image_base=0x1000, sections=[section],
                                relocations=[], directories=[])
        # The real parser provides the relocation list through this method.
        image.base_relocations = lambda: []
        reader = typed.ScalarReader(image)
        for _ in range(43):
            self.assertEqual(reader.read(0x1000, 8), bytes(8))
        with self.assertRaises(ValueError):
            reader.read(0x1000, 8)
        section.characteristics |= typed.WRITE
        with self.assertRaises(ValueError):
            typed.ScalarReader(image).read(0x1000, 4)

    def test_full_owner_and_alias_authority_fail_closed(self):
        import radial_rng_binding_core as core
        import radial_rng_typed_fields as typed
        with self.assertRaises(core.BindingError):
            core._rng_authority_gate(None)
        with self.assertRaises(core.BindingError):
            core._field_gate([])
        with self.assertRaises(ValueError):
            typed.enrich_field_operands(bytes(802), 0x1000, [], decoder=object())


if __name__ == '__main__':
    unittest.main()
