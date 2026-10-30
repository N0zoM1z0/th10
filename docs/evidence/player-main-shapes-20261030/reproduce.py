#!/usr/bin/env python3
"""Cold-build the retained nonexact Player graph; no match admission."""
from __future__ import annotations

import argparse
import json
from pathlib import Path
import runpy
import sys

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / 'scripts'))
from ltcg_link import cold_link, sha256, tool_environment


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output-dir', type=Path, required=True)
    parser.add_argument('--prepare-only', action='store_true')
    args = parser.parse_args()
    directory = args.output_dir.resolve()
    assert directory.is_relative_to(ROOT / 'build') and not directory.exists()
    record = json.loads(Path(__file__).with_name('results.json').read_text())
    context = record['final_cold_context']
    graph = context['graph']
    runpy.run_path(str(ROOT / 'scripts/compare-linked-function.py'))['verified_target']()
    for relative, digest in context['pins'].items():
        assert sha256(ROOT / relative) == digest, relative
    linker, environment = tool_environment()
    if args.prepare_only:
        print(json.dumps({'result': 'source-pins-and-target-verified',
                          'inputs': 1 + len(graph['support_sources']),
                          'graph': graph}, indent=2))
        return
    result = cold_link(
        ROOT / graph['source'], directory, graph['entry_symbol'], linker, environment,
        profile=graph['profile'],
        support_sources=[ROOT / f for f in graph['support_sources']],
        support_profiles=graph['support_profiles'],
    )
    print(json.dumps({k: str(result[k]) for k in ['image', 'map', 'pdb']}, indent=2))
    print('Nonexact diagnostic artifact; no target-match or runtime claim.')


if __name__ == '__main__':
    main()
