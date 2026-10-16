#!/usr/bin/env python3
"""Exercise actual hint loader/parser source using defined, padded synthetic input.
No original game, game resources, actual file IO, or native build is executed.
"""
from pathlib import Path
import argparse
import re
import subprocess
import tempfile
SCRIPT = Path(__file__).resolve()
MUTATIONS = [('wrong_open_mode',
  'FileSystem::OpenFile(path, &remaining, 1)',
  'FileSystem::OpenFile(path, &remaining, 0)'),
 ('open_failure_success',
  'if (file == 0)\n        return -1;',
  'if (file == 0)\n        return 0;'),
 ('reject_stage7', 'stage > 7', 'stage > 6'),
 ('allow_256', 'entries >= 255', 'entries >= 256'),
 ('no_stage_reset', '            entries = 0;', '            entries += 0;'),
 ('clone_user_instead', 'if (mode == 0)', 'if (mode == 1)'),
 ('clone_wrong_self', 'copy->link.entry = copy;', 'copy->link.entry = entry;'),
 ('version_failure_minus1',
  'ClearHintLists(owner);\n                break;',
  'ClearHintLists(owner);\n                return -1;'),
 ('truncate_text64', 'strncpy(entry->text, first, 65);', 'strncpy(entry->text, first, 64);'),
 ('missing_quote_continue',
  'if (last == 0)\n                            break;',
  'if (last == 0)\n                            continue;'),
 ('lookup_unknown_minus1',
  '    return 0;\n}\n\n// Target41AB10.',
  '    return -1;\n}\n\n// Target41AB10.'),
 ('float_width_intermediate',
  'static_cast<float>(static_cast<double>(strlen(entry->text)) * entry->scale * 7.5)',
  'static_cast<float>(strlen(entry->text)) * entry->scale * 7.5f'),
 ('nan_to_upper', 'if (parsedScale > 4.0)', 'if (!(parsedScale <= 4.0))'),
 ('tail_advance',
  'memcpy(line, cursor, *remaining);\n            *remaining = 0;',
  'memcpy(line, cursor, *remaining);\n'
  '            cursor += *remaining;\n'
  '            *remaining = 0;'),
 ('tail_cap',
  'memcpy(line, cursor, *remaining);',
  'memcpy(line, cursor, *remaining < static_cast<int>(capacity - 1) ? *remaining : capacity - 1);'),
 ('tail_trim',
  'memcpy(line, cursor, *remaining);',
  'memcpy(line, cursor, *remaining);\n            HintTrimWhitespace(line);'),
 ('cr_first',
  "char *end = strchr(cursor, '\\n');\n"
  '        if (end == 0)\n'
  "            end = strchr(cursor, '\\r');",
  "char *end = strchr(cursor, '\\r');\n"
  '        if (end == 0)\n'
  "            end = strchr(cursor, '\\n');"),
 ('clear_stale_value', '    strcpy(key, line);', '    *value = 0;\n    strcpy(key, line);'),
 ('trim_vertical_tab',
  "while (*text == ' ' || *text == '\\t' || *text == '\\n' || *text == '\\r')",
  "while (*text == '\\v' || *text == '\\f' || *text == ' ' || *text == '\\t' || *text == '\\n' || "
  "*text == '\\r')"),
 ('reset_incoming_next', '    head->next = node;', '    node->next = 0;\n    head->next = node;'),
 ('no_partial_red', 'entry->red = static_cast<unsigned char>(atol(value));', 'entry->red = 255;'),
 ('default_remaining_zero', 'remaining = -1;', 'remaining = 0;')]

def vector_dependency_header(root):
    source = (root / 'src/AnmManager.hpp').read_text()
    start = 'struct AnmFloat3View'
    end = 'struct AnmFloat4View'
    require(source.count(start) == 1 and source.count(end) == 1,
            'Expected unique maintained vector declarations')
    vector = start + source.split(start, 1)[1].split(end, 1)[0]
    return '#pragma once\n#include "AnmVmId.hpp"\n' + vector


def require(condition, message):
    if not condition:
        raise RuntimeError(message)

def check_failure(result, label):
    output = result.stdout + result.stderr
    require(result.returncode == 1,
            label + ': expected exit1, got ' + str(result.returncode) + '\n' + output)
    require(not re.search('runtime error:|UndefinedBehaviorSanitizer|UBSan', output, re.IGNORECASE),
            label + ': sanitizer failure\n' + output)
    require(re.fullmatch('HINT_LOAD_CHECK_FAILED line [0-9]+: [^\\r\\n]+\\n', result.stderr) is not None,
            label + ': missing exact CHECK diagnostic\n' + output)

def run(root, header, mutation=None):
    source = (root / 'src/Hint.cpp').read_text().split('// Target419120.', 1)[0]
    # This fixture tests loader/exporter; the frame owner has its own fixture.
    if mutation:
        label, old, new = mutation
        require(source.count(old) == 1, label + ': mutation token count ' + str(source.count(old)))
        source = source.replace(old, new)
    # Substitute only allocation/deallocation dependency boundaries.
    # The maintained constructor and list/cleanup bodies still execute.
    require(source.count('new HintEntryView;') == 2, 'Expected two entry allocations')
    require(source.count('delete node->entry;') == 2, 'Expected two entry deletion sites')
    source = source.replace('new HintEntryView;', 'MockNewEntry();')
    source = source.replace('delete node->entry;', 'MockDelete(node->entry);')
    files = {
        'Hint.cpp': source,
        'Hint.hpp': (root / 'src/Hint.hpp').read_text(),
        'AnmVmId.hpp': (root / 'src/AnmVmId.hpp').read_text(),
        # Extract the actual vector declaration; do not substitute production headers.
        'AnmManager.hpp': vector_dependency_header(root),
        'FileSystem.hpp': header.read_text(),
        'fixture.cpp': (SCRIPT.parent / 'fixtures/hint-load.cpp').read_text(),
        'direct.h': '// _mkdir is mocked; no original runtime IO.\n',
    }
    with tempfile.TemporaryDirectory(prefix='hint-load-') as td:
        directory = Path(td)
        for name, text in files.items():
            (directory / name).write_text(text)
        compiled = subprocess.run([
            'g++', '-std=c++11', '-O1', '-g', '-fsanitize=undefined',
            '-fno-sanitize-recover=all', '-I', td,
            str(directory / 'fixture.cpp'), '-o', str(directory / 'fixture'),
        ], capture_output=True, text=True, timeout=60)
        require(compiled.returncode == 0, 'Compile failure\n' + compiled.stdout + compiled.stderr)
        return subprocess.run([str(directory / 'fixture')], cwd=td,
                              capture_output=True, text=True, timeout=30)

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--repo-root', type=Path, default=SCRIPT.parents[1])
    parser.add_argument('--file-system-header', type=Path)
    parser.add_argument('--skip-mutations', action='store_true')
    args = parser.parse_args()
    header = args.file_system_header or args.repo_root / 'src/FileSystem.hpp'
    result = run(args.repo_root, header)
    require(result.returncode == 0 and not result.stderr,
            'Baseline failure\n' + result.stdout + result.stderr)
    print(result.stdout.strip(), flush=True)
    if args.skip_mutations:
        print('PASS: baseline only; mutation controls explicitly skipped')
        return
    for mutation in MUTATIONS:
        result = run(args.repo_root, header, mutation)
        check_failure(result, mutation[0])
        print('MUTATION KILLED:', mutation[0], flush=True)
    print('PASS:', len(MUTATIONS), 'functional controls, exact exit1 CHECK and no sanitizer diagnostic')
if __name__ == '__main__':
    main()
