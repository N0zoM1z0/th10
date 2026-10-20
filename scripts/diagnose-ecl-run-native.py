#!/usr/bin/env python3
"""Compare the complete ECL Run owner without granting canonical exact credit.

Build the selected canonical context first. This command inspects that artifact;
it does not assert that an existing artifact was compiled from current source.
Exit 0 means complete equality, 1 means an ordinary mismatch, and 2 means an
unsupported or unbound comparison. All results remain noncanonical diagnostics.
"""

from __future__ import annotations

import argparse
import importlib.util
import json
from pathlib import Path
import sys
import tempfile
import tomllib

from linked_image import PEImage, linked_functions, map_publics


ROOT = Path(__file__).resolve().parents[1]
RUN_SYMBOL = "?Run@EclVmContext@@QAEHM@Z"
RUN_ADDRESS = 0x0044E1A0
RUN_SIZE = 7020


def module(name: str, path: str):
    spec = importlib.util.spec_from_file_location(name, ROOT / path)
    result = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(result)
    return result


def toml_value(value):
    if isinstance(value, dict):
        return "{ " + ", ".join(
            json.dumps(key) + " = " + toml_value(item)
            for key, item in value.items()
        ) + " }"
    if isinstance(value, list):
        return "[" + ", ".join(map(toml_value, value)) + "]"
    return json.dumps(value)


def compare(args):
    manifest = tomllib.loads((ROOT / "config/match-units.toml").read_text())
    units = manifest["units"]
    context = units[args.unit]
    if context["artifact_kind"] != "linked-pe":
        raise ValueError("the context must be a declared linked-PE unit")
    if "src/EclVm.cpp" not in [context["source"], *context.get("support_sources", [])]:
        raise ValueError("the context does not compile the maintained ECL source")
    directory = (ROOT / (args.artifact_dir or context["output_dir"])).resolve()
    directory.relative_to(ROOT / "build")
    image_path = directory / "source.exe"
    linked = linked_functions(image_path, directory / "source.map", directory / "source.pdb")
    owners = [row for row in linked["functions"] if row["symbol"] == RUN_SYMBOL]
    if len(owners) != 1 or int(owners[0]["size"]) != RUN_SIZE:
        raise ValueError("Run must have one complete 7020-byte PDB extent")

    comparator = module("ecl_run_compare", "scripts/compare-linked-function.py")
    probe = module("ecl_run_probe", "scripts/probe-ltcg-backlog.py")
    target = comparator.verified_target()
    structural = probe.structural_compare(
        PEImage(image_path), map_publics(directory / "source.map"),
        int(owners[0]["address"], 0), RUN_SIZE, target, RUN_ADDRESS, RUN_SIZE,
    )

    def contextual(template):
        unit = dict(template)
        for key in (
            "source", "support_sources", "normal_support_sources", "support_profiles",
            "profile", "link_profile", "harness", "entry_symbol",
        ):
            if key in context:
                unit[key] = context[key]
            else:
                unit.pop(key, None)
        unit.update(output_dir=str(directory.relative_to(ROOT)), pdb_source="src/EclVm.cpp")
        return unit

    guard = contextual(units["ecl-vm-read-float-value"])
    run = contextual(units["ecl-vm-host-run"])
    known = set()
    for unit in units.values():
        known.add((unit["symbol"], int(unit["target_address"])))
        for field in unit.get("linkages", []):
            if "symbol" in field:
                known.add((field["symbol"], int(field["target"])))

    fields = []
    for field in structural["linkage_fields"]:
        address = int(field["target_candidate"], 0)
        row = {"offset": field["offset"], "type": field["type"], "target": address}
        if RUN_ADDRESS <= address < RUN_ADDRESS + RUN_SIZE:
            if field.get("candidate_symbols"):
                raise ValueError("local field unexpectedly names an external owner")
            row["local"] = True
        else:
            choices = {
                name for name in field.get("candidate_symbols", [])
                if (name, address) in known
            }
            if len(choices) != 1:
                raise ValueError(f"external field lacks one prebound target: {field}")
            row["symbol"] = choices.pop()
        fields.append(row)
    if len(fields) != 189 or sum(bool(row.get("local")) for row in fields) != 62:
        raise ValueError("the reviewed 189-field/62-local layout has changed")
    run.update(
        symbol=RUN_SYMBOL, functions=["EclVmContext::Run"],
        target_address=RUN_ADDRESS, size=RUN_SIZE, linkages=fields,
        # Only the final selector byte is not consumed by linear decoding.
        # Declaring it as data still compares it; nothing is masked or omitted.
        data_spans=[{"offset": 7019, "width": 1}],
    )
    analysis = ROOT / ".analysis"
    analysis.mkdir(exist_ok=True)
    with tempfile.TemporaryDirectory(prefix="ecl-run-native-", dir=analysis) as temporary:
        path = Path(temporary) / "units.toml"
        path.write_text(
            "target_sha256 = " + toml_value(manifest["target_sha256"]) + "\n"
            + "".join(
                "[units." + name + "]\n" + "".join(
                    key + " = " + toml_value(value) + "\n" for key, value in unit.items()
                ) for name, unit in [("float-value-guard", guard), ("run-diagnostic", run)]
            )
        )
        comparator.UNITS_MANIFEST = path
        guarded = comparator.compare_unit("float-value-guard")
        if guarded["result"] != "exact":
            raise ValueError("the complete same-image float-value callee is not exact")
        result = comparator.compare_unit("run-diagnostic")
    return {
        "scope": "Noncanonical complete-owner diagnostic; no exact ledger credit.",
        "context_unit": args.unit,
        "callee_guard": guarded,
        "run": result,
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--unit", default="ecl-vm-host-run")
    parser.add_argument("--artifact-dir", help="existing artifact under build/ with the same graph")
    args = parser.parse_args()
    try:
        result = compare(args)
    except (OSError, KeyError, TypeError, ValueError, tomllib.TOMLDecodeError) as exc:
        print(json.dumps({"result": "error", "error": str(exc)}, indent=2))
        return 2
    print(json.dumps(result, indent=2))
    return 0 if result["run"]["result"] == "exact" else 1


if __name__ == "__main__":
    sys.exit(main())
