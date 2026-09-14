#!/usr/bin/env python3
"""List linker-map functions with extents from an MSF 7/PDB DBI stream."""

from __future__ import annotations

import argparse
import json
from pathlib import Path
import struct
import sys

from linked_image import LinkedImageError, linked_functions
from msvc_symbols import source_name_hint


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("image", type=Path)
    parser.add_argument("map", type=Path)
    parser.add_argument("pdb", type=Path)
    parser.add_argument("--contains", default="")
    parser.add_argument("--object", default="")
    parser.add_argument("--json", action="store_true")
    args = parser.parse_args()
    try:
        report = linked_functions(args.image, args.map, args.pdb)
        needle = args.contains.lower()
        object_needle = args.object.lower()
        functions = []
        for function in report["functions"]:
            if needle and needle not in str(function["symbol"]).lower():
                continue
            owners = (str(function["map_object"]), str(function["pdb_object"]))
            if object_needle and not any(object_needle in owner.lower() for owner in owners):
                continue
            functions.append(
                {**function, "source_name_hint": source_name_hint(str(function["symbol"]))}
            )
        report["functions"] = functions
        report["function_count"] = len(functions)
        report.update(
            {
                "schema_version": 1,
                "result": "ok",
                "acceptance_authority": "none",
            }
        )
    except (OSError, LinkedImageError, struct.error) as exc:
        report = {"result": "error", "error": str(exc)}
    if args.json:
        print(json.dumps(report, indent=2, sort_keys=True))
    elif report["result"] == "error":
        print(f"error: linked-function inspection failed: {report['error']}", file=sys.stderr)
    else:
        for function in report["functions"]:
            print(
                f"{function['address']} {function['size']:5d} "
                f"{function['symbol']} [{function['pdb_object']}]"
            )
        print(
            f"linked function inventory: {report['function_count']} public(s); "
            f"{report['unowned_code_contribution_count']} contribution(s) lack a public"
        )
        print("linked inventory is diagnostic and grants no exactness credit")
    return 0 if report["result"] == "ok" else 1


if __name__ == "__main__":
    raise SystemExit(main())
