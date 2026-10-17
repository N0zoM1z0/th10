#!/usr/bin/env python3
"""Host-only behavioral contract for the complete production statistics save owner.

Uses the complete GameStatistics.cpp lexical source with only includes removed,
all canonical header declarations and pointer-free assertions, and complete actual
FileSystem::WriteToOpenFile/CloseWriteFile source. Three marked production x86
pointer-layout assertions are replaced by an explicit host64 seam in the fixture.
Compressor, Encrypt, OpenWriteFile, Win32, and allocation are mocked boundaries;
this does not establish compression/encryption integration, native ABI, layout,
retention, or any exact-byte credit. No real score file or OS lock is touched.

Each mutant must compile and then exit 1 at its predeclared exact CHECK expression
AND generated source line. Crashes, sanitizer output, timeouts, build errors, extra
output, and a different failure are harness errors. Python -O preserves checks.
"""
from pathlib import Path
import argparse
import hashlib
import os
import re
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]
DIAGNOSTIC = re.compile(r'STATISTICS_CHECK_FAILED line ([1-9][0-9]*): ([^\r\n]+)\n\Z')


def require(condition, message):
    if not condition:
        raise RuntimeError(message)


def lexical_mask(source):
    # Preserve offsets, blanking comments and quoted strings before brace scans.
    pattern = r'//[^\n]*|/\*[\s\S]*?\*/|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\''
    return re.sub(pattern, lambda m: ''.join('\n' if c == '\n' else ' ' for c in m.group()), source)


def balanced(source, anchor):
    require(source.count(anchor) == 1, 'ambiguous extraction anchor: ' + anchor)
    start = source.index(anchor)
    masked = lexical_mask(source)
    brace = masked.index('{', start)
    depth, end = 1, brace + 1
    while depth:
        require(end < len(source), 'unbalanced complete function: ' + anchor)
        depth += (masked[end] == '{') - (masked[end] == '}')
        end += 1
    return source[start:end]


def strip_includes(source, allowed=None):
    includes = re.findall(r'^\s*#include\s+([<"][^>"\n]+[>"])\s*$', source, re.M)
    if allowed is not None:
        require(includes == allowed, 'canonical include inventory changed: ' + repr(includes))
    return re.sub(r'^\s*#include[^\n]*$', '// canonical include removed for amalgamation', source, flags=re.M).replace('#pragma once', '// canonical pragma once removed for amalgamation')


def canonical_path(root, name):
    paths = [root / 'src' / name, root / 'baseline/src' / name]
    path = next((p for p in paths if p.is_file()), None)
    require(path is not None, 'missing canonical dependency: ' + name)
    return path


def canonical(root, name):
    return canonical_path(root, name).read_text()


def extract(root):
    source = canonical(root, 'GameStatistics.cpp')
    header = canonical(root, 'GameStatistics.hpp')
    fs_source = canonical(root, 'FileSystem.cpp')
    fs_header = canonical(root, 'FileSystem.hpp')
    lzss_header = canonical(root, 'Lzss.hpp')
    require(source.count('int GameFinalizeStatistics42B1E0(GameStatisticsOwner *statistics)') == 1,
            'complete statistics owner boundary changed')
    body = balanced(source, 'int GameFinalizeStatistics42B1E0(GameStatisticsOwner *statistics)')
    require(source.rstrip().endswith(body), 'expected complete owner through EOF')
    owner = strip_includes(source, ['"GameStatistics.hpp"', '"FileSystem.hpp"', '"Lzss.hpp"', '<stdlib.h>', '<string.h>'])
    begin = '// GAME_STATISTICS_X86_LAYOUT_BEGIN'
    end = '// GAME_STATISTICS_X86_LAYOUT_END'
    require(header.count(begin) == 1 and header.count(end) == 1, 'host layout seam markers drift')
    block = header[header.index(begin):header.index(end) + len(end)]
    expected = '''// GAME_STATISTICS_X86_LAYOUT_BEGIN
typedef char GameStatisticsRecordsAt8[(offsetof(GameStatisticsOwner, records) == 8) ? 1 : -1];
typedef char GameStatisticsTailAt1D86C[(offsetof(GameStatisticsOwner, tail) == 0x1d86c) ? 1 : -1];
typedef char GameStatisticsOwnerSize1DCB4[(sizeof(GameStatisticsOwner) == 0x1dcb4) ? 1 : -1];
// GAME_STATISTICS_X86_LAYOUT_END'''
    require(block == expected, 'host seam may remove ONLY the three audited x86 owner assertions')
    header = strip_includes(header.replace(block, '// Three audited x86-only owner assertions removed; explicit host64 seam below.'), ['<stddef.h>', '<string.h>'])
    for declaration in ['extern HANDLE gReplayFileHandle;',
                        'extern CRITICAL_SECTION gReplayFileCriticalSection;',
                        'extern unsigned char gReplayFileOpenCount;']:
        require(declaration in fs_source, 'canonical FileSystem global seam drift: ' + declaration)
    protocol = '\n\n'.join(balanced(fs_source, signature) for signature in [
        'void WriteToOpenFile(const void *data, unsigned int size)', 'int CloseWriteFile()'])
    require('unsigned char *__stdcall CompressData(unsigned char *input, int inputSize,' in lzss_header,
            'canonical compression boundary drift')
    callee_headers = strip_includes(fs_header) + '\n' + strip_includes(lzss_header)
    return {'owner': owner, 'header': header, 'protocol': protocol, 'callee_headers': callee_headers,
            'owner_file': source, 'header_file': canonical(root, 'GameStatistics.hpp'),
            'filesystem_file': fs_source, 'lzss_header': lzss_header}


# Target, unique production anchor, replacement, exact expected CHECK expression.
# Expected failure sites are declared before running; never learned from output.
MUTATIONS = [
 ('null-result', 'owner', 'if (statistics->header == NULL)\n        return -1;', 'if (statistics->header == NULL)\n        return 0;', 'result == -1'),
 ('scratch-size', 'owner', 'malloc(0x200000)', 'malloc(0x1fffff)', 'size == ScratchSize'),
 ('header-cached-before-malloc', 'owner', 'unsigned char *scratch = static_cast<unsigned char *>(malloc(0x200000));', 'GameStatisticsFileHeader *cachedBeforeMalloc = statistics->header;\n    unsigned char *scratch = static_cast<unsigned char *>(malloc(0x200000));', 'std::memcmp(scratchPointer, &initialHeader, 0x18) == 0'),
 ('record-count-six', 'owner', 'i < 7', 'i < 6', 'inputSize == static_cast<int>(expectedPayload.size())'),
 ('magic-wrong', 'owner', 'record.magic == 0x5243', 'record.magic == 0x5242', 'inputSize == static_cast<int>(expectedPayload.size())'),
 ('record-index-omitted', 'owner', 'record.recordIndex = i;', ';', 'record.recordIndex == expectedRecord.recordIndex'),
 ('record-index-after-checksum', 'owner', 'record.recordIndex = i;', ';', 'record.checksum == expectedRecord.checksum'),
 ('record-signed-checksum', 'owner', 'checksum += payload[j];', 'checksum += static_cast<signed char>(payload[j]);', 'record.checksum == expectedRecord.checksum'),
 ('record-checksum-short', 'owner', 'j < 0x4374', 'j < 0x4373', 'record.checksum == expectedRecord.checksum'),
 ('record-checksum-includes-prefix', 'owner', 'reinterpret_cast<const unsigned char *>(&record) + 8', 'reinterpret_cast<const unsigned char *>(&record) + 4', 'record.checksum == expectedRecord.checksum'),
 ('record-copy-short', 'owner', 'memcpy(scratch + appendOffset, &record, 0x437c);', 'memcpy(scratch + appendOffset, &record, 0x437b);', 'std::memcmp(input, expectedPayload.data(), expectedPayload.size()) == 0'),
 ('record-append-stride', 'owner', 'appendOffset += 0x437c;', 'appendOffset += 0x4378;', 'inputSize == static_cast<int>(expectedPayload.size())'),
 ('tail-signed-checksum', 'owner', 'checksum += statistics->tail.payload[j];', 'checksum += static_cast<signed char>(statistics->tail.payload[j]);', 'owner->tail.checksum == expectedTail.checksum'),
 ('tail-checksum-short', 'owner', 'j < 0x440', 'j < 0x43f', 'owner->tail.checksum == expectedTail.checksum'),
 ('tail-checksum-omitted', 'owner', 'statistics->tail.checksum = checksum;', ';', 'owner->tail.checksum == expectedTail.checksum'),
 ('tail-copy-omitted', 'owner', 'memcpy(scratch + appendOffset, &statistics->tail, 0x448);', ';', 'std::memcmp(input, expectedPayload.data(), expectedPayload.size()) == 0'),
 ('payload-size-includes-header', 'owner', 'appendOffset + 0x430', 'appendOffset + 0x448', 'inputSize == static_cast<int>(expectedPayload.size())'),
 ('compress-scratch-header', 'owner', 'scratch + 0x18, statistics->header->uncompressedSize,', 'scratch, statistics->header->uncompressedSize,', 'input == scratchPointer + 0x18'),
 ('compress-output-pointer', 'owner', '&statistics->header->compressedSize);', '&statistics->header->uncompressedSize);', 'outputSize == &headerStorage[inputHeaderIndex].value.compressedSize'),
 ('header-size-wrong', 'owner', 'statistics->header->compressedSize + 0x18;', 'statistics->header->compressedSize + 0x14;', 'owner->header->fileSize == static_cast<unsigned int>(owner->header->compressedSize + 0x18)'),
 ('header-cached-before-compress', 'owner', 'unsigned char *compressed = CompressData(', 'GameStatisticsFileHeader *cachedHeader = statistics->header;\n    unsigned char *compressed = CompressData(', 'owner->header->fileSize == static_cast<unsigned int>(owner->header->compressedSize + 0x18)'),
 ('encrypt-key', 'owner', '0xac, 0x35, 0x10, statistics->header->compressedSize', '0xad, 0x35, 0x10, statistics->header->compressedSize', 'key == 0xac && step == 0x35 && chunkSize == 0x10 && maxBytes == size'),
 ('encrypt-step', 'owner', '0xac, 0x35, 0x10, statistics->header->compressedSize', '0xac, 0x34, 0x10, statistics->header->compressedSize', 'key == 0xac && step == 0x35 && chunkSize == 0x10 && maxBytes == size'),
 ('encrypt-chunk', 'owner', '0xac, 0x35, 0x10, statistics->header->compressedSize', '0xac, 0x35, 0x20, statistics->header->compressedSize', 'key == 0xac && step == 0x35 && chunkSize == 0x10 && maxBytes == size'),
 ('encrypt-limit', 'owner', '0xac, 0x35, 0x10, statistics->header->compressedSize', '0xac, 0x35, 0x10, statistics->header->compressedSize - 1', 'key == 0xac && step == 0x35 && chunkSize == 0x10 && maxBytes == size'),
 ('encrypt-return-used', 'owner', 'FileSystem::Encrypt(compressed,', 'compressed = FileSystem::Encrypt(compressed,', 'data == compressedPointer'),
 ('wrong-filename', 'owner', '"scoreth10.dat"', '"scoreth11.dat"', 'std::strcmp(path, "scoreth10.dat") == 0'),
 ('failed-open-freed', 'owner', 'if (FileSystem::OpenWriteFile("scoreth10.dat") != 0)\n        return -1;', 'if (FileSystem::OpenWriteFile("scoreth10.dat") != 0) { free(compressed); free(scratch); return -1; }', 'allocations[0].live && allocations[1].live'),
 ('positive-open-error-ignored', 'owner', 'FileSystem::OpenWriteFile("scoreth10.dat") != 0', 'FileSystem::OpenWriteFile("scoreth10.dat") < 0', 'result == (s.openResult == 0 ? 0 : -1)'),
 ('header-write-omitted', 'owner', 'FileSystem::WriteToOpenFile(statistics->header, 0x18);', ';', 'data == expectedHeaderWrite'),
 ('header-write-size', 'owner', 'FileSystem::WriteToOpenFile(statistics->header, 0x18);', 'FileSystem::WriteToOpenFile(statistics->header, 0x14);', 'size == 0x18'),
 ('payload-header-size-cached', 'owner', 'FileSystem::WriteToOpenFile(statistics->header, 0x18);', 'int cachedSize = statistics->header->compressedSize;\n    FileSystem::WriteToOpenFile(statistics->header, 0x18);', 'size == static_cast<unsigned>(expectedPayloadWriteSize)'),
 ('close-omitted', 'owner', 'FileSystem::CloseWriteFile();', ';', 'observedIo == expectedIo'),
 ('handle-cleared', 'owner', 'FileSystem::CloseWriteFile();', 'FileSystem::CloseWriteFile();\n    gReplayFileHandle = INVALID_HANDLE_VALUE;', 'gReplayFileHandle == expectedFinalHandle'),
 ('free-order-reversed', 'owner', 'if (compressed != NULL)\n        free(compressed);\n    free(scratch);', 'free(scratch);\n    if (compressed != NULL)\n        free(compressed);', 'pointer == (freeIndex == 0 ? compressedPointer : scratchPointer)'),
 ('scratch-leaked', 'owner', 'free(scratch);', ';', '!allocations[0].live && !allocations[1].live'),
 ('write-error-propagated', 'owner', '    return 0;\n}', '    return -1;\n}', 'result == (s.openResult == 0 ? 0 : -1)'),
 ('write-bool-used', 'protocol', '::WriteFile(gReplayFileHandle, data, size, &bytesWritten, NULL);', 'if (!::WriteFile(gReplayFileHandle, data, size, &bytesWritten, NULL)) return;', 'observedIo == expectedIo'),
 ('write-handle-cached', 'protocol', 'DWORD bytesWritten;', 'HANDLE cachedHandle = gReplayFileHandle;\n    DWORD bytesWritten;', 'observedIo == expectedIo'),
 ('short-write-handle-cleared', 'protocol', '        CloseHandle(gReplayFileHandle);', '        CloseHandle(gReplayFileHandle);\n        gReplayFileHandle = INVALID_HANDLE_VALUE;', 'writeIndex == expectedApiWrites'),
 ('short-write-decrement-omitted', 'protocol', '        --gReplayFileOpenCount;', '        ;', 'observedIo == expectedIo'),
 ('decrement-before-unlock', 'protocol', '        LeaveCriticalSection(&gReplayFileCriticalSection);\n        --gReplayFileOpenCount;', '        --gReplayFileOpenCount;\n        LeaveCriticalSection(&gReplayFileCriticalSection);', 'observedIo == expectedIo'),
 ('stage-score-offset', 'header', 'stage * 0xf0 + 0x10, sizeof(value)', 'stage * 0xf0 + 0x18, sizeof(value)', 'owner->records[i].StageInitialScore(stage) == score'),
 ('stage-startup-offset', 'header', 'stage * 0xf0 + 0x15, sizeof(value)', 'stage * 0xf0 + 0x1d, sizeof(value)', 'owner->records[i].StageStartupValue(stage) == startup'),
]

# Repeated CHECK expression uses an explicit intended scope, never first-match.
SCOPES = {'encrypt-return-used': 'BOOL WriteFile(HANDLE handle, const void *data, DWORD size, DWORD *bytesWritten, void *overlapped)'}


def replace_exact(text, old, new, count=1):
    require(text.count(old) == count, 'mutation anchor changed: expected %d, got %d: %r' % (count, text.count(old), old))
    return text.replace(old, new)


def mutate(parts, spec):
    name, target, old, new, _ = spec
    parts = dict(parts)
    count = 2 if name in {'short-write-handle-cleared', 'short-write-decrement-omitted', 'decrement-before-unlock'} else 1
    if name in {'short-write-handle-cleared', 'short-write-decrement-omitted'}:
        require(parts[target].count(old) == count, 'protocol double anchor drift')
        parts[target] = parts[target].replace(old, new, 1)
    else:
        parts[target] = replace_exact(parts[target], old, new, count)
    if name == 'header-cached-before-malloc':
        parts[target] = replace_exact(parts[target], 'memcpy(scratch, statistics->header, 0x18);', 'memcpy(scratch, cachedBeforeMalloc, 0x18);')
    elif name == 'record-index-after-checksum':
        parts[target] = replace_exact(parts[target], 'record.checksum = checksum;', 'record.checksum = checksum;\n            record.recordIndex = i;')
    elif name == 'header-cached-before-compress':
        parts[target] = replace_exact(parts[target], 'statistics->header->fileSize = statistics->header->compressedSize + 0x18;', 'cachedHeader->fileSize = cachedHeader->compressedSize + 0x18;')
    elif name == 'payload-header-size-cached':
        parts[target] = replace_exact(parts[target], 'FileSystem::WriteToOpenFile(compressed, statistics->header->compressedSize);', 'FileSystem::WriteToOpenFile(compressed, cachedSize);')
    elif name == 'write-handle-cached':
        require(parts[target].count('CloseHandle(gReplayFileHandle);') == 2, 'protocol close anchor drift')
        parts[target] = parts[target].replace('CloseHandle(gReplayFileHandle);', 'CloseHandle(cachedHandle);', 1)
    return parts


def assemble(root, parts):
    fixture = (root / 'scripts/fixtures/statistics-save.cpp').read_text()
    substitutions = {
        '@CANONICAL_STATISTICS_HEADER@': parts['header'],
        '@CANONICAL_CALLEE_HEADERS@': parts['callee_headers'],
        '@ACTUAL_STATISTICS_SOURCE@': parts['owner'],
        '@ACTUAL_WRITE_CLOSE_SOURCE@': parts['protocol'],
        '@MOCK_BOUNDARIES@': (root / 'scripts/fixtures/statistics-boundaries.inc').read_text(),
        '@SCENARIOS@': (root / 'scripts/fixtures/statistics-scenarios.inc').read_text(),
    }
    for marker, value in substitutions.items():
        require(fixture.count(marker) == 1, 'fixture marker missing or ambiguous: ' + marker)
        fixture = fixture.replace(marker, value)
    return fixture


def intended_line(cpp, spec):
    name, expression = spec[0], spec[4]
    start, end = 0, len(cpp)
    if name in SCOPES:
        region = balanced(cpp, SCOPES[name])
        start = cpp.index(SCOPES[name])
        end = start + len(region)
    matches, offset = [], 0
    for line_number, line in enumerate(cpp.splitlines(keepends=True), 1):
        if start <= offset < end and 'CHECK(' + expression + ');' in line:
            matches.append(line_number)
        offset += len(line)
    require(len(matches) == 1, 'ambiguous/missing intended CHECK for %s: %r' % (name, matches))
    return matches[0]


def build_run(cpp, directory, name, opt, sanitize, lsan_ptrace_workaround=False):
    source, executable = directory / (name + '.cpp'), directory / name
    source.write_text(cpp)
    command = [os.environ.get('CXX', 'g++'), '-std=c++17', '-' + opt, '-Wall', '-Wextra', '-Werror',
               '-Wno-unused-function', '-Wno-unused-parameter', str(source), '-o', str(executable)]
    if sanitize:
        command[1:1] = ['-fsanitize=address,undefined', '-fno-omit-frame-pointer', '-fno-pie', '-no-pie']
    result = subprocess.run(command, text=True, capture_output=True, timeout=90)
    require(result.returncode == 0, 'HARNESS compile failure %s:\n%s%s' % (name, result.stdout, result.stderr))
    return subprocess.run([str(executable)], text=True, capture_output=True, timeout=60,
                          env={**os.environ, 'ASAN_OPTIONS': 'detect_leaks=%d:halt_on_error=1' % (0 if lsan_ptrace_workaround else 1),
                               'UBSAN_OPTIONS': 'halt_on_error=1:print_stacktrace=1'})


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, default=ROOT)
    parser.add_argument('--opt', choices=('O0', 'O1', 'O2'), default='O2')
    parser.add_argument('--sanitize', action='store_true')
    parser.add_argument('--baseline-only', action='store_true')
    parser.add_argument('--lsan-ptrace-workaround', action='store_true',
                        help='Disable LSan only after documenting this host ptrace incompatibility; ASan/UBSan stay enabled')
    args = parser.parse_args()
    parts = extract(args.root)
    if args.lsan_ptrace_workaround:
        require(args.sanitize, '--lsan-ptrace-workaround requires --sanitize')
        print('LSan EXCLUDED: explicit documented host ptrace incompatibility workaround; ASan/UBSan active', flush=True)
    elif args.sanitize:
        print('ASan/UBSan/LSan active (detect_leaks=1)', flush=True)
    for name in ['GameStatistics.cpp', 'GameStatistics.hpp', 'FileSystem.cpp', 'FileSystem.hpp', 'Lzss.hpp']:
        print('Canonical %s file SHA256 %s' % (name, hashlib.sha256(canonical_path(args.root, name).read_bytes()).hexdigest()), flush=True)
    print('Extracted protocol normalized-source SHA256 ' + hashlib.sha256(parts['protocol'].encode()).hexdigest(), flush=True)
    print('Host seam: canonical declarations; only three marked x86 owner assertions replaced by explicit host64 assertions', flush=True)
    with tempfile.TemporaryDirectory(prefix='statistics-save-') as temporary:
        directory = Path(temporary)
        baseline = build_run(assemble(args.root, parts), directory, 'baseline', args.opt, args.sanitize, args.lsan_ptrace_workaround)
        require(baseline.returncode == 0 and baseline.stderr == '' and baseline.stdout.startswith('PASS statistics save contract:'),
                'HARNESS baseline failed: code=%d stdout=%r stderr=%r' % (baseline.returncode, baseline.stdout, baseline.stderr))
        print(baseline.stdout.rstrip(), flush=True)
        if not args.baseline_only:
            for spec in MUTATIONS:
                cpp = assemble(args.root, mutate(parts, spec))
                expected_line = intended_line(cpp, spec)
                result = build_run(cpp, directory, spec[0], args.opt, args.sanitize, args.lsan_ptrace_workaround)
                match = DIAGNOSTIC.fullmatch(result.stderr)
                require(result.returncode == 1 and result.stdout == '' and match is not None and
                        int(match.group(1)) == expected_line and match.group(2) == spec[4],
                        'HARNESS invalid mutant %s: expected line %d: %s; code=%d stdout=%r stderr=%r' %
                        (spec[0], expected_line, spec[4], result.returncode, result.stdout, result.stderr))
                print('PASS compiled mutant %s -> exact CHECK line %d: %s' % (spec[0], expected_line, spec[4]), flush=True)
            print('PASS %d compiled mutants, each at its exact intended CHECK line and expression' % len(MUTATIONS), flush=True)


if __name__ == '__main__':
    try:
        main()
    except (RuntimeError, subprocess.TimeoutExpired) as error:
        raise SystemExit(str(error))
