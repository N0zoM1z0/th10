#!/usr/bin/env python3
"""Finite regression of exact key conditions extracted from native Input.cpp."""
import hashlib, json, os, pathlib, re, subprocess, tempfile
ROOT = pathlib.Path(__file__).resolve().parent

def require(ok, message):
    if not ok:
        raise RuntimeError(message)

def extract(source):
    start = source.index('unsigned short UpdateInput(')
    owner = source[start:]
    a = owner.index('            GetKeyboardState(keys);')
    a = owner.index('\n', a) + 1
    b = owner.index('        } else if (', a)
    c = owner.index('        } else {\n', b) + len('        } else {\n')
    d = owner.index('        }\n', c)
    blocks = [owner[a:b], owner[c:d]]
    for block in blocks:
        require(len(block.splitlines()) == 19, 'expected all 19 key branches')
        require(all(re.fullmatch(r'\s*if \(.+\) keyboardMask \|= 0x[0-9A-F]+;', line) for line in block.splitlines()), 'unexpected extracted statement')
    return blocks

def translated(blocks):
    result = 'namespace input_candidate {\n'
    for name, block in zip(['Win32KeyMask', 'DirectInputKeyMask'], blocks):
        result += 'unsigned short ' + name + '(const unsigned char keys[256]) {\nunsigned short keyboardMask = 0;\n' + block + 'return keyboardMask;\n}\n'
    return result + '}\n'

def main():
    os.nice(10)
    source_bytes = (ROOT/'src/Input.cpp').read_bytes()
    source = source_bytes.decode('utf-8').replace('\r\n', '\n')
    require('\r' not in source and '\0' not in source, 'invalid source text')
    require(all(x not in source for x in ['IsDown(', 'Win32KeyboardMask(', 'DirectInputKeyboardMask(']), 'out-of-owner helper remains')
    blocks = extract(source)
    variants = [('baseline', blocks)]
    edits = [('key-index', 'keys[0x5A]', 'keys[0x59]'), ('action-bit', 'keyboardMask |= 0x0001;', 'keyboardMask |= 0x0002;'), ('high-bit', '& 0x80u', '& 0x01u'), ('alternative-and', ' || ', ' && ')]
    for mode in range(2):
        for label, old, new in edits:
            if mode == 1 and label == 'key-index': old, new = 'keys[0x2C]', 'keys[0x2B]'
            require(old in blocks[mode], 'mutant anchor missing')
            changed = list(blocks)
            changed[mode] = changed[mode].replace(old, new, 1)
            variants.append(('mode%d-%s' % (mode, label), changed))
    results = []
    with tempfile.TemporaryDirectory(prefix='input-key-owner-') as work:
        work = pathlib.Path(work)
        for name, variant in variants:
            (work/'extracted_keys.hpp').write_text(translated(variant))
            cmd = ['g++', '-std=c++03', '-pedantic-errors', '-Wall', '-Wextra', '-Werror', '-O2', '-I'+str(work), '-I'+str(ROOT/'tests'), str(ROOT/'tests/test_keys.cpp'), '-o', str(work/'test')]
            build = subprocess.run(cmd, capture_output=True, text=True, timeout=45)
            require(build.returncode == 0, name+' compile failure is not a semantic kill: '+build.stderr)
            run = subprocess.run([str(work/'test')], capture_output=True, text=True, timeout=20)
            if name == 'baseline':
                require(run.returncode == 0 and run.stdout.strip() == 'PASS cases=270848', 'baseline failed: '+run.stdout+run.stderr)
            else:
                require(run.returncode == 1 and run.stdout.startswith('MISMATCH '), name+' did not produce a semantic mismatch')
            results.append({'name':name, 'returncode':run.returncode, 'output':run.stdout.strip()})
    print(json.dumps({'scope':'finite exact-source key-block extraction; not native SDK/ABI or universal proof', 'source_canonical_lf_sha256':hashlib.sha256(source.encode()).hexdigest(), 'cases':270848, 'semantic_mutants':8, 'results':results}, indent=2))
if __name__ == '__main__':
    main()
