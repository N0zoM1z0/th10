#!/usr/bin/env python3
"""Compare the complete ECL Run owner without granting canonical exact credit.

Build the selected declared context first. This command inspects that artifact;
it does not assert that an existing artifact was compiled from current source.
An explicit context manifest can describe an isolated noncanonical build graph;
target bindings and callee templates always come from the canonical manifest.
Exit 0 means complete equality, 1 means an ordinary mismatch, and 2 means an
unsupported or unbound comparison. All results remain noncanonical diagnostics.
"""

from __future__ import annotations

import argparse
import hashlib
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
RUN_CODE_SIZE = 6692
RUN_TABLE_ENTRIES = 60
RUN_SELECTOR_OFFSET = RUN_CODE_SIZE + RUN_TABLE_ENTRIES * 4


def run_data_spans(image, structural):
    """Annotate only undecoded selector bytes after validating both tables."""
    base = int(structural["candidate_address"], 0)
    fields = structural["linkage_fields"]
    for relative in (RUN_CODE_SIZE, RUN_SELECTOR_OFFSET):
        references = [field for field in fields
                      if field["type"] == "DIR32" and field["width"] == 4
                      and field["offset"] < RUN_CODE_SIZE
                      and int(field["candidate_target"], 0) == base + relative
                      and int(field["target_candidate"], 0) == RUN_ADDRESS + relative]
        if len(references) != 1:
            raise ValueError("Run lacks the reviewed unique dispatch-table reference")
    table = [field for field in fields
             if RUN_CODE_SIZE <= field["offset"] < RUN_SELECTOR_OFFSET]
    if (sorted(field["offset"] for field in table)
            != list(range(RUN_CODE_SIZE, RUN_SELECTOR_OFFSET, 4))):
        raise ValueError("Run lacks the complete reviewed 60-entry jump table")
    for field in table:
        if (field["type"] != "DIR32" or field["width"] != 4
                or not base <= int(field["candidate_target"], 0) < base + RUN_CODE_SIZE
                or not RUN_ADDRESS <= int(field["target_candidate"], 0)
                < RUN_ADDRESS + RUN_CODE_SIZE):
            raise ValueError("Run jump-table entry leaves its executable body")
    if any(field["offset"] >= RUN_SELECTOR_OFFSET for field in fields):
        raise ValueError("Run selector data unexpectedly contains a link field")
    selector = image.read_address(base + RUN_SELECTOR_OFFSET, RUN_SIZE - RUN_SELECTOR_OFFSET)
    if len(selector) != 88 or any(value >= RUN_TABLE_ENTRIES for value in selector):
        raise ValueError("Run selector data leaves the reviewed jump-table domain")
    decoded = int(structural["decoded_candidate_bytes"])
    if not RUN_CODE_SIZE <= decoded <= RUN_SIZE:
        raise ValueError("Run decoding stops before the reviewed executable body")
    if structural["normalization_complete"]:
        if decoded != RUN_SIZE:
            raise ValueError("Run selector coverage is inconsistent with decoding")
        return []
    start = max(decoded, RUN_SELECTOR_OFFSET)
    if start == RUN_SIZE:
        raise ValueError("Run incomplete decoding has no selector suffix")
    return [{"offset": start, "width": RUN_SIZE - start}]


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


def select_ecl_source(context, requested=None):
    """Select one existing repository-local input, including private probes."""
    source = (ROOT / (requested or "src/EclVm.cpp")).resolve()
    try:
        relative = source.relative_to(ROOT.resolve()).as_posix()
    except ValueError as exc:
        raise ValueError("ECL source must stay inside the repository") from exc
    if not source.is_file():
        raise ValueError("ECL source must be an existing file")
    inputs = [context["source"], *context.get("support_sources", [])]
    if sum((ROOT / value).resolve() == source for value in inputs) != 1:
        raise ValueError("context must compile the selected ECL source exactly once")
    return source, relative


def compare(args):
    canonical_path = ROOT / "config/match-units.toml"
    canonical_bytes = canonical_path.read_bytes()
    canonical = tomllib.loads(canonical_bytes.decode("utf-8"))
    manifest_path = Path(
        getattr(args, "context_manifest", None) or canonical_path
    ).resolve()
    manifest_bytes = manifest_path.read_bytes()
    manifest = tomllib.loads(manifest_bytes.decode("utf-8"))
    if manifest.get("target_sha256") != canonical["target_sha256"]:
        raise ValueError("context manifest target hash differs from canonical bindings")
    units = manifest["units"]
    context = units[args.unit]
    if context["artifact_kind"] != "linked-pe":
        raise ValueError("the context must be a declared linked-PE unit")
    source_path, source_selector = select_ecl_source(
        context, getattr(args, "ecl_source", None)
    )
    source_bytes = source_path.read_bytes()
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
        unit.update(output_dir=str(directory.relative_to(ROOT)), pdb_source=source_selector)
        return unit

    guard = contextual(canonical["units"]["ecl-vm-read-float-value"])
    run = contextual(canonical["units"]["ecl-vm-host-run"])
    known = set()
    for unit in canonical["units"].values():
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
        # Link addresses change how far linear decoding enters owned table data.
        # PE fields cover the jump table; this annotates the remaining selector
        # suffix. Every byte is still compared, including all 88 selectors.
        data_spans=run_data_spans(PEImage(image_path), structural),
    )
    analysis = ROOT / ".analysis"
    analysis.mkdir(exist_ok=True)
    with tempfile.TemporaryDirectory(prefix="ecl-run-native-", dir=analysis) as temporary:
        path = Path(temporary) / "units.toml"
        path.write_text(
            "target_sha256 = " + toml_value(canonical["target_sha256"]) + "\n"
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
    if manifest_path.read_bytes() != manifest_bytes:
        raise ValueError("context manifest changed during comparison")
    if canonical_path.read_bytes() != canonical_bytes:
        raise ValueError("canonical bindings changed during comparison")
    if source_path.read_bytes() != source_bytes:
        raise ValueError("selected ECL source changed during comparison")
    return {
        "scope": "Noncanonical complete-owner diagnostic; no exact ledger credit.",
        "context_unit": args.unit,
        "context_manifest": str(manifest_path.relative_to(ROOT))
        if manifest_path.is_relative_to(ROOT) else str(manifest_path),
        "context_manifest_sha256": hashlib.sha256(manifest_bytes).hexdigest(),
        "canonical_bindings_sha256": hashlib.sha256(canonical_bytes).hexdigest(),
        "observed_ecl_source": {
            "path": source_selector,
            "sha256": hashlib.sha256(source_bytes).hexdigest(),
            "scope": "Input observed during diagnosis; artifact/source correspondence is not asserted.",
        },
        "callee_guard": guarded,
        "run": result,
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--unit", default="ecl-vm-host-run")
    parser.add_argument("--artifact-dir", help="existing artifact under build/ with the same graph")
    parser.add_argument(
        "--context-manifest", type=Path,
        help="declared build context (target bindings remain canonical)",
    )
    parser.add_argument(
        "--ecl-source", type=Path,
        help="explicit ECL input in the selected context (default: src/EclVm.cpp)",
    )
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
