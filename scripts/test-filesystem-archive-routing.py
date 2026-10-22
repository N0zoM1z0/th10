#!/usr/bin/env python3
"""Exercise maintained archive-or-disk routing against explicit dependency seams."""
from __future__ import annotations

import argparse
import hashlib
import os
from pathlib import Path
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]

HARNESS = r'''
#include "ArchiveOrDiskPath.hpp"
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

static unsigned checks;
#define CHECK(x) do { ++checks; if (!(x)) throw std::runtime_error(#x); } while (0)
static std::vector<std::string> trace;
static std::string lastPath, lastEntry;
static BYTE *lastBuffer;
static bool archiveFailure, diskOpen;
static DWORD archiveSize, diskSize, wholeLimit;
static BYTE archiveData[8], diskData[8];
static char readMode[] = "rb", writeMode[] = "wb", appendMode[] = "ab";
char *g_PbgFileOpenModes[3] = {readMode, writeMode, appendMode};

// Host-only dependency seams. No native PbgArchive/CPbgFile layout or Windows
// API implementation is substituted into the maintained routing methods.
class PbgArchive {
public:
    const char *filename;
    const char *GetArchiveFilename() const { return filename; }
    BYTE *ReadDecompressEntry(const char *entry, BYTE *buffer) {
        trace.push_back("archive-read"); lastEntry = entry; lastBuffer = buffer;
        return archiveFailure ? NULL : (buffer ? buffer : archiveData);
    }
    unsigned int GetEntryDecompressedSize(const char *entry) {
        trace.push_back("archive-size"); lastEntry = entry; return archiveSize;
    }
};
PbgArchive g_PbgArchives[20];
int g_PbgArchiveCount;
class CPbgFile {
public:
    CPbgFile() { trace.push_back("ctor"); }
    ~CPbgFile() { trace.push_back("dtor"); }
    bool Open(const char *path, char *mode) {
        trace.push_back("open"); lastPath = path;
        CHECK(mode == g_PbgFileOpenModes[0]); return diskOpen;
    }
    DWORD GetSize() {
        trace.push_back("disk-size"); return diskOpen ? diskSize : 0;
    }
    void *ReadWholeFile(DWORD limit) {
        trace.push_back("whole"); wholeLimit = limit;
        return diskOpen ? diskData : NULL;
    }
};

@SOURCE@

static ArchiveOrDiskPathView *view(const char *path) {
    return reinterpret_cast<ArchiveOrDiskPathView *>(const_cast<char *>(path));
}
static void reset() {
    trace.clear(); lastPath.clear(); lastEntry.clear(); lastBuffer = NULL;
    archiveFailure = false; diskOpen = true; archiveSize = 91; diskSize = 123;
    wholeLimit = 0; g_PbgArchiveCount = 0;
    for (unsigned i = 0; i < 20; ++i) g_PbgArchives[i].filename = "unused.dat";
}
static void expectTrace(std::initializer_list<const char *> expected) {
    std::vector<std::string> strings;
    for (const char *s : expected) strings.push_back(s);
    CHECK(trace == strings);
}
int main() {
    try {
        struct PathCase { const char *path; const char *archive; };
        const PathCase paths[] = {
            {"pack/item", "pack.dat"}, {"pack/dir/item", "pack.dat"},
            {"pack//item", "pack.dat"}, {"pack/", "pack.dat"},
            {"/item", ".dat"}, {"pack.dat", "pack.dat"}, {"", ""},
            {"a.b/item", "a.b.dat"}, {"a\\b/item", "a\\b.dat"},
            {"a\\b", "a\\b"}
        };
        for (const PathCase &p : paths) {
            reset(); g_PbgArchiveCount = 2;
            g_PbgArchives[0].filename = "other.dat";
            g_PbgArchives[1].filename = p.archive;
            CHECK(view(p.path)->FindArchive() == &g_PbgArchives[1]);
            CHECK(trace.empty());
        }
        reset(); g_PbgArchiveCount = 2;
        g_PbgArchives[0].filename = "pack.dat";
        g_PbgArchives[1].filename = "pack.dat";
        CHECK(view("pack/item")->FindArchive() == &g_PbgArchives[0]);
        CHECK(view("PACK/item")->FindArchive() == NULL);
        CHECK(view("pack\\item")->FindArchive() == NULL);
        g_PbgArchiveCount = -1; CHECK(view("pack/item")->FindArchive() == NULL);
        g_PbgArchiveCount = 0; CHECK(view("pack/item")->FindArchive() == NULL);

        reset(); std::vector<std::string> names;
        for (unsigned i = 0; i < 20; ++i) names.push_back("pack" + std::to_string(i) + ".dat");
        for (unsigned i = 0; i < 20; ++i) g_PbgArchives[i].filename = names[i].c_str();
        g_PbgArchiveCount = 20;
        CHECK(view("pack19/item")->FindArchive() == &g_PbgArchives[19]);
        CHECK(view("pack20/item")->FindArchive() == NULL);

        reset(); std::string prefix(255, 'p'), archive = prefix + ".dat";
        g_PbgArchiveCount = 1; g_PbgArchives[0].filename = archive.c_str();
        CHECK(view((prefix + "/x").c_str())->FindArchive() == &g_PbgArchives[0]);
        std::string plain(259, 'q'); g_PbgArchives[0].filename = plain.c_str();
        CHECK(view(plain.c_str())->FindArchive() == &g_PbgArchives[0]);

        for (const char *path : {"pack/item", "pack/dir/item", "pack//item", "pack/", "pack.dat"}) {
            reset(); g_PbgArchiveCount = 1; g_PbgArchives[0].filename = "pack.dat";
            const char *slash = std::strchr(path, '/');
            std::string expected = slash ? slash + 1 : path;
            BYTE output[8] = {};
            CHECK(view(path)->Read(output) == output);
            CHECK(lastBuffer == output && lastEntry == expected);
            expectTrace({"archive-read"});
            trace.clear(); CHECK(view(path)->Read(NULL) == archiveData);
            CHECK(lastBuffer == NULL && lastEntry == expected); expectTrace({"archive-read"});
            trace.clear(); archiveFailure = true;
            CHECK(view(path)->Read(output) == NULL); expectTrace({"archive-read"});
            trace.clear(); CHECK(view(path)->GetSize() == archiveSize);
            CHECK(lastEntry == expected); expectTrace({"archive-size"});
            trace.clear(); archiveSize = 0;
            CHECK(view(path)->GetSize() == 0); expectTrace({"archive-size"});
        }
        for (bool open : {true, false}) {
            reset(); diskOpen = open; BYTE output[8];
            CHECK(view("missing/path")->Read(output) == (open ? diskData : NULL));
            CHECK(lastPath == "missing/path" && wholeLimit == (open ? diskSize : 0));
            expectTrace({"ctor", "open", "disk-size", "whole", "dtor"});
            trace.clear(); CHECK(view("missing/path")->GetSize() == (open ? diskSize : 0));
            expectTrace({"ctor", "open", "disk-size", "dtor"});
        }
        reset(); diskSize = 0xffffffffU;
        CHECK(view("missing")->Read(NULL) == diskData); CHECK(wholeLimit == 0xffffffffU);
        expectTrace({"ctor", "open", "disk-size", "whole", "dtor"});
        std::cout << "PASS " << checks << " source assertions\n";
        return 0;
    } catch (const std::exception &error) {
        std::cout << "FAIL: " << error.what() << "\n"; return 3;
    }
}
'''


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--repo-root', type=Path, default=ROOT)
    parser.add_argument('--baseline-only', action='store_true')
    args = parser.parse_args()
    root = args.repo_root.resolve()
    source_path = root / 'src/FileSystem.cpp'
    header_path = root / 'src/ArchiveOrDiskPath.hpp'
    sources = {path: path.read_bytes() for path in [source_path, header_path]}
    text = sources[source_path].decode().replace('\r\n', '\n')
    begin = text.index('PbgArchive *ArchiveOrDiskPathView::FindArchive()')
    end = text.index('// Maintained reconstruction source.', begin)
    selected = text[begin:end]
    for signature in ['PbgArchive *ArchiveOrDiskPathView::FindArchive()',
                      'BYTE *ArchiveOrDiskPathView::Read(BYTE *optionalBuffer)',
                      'DWORD ArchiveOrDiskPathView::GetSize()']:
        if selected.count(signature) != 1:
            raise ValueError('routing source selection changed: ' + signature)
    mutations = {
        'last-slash': ("strchr(path, '/')", "strrchr(path, '/')"),
        'wrong-name-comparison': ('archiveFilename) == 0', 'archiveFilename) != 0'),
        'wrong-array-element': ('return &g_PbgArchives[index];', 'return &g_PbgArchives[0];'),
        'drop-output-buffer': ('return archive->ReadDecompressEntry(entryName, optionalBuffer);',
                               'return archive->ReadDecompressEntry(entryName, NULL);'),
        'fallback-after-archive-error': ('return archive->ReadDecompressEntry(entryName, optionalBuffer);',
            'BYTE *result = archive->ReadDecompressEntry(entryName, optionalBuffer);\n        if (result != NULL) return result;'),
        'guard-disk-open': ('file.Open(path, g_PbgFileOpenModes[0]);',
                           'if (!file.Open(path, g_PbgFileOpenModes[0])) return NULL;'),
        'drop-disk-size': ('return file.GetSize();', 'return 0;'),
        'drop-entry-suffix': ('entryName = entryName == NULL ? path : entryName + 1;\n        return archive->ReadDecompressEntry',
                             'entryName = path;\n        return archive->ReadDecompressEntry'),
    }
    cases = [('baseline', selected)]
    if not args.baseline_only:
        for name, (before, after) in mutations.items():
            if before not in selected:
                raise ValueError('mutation selection changed: ' + name)
            cases.append((name, selected.replace(before, after, 1)))
    analysis = root / '.analysis'
    analysis.mkdir(exist_ok=True)
    with tempfile.TemporaryDirectory(prefix='filesystem-routing-host-', dir=analysis) as temporary:
        work = Path(temporary)
        (work / 'windows.h').write_text('#pragma once\n#include <cstdint>\nusing BYTE = unsigned char;\nusing DWORD = std::uint32_t;\n#define MAX_PATH 260\n')
        (work / 'ArchiveOrDiskPath.hpp').write_bytes(sources[header_path])
        for name, body in cases:
            cpp = work / 'routing.cpp'; exe = work / 'routing'
            cpp.write_text(HARNESS.replace('@SOURCE@', body))
            build = subprocess.run(['g++', '-std=c++11', '-O1', '-g', '-fsanitize=address,undefined',
                '-fno-omit-frame-pointer', '-fno-pie', '-no-pie', '-I', str(work), str(cpp), '-o', str(exe)],
                capture_output=True, text=True)
            if build.returncode:
                raise ValueError(name + ' did not compile:\n' + build.stdout + build.stderr)
            result = subprocess.run([str(exe)], capture_output=True, text=True,
                env={**os.environ, 'ASAN_OPTIONS': 'detect_leaks=1:halt_on_error=1', 'UBSAN_OPTIONS': 'halt_on_error=1'})
            expected = 0 if name == 'baseline' else 3
            if result.returncode != expected or (name != 'baseline' and not result.stdout.startswith('FAIL: ')):
                raise ValueError(name + ' unexpected result:\n' + result.stdout + result.stderr)
            if name == 'baseline':
                print(result.stdout.strip())
    if any(path.read_bytes() != data for path, data in sources.items()):
        raise ValueError('routing inputs changed during host checks')
    print('PASS: ' + str(len(cases) - 1) + ' strict functional controls detected')
    print('Scope: maintained routing source with explicit archive/disk dependency seams; no target ABI, class lifetime, Windows API, binary or engine-runtime claim.')
    return 0


if __name__ == '__main__':
    try:
        raise SystemExit(main())
    except (OSError, ValueError, subprocess.SubprocessError) as error:
        raise SystemExit('FAIL: ' + str(error))
