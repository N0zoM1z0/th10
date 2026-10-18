#!/usr/bin/env python3
"""Replay synthetic suites from exact archived files, rewriting only fixture paths in a temporary copy."""
import sys
if not __debug__:
    raise SystemExit("Optimized Python is unsupported; assertions are required.")

import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import tempfile

ROOT=Path(__file__).resolve().parent
manifest=json.loads((ROOT/'archive-manifest.json').read_text())
required={'REPLAY_TESTS.py','TEST_REPLAY_WRAPPER.py','inventory.py','localization.py',
          'test_inventory.py','test_localization.py'}
required.update('synthetic-fixtures/'+name for name in (
    'test_op82_baseline_order.py','test_op82_local_order.py','test_op82_metadata.py',
    'op82_baseline_order.py','op82_local_order.py','op82_metadata.py','test_owner.py','run.py'))
actual=set()
for entry in ROOT.rglob('*'):
    if entry.is_symlink() or (not entry.is_dir() and not entry.is_file()):
        raise SystemExit('Archive manifest validation failed.')
    if entry.is_file():actual.add(str(entry.relative_to(ROOT)))
if not required<=set(manifest['files']) or actual!=set(manifest['files'])|{'archive-manifest.json'}:
    raise SystemExit('Archive manifest validation failed.')
for name,digest in manifest['files'].items():
    relative=Path(name)
    if relative.is_absolute() or '..' in relative.parts:
        raise SystemExit('Archive manifest validation failed.')
    member=ROOT/relative
    if any(part.is_symlink() for part in (member,*member.parents)):
        raise SystemExit('Archive manifest validation failed.')
    if not member.is_file() or hashlib.sha256(member.read_bytes()).hexdigest()!=digest:
        raise SystemExit('Archive manifest validation failed.')
os.nice(10)
os.sched_setaffinity(0,{min(os.sched_getaffinity(0))})
with tempfile.TemporaryDirectory(prefix='ecl-inventory-synthetic-') as name:
    work=Path(name)
    for filename in ('inventory.py','localization.py','test_inventory.py','test_localization.py'):
        shutil.copyfile(ROOT/filename,work/filename)
    shutil.copytree(ROOT/'synthetic-fixtures',work/'fixtures')
    changes={work/'test_inventory.py':{
      '/workspace/shared/ecl-vm-reference-review/verifier/test_op82_baseline_order.py':str(work/'fixtures/test_op82_baseline_order.py')},
      work/'fixtures/test_op82_baseline_order.py':{
      '/workspace/shared/ecl-run-baseline-plan-v2/test_owner.py':str(work/'fixtures/test_owner.py')}}
    for path,replacements in changes.items():
        source=path.read_text()
        for before,after in replacements.items():
            if source.count(before)!=1:
                raise SystemExit("Fixture path substitution validation failed.")
            source=source.replace(before,after)
        path.write_text(source)
    env=dict(os.environ,PYTHONDONTWRITEBYTECODE='1')
    for filename in ('test_inventory.py','test_localization.py'):
        result=subprocess.run([sys.executable,str(work/filename)],cwd=work,env=env,
          stdout=subprocess.PIPE,stderr=subprocess.STDOUT,text=True,timeout=60)
        print(filename+':\n'+'\n'.join(result.stdout.splitlines()[-5:]))
        if result.returncode:raise SystemExit(result.returncode)
