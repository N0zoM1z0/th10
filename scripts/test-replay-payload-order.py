#!/usr/bin/env python3
"""Exercise the maintained replay-copy loops with synthetic nodes, without game IO.

This host-only layout test is not a historical-codegen or whole-game Oracle.
"""
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]


def copy_loops():
    source = (ROOT / "src/ReplayManager.cpp").read_text()
    source = source.split("int ReplayManager::SaveReplay(", 1)[1]
    source = source.split("int uncompressedSize = sizeof(ReplayDataHeader);", 1)[1]
    source = source.split("    int compressedSize;", 1)[0]
    loops = []
    cursor = 0
    while True:
        start = source.find("for (ReplayListNode *link", cursor)
        if start < 0:
            break
        brace = source.index("{", start)
        depth = 1
        end = brace + 1
        while depth:
            depth += (source[end] == "{") - (source[end] == "}")
            end += 1
        loops.append(source[start:end])
        cursor = end
    return loops


class ReplayPayloadOrder(unittest.TestCase):
    def test_two_distinct_passes(self):
        loops = copy_loops()
        self.assertEqual(len(loops), 2)
        self.assertIn("frameData->records", loops[0])
        self.assertNotIn("frameData->fpsSamples", loops[0])
        self.assertIn("frameData->fpsSamples", loops[1])
        self.assertNotIn("frameData->records", loops[1])

    @unittest.skipUnless(shutil.which("c++"), "host C++ compiler unavailable")
    def test_actual_copy_loops_on_synthetic_nodes(self):
        loops = "\n".join(copy_loops())
        program = r'''
#include <cassert>
#include <cstring>
#include <vector>
struct ReplayRecData { unsigned char bytes[6]; };
struct ReplayFrameData {
    ReplayRecData records[3];
    ReplayRecData *recordEnd;
    unsigned char fpsSamples[4];
    unsigned char *fpsEnd;
};
struct ReplayListNode { void *value; ReplayListNode *next; };
static std::vector<unsigned char> serialize(ReplayListNode *head) {
    ReplayListNode stageFrameLists[1] = {{0, head}};
    int stage = 0, uncompressedSize = 0;
    unsigned char uncompressedData[128] = {};
''' + loops + r'''
    return std::vector<unsigned char>(uncompressedData, uncompressedData + uncompressedSize);
}
int main() {
    ReplayFrameData a = {}, b = {};
    memset(a.records[0].bytes, 'A', 6);
    memset(a.records[1].bytes, 'B', 6);
    memset(b.records[0].bytes, 'C', 6);
    a.recordEnd = a.records + 2; b.recordEnd = b.records + 1;
    a.fpsSamples[0] = 'x'; b.fpsSamples[0] = 'y'; b.fpsSamples[1] = 'z';
    a.fpsEnd = a.fpsSamples + 1; b.fpsEnd = b.fpsSamples + 2;
    ReplayListNode second = {&b, 0}, first = {&a, &second};
    const unsigned char expected[] = "AAAAAABBBBBBCCCCCCxyz";
    std::vector<unsigned char> actual = serialize(&first);
    assert(actual == std::vector<unsigned char>(expected, expected + sizeof(expected) - 1));
    const unsigned char oldInterleaved[] = "AAAAAABBBBBBxCCCCCCyz";
    assert(actual != std::vector<unsigned char>(oldInterleaved, oldInterleaved + sizeof(oldInterleaved) - 1));
    assert(serialize(0).empty());
    first.next = 0;
    const unsigned char one[] = "AAAAAABBBBBBx";
    assert(serialize(&first) == std::vector<unsigned char>(one, one + sizeof(one) - 1));
    a.recordEnd = a.records; a.fpsEnd = a.fpsSamples;
    assert(serialize(&first).empty());
    return 0;
}
'''
        (ROOT / "build").mkdir(exist_ok=True)
        with tempfile.TemporaryDirectory(prefix="replay-order-", dir=ROOT / "build") as directory:
            source = Path(directory) / "test.cpp"
            binary = Path(directory) / "test"
            source.write_text(program)
            subprocess.run([shutil.which("c++"), "-std=c++11", "-O0", str(source), "-o", str(binary)], check=True)
            subprocess.run([str(binary)], check=True)


    @unittest.skipUnless(shutil.which("c++"), "host C++ compiler unavailable")
    def test_postcopy_uses_reread_of_captured_frame(self):
        # Deliberate host-only copy observer: mutate the captured frame's end
        # and replace link->value after the copy. This tests the extracted
        # expressions, not normal memcpy behavior or game runtime aliasing.
        loops = copy_loops()
        prefix = r'''
#include <cassert>
#include <cstring>
struct ReplayRecData { unsigned char bytes[6]; };
struct ReplayFrameData {
    ReplayRecData records[3]; ReplayRecData *recordEnd;
    unsigned char fpsSamples[4]; unsigned char *fpsEnd;
};
struct ReplayListNode { void *value; ReplayListNode *next; };
static ReplayFrameData original, replacement;
static ReplayListNode *observedLink;
static int copies;
static void observedCopy(void *dst, const void *src, unsigned size) {
    std::memcpy(dst, src, size);
    ++copies;
    original.recordEnd = original.records + 1;
    original.fpsEnd = original.fpsSamples + 1;
    observedLink->value = &replacement;
}
static int exercise() {
    original.recordEnd = original.records + 2;
    original.fpsEnd = original.fpsSamples + 2;
    replacement.recordEnd = replacement.records + 3;
    replacement.fpsEnd = replacement.fpsSamples + 3;
    ReplayListNode node = {&original, 0}; observedLink = &node;
    ReplayListNode stageFrameLists[1] = {{0, &node}};
    int stage = 0, uncompressedSize = 0;
    unsigned char uncompressedData[128] = {};
'''
        (ROOT / "build").mkdir(exist_ok=True)
        with tempfile.TemporaryDirectory(prefix="replay-capture-", dir=ROOT / "build") as directory:
            for index, loop in enumerate(loops):
                # Positive: reread the original captured frame (6 / 1 bytes).
                # Negatives: cached length and reloading link->value (12 / 2,
                # respectively 18 / 3 bytes) must both be discriminated.
                expected = 6 if index == 0 else 1
                expressions = [loop, loop.replace(
                    "uncompressedSize += (frameData->recordEnd - frameData->records) *\n                                sizeof(ReplayRecData);",
                    "uncompressedSize += recordBytes;").replace(
                    "uncompressedSize += frameData->fpsEnd - frameData->fpsSamples;",
                    "uncompressedSize += fpsBytes;"),
                    loop.replace("uncompressedSize +=", "frameData = (ReplayFrameData *)link->value; uncompressedSize +=")]
                self.assertEqual(len(set(expressions)), 3)
                for variant, expression in enumerate(expressions):
                    code = prefix + expression.replace("memcpy(", "observedCopy(")
                    code += "\nreturn uncompressedSize;\n}\nint main() { assert(copies == 0); int n = exercise(); assert(copies == 1); assert(n "
                    code += ("==" if variant == 0 else "!=") + " " + str(expected) + "); }\n"
                    source = Path(directory) / ("case-%d-%d.cpp" % (index, variant))
                    binary = source.with_suffix("")
                    source.write_text(code)
                    subprocess.run([shutil.which("c++"), "-std=c++11", "-O2", str(source), "-o", str(binary)], check=True)
                    subprocess.run([str(binary)], check=True)


if __name__ == "__main__":
    unittest.main()
