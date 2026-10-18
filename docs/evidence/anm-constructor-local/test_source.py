"""Source invariants and a defined-field assignment model; no C++ or native work.

Importing this module parses no arguments and reads no archive/source files.
The model omits reserved storage entirely. It does not model containing-word
read/modify/write, allocation failure, callback ABI, registration, or lifetime.
"""
import argparse
import hashlib
import io
from pathlib import Path
import re
import types
import unittest
from unittest import mock

REPRODUCER_SHA = "fe3166297336cb310047508345fd47ee70ec3096bf344557bb43e072bead1914"

CONSTRUCTOR_ASSIGNMENTS = (
    ("priority", "0"), ("heapOwned", "0"), ("callback", "NULL"),
    ("addedCallback", "NULL"), ("removedCallback", "NULL"),
    ("self", "this"), ("next", "NULL"), ("previous", "NULL"),
)
HELPER_ASSIGNMENTS = (
    ("callback", "callback"), ("heapOwned", "1"), ("registrationFlag", "1"),
)
CALLBACKS = ("OnUpdatePrimary", "OnUpdateSecondary", *(
    "DrawLayer" + str(i) for i in range(17)), "DrawLayer19")


def defined_model(need, constructor_assignments, helper_assignments, callback):
    """Only the reviewed named-field assignments, never a reserved-bit value."""
    need(tuple(constructor_assignments) == CONSTRUCTOR_ASSIGNMENTS)
    need(tuple(helper_assignments) == HELPER_ASSIGNMENTS)
    obj = {}
    values = {"0": 0, "1": 1, "NULL": None, "callback": callback, "this": obj}
    for field, value in constructor_assignments:
        obj[field] = values[value]
    need(obj["heapOwned"] == 0 and obj["callback"] is None)
    need(set(obj) == {name for name, _ in CONSTRUCTOR_ASSIGNMENTS})
    for field, value in helper_assignments:
        obj[field] = values[value]
    return obj


def make_tests(r, source_root):
    class SourceTests(unittest.TestCase):
        @classmethod
        def setUpClass(cls):
            cls.base = r.read_sources(source_root)
            cls.new = r.reconstruct(cls.base)
            cls.cpp = cls.new["AnmManager.cpp"].decode("utf-8")
            cls.hpp = cls.new["AnmManager.hpp"].decode("utf-8")
            cls.start = cls.cpp.index("// Task creation adapted from")
            cls.ctor = cls.cpp.index(r.CTOR.decode("utf-8"))
            cls.end = cls.cpp.index(r.CTOR_END.decode("utf-8"), cls.ctor)
            cls.helper = cls.cpp[cls.start:cls.ctor]
            cls.body = cls.cpp[cls.ctor:cls.end]
            constructor = cls.helper.split("AnmRenderTask()", 1)[1].split("};", 1)[0]
            cls.constructor_assignments = tuple(re.findall(
                r"^        (\w+) = (\w+);", constructor, re.M))
            cls.helper_assignments = tuple(re.findall(
                r"element->(\w+) = (\w+);", cls.helper))

        def reject(self, call):
            try:
                call()
            except ValueError:
                return
            r.need(False)

        def test_exact_reconstruction_and_eol(self):
            r.check_sources(self.base, r.BASE_SHA, r.BASE_EOL)
            r.check_sources(self.new, r.CANDIDATE_SHA, r.CANDIDATE_EOL)
            before = dict(self.base)
            r.need(r.reconstruct(self.base) == self.new and self.base == before)

        def test_exact_diff_and_license(self):
            archive = Path(r.__file__).parent
            patch = r.read_regular(archive / "candidate.diff")
            r.verify_patch(self.base, self.new, patch)
            r.need(r.eol_profile(patch) == (144, 45, 0))
            r.need(r.sha(r.read_regular(archive / "ANM_TASK_CREATION_LICENSE.txt")) == r.LICENSE_SHA)

        def test_inverse_cpp_preserves_every_other_byte(self):
            restored = (self.cpp[:self.start] + self.cpp[self.ctor:]).replace(
                "CreateAnmRenderTask(", "AnmCreateChainElement(")
            r.need(restored.encode("utf-8") == self.base["AnmManager.cpp"])
            r.need(self.cpp.count("CreateAnmRenderTask(") == 21)
            r.need(self.body.count("CreateAnmRenderTask(") == 20)

        def test_inverse_header_preserves_every_other_byte(self):
            r.need(self.new["AnmManager.hpp"].count(r.NEW_RECORD) == 1)
            r.need(self.new["AnmManager.hpp"].replace(r.NEW_RECORD, r.OLD_RECORD) ==
                   self.base["AnmManager.hpp"])

        def test_twenty_callbacks_context_and_registration_unchanged(self):
            callbacks = tuple(re.findall(
                r"CreateAnmRenderTask\(\r\n        reinterpret_cast<AnmChainCallback>\((\w+)\)\);",
                self.body))
            r.need(callbacks == CALLBACKS)
            old = self.base["AnmManager.cpp"].decode("utf-8")
            begin = old.index(r.CTOR.decode("utf-8"))
            end = old.index(r.CTOR_END.decode("utf-8"), begin)
            r.need(self.body.replace("CreateAnmRenderTask(", "AnmCreateChainElement(") == old[begin:end])
            r.need(self.body.count("element->argument = this;") == 20)
            r.need(self.body.count("AnmAddCalcChainElement(") == 2)
            r.need(self.body.count("AnmAddDrawChainElement(") == 18)

        def test_ordinary_constructor_and_passive_base(self):
            r.need(not re.search(r"__forceinline|__declspec|__stdcall|__fastcall|__asm|#pragma", self.helper))
            r.need("new AnmRenderTask;" in self.helper)
            r.need("namespace\r\n{" in self.helper)
            r.need("struct AnmRenderTask : AnmChainElementView" in self.helper)
            r.need("sizeof(AnmRenderTask) == 0x24" in self.helper)
            r.need("~AnmRenderTask" not in self.helper)
            record = r.NEW_RECORD.decode("utf-8")
            r.need(not re.search(r"~AnmChainElementView|AnmChainElementView\s*\(", record))

        def test_fixed_layout_assertion_presence_only(self):
            for field, offset in (("priority", "00"), ("callback", "08"),
                                  ("addedCallback", "0c"), ("removedCallback", "10"),
                                  ("self", "14"), ("next", "18"),
                                  ("previous", "1c"), ("argument", "20")):
                r.need(f"offsetof(AnmChainElementView, {field}) == 0x{offset}" in self.hpp)
            r.need("sizeof(AnmChainElementView) == 0x24" in self.hpp)
            r.need("unsigned int heapOwned : 1;" in self.hpp)
            r.need("unsigned int registrationFlag : 1;" in self.hpp)
            r.need("unsigned int reservedFlags : 30;" in self.hpp)

        def test_defined_field_model_for_twenty_callbacks(self):
            r.need(self.constructor_assignments == CONSTRUCTOR_ASSIGNMENTS)
            r.need(self.helper_assignments == HELPER_ASSIGNMENTS)
            for callback in CALLBACKS:
                obj = defined_model(r.need, self.constructor_assignments,
                                    self.helper_assignments, callback)
                r.need(obj["priority"] == 0 and obj["heapOwned"] == 1 and obj["registrationFlag"] == 1)
                r.need(obj["callback"] == callback and obj["self"] is obj)
                r.need(all(obj[field] is None for field in
                           ("addedCallback", "removedCallback", "next", "previous")))
                r.need(set(obj) == {name for name, _ in CONSTRUCTOR_ASSIGNMENTS + HELPER_ASSIGNMENTS})
                owner = object()
                obj["argument"] = owner
                r.need(obj["argument"] is owner)

        def test_wrong_source_and_schema_rejected(self):
            for name in r.FILES:
                changed = dict(self.base)
                changed[name] += b"\n"
                self.reject(lambda: r.reconstruct(changed))
            self.reject(lambda: r.reconstruct({}))
            changed = dict(self.base)
            changed["AnmManager.cpp"] = bytearray(changed["AnmManager.cpp"])
            self.reject(lambda: r.reconstruct(changed))
            self.reject(lambda: r.reconstruct(self.new))

        def test_mutated_candidate_and_patch_rejected(self):
            patch = r.patch_bytes(self.base, self.new)
            for changed in (patch + b"\n", patch.replace(b"\r\n", b"\n"), patch[:-1]):
                self.reject(lambda: r.verify_patch(self.base, self.new, changed))
            changed = dict(self.new)
            changed["AnmManager.hpp"] += b"\n"
            self.reject(lambda: r.patch_bytes(self.base, changed))

        def test_output_claims_are_source_only(self):
            result = r.verify(source_root)
            r.need(result["source_verified"] is True)
            r.need(result["source_file_count"] == 2 and result["source_creation_expression_count"] == 20)
            for field in ("native_executed", "target_read", "production_changed",
                          "exactness_verified", "runtime_equivalence_verified"):
                r.need(result[field] is False)

        def test_imports_have_no_argument_read_or_write_side_effects(self):
            for filename in (Path(r.__file__), Path(__file__)):
                code = compile(r.read_regular(filename), str(filename), "exec")
                module = types.ModuleType("source_import_probe")
                module.__file__ = str(filename)
                with mock.patch.object(Path, "open", side_effect=RuntimeError("unexpected-open")), \
                     mock.patch.object(argparse.ArgumentParser, "parse_args", side_effect=RuntimeError("unexpected-parse")), \
                     mock.patch("sys.stdout", new_callable=io.StringIO) as out:
                    exec(code, module.__dict__)
                    r.need(out.getvalue() == "")

    return SourceTests


def main(argv=None):
    path = Path(__file__).with_name("reproduce_source.py")
    if path.is_symlink() or not path.is_file():
        raise ValueError("source-test-loader-gate")
    with path.open("rb") as stream:
        raw = stream.read(65537)
    if len(raw) > 65536 or hashlib.sha256(raw).hexdigest() != REPRODUCER_SHA:
        raise ValueError("source-test-loader-gate")
    # Source execution avoids SourceFileLoader's optional archive .pyc writes.
    r = types.ModuleType("anm_constructor_reproducer")
    r.__file__ = str(path)
    exec(compile(raw, str(path), "exec"), r.__dict__)
    parser = argparse.ArgumentParser(description=__doc__)
    r.source_argument(parser)
    args = parser.parse_args(argv)
    suite = unittest.defaultTestLoader.loadTestsFromTestCase(make_tests(r, r.select_source(args)))
    return 0 if unittest.TextTestRunner(verbosity=2).run(suite).wasSuccessful() else 1


if __name__ == "__main__":
    raise SystemExit(main())
