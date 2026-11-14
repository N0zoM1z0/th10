#!/usr/bin/env python3
"""Triage only, never exact acceptance: rebuild one unit's linked image while
recompiling only the named --dirty sources and reusing every other existing
object in the unit's output directory. Final acceptance still needs the cold
`scripts/replay-exact-units.py --unit UNIT` run.

usage: scripts/repo-python scripts/diagnostics/triage/quick-relink.py UNIT [--dirty src/A.cpp ...]
"""
import pathlib, sys
root = pathlib.Path(__file__).resolve().parents[3]
sys.path.insert(0, str(root / 'scripts'))
import ltcg_link

argv = sys.argv[1:]
unit = argv[0]
dirty = {(root / p).resolve() for p in argv[argv.index('--dirty') + 1:]} if '--dirty' in argv else set()
real_compile = ltcg_link.compile_source

def compile_source(source, output, profile):
    if output.is_file() and source.resolve() not in dirty and output.name != 'link-anchors.obj':
        print(f'reuse {output.name}', flush=True)
        return
    print(f'compile {source.name}', flush=True)
    real_compile(source, output, profile)

ltcg_link.compile_source = compile_source
sys.argv = [str(root / 'scripts/build-match-unit.py'), '--unit', unit]
import runpy
runpy.run_path(str(root / 'scripts/build-match-unit.py'), run_name='__main__')
