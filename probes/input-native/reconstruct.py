#!/usr/bin/env python3
"""Reconstruct five reviewed a003 overlays in a new, separate source-only tree."""
import argparse
import hashlib
import json
import pathlib
import subprocess
import tempfile

ROOT = pathlib.Path(__file__).resolve().parent

def require(ok, message):
    if not ok:
        raise ValueError(message)

def sha(data):
    return hashlib.sha256(data).hexdigest()

def reconstruct(base, output):
    base = pathlib.Path(base).resolve()
    output = pathlib.Path(output).absolute()
    require(not output.exists() and not output.is_symlink(), 'output must not exist')
    output = output.resolve()
    require(base not in output.parents and output not in base.parents and output != base,
            'output must be separate from base tree')
    require(ROOT not in output.parents and output not in ROOT.parents and output != ROOT,
            'output must be separate from packet')
    manifest = json.loads((ROOT / 'manifest.json').read_text())
    inputs = {}
    for rel, digest in manifest['base_files'].items():
        data = (base / rel).read_bytes()
        require(sha(data) == digest, 'base raw hash mismatch: ' + rel)
        inputs[rel] = data
    # All patch writes are isolated and discarded on failure. No source tree is changed.
    with tempfile.TemporaryDirectory(prefix='input-reconstruct-') as temp:
        work = pathlib.Path(temp)
        result = {}
        for rel, digest in manifest['overlays'].items():
            if rel in inputs:
                text = inputs[rel].decode('utf-8').replace('\r\n', '\n')
                require('\r' not in text, 'mixed/unrecognized newline')
                dest = work / pathlib.Path(rel).name
                dest.write_bytes(text.encode('utf-8'))
                patch = ROOT / 'patches' / (dest.name + '.patch')
                proc = subprocess.run(['patch', '--batch', '--forward', '--fuzz=0',
                                       str(dest), str(patch)], capture_output=True, text=True)
                require(proc.returncode == 0, 'zero-fuzz patch failed: ' + proc.stdout + proc.stderr)
                data = dest.read_bytes()
                require(b'\r' not in data, 'patch must produce canonical LF')
                if manifest['output_newlines'][rel] == 'CRLF':
                    data = data.replace(b'\n', b'\r\n')
            else:
                data = (ROOT / rel).read_bytes()
            require(sha(data) == digest, 'final raw hash mismatch: ' + rel)
            result[rel] = data
        # Claim an absent destination only after every input and output is verified.
        output.mkdir(parents=False, exist_ok=False)
        (output / 'src').mkdir()
        for rel, data in result.items():
            (output / rel).write_bytes(data)
        for rel, digest in manifest['overlays'].items():
            require(sha((output / rel).read_bytes()) == digest, 'written hash mismatch: ' + rel)
    return manifest['overlays']

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--base-tree', required=True, type=pathlib.Path)
    parser.add_argument('--output', required=True, type=pathlib.Path)
    args = parser.parse_args()
    print(json.dumps(reconstruct(args.base_tree, args.output), indent=2))

if __name__ == '__main__':
    main()
