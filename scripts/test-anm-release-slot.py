#!/usr/bin/env python3
"""Actual ReleaseAnm body with explicit synthetic owner/dependency roles.

The replacement callback witnesses the target-observed slot reload. It does
not claim that the maintained Release normally replaces its manager slot.
Host layouts, Release and destructor observations are seams, not native proof.
"""
from pathlib import Path
import argparse
import os
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]
PREFIX = r'''
#include <climits>
#include <cstdio>
#include <vector>
struct AnmRenderManagerView;
static std::vector<int> events;
static bool deleted[4];
struct AnmLoadedView {
    int id, slot;
    AnmRenderManagerView *manager;
    AnmLoadedView *replacement;
    AnmLoadedView(int n, int s, AnmRenderManagerView *m)
        : id(n), slot(s), manager(m), replacement(0) {}
    void Release();
    ~AnmLoadedView() { events.push_back(200 + id); deleted[id] = true; }
};
// HOST: only the consumed slot-array role is represented, not target layout.
struct AnmRenderManagerView {
    AnmLoadedView *loadedAnms[33];
    void ReleaseAnm(int index);
};
void AnmLoadedView::Release() {
    events.push_back(100 + id);
    if (replacement) manager->loadedAnms[slot] = replacement;
}
'''
SUFFIX = r'''
int main() {
    AnmRenderManagerView manager = {};
    AnmLoadedView *sentinel = new AnmLoadedView(0, 0, &manager);
    manager.loadedAnms[0] = sentinel;
    const int invalid[] = {INT_MIN, -1, 33, INT_MAX};
    for (int index : invalid) manager.ReleaseAnm(index);
    manager.ReleaseAnm(32); // Valid empty upper slot.
    if (!events.empty() || manager.loadedAnms[0] != sentinel) {
        std::fprintf(stderr, "CHECK bounds-or-empty\n"); return 1;
    }
    delete sentinel;
    manager.loadedAnms[0] = 0;
    events.clear();
    manager.loadedAnms[32] = new AnmLoadedView(1, 32, &manager);
    manager.ReleaseAnm(32);
    if (manager.loadedAnms[32] || !deleted[1] ||
        events != std::vector<int>({101, 201})) {
        std::fprintf(stderr, "CHECK ordinary-release\n"); return 1;
    }
    events.clear();
    AnmLoadedView *first = new AnmLoadedView(2, 3, &manager);
    AnmLoadedView *second = new AnmLoadedView(3, 3, &manager);
    first->replacement = second;
    manager.loadedAnms[3] = first;
    manager.ReleaseAnm(3);
    const bool reloaded = manager.loadedAnms[3] == 0 && !deleted[2] &&
        deleted[3] && events == std::vector<int>({102, 203});
    // Clean survivors for both real and rejected cached-pointer bodies.
    if (!deleted[2]) delete first;
    if (!deleted[3]) delete second;
    if (!reloaded) {
        std::fprintf(stderr, "CHECK slot-reload\n"); return 1;
    }
    return 0;
}
'''


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--sanitize", action="store_true")
    args = parser.parse_args()
    text = (ROOT / "src/AnmManager.cpp").read_text()
    signature = "void AnmRenderManagerView::ReleaseAnm(int index)"
    if text.count(signature) != 1:
        raise ValueError("ReleaseAnm definition inventory changed")
    start = text.index(signature)
    end = text.index("\n}", start) + 2
    body = text[start:end]
    if body.count("delete loadedAnms[index];") != 1:
        raise ValueError("slot-delete mutation anchor changed")
    variants = [body, body.replace("delete loadedAnms[index];", "delete loaded;")]
    host_root = ROOT / "build/host-tests"
    host_root.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(prefix="anm-release-slot-", dir=host_root) as folder:
        folder = Path(folder)
        for index, variant in enumerate(variants):
            source, binary = folder / f"case{index}.cpp", folder / f"case{index}"
            source.write_text(PREFIX + variant + SUFFIX)
            command = ["flock", "/tmp/th10-anm-release-slot.lock", "nice", "-n", "10",
                       "g++", "-std=c++17", "-O1", "-Wall", "-Wextra",
                       "-Wno-sign-compare", str(source), "-o", str(binary)]
            if args.sanitize:
                command += ["-g", "-fsanitize=address,undefined",
                            "-fno-sanitize-recover=all", "-fno-pie", "-no-pie"]
            subprocess.run(command, check=True)
            result = subprocess.run([str(binary)], capture_output=True, text=True,
                                    env={**os.environ, "ASAN_OPTIONS": "detect_leaks=1"})
            expected = (0, "", "") if index == 0 else (1, "", "CHECK slot-reload\n")
            if (result.returncode, result.stdout, result.stderr) != expected:
                raise RuntimeError(f"unexpected fixture outcome {index}: {result}")
    print("PASS ReleaseAnm bounds/empty/ordinary/slot-replacement; cached-pointer mutant rejected")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
