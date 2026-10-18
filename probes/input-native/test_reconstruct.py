#!/usr/bin/env python3
"""Run against an explicitly supplied pinned base tree; leaves it untouched."""
import argparse
import hashlib
import json
import pathlib
import shutil
import tempfile
from reconstruct import reconstruct, ROOT

def main():
    p = argparse.ArgumentParser()
    p.add_argument('--base-tree', required=True, type=pathlib.Path)
    args = p.parse_args()
    manifest = json.loads((ROOT/'manifest.json').read_text())
    with tempfile.TemporaryDirectory(prefix='input-reconstruction-test-') as temp:
        work = pathlib.Path(temp)
        base = work/'base'
        (base/'src').mkdir(parents=True)
        for rel, digest in manifest['base_files'].items():
            raw = (args.base_tree/rel).read_bytes()
            if hashlib.sha256(raw).hexdigest() != digest:
                raise ValueError('test fixture is not pinned: '+rel)
            (base/rel).write_bytes(raw)
        reconstruct(base, work/'out')
        for rel, digest in manifest['overlays'].items():
            raw = (work/'out'/rel).read_bytes()
            if hashlib.sha256(raw).hexdigest() != digest:
                raise RuntimeError('wrong reconstruction: '+rel)
        def rejected(destination):
            try:
                reconstruct(base, destination)
            except ValueError:
                return
            raise RuntimeError('unsafe reconstruction accepted')
        rejected(work/'out')
        rejected(base/'inside')
        rejected(base)
        rejected(ROOT/'inside')
        (work/'link').symlink_to(work/'out', target_is_directory=True)
        rejected(work/'link')
        original = (base/'src/Main.cpp').read_bytes()
        (base/'src/Main.cpp').write_bytes(original+b'\n')
        rejected(work/'bad-base')
        if (work/'bad-base').exists():
            raise RuntimeError('hash rejection created output')
        (base/'src/Main.cpp').write_bytes(original)
        for rel, digest in manifest['base_files'].items():
            if hashlib.sha256((base/rel).read_bytes()).hexdigest() != digest:
                raise RuntimeError('base changed')
    print('PASS five exact overlay hashes; six rejected unsafe/stale outputs; base unchanged')

if __name__ == '__main__':
    main()
