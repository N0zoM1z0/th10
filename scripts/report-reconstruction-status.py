#!/usr/bin/env python3
"""Report TH10 state from ledgers rather than prose."""

from __future__ import annotations

import argparse
import json

from progress import measures


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--json", action="store_true")
    args = parser.parse_args()
    values = measures()
    report = {
        **values,
        "target": "th10-v1.00a-original-japanese",
        "current_stage": "boundary-and-origin-inventory",
        "whole_build_closed": False,
        "windows_i386_runtime_validated": False,
        "semantic_reconstruction_started": False,
        "port_started": False,
    }
    if args.json:
        print(json.dumps(report, indent=2))
    else:
        print(f"target:                    {report['target']}")
        print(f"stage:                     {report['current_stage']}")
        print(f"function candidates:       {values['functions']}")
        print(f"boundary reviewed/pending: {values['boundary_reviewed']} / {values['boundary_pending']}")
        print(f"  provisional / focused:   {values['boundary_provisional']} / {values['boundary_needs_review']}")
        print(f"origin reviewed/pending:   {values['origin_reviewed']} / {values['origin_pending']}")
        print(f"authored / excluded:       {values['authored']} / {values['excluded']}")
        print(f"origin indeterminate:      {values['indeterminate']}")
        print(f"source-present / exact:    {values['implemented']} / {values['matches']}")
        print(f"authored source backlog:   {values['authored_exact_backlog']}")
        print(f"mapped origin pending:     {values['mapped_review']}")
        print(f"mapped indeterminate:      {values['mapped_indeterminate']}")
        print("whole Windows i386 build:  open")
        print("semantic and port stages:  not started")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
