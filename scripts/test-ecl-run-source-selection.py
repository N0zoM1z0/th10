#!/usr/bin/env python3
"""Guard explicit ECL diagnostic source routing without target artifacts."""
from __future__ import annotations

import importlib.util
from pathlib import Path
import tempfile


ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    "ecl_source_diagnostic", ROOT / "scripts/diagnose-ecl-run-native.py"
)
diagnostic = importlib.util.module_from_spec(spec)
spec.loader.exec_module(diagnostic)


def rejected(context, source=None):
    try:
        diagnostic.select_ecl_source(context, source)
    except ValueError:
        return
    raise AssertionError("invalid source route accepted")


def main():
    with tempfile.TemporaryDirectory(prefix="ecl-source-routing-") as temporary:
        root = Path(temporary)
        diagnostic.ROOT = root
        canonical = root / "src/EclVm.cpp"
        private = root / ".analysis/probe/EclVm.cpp"
        for path in [canonical, private]:
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text("// Input routing fixture; not compiled.\n")
        context = {"source": "src/EclVm.cpp", "support_sources": []}
        assert diagnostic.select_ecl_source(context) == (canonical, "src/EclVm.cpp")
        private_context = {
            "source": "src/EclVm.cpp", "support_sources": [str(private)]
        }
        assert diagnostic.select_ecl_source(private_context, private) == (
            private, ".analysis/probe/EclVm.cpp"
        )
        assert diagnostic.select_ecl_source(
            private_context, ".analysis/probe/../probe/EclVm.cpp"
        )[0] == private
        rejected(context, private)
        rejected(private_context, ".analysis/missing.cpp")
        rejected(context, "src")
        rejected({"source": str(private)}, canonical)
        rejected({"source": "src/EclVm.cpp", "support_sources": [str(canonical)]})
        alias = root / "source-alias.cpp"
        alias.symlink_to(canonical)
        rejected({"source": str(canonical), "support_sources": [str(alias)]})
        outside = root.parent / (root.name + "-outside.cpp")
        outside.write_text("// Outside routing fixture.\n")
        try:
            rejected({"source": str(outside)}, outside)
            escaping = root / "escaping.cpp"
            escaping.symlink_to(outside)
            rejected({"source": str(escaping)}, escaping)
        finally:
            outside.unlink()
    print("ECL source routing: 3 valid routes and 8 rejection guards pass")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
