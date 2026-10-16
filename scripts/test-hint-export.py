#!/usr/bin/env python3
"""Run the maintained hint exporter with source-extracted, unchanged file effects.

No game executable/resources, hint-file IO, or Windows API calls are executed.
The compiler runs only host fixture code in a fresh temporary directory.
"""
from pathlib import Path
import argparse
import re
import subprocess
import tempfile
SCRIPT = Path(__file__).resolve()
MUTATIONS = [
 ('repair_negative_repeat','Hint.cpp','if (entry->remaining > 0)','if (entry->remaining != 0)'),
 ('one_based_stage','Hint.cpp','"Stage : %d\\r\\n\\r\\n", stage','"Stage : %d\\r\\n\\r\\n", stage + 1'),
 ('limit_254','Hint.cpp','emitted >= 255','emitted >= 254'),
 ('limit_256','Hint.cpp','emitted >= 255','emitted >= 256'),
 ('late_next_read','Hint.cpp','                node = next;','                node = node->next;'),
 ('swap_red_blue','Hint.cpp','static_cast<int>(entry->red),','static_cast<int>(entry->blue),'),
 ('insert_card11','Hint.cpp','{"boss_card_12",53}','{"boss_card_11",53}'),
 ('reset_failed_handle','FileSystemWrite.inc','--gReplayFileOpenCount;\n    }\n}\n\nint CloseWriteFile','--gReplayFileOpenCount;\n        gReplayFileHandle = INVALID_HANDLE_VALUE;\n    }\n}\n\nint CloseWriteFile'),
 ('trust_bool','FileSystemWrite.inc','::WriteFile(gReplayFileHandle, data, size, &bytesWritten, NULL);','if (!::WriteFile(gReplayFileHandle, data, size, &bytesWritten, NULL)) bytesWritten = 0;'),
 ('cached_failed_handle','FileSystemWrite.inc','::WriteFile(gReplayFileHandle, data, size, &bytesWritten, NULL);','int savedHandle = gReplayFileHandle;\n    ::WriteFile(gReplayFileHandle, data, size, &bytesWritten, NULL);\n    gReplayFileHandle = savedHandle;'),
 ('unknown_empty','Hint.cpp','const char *baseName = 0;','const char *baseName = "";'),
 ('skip_stage7','Hint.cpp','        for (int stage = 0; stage < 8; ++stage)','        for (int stage = 0; stage < 7; ++stage)'),
]

def require(condition, message):
    if not condition:
        raise RuntimeError(message)


def require_mutation_failure(result, name):
    require(result.returncode == 1,
            name+': expected CHECK exit 1, got '+str(result.returncode)+'\n'+result.stdout+result.stderr)
    combined = result.stdout + result.stderr
    require(not re.search(r'runtime error:|UndefinedBehaviorSanitizer|UBSan', combined, re.IGNORECASE),
            name+': sanitizer diagnostic is not a functional mutation failure\n'+combined)
    require(re.fullmatch(r'HINT_CHECK_FAILED line [0-9]+: [^\r\n]+\n', result.stderr) is not None,
            name+': expected exactly one labeled CHECK diagnostic\n'+combined)


def extract_function(source, signature):
    require(source.count(signature) == 1, "Missing or duplicate function: "+signature)
    start = source.index(signature)
    opening = source.find('{', start)
    require(opening != -1, 'Missing function body: '+signature)
    depth = 1
    cursor = opening + 1
    while depth:
        require(cursor < len(source), 'Unclosed function body: '+signature)
        depth += (source[cursor] == '{') - (source[cursor] == '}')
        cursor += 1
    return source[start:cursor]

def source_inputs(root):
    filesystem = (root/'src/FileSystem.cpp').read_text()
    header = (root/'src/FileSystem.hpp').read_text()
    require('void WriteToOpenFile(const void *data, unsigned int size);' in header,
            'Expected unchanged void FileSystem writer declaration')
    bodies = '\n\n'.join(extract_function(filesystem, signature) for signature in [
        'void WriteToOpenFile(const void *data, unsigned int size)',
        'int CloseWriteFile()'])
    return {
        'Hint.cpp': (root/'src/Hint.cpp').read_text(),
        'Hint.hpp': (root/'src/Hint.hpp').read_text(),
        'FileSystem.hpp': header,
        'FileSystemWrite.inc': 'namespace FileSystem\n{\n'+bodies+'\n}\n',
        'host-test.cpp': (SCRIPT.parent/'fixtures/hint-export.cpp').read_text(),
        'direct.h': '// _mkdir is replaced by the fixture mock.\n',
    }

def run(inputs, mutation=None):
    if mutation:
        require(mutation[1] in inputs, 'Mutation source missing: '+mutation[1])
    with tempfile.TemporaryDirectory(prefix='hint-export-') as td:
        root = Path(td)
        for name, original in inputs.items():
            text = original
            if mutation and mutation[1] == name:
                _, _, old, new = mutation
                require(text.count(old) == 1,
                        'Mutation token missing or nonunique: '+repr((name, old, text.count(old))))
                text = text.replace(old, new)
            (root/name).write_text(text)
        compiled = subprocess.run([
            'g++','-std=c++11','-O1','-g','-fsanitize=undefined',
            '-fno-sanitize-recover=all','-I',str(root),
            str(root/'host-test.cpp'),'-o',str(root/'test')],
            capture_output=True,text=True,timeout=60)
        require(compiled.returncode == 0, 'Fixture compilation failed\n'+compiled.stdout+compiled.stderr)
        return subprocess.run([str(root/'test')],cwd=root,
                              capture_output=True,text=True,timeout=30)

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--repo-root',type=Path,default=SCRIPT.parents[1])
    parser.add_argument('--skip-mutations',action='store_true',
                        help='Run only the baseline effect fixtures for lightweight CI')
    args=parser.parse_args()
    inputs=source_inputs(args.repo_root)
    result=run(inputs)
    require(result.returncode == 0 and result.stderr == '',
            'Baseline fixture failed\n'+result.stdout+result.stderr)
    print(result.stdout.strip())
    if args.skip_mutations:
        print('PASS: baseline only; mutation controls explicitly skipped')
        return
    for mutation in MUTATIONS:
        result=run(inputs,mutation)
        require_mutation_failure(result, mutation[0])
        print('MUTATION KILLED:',mutation[0])
    print('PASS:',len(MUTATIONS),'functional mutations; actual void file helper extracted')

if __name__ == '__main__':
    main()
