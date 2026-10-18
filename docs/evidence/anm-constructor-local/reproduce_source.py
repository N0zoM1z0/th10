"""Reconstruct the archived source hypothesis in memory; never compile or write files.

Both original source files, the exact mixed-EOL diff, and the MIT notice are pinned.
Passing this check establishes source identity only, not C++ layout, bit ordering,
native equality, allocation/lifetime safety, or runtime equivalence.
"""
import argparse
import difflib
import hashlib
import json
import os
from pathlib import Path

FILES = ("AnmManager.cpp", "AnmManager.hpp")
BASE_SHA = {
    "AnmManager.cpp": "26bf39d6a7446311f373fd972477207c8ef950d54c59c0b68f85f0ccc64669ab",
    "AnmManager.hpp": "961a01f8267f9974838aea4d4102a296548ced89dbc0415ebc471af8f5a549c9",
}
CANDIDATE_SHA = {
    "AnmManager.cpp": "67bfc539c4660c479b6936e6a780125a84bfe02b6abe1c7e4e1d6a87f4e4019e",
    "AnmManager.hpp": "3aef68063bb6b6478e81a52b6fedd1f4e92f88bdde879bc6fed174d8b0ef8c68",
}
PATCH_SHA = "2b8622a2c463726833d5347ae71562b4e6c2aa24e53e6507d5affe70e1fb85e7"
LICENSE_SHA = "8dba0c16cce6c1ad871d978178a78d92f40ae44eb483443486e65bea5a1cc3db"
BASE_EOL = {"AnmManager.cpp": (6203, 0, 0), "AnmManager.hpp": (0, 1357, 0)}
CANDIDATE_EOL = {"AnmManager.cpp": (6236, 0, 0), "AnmManager.hpp": (0, 1379, 0)}
PATCH_EOL = (144, 45, 0)
MAX_SOURCE_BYTES = 4 * 1024 * 1024
CTOR = b"AnmRenderManagerView::AnmRenderManagerView()\r\n"
CTOR_END = b"\r\n// Target 0x004462F0"

OLD_RECORD = """struct AnmChainElementView
{
    void *unknown000;
    unsigned int flags;
    unsigned char unknown008[0x018];
    void *argument;
};""".encode("utf-8")

NEW_RECORD = """// Task layout adapted from levzzz5154/th10-decomp, commit
// 4e4ab0b73eb75f1d3c2e49b39330deabd5fdcf4a. See ANM_TASK_CREATION_LICENSE.txt.
// Low flag assignments are a source-family hypothesis, not target-proven.
struct AnmChainElementView
{
    int priority;
    unsigned int heapOwned : 1;
    unsigned int registrationFlag : 1;
    unsigned int reservedFlags : 30;
    AnmChainCallback callback;
    AnmChainCallback addedCallback;
    AnmChainCallback removedCallback;
    AnmChainElementView *self;
    AnmChainElementView *next;
    AnmChainElementView *previous;
    void *argument;
};

typedef char AnmChainElementSize24[
    (sizeof(AnmChainElementView) == 0x24) ? 1 : -1];
typedef char AnmChainElementFieldOffsets[
    (offsetof(AnmChainElementView, priority) == 0x00 &&
     offsetof(AnmChainElementView, callback) == 0x08 &&
     offsetof(AnmChainElementView, addedCallback) == 0x0c &&
     offsetof(AnmChainElementView, removedCallback) == 0x10 &&
     offsetof(AnmChainElementView, self) == 0x14 &&
     offsetof(AnmChainElementView, next) == 0x18 &&
     offsetof(AnmChainElementView, previous) == 0x1c &&
     offsetof(AnmChainElementView, argument) == 0x20) ? 1 : -1];""".encode("utf-8")

TASK_HELPER = """// Task creation adapted from levzzz5154/th10-decomp, commit
// 4e4ab0b73eb75f1d3c2e49b39330deabd5fdcf4a. See ANM_TASK_CREATION_LICENSE.txt.
// Do not read or initialize reservedFlags. Its representation is unspecified.
namespace
{
struct AnmRenderTask : AnmChainElementView
{
    AnmRenderTask()
    {
        priority = 0;
        heapOwned = 0;
        callback = NULL;
        addedCallback = NULL;
        removedCallback = NULL;
        self = this;
        next = NULL;
        previous = NULL;
    }
};

typedef char AnmRenderTaskSize24[
    (sizeof(AnmRenderTask) == 0x24) ? 1 : -1];
}

static AnmChainElementView *CreateAnmRenderTask(AnmChainCallback callback)
{
    AnmChainElementView *element = new AnmRenderTask;
    element->callback = callback;
    element->heapOwned = 1;
    element->registrationFlag = 1;
    return element;
}

""".replace("\n", "\r\n").encode("utf-8")


def need(value):
    if not value:
        raise ValueError("source-reconstruction-gate")


def sha(raw):
    return hashlib.sha256(raw).hexdigest()


def eol_profile(raw):
    """Count CRLF, bare LF, bare CR without newline normalization."""
    crlf = raw.count(b"\r\n")
    return crlf, raw.count(b"\n") - crlf, raw.count(b"\r") - crlf


def check_sources(sources, pins, eols):
    need(type(sources) is dict and set(sources) == set(FILES))
    for name in FILES:
        need(type(sources[name]) is bytes)
        need(sha(sources[name]) == pins[name])
        need(eol_profile(sources[name]) == eols[name])


def reconstruct(baseline):
    """Return fresh candidate byte values, leaving the supplied baseline unchanged."""
    check_sources(baseline, BASE_SHA, BASE_EOL)
    cpp, hpp = (baseline[name] for name in FILES)
    need(hpp.count(OLD_RECORD) == 1)
    need(cpp.count(CTOR) == 1)
    start = cpp.index(CTOR)
    need(cpp.count(CTOR_END, start) == 1)
    end = cpp.index(CTOR_END, start)
    body = cpp[start:end]
    need(body.count(b"AnmCreateChainElement(") == 20)
    candidate = {
        "AnmManager.cpp": (cpp[:start] + TASK_HELPER +
                           body.replace(b"AnmCreateChainElement(", b"CreateAnmRenderTask(") +
                           cpp[end:]),
        "AnmManager.hpp": hpp.replace(OLD_RECORD, NEW_RECORD),
    }
    check_sources(candidate, CANDIDATE_SHA, CANDIDATE_EOL)
    return candidate


def patch_bytes(baseline, candidate):
    """Reproduce the original ordered two-file unified diff including raw EOLs."""
    check_sources(baseline, BASE_SHA, BASE_EOL)
    check_sources(candidate, CANDIDATE_SHA, CANDIDATE_EOL)
    patch = ""
    for name in FILES:
        patch += "".join(difflib.unified_diff(
            baseline[name].decode("utf-8").splitlines(True),
            candidate[name].decode("utf-8").splitlines(True),
            fromfile="a/src/" + name, tofile="b/src/" + name))
    return patch.encode("utf-8")


def verify_patch(baseline, candidate, patch):
    need(type(patch) is bytes and sha(patch) == PATCH_SHA)
    need(eol_profile(patch) == PATCH_EOL)
    need(patch_bytes(baseline, candidate) == patch)


def read_regular(path, limit=MAX_SOURCE_BYTES):
    """Read a bounded regular source file, rejecting symlinked path components."""
    path = Path(os.path.abspath(path))
    need(all(not parent.is_symlink() for parent in (path, *path.parents)))
    need(path.is_file() and path.stat().st_size <= limit)
    with path.open("rb") as stream:
        raw = stream.read(limit + 1)
    need(len(raw) <= limit)
    return raw


def read_sources(source_root):
    return {name: read_regular(Path(source_root) / name) for name in FILES}


def verify(source_root):
    baseline = read_sources(source_root)
    candidate = reconstruct(baseline)
    archive = Path(__file__).parent
    verify_patch(baseline, candidate, read_regular(archive / "candidate.diff"))
    need(sha(read_regular(archive / "ANM_TASK_CREATION_LICENSE.txt")) == LICENSE_SHA)
    return {
        "source_verified": True,
        "baseline_sha256": dict(BASE_SHA),
        "candidate_sha256": dict(CANDIDATE_SHA),
        "patch_sha256": PATCH_SHA,
        "license_sha256": LICENSE_SHA,
        "source_file_count": 2,
        "source_creation_expression_count": 20,
        "native_executed": False,
        "target_read": False,
        "production_changed": False,
        "exactness_verified": False,
        "runtime_equivalence_verified": False,
    }


def source_argument(parser):
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument("--repo", type=Path, help="Repository root containing src/")
    group.add_argument("--source-root", type=Path, help="Directory containing the two source files")


def select_source(args):
    return args.repo / "src" if args.repo is not None else args.source_root


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    source_argument(parser)
    args = parser.parse_args(argv)
    try:
        result = verify(select_source(args))
    except (OSError, ValueError, UnicodeError):
        print(json.dumps({"source_verified": False, "error": "source-verification-failed"}))
        return 1
    print(json.dumps(result, sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
