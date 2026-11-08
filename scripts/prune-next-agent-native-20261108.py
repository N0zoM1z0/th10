#!/usr/bin/env python3
"""One-time, SHA-audited prune of explicitly reviewed, ignored TH10 trials.

Dry run by default. The opt-in --apply only removes .obj/.exe/.pdb in the
allowlisted old experiment families. Never touch current canonical graphs,
original target/toolchain, archives, source snapshots, drivers, JSON or MAPs.
An SHA deletion manifest is *not* a restorable copy; rebuild if needed.
"""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parent.parent
TRIALS = ROOT / 'build/gpt-web'
REPORT = ROOT / '.analysis/maintenance-next-agent-20261108'
EXTENSIONS = frozenset(('.exe', '.obj', '.pdb'))

# Each path was explicitly reviewed as old exploratory, rejected, or superseded
# generated binary output. Current exact acceptance graphs are elsewhere.
# Do not replace this with a recursive blanket cleanup of build/ or .analysis/.
REBUILDABLE_FAMILIES = (
    '20261107-session-th10-enemy-v2',
    '20261107-shake-envelope-current',
    '20261107-active-enemy-graph',
    '20261107-active-transition-lifetime',
    '20261107-enemy-flag-shift',
    '20261107-turn5-enemy',
    '20261107-active-bullet-absolute-register',
    '20261107-bullet-spawn-native',
    '20261107-turn4-enemy',
    '20261107-shake-envelope',
    '20261106-replay-initialize',
    '20261108-enemy-destructor-words',
    '20261107-enemy-laser-native-words',
    '20261108-enemy-ecl-index-boss',
    '20261106-chat-th10-enemy-rank',
    '20261107-enemy-single-resource-words',
    '20261107-anm-op91-v2',
    '20261106-ecl-count-reference',
    '20261108-enemy-spawn-native-counts',
    '20261107-active-anm91-lifetimes',
    '20261104-live-enemy',
    '20261108-ecl-pop-native',
    '20261107-turn2-sound-sample',
    '20261107-turn4-anm-world',
    '20261106-laser-type1-circle',
    '20261107-bullet-register',
    '20261106-chat-th10-table',
    '20261107-shake-output-register',
    '20261107-turn3-anm91',
    '20261106-chat-th10-capture',
    '20261108-ecl-x87-order',
    '20261107-session-th10-ecl',
    '20261107-active-sound-sample-native',
    '20261107-bullet-timer-wrap',
    '20261106-upstream-review',
    '20261106-chat-th10-ecl',
    '20261106-chat-th10-bullet-abs',
    '20261107-active-sound-sample-words',
    '20261107-active-ecl-advance',
    '20261107-filesystem-archive-words',
    '20261107-current-sound-trials',
    '20261107-anm-init-receiver',
    '20261107-active-ecl-counter',
    '20261107-active-ecl-pop-address',
    '20261107-session-th10-enemy',
    '20261108-anm-tail-native',
    '20261107-anm-stream-word',
    '20261106-chat-th10-popup-factory',
    '20261107-active-ecl-stop',
    '20261105-enemy-pattern-helper',
    '20261107-shake-switch',
    '20261107-ecl-format-native-index',
    '20261107-ecl-add-script-word',
    '20261106-chat-th10-bullet',
    '20261107-turn2-wave-cursor',
    '20261107-turn5-ecl-record',
    '20261107-turn2-ecl-format',
)

# These are representative current accepted witnesses. Their SHA and the
# protected directory sizes are checked both before and after deletion.
PROTECTED_FILES = (
    'resources/th10.exe',
    'build/match-linked/SoundInitBuffers/source.exe',
    'build/match-linked/TextRenderMainHybrid/source.exe',
    'build/match-linked/EclVmHostRunEnemy/source.exe',
    'build/match-linked/BulletManagerUpdateCallback/source.exe',
)
PROTECTED_DIRS = (
    'build/match-linked', '.analysis/archive', '.tools', '_reference',
    'ghidra-project',
)


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open('rb') as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b''):
            digest.update(block)
    return digest.hexdigest()


def usage(path: Path) -> int:
    return int(subprocess.check_output(
        ['du', '-s', '-B1', str(path)], text=True).split()[0])


def discover() -> tuple[list[Path], dict[str, dict[str, int]]]:
    if ROOT.name != 'th10' or not TRIALS.is_dir() or TRIALS.is_symlink():
        raise RuntimeError('refusing noncanonical or symlinked TH10 build path')
    tracked = subprocess.check_output(
        ['git', 'ls-files', '--', 'build/gpt-web'], text=True, cwd=ROOT)
    if tracked.strip():
        raise RuntimeError('refusing a trial path containing tracked files')
    files: list[Path] = []
    summary: dict[str, dict[str, int]] = {}
    for family in REBUILDABLE_FAMILIES:
        path = TRIALS / family
        if not path.is_dir() or path.is_symlink() or path.resolve().parent != TRIALS.resolve():
            raise RuntimeError(f'allowlisted family is absent or unsafe: {path}')
        members = []
        for file in sorted(path.rglob('*')):
            if file.is_symlink():
                raise RuntimeError(f'symlink inside reviewed trial tree: {file}')
            if not file.is_file() or file.suffix.lower() not in EXTENSIONS:
                continue
            if not file.resolve().is_relative_to(path.resolve()):
                raise RuntimeError(f'non-owned file: {file}')
            # Retain any explicitly named acceptance/current cold receipts.
            if {'accepted-exact', 'canonical-cold', 'installed-cold'} & set(file.relative_to(path).parts):
                continue
            members.append(file)
        files.extend(members)
        summary[family] = {'files': len(members),
                           'payload_bytes': sum(f.stat().st_size for f in members)}
    if len(files) != len(set(files)):
        raise RuntimeError('duplicate paths in explicit allowlist')
    return files, summary


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--apply', action='store_true', help='perform one-time audited deletion')
    args = parser.parse_args()
    files, summary = discover()
    payload = sum(p.stat().st_size for p in files)
    print(json.dumps({
        'mode': 'apply' if args.apply else 'dry-run',
        'allowlisted_roots': len(REBUILDABLE_FAMILIES),
        'native_files': len(files),
        'payload_bytes': payload,
        'payload_MiB': round(payload / (1024 * 1024), 2),
        'largest_roots': sorted(summary.items(), key=lambda item: -item[1]['payload_bytes'])[:12],
    }, indent=2))
    if not args.apply:
        return
    if not files or REPORT.exists():
        raise RuntimeError('this cleanup is one-time and must not be repeated')
    if subprocess.check_output(['git', 'status', '--porcelain'], text=True, cwd=ROOT).strip():
        raise RuntimeError('commit tracked changes and resolve dirty work before applying')
    before = usage(TRIALS)
    frozen = {p: sha256(ROOT / p) for p in PROTECTED_FILES}
    sizes = {p: usage(ROOT / p) for p in PROTECTED_DIRS}
    for p in PROTECTED_FILES:
        if not (ROOT / p).is_file():
            raise RuntimeError(f'protected witness missing: {p}')
    REPORT.mkdir(parents=True, exist_ok=False)
    manifest = REPORT / 'removed-native.jsonl'
    with manifest.open('w', encoding='utf-8') as out:
        for file in files:
            stat = file.stat()
            out.write(json.dumps({
                'path': str(file.relative_to(ROOT)),
                'bytes': stat.st_size,
                'allocated_bytes': stat.st_blocks * 512,
                'sha256': sha256(file),
            }, sort_keys=True) + '\n')
    for file in files:
        file.unlink()
    after = usage(TRIALS)
    if after >= before:
        raise RuntimeError('reclamation did not reduce allocation')
    for name, hash_before in frozen.items():
        if sha256(ROOT / name) != hash_before:
            raise RuntimeError(f'protected canonical witness changed: {name}')
    for name, size_before in sizes.items():
        if usage(ROOT / name) != size_before:
            raise RuntimeError(f'protected directory changed: {name}')
    report = {
        'date': '2026-11-08',
        'scope': 'rebuildable ignored VC7.1 native experiment outputs only',
        'families': summary,
        'deleted_files': len(files),
        'payload_bytes': payload,
        'allocated_before': before,
        'allocated_after': after,
        'allocated_reclaimed': before - after,
        'manifest': str(manifest.relative_to(ROOT)),
        'manifest_sha256': sha256(manifest),
        'protected_sha256': frozen,
        'protected_allocated_bytes': sizes,
        'archive_created': False,
        'retained': ['original target', 'pinned compiler', 'Ghidra and reference inputs',
                     'all historical archives', 'all canonical linked PEs',
                     'all source, snapshots, JSON, MAP, logs and probe drivers'],
        'warning': 'A deletion SHA manifest is not an archive; regenerate variant binaries from pinned source and compiler.',
    }
    (REPORT / 'cleanup-result.json').write_text(json.dumps(report, indent=2) + '\n')
    print('CLEANUP_OK reclaimed', before - after, 'bytes; manifest', report['manifest_sha256'])


if __name__ == '__main__':
    main()
