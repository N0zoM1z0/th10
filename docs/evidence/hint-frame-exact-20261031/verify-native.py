#!/usr/bin/env python3
"""Reproduce the complete exact Hint frame owner and its real callback."""

import argparse
import copy
import hashlib
import json
from pathlib import Path
import runpy
import sys
import tomllib

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / "scripts"))
from linked_image import PEImage, linked_code_fields, linked_functions, map_publics, verify_capstone

PREVIOUS = ROOT / "docs/evidence/hint-frame-values-20261031"
REVIEW = runpy.run_path(str(PREVIOUS / "verify-native.py"))


def observations(directory, proof):
    assert hashlib.sha256((PREVIOUS / "results.json").read_bytes()).hexdigest() == proof["previous_proof_sha256"]
    verify_capstone(tomllib.loads((ROOT / "config/tools.lock.toml").read_text())["capstone"])
    target = PEImage(ROOT / "resources/th10.exe")
    assert target.sha256 == proof["target_sha256"]
    image = PEImage(directory / "source.exe")
    functions = linked_functions(directory / "source.exe", directory / "source.map",
                                 directory / "source.pdb")["functions"]
    publics = map_publics(directory / "source.map")
    names = {p["symbol"]: p["address"] for p in publics}
    symbol, target_address, size = "?Update@HintOwnerView@@QAEHXZ", 0x419650, 571
    owners = [f for f in functions if f["symbol"] == symbol]
    assert len(owners) == 1
    owner = owners[0]
    assert owner["map_object"] == "source.ltcg.obj" and owner["size"] == size
    assert owner["extent_source"] == "pdb-section-contribution"
    address = int(owner["address"], 0)
    decode, geometry = REVIEW["decode"], REVIEW["geometry"]
    ti, ni = decode(target, target_address, size), decode(image, address, size)
    tg, ng = geometry(ti, target_address), geometry(ni, address)
    assert tg == ng and tg["instructions"] == 169 and len(tg["branches"]) == 23
    assert tg["returns"] == [dict(offset=570, operand="")]
    tf = REVIEW["enumerate_target_fields"](target, ti)
    roles = proof["target_field_roles"]
    nf = linked_code_fields(image, publics, address, size)
    assert nf["normalization_complete"] and len(tf) == len(nf["fields"]) == len(roles) == 17
    tc, nc = bytearray(target.read_address(target_address, size)), bytearray(image.read_address(address, size))
    bindings = []
    for t, n, r in zip(tf, nf["fields"], roles):
        assert t == {k: r[k] for k in ("offset", "width", "type", "target")}
        assert n["offset"] == t["offset"] and n["width"] == 4 and n["type"] == t["type"]
        assert n["candidate_target"] == names[r["symbol"]]
        binding = {**r, "native_offset": n["offset"]}
        if r["symbol"].startswith("__real@"):
            payload = target.read_address(r["target"], 4)
            assert image.read_address(names[r["symbol"]], 4) == payload
            binding["payload_hex"] = payload.hex()
        tc[t["offset"]:t["offset"] + 4] = bytes(4)
        nc[n["offset"]:n["offset"] + 4] = bytes(4)
        bindings.append(binding)
    # No local branch or ordinary instruction byte is normalized.
    assert tc == nc
    consume_symbol = "?ConsumeEntries@HintOwnerView@@QAEHPAUHintLinkView@@H@Z"
    callees = [f for f in functions if f["symbol"] == consume_symbol
               and int(f["address"], 0) == names[consume_symbol]]
    assert len(callees) == 1 and callees[0]["size"] == 1308
    assert callees[0]["map_object"] == "source.ltcg.obj"
    assert callees[0]["extent_source"] == "pdb-section-contribution"
    manifest = tomllib.loads((ROOT / "config/match-units.toml").read_text())
    results = []
    compare = runpy.run_path(str(ROOT / "scripts/compare-linked-function.py"))["compare_unit"]
    for name in ("hint-frame-update", "hint-update-callback"):
        unit = copy.deepcopy(proof["units"][name])
        unit["output_dir"] = str(directory.relative_to(ROOT))
        manifest["units"][name] = unit
        result = compare(name, _unit_manifest=manifest)
        assert result["result"] == "exact" and not result["first_differences"]
        results.append(dict(unit=name, size=result["size"], fields=len(result["linkages"])))
    return dict(result="complete-frame-and-callback-exact", geometry=tg,
                normalized_sha256=hashlib.sha256(tc).hexdigest(), field_bindings=bindings,
                actual_consume_callee=dict(symbol=consume_symbol, native_size=1308,
                                          target_size=1317, result="nonexact"),
                complete_comparisons=results, new_authored_exact_bytes=571,
                existing_callback_exact_bytes=10)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--artifact-dir", required=True, type=Path)
    directory = parser.parse_args().artifact_dir.resolve()
    directory.relative_to(ROOT / "build")
    proof = json.loads(Path(__file__).with_name("results.json").read_text())
    actual = observations(directory, proof)
    assert actual == proof["native_reproduction"]
    print(json.dumps(dict(result=actual["result"], new_authored_exact_bytes=571,
                          maintained_callback_bytes=10, fields=17, local_destinations=23)))


if __name__ == "__main__":
    main()
