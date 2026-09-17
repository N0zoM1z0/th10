"""Audit target function origins against the pinned VC7.1 static runtime.

Only complete COFF function extents are considered. This is an origin and
boundary review aid, never an exact source/codegen Oracle.
"""

import argparse
import csv
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'scripts'))
from target_identity import pe_bytes_at, resolve_target, verify_target

LIBRARY_HASHES = {
    'libcmt.lib': '6bed2e73e7ecf52818133011058e240cd38b122f51302c0c0194d5ff3167357e',
    'libcpmt.lib': 'd93f56389e8b78661a00fe63aa4575f121a76c14c83ed9f87ca97acb8874ad16',
}
EVIDENCE_ID = 'vc71-static-runtime-body-2026-09-17'
AMBIGUOUS_EVIDENCE_ID = 'vc71-static-runtime-family-2026-09-17'
SHORT_EVIDENCE_ID = 'vc71-static-runtime-short-body-2026-09-17'
EXTENDED_EVIDENCE_ID = 'vc71-static-runtime-extended-body-2026-09-17'
CONTAINED_EVIDENCE_ID = 'vc71-static-runtime-contained-fragment-2026-09-17'
NAMED_EVIDENCE_ID = 'vc71-static-runtime-named-body-2026-09-17'

spec = importlib.util.spec_from_file_location('compare_coff_function', ROOT / 'scripts' / 'compare-coff-function.py')
coff = importlib.util.module_from_spec(spec)
spec.loader.exec_module(coff)


def members(path):
    data = path.read_bytes()
    if data[:8] != b'!<arch>\n':
        raise ValueError(f'not a COFF archive: {path}')
    pos = 8
    longnames = b''
    while pos < len(data):
        header = data[pos:pos + 60]
        if len(header) != 60 or header[58:60] != b'`\n':
            raise ValueError(f'invalid archive member at {pos}')
        size = int(header[48:58].strip())
        payload = data[pos + 60:pos + 60 + size]
        if len(payload) != size:
            raise ValueError(f'truncated archive member at {pos}')
        raw_name = header[:16].decode('ascii').strip()
        if raw_name == '//':
            longnames = payload
        elif raw_name.startswith('/') and raw_name[1:].isdigit():
            start = int(raw_name[1:])
            end = longnames.find(b'\x00', start)
            name = longnames[start:end].decode('ascii') if end >= 0 else raw_name
            yield name, payload
        elif raw_name != '/':
            yield raw_name.rstrip('/'), payload
        pos += 60 + size + (size & 1)


def csv_rows(name):
    with (ROOT / 'config' / name).open(newline='', encoding='utf-8') as stream:
        return list(csv.DictReader(stream))


def write_rows(name, rows):
    path = ROOT / 'config' / name
    prior_lines = path.read_text(encoding='utf-8').splitlines(keepends=True)
    prior_rows = list(csv.DictReader(prior_lines))
    if len(prior_lines) != len(rows) + 1 or len(prior_rows) != len(rows):
        raise ValueError(f'CSV physical rows changed unexpectedly: {path}')
    with path.open('w', newline='', encoding='utf-8') as stream:
        writer = csv.DictWriter(stream, fieldnames=list(rows[0]), lineterminator='\n')
        stream.write(prior_lines[0])
        for old, new, raw in zip(prior_rows, rows, prior_lines[1:]):
            if old['address'] != new['address']:
                raise ValueError(f'CSV row order changed unexpectedly: {path}')
            if old == new:
                stream.write(raw)
            else:
                writer.writerow(new)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--apply', action='store_true', help='record qualified origin and boundary reviews')
    args = parser.parse_args()
    target_path = resolve_target()
    observed, problems = verify_target(target_path)
    if problems:
        raise ValueError(problems)
    image = target_path.read_bytes()
    origin_rows = csv_rows('function-origins.csv')
    origins = {r['address']: r for r in origin_rows}
    function_rows = csv_rows('functions.csv')
    boundaries = csv_rows('function-boundaries.csv')
    candidates = {}
    prefix_candidates = []
    for row in function_rows:
        address = row['address']
        origin = origins[address]
        if origin['disposition'] != 'review' and origin['evidence_id'] not in (
                EVIDENCE_ID, AMBIGUOUS_EVIDENCE_ID, SHORT_EVIDENCE_ID,
                EXTENDED_EVIDENCE_ID, CONTAINED_EVIDENCE_ID,
                NAMED_EVIDENCE_ID):
            continue
        size = int(row['size'])
        if size < 8:
            continue
        code = pe_bytes_at(image, int(address, 0), size)
        candidates.setdefault(size, []).append((address, code, row['current_name']))
        if int(address, 0) >= 0x00452000:
            prefix_candidates.append((address, size, row['current_name']))
    selection = ROOT / '.tools' / 'msvc710-sp1' / 'Vc7' / 'lib'
    libraries = [selection / name for name in ('libcmt.lib', 'libcpmt.lib')]
    matches = []
    extended_matches = []
    stats = {}
    with tempfile.TemporaryDirectory() as directory:
        object_path = Path(directory) / 'member.obj'
        for library in libraries:
            digest = hashlib.sha256(library.read_bytes()).hexdigest()
            if digest != LIBRARY_HASHES[library.name]:
                raise ValueError(f'wrong VC7.1 runtime library: {library}')
            inspected = 0
            usable = 0
            errors = 0
            for member, payload in members(library):
                if payload[:2] != b'\x4c\x01':
                    continue
                inspected += 1
                object_path.write_bytes(payload)
                try:
                    functions = coff.object_functions(object_path)
                except (ValueError, UnicodeDecodeError, IndexError):
                    errors += 1
                    continue
                for function in functions:
                    size = function['size']
                    if size is None or (size not in candidates and (
                            library.name != 'libcmt.lib' or size < 32)):
                        continue
                    try:
                        code, relocations = coff.object_function(object_path, function['symbol'])
                    except (ValueError, UnicodeDecodeError, IndexError):
                        continue
                    if any(r['type'] not in ('DIR32', 'REL32') for r in relocations):
                        continue
                    usable += 1
                    excluded = set()
                    for relocation in relocations:
                        excluded.update(range(relocation['offset'], relocation['offset'] + 4))
                    comparable = tuple(i for i in range(size) if i not in excluded)
                    if len(comparable) < 12:
                        continue
                    for address, target, ida_name in candidates.get(size, []):
                        if all(code[i] == target[i] for i in comparable):
                            matches.append({'address': address, 'size': size,
                                'ledger_name': ida_name, 'library': library.name,
                                'library_sha256': digest,
                                'member': member, 'symbol': function['symbol'],
                                'relocations': relocations, 'comparable_bytes': len(comparable)})
                    if library.name == 'libcmt.lib' and size >= 32 and len(comparable) >= 24:
                        signature = comparable[:12]
                        for address, ledger_size, ida_name in prefix_candidates:
                            if ledger_size >= size:
                                continue
                            try:
                                target = pe_bytes_at(image, int(address, 0), size)
                            except ValueError:
                                continue
                            if not all(code[index] == target[index] for index in signature):
                                continue
                            if all(code[index] == target[index] for index in comparable):
                                extended_matches.append({
                                    'address': address, 'ledger_size': ledger_size,
                                    'size': size, 'ledger_name': ida_name,
                                    'library': library.name, 'library_sha256': digest,
                                    'member': member, 'symbol': function['symbol'],
                                    'relocations': relocations,
                                    'comparable_bytes': len(comparable)})
            stats[library.name] = {'archive_members': inspected, 'usable_functions': usable, 'parse_errors': errors}
    by_address = {}
    for match in matches:
        by_address.setdefault(match['address'], []).append(match)
    qualified = [group[0] for group in by_address.values()
                 if len(group) == 1 and group[0]['size'] >= 32
                 and group[0]['comparable_bytes'] >= 24]
    qualified_short = [group[0] for group in by_address.values()
                       if len(group) == 1 and 12 <= group[0]['size'] < 32
                       and group[0]['comparable_bytes'] >= 12]
    qualified_ambiguous = [group for group in by_address.values()
                           if len(group) > 1 and group[0]['size'] >= 32 and
                           all(match['library'] == 'libcmt.lib' and
                               match['comparable_bytes'] >= 24 for match in group)]
    already_qualified = {match['address'] for match in qualified + qualified_short}
    already_qualified.update(group[0]['address'] for group in qualified_ambiguous)
    qualified_named = []
    for address, group in by_address.items():
        if address in already_qualified or not (
                origins[address]['disposition'] == 'review' or
                origins[address]['evidence_id'] == NAMED_EVIDENCE_ID):
            continue
        if any(match['library'] != 'libcmt.lib' for match in group):
            continue
        ledger_name = group[0]['ledger_name'].split(':')[-1].strip('_').lower()
        named = [match for match in group if len(ledger_name) >= 4 and
                 ledger_name in match['symbol'].lower() and
                 match['size'] >= 9 and match['comparable_bytes'] >= 5]
        if named:
            qualified_named.append(max(named, key=lambda match: match['comparable_bytes']))
    extended_by_address = {}
    for match in extended_matches:
        extended_by_address.setdefault(match['address'], []).append(match)
    qualified_extended = [group[0] for group in extended_by_address.values()
                          if len(group) == 1]
    parent_matches = [(match, 'high') for match in qualified + qualified_extended]
    parent_matches += [(match, 'medium') for match in qualified_named]
    parent_matches += [(group[0], 'medium') for group in qualified_ambiguous]
    separately_matched = {match['address'] for match in qualified + qualified_short + qualified_extended + qualified_named}
    separately_matched.update(group[0]['address'] for group in qualified_ambiguous)
    contained = []
    for child in function_rows:
        address = child['address']
        origin = origins[address]
        if address in separately_matched or (origin['disposition'] != 'review' and
                origin['evidence_id'] != CONTAINED_EVIDENCE_ID):
            continue
        start, end = int(address, 0), int(child['span_end'], 0)
        parents = [(match, confidence) for match, confidence in parent_matches
                   if int(match['address'], 0) < start and
                   end < int(match['address'], 0) + match['size']]
        if len(parents) > 1:
            raise ValueError(f'ambiguous containing runtime owner: {address}')
        if parents:
            parent, confidence = parents[0]
            contained.append({'address': address, 'size': int(child['size']),
                              'parent_address': parent['address'],
                              'parent_symbol': parent['symbol'],
                              'parent_member': parent['member'],
                              'parent_extent': parent['size'],
                              'confidence': confidence})
    # Link the members to each other in the target. A symbol with several
    # plausible target entries is deliberately left unresolved.
    by_symbol = {}
    for match in qualified + qualified_short + qualified_extended + qualified_named:
        by_symbol.setdefault(match['symbol'], set()).add(int(match['address'], 0))
    resolved = 0
    for match in qualified + qualified_short + qualified_extended + qualified_named:
        address = int(match['address'], 0)
        target = pe_bytes_at(image, address, match['size'])
        for relocation in match['relocations']:
            expected = by_symbol.get(relocation['symbol'], set())
            if len(expected) != 1:
                continue
            offset = relocation['offset']
            if relocation['type'] == 'REL32':
                displacement = struct.unpack_from('<i', target, offset)[0]
                actual = (address + offset + 4 + displacement) & 0xffffffff
            else:
                actual = struct.unpack_from('<I', target, offset)[0]
            if actual not in expected:
                raise ValueError(f"runtime cross-reference disagrees: {match['address']} -> {relocation['symbol']}")
            resolved += 1
    report = {'target_sha256': observed['sha256'], 'library_sha256': LIBRARY_HASHES,
              'stats': stats, 'match_count': len(matches),
              'qualified_count': len(qualified),
              'qualified_short_count': len(qualified_short),
              'qualified_extended_count': len(qualified_extended),
              'qualified_named_count': len(qualified_named),
              'contained_count': len(contained),
              'resolved_runtime_references': resolved,
              'qualified': qualified, 'qualified_short': qualified_short,
              'qualified_extended': qualified_extended,
              'qualified_named': qualified_named,
              'contained': contained,
              'qualified_ambiguous': qualified_ambiguous,
              'ambiguous': [group for group in by_address.values()
                                                if len(group) != 1]}
    out = ROOT / '.analysis' / 'gpt-5.6-sol' / '20260917-origin-review' / 'runtime-origin-review.json'
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    promoted_boundaries = 0
    if args.apply:
        qualified_by_address = {match['address']: match for match in qualified}
        short_by_address = {match['address']: match for match in qualified_short}
        extended_by_address = {match['address']: match for match in qualified_extended}
        named_by_address = {match['address']: match for match in qualified_named}
        ambiguous_by_address = {group[0]['address']: group for group in qualified_ambiguous}
        contained_by_address = {match['address']: match for match in contained}
        for row in origin_rows:
            match = qualified_by_address.get(row['address'])
            short = short_by_address.get(row['address'])
            extended = extended_by_address.get(row['address'])
            named = named_by_address.get(row['address'])
            group = ambiguous_by_address.get(row['address'])
            child = contained_by_address.get(row['address'])
            if (match is None and short is None and extended is None and
                    named is None and group is None and child is None):
                continue
            if row['disposition'] not in ('review', 'exclude'):
                raise ValueError(f"refusing to replace origin: {row['address']}")
            evidence_id = (EVIDENCE_ID if match is not None else
                           SHORT_EVIDENCE_ID if short is not None else
                           EXTENDED_EVIDENCE_ID if extended is not None else
                           NAMED_EVIDENCE_ID if named is not None else
                           CONTAINED_EVIDENCE_ID if child is not None else
                           AMBIGUOUS_EVIDENCE_ID)
            row.update(origin='library', subsystem='CRT', disposition='exclude',
                       confidence=(child['confidence'] if child is not None else
                                   'high' if match is not None or extended is not None else
                                   'medium'),
                       evidence_id=evidence_id)
        for row in function_rows:
            match = qualified_by_address.get(row['address'])
            short = short_by_address.get(row['address'])
            extended = extended_by_address.get(row['address'])
            named = named_by_address.get(row['address'])
            group = ambiguous_by_address.get(row['address'])
            child = contained_by_address.get(row['address'])
            if (match is None and short is None and extended is None and
                    named is None and group is None and child is None):
                continue
            if row['owner'] not in ('', 'library'):
                raise ValueError(f"refusing to replace function owner: {row['address']}")
            row.update(module='CRT', status='excluded', owner='library')
            if (match is not None or short is not None or extended is not None or named is not None) and not row['proposed_name']:
                row['proposed_name'] = (match or short or extended or named)['symbol']
            if match is not None:
                note = (f"Pinned VC7.1 {match['library']} member {match['member']} symbol "
                        f"{match['symbol']}: complete {match['size']}-byte COFF extent and "
                        f"{match['comparable_bytes']} non-relocation bytes match target; "
                        f"origin only, no source-exact claim ({EVIDENCE_ID}).")
                evidence_id = EVIDENCE_ID
            elif short is not None:
                note = (f"Pinned VC7.1 {short['library']} member {short['member']} symbol "
                        f"{short['symbol']}: unique complete {short['size']}-byte COFF body "
                        f"with {short['comparable_bytes']} exact non-relocation bytes; "
                        f"short-body provenance is medium confidence ({SHORT_EVIDENCE_ID}).")
                evidence_id = SHORT_EVIDENCE_ID
            elif extended is not None:
                note = (f"Pinned VC7.1 {extended['library']} member {extended['member']} "
                        f"symbol {extended['symbol']}: complete {extended['size']}-byte "
                        f"COFF body matches target across {extended['comparable_bytes']} "
                        f"non-relocation bytes. Ledger entry is the "
                        f"{extended['ledger_size']}-byte contiguous fragment; the "
                        f"COFF extent also owns a suffix/shared helper "
                        f"({EXTENDED_EVIDENCE_ID}).")
                evidence_id = EXTENDED_EVIDENCE_ID
            elif named is not None:
                note = (f"Pinned VC7.1 libcmt.lib member {named['member']} symbol "
                        f"{named['symbol']} has a complete same-size body and "
                        f"{named['comparable_bytes']} exact non-relocation bytes; "
                        f"the target function name independently agrees "
                        f"({NAMED_EVIDENCE_ID}).")
                evidence_id = NAMED_EVIDENCE_ID
            elif child is not None:
                note = (f"Local fragment lies wholly inside pinned VC7.1 libcmt.lib "
                        f"member {child['parent_member']} symbol "
                        f"{child['parent_symbol']} at {child['parent_address']} "
                        f"({child['parent_extent']} matching COFF bytes); "
                        f"separate authored ownership is not claimed "
                        f"({CONTAINED_EVIDENCE_ID}).")
                evidence_id = CONTAINED_EVIDENCE_ID
            else:
                first = group[0]
                note = (f"Pinned VC7.1 libcmt.lib has {len(group)} matching full "
                        f"{first['size']}-byte COFF bodies with at least 24 exact "
                        f"non-relocation bytes; runtime origin is supported, symbol "
                        f"identity remains ambiguous ({AMBIGUOUS_EVIDENCE_ID}).")
                evidence_id = AMBIGUOUS_EVIDENCE_ID
            if evidence_id not in row['notes']:
                row['notes'] = (row['notes'] + ' ' + note).strip()
        intervals = [(int(row['address'], 0), int(row['span_end'], 0), row['address'])
                     for row in function_rows]
        for row in boundaries:
            match = qualified_by_address.get(row['address'])
            group = ambiguous_by_address.get(row['address'])
            if (match is None and group is None) or row['state'] == 'reviewed':
                continue
            start, end = int(row['address'], 0), int(row['span_end'], 0)
            if any(other != row['address'] and begin <= end and finish >= start
                   for begin, finish, other in intervals):
                continue
            if row['state'] == 'provisional' and row['notes'].startswith('Complete dense decode'):
                pass
            elif match is not None and row['state'] == 'needs_review' and (
                    row['notes'].startswith('remote-ghidra-body') or
                    row['notes'].startswith('tail=int3') or
                    row['notes'].startswith('overlapping-extents') or
                    row['notes'].startswith('external-interior-branch')):
                pass
            elif group is not None and row['state'] == 'needs_review' and (
                    row['notes'].startswith('dense-decode=')):
                code = pe_bytes_at(image, start, group[0]['size'])
                relocations = group[0]['relocations']
                if len(relocations) < 8 or any(
                        relocation['type'] != 'DIR32' or not (
                            start <= struct.unpack_from('<I', code, relocation['offset'])[0] <= end)
                        for relocation in relocations):
                    continue
            else:
                continue
            evidence_id = EVIDENCE_ID if match is not None else AMBIGUOUS_EVIDENCE_ID
            detail = (f"independent {match['library']} COFF symbol {match['symbol']}"
                      if match is not None else
                      f"{len(group)} pinned libcmt.lib COFF symbols with identical complete extent")
            row.update(state='reviewed', confidence='high',
                       evidence_id=row['evidence_id'] + '+' + evidence_id,
                       notes=row['notes'] + f"; {detail} and matching non-relocation body establish the physical extent")
            promoted_boundaries += 1
        write_rows('function-origins.csv', origin_rows)
        write_rows('functions.csv', function_rows)
        write_rows('function-boundaries.csv', boundaries)
    print(json.dumps({'stats': stats, 'match_count': len(matches),
                      'qualified_origins': len(qualified),
                      'qualified_short_origins': len(qualified_short),
                      'qualified_extended_origins': len(qualified_extended),
                      'qualified_named_origins': len(qualified_named),
                      'contained_runtime_fragments': len(contained),
                      'ambiguous_runtime_origins': len(qualified_ambiguous),
                      'resolved_runtime_references': resolved,
                      'promoted_boundaries': promoted_boundaries, 'output': str(out)}))

if __name__ == '__main__':
    main()
