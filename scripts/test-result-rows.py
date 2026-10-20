#!/usr/bin/env python3
"""Actual result-row/name owners and ASCII formatter/queue, with host layouts.

Native layout assertions are omitted; timestamps use int32. Constructors supply
host prefill. localtime is a deterministic role; remapping probes sampling,
not reachable CRT/game callbacks. Actual ASCII helpers have no fake callbacks.
No native ABI/x87/runtime or product claim.
"""
import argparse, hashlib, json, os, re, runpy, subprocess, tempfile
from pathlib import Path
ROOT = Path(__file__).resolve().parents[1]
body = runpy.run_path(str(ROOT/'scripts/test-frontend-replay-save.py'))['body']


def run(args, out):
    out.mkdir(parents=True, exist_ok=True)
    hashes = {}; pending = ['ResultMenu.hpp', 'ReplayManager.hpp']; seen = set()
    while pending:
        name = pending.pop()
        if name in seen: continue
        seen.add(name); p = ROOT/'src'/name; text = p.read_text()
        hashes['src/'+name] = hashlib.sha256(p.read_bytes()).hexdigest()
        pending.extend(re.findall(r'^\s*#\s*include\s*"([^"]+)"', text, re.M))
        text = re.sub(r'typedef\s+char\s+\w+\s*\[[\s\S]*?\]\s*;', '// HOST native assertion omitted.', text)
        text = text.replace('AnmVmTimerView() { flags &= ~1u; }', 'AnmVmTimerView() : flags(0) {}')
        (out/name).write_text(text)
    p = ROOT/'src/FrontEnd.cpp'; text = p.read_text(); hashes['src/FrontEnd.cpp'] = hashlib.sha256(p.read_bytes()).hexdigest()
    types = body(text, 'struct FrontEndScoreRecordView\n').replace('time_t', 'std::int32_t')+';\n'
    (out/'types.inc').write_text(types)
    owners = body(text, 'static FrontEndScoreRecordView *GetPracticeScoreRecord(')+'\n'+'\n'.join(body(text, s) for s in ['void __stdcall ResultMenuDrawNameEntry(', 'int __stdcall ResultMenuDrawRows(', 'int __fastcall ResultMenuDrawRowsCallback('])
    owners = owners.replace('reinterpret_cast<time_t *>', 'reinterpret_cast<std::int32_t *>')
    p = ROOT/'src/AnmManager.cpp'; text = p.read_text(); hashes['src/AnmManager.cpp'] = hashlib.sha256(p.read_bytes()).hexdigest()
    (out/'helpers.inc').write_text(body(text, 'void AsciiManagerView::AddString(')+'\n'+body(text, 'void AsciiManagerView::AddFormatText('))
    variants = [('positive', owners, None)]
    stale_record = '''                record = GetPracticeScoreRecord(
                    g_ReplayCharacter * 3 + g_ReplayShotType, g_ReplayDifficulty, row);'''
    mutations = [
        ('no-shadow', 'drawShadow = 1;', 'drawShadow = 0;', 'shadow-queued'),
        ('short-list', 'row < 25', 'row < 24', 'replay-row-count'),
        ('wrong-pitch', 'position.y += 15.0f;', 'position.y += 18.0f;', 'replay-row-position'),
        ('rebound-header', 'data->replayName', 'file->replayData->replayName', 'replay-retained-header'),
        ('early-overlay', 'frame < 10', 'frame < 9', 'overlay-position'),
        ('wrong-stage', 'g_ReplayCompletionNames[g_FrontEndSelectedStage]', 'g_ReplayCompletionNames[data->completionState]', 'overlay-stage'),
        ('ignore-unavailable', 'menu->scoreUnavailable == 0', 'true', 'score-unavailable'),
        ('unsigned-suffix', 'record->scoreSuffix', 'static_cast<unsigned char>(record->scoreSuffix)', 'score-signed-suffix'),
        ('stale-record', stale_record, '                // Incorrectly retain earlier record.', 'score-live-record'),
        ('wrong-underline', 'menu->nameIndex == 8', 'menu->nameIndex == 7', 'full-name-underline'),
        ('wrong-control-glyph', '? 0x81 :', '? 0x82 :', 'final-control-glyphs'),
        ('wrong-wrap', '% 13 == 12', '% 13 == 11', 'grid-wrap'),
        ('bad-callback', 'return ResultMenuDrawRows(menu);', 'ResultMenuDrawRows(menu); return 0;', 'callback-result'),
    ]
    if not args.no_mutations:
        for name, old, new, label in mutations:
            if owners.count(old) != (2 if name == 'unsigned-suffix' else 1):
                raise ValueError('nonunique mutation: '+name)
            variants.append((name, owners.replace(old, new), label))
        start = owners.index('int __stdcall ResultMenuDrawRows('); end = owners.index('int __fastcall ResultMenuDrawRowsCallback(')
        rows = owners[start:end].replace('    g_AsciiManagerView->drawShadow = 1;', '    AsciiManagerView *cachedAscii = g_AsciiManagerView;\n    cachedAscii->drawShadow = 1;')
        rows = rows.replace('g_AsciiManagerView->', 'cachedAscii->')
        variants.append(('stale-ascii', owners[:start]+rows+owners[end:], 'replay-live-ascii'))
    for name, variant, expected in variants:
        (out/'owners.inc').write_text(variant)
        command = ['nice','-n','10','g++','-std=c++17','-O1','-g','-Wall','-Wextra','-Wno-class-memaccess','-I',str(out),str(ROOT/'scripts/fixtures/result-rows.cpp'),'-o',str(out/'fixture')]
        if args.sanitize: command += ['-fsanitize=address,undefined','-fno-sanitize-recover=all','-fno-pie','-no-pie']
        built = subprocess.run(command, capture_output=True, text=True)
        if built.returncode: raise RuntimeError(name+'\n'+built.stdout+built.stderr)
        result = subprocess.run([str(out/'fixture')], capture_output=True, text=True, env={**os.environ,'ASAN_OPTIONS':'detect_leaks=1'})
        want = (0,'','') if expected is None else (1,'','CHECK '+expected+'\n')
        if (result.returncode,result.stdout,result.stderr) != want: raise RuntimeError(f'{name}: {result.returncode}\n{result.stdout}{result.stderr}')
        print('PASS '+name, flush=True)
    (out/'manifest.json').write_text(json.dumps({'scope':__doc__,'inputs':hashes,'sanitize':args.sanitize,'variants':[v[0] for v in variants]},indent=2)+'\n')


def main():
    p = argparse.ArgumentParser(description=__doc__); p.add_argument('--sanitize',action='store_true'); p.add_argument('--no-mutations',action='store_true'); p.add_argument('--out',type=Path); args = p.parse_args()
    if args.out: run(args,args.out)
    else:
        parent=ROOT/'build/host-tests'; parent.mkdir(parents=True,exist_ok=True)
        with tempfile.TemporaryDirectory(prefix='result-rows-',dir=parent) as directory: run(args,Path(directory))


if __name__ == '__main__': main()
