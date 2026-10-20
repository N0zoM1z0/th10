#!/usr/bin/env python3
"""Run the complete maintained Midi.cpp with host WinMM/allocation roles.

The canonical header is adapted only for host SDK types/layout assertions.
Allocation call sites are redirected to observable host roles. Three message
bytes are observed; the untouched native high byte, x87 and ABI are not proved.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import tempfile

ROOT=Path(__file__).resolve().parents[1]


def run(args, output):
    output.mkdir(parents=True,exist_ok=True)
    source=(ROOT/'src/Midi.cpp').read_text(); header=(ROOT/'src/Midi.hpp').read_text()
    hashes={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in
            [ROOT/'src/Midi.cpp',ROOT/'src/Midi.hpp',ROOT/'scripts/fixtures/midi-events.cpp']}
    header=re.sub(r'typedef\s+char\s+\w+\s*\[[\s\S]*?\]\s*;', '// HOST native assertion omitted.',header)
    (output/'Midi.hpp').write_text(header)
    (output/'windows.h').write_text('''#pragma once
#include <cstdint>
#define __stdcall
#define __int64 long long
typedef std::uint32_t DWORD;
typedef void *HMIDIOUT;
typedef unsigned int MMRESULT;
''')
    (output/'mmsystem.h').write_text('''#pragma once
#include "windows.h"
struct midihdr_tag {
 char *lpData; DWORD dwBufferLength,dwBytesRecorded; std::uintptr_t dwUser;
 DWORD dwFlags; midihdr_tag *lpNext; std::uintptr_t reserved;
 DWORD dwOffset; std::uintptr_t dwReserved[8];
};
typedef midihdr_tag MIDIHDR;
MMRESULT midiOutPrepareHeader(HMIDIOUT,MIDIHDR*,unsigned int);
MMRESULT midiOutLongMsg(HMIDIOUT,MIDIHDR*,unsigned int);
MMRESULT midiOutUnprepareHeader(HMIDIOUT,MIDIHDR*,unsigned int);
MMRESULT midiOutShortMsg(HMIDIOUT,DWORD);
MMRESULT midiOutReset(HMIDIOUT);
MMRESULT midiOutClose(HMIDIOUT);
MMRESULT timeKillEvent(unsigned int);
MMRESULT timeEndPeriod(unsigned int);
''')
    variants=[('positive',source,None)]
    mutations=[
        ('tempo-standard-base','* 0x101u +','* 0x100u +','tempo-target-recurrence'),
        ('wrong-running-status','status = track->runningStatus;','status = 0x90;','running-status'),
        ('zero-note-held','if (data2 != 0) {','if (true) {','zero-velocity-off'),
        ('loop-after-delta','saved->loopEventTick = saved->nextEventTick;','saved->loopEventTick = saved->nextEventTick + 1;','loop-start'),
        ('lost-loop-clock','output->tempoMicroseconds = output->loopTempoMicroseconds;','output->tempoMicroseconds = 1;','loop-end'),
        ('end-track-schedules','track->active = 0;\n                return;','track->active = 0;\n                break;','end-track-return'),
        ('wrong-ring-wrap','(output->pendingLongHeaderIndex + 1) % 32','(output->pendingLongHeaderIndex + 1) % 31','sysex-ring-wrap'),
        ('prepare-ignored','if (midiOutPrepareHeader(output->outputHandle, header, sizeof(*header)) != 0)','if ((midiOutPrepareHeader(output->outputHandle, header, sizeof(*header)), false))','sysex-prepare-failure'),
        ('lost-live-delta','MidiReadVariableLength(&track->cursor));\n}', '0);\n}', 'program-change'),
        ('wrong-negative-event','static_cast<unsigned __int64>(\n                    static_cast<__int64>(track->nextEventTick)) > eventTime','track->nextEventTick > static_cast<__int64>(eventTime)','negative-event-not-due'),
        ('narrow-tempo-divisor','static_cast<__int64>(output->tempoMicroseconds)','static_cast<unsigned int>(output->tempoMicroseconds)','signed-tempo-extension'),
        ('early-fade-end','fadePosition >= fadeDuration','fadePosition > fadeDuration','fade-terminal'),
        ('reset-zero-active','track->active = 1;','track->active = 0;','reset-track-state'),
    ]
    if not args.no_mutations:
        for name,old,new,label in mutations:
            assert source.count(old)==1,(name,source.count(old))
            variants.append((name,source.replace(old,new),label))
    for name,text,expected in variants:
        # Explicit host boundary substitution; the maintained owner logic is intact.
        text=text.replace('malloc(', 'HostMalloc(').replace('free(', 'HostFree(')
        (output/'Midi.cpp').write_text(text)
        command=['nice','-n','10','g++','-std=c++17','-O1','-g','-Wall','-Wextra',
                 '-Wno-class-memaccess','-Wno-implicit-fallthrough','-I',str(output),
                 str(ROOT/'scripts/fixtures/midi-events.cpp'),'-o',str(output/'fixture')]
        if args.sanitize: command+=['-fsanitize=address,undefined','-fno-sanitize-recover=all','-fno-pie','-no-pie']
        built=subprocess.run(command,capture_output=True,text=True)
        if built.returncode:raise RuntimeError(name+'\n'+built.stdout+built.stderr)
        result=subprocess.run([str(output/'fixture')],capture_output=True,text=True,
                              env={**os.environ,'ASAN_OPTIONS':'detect_leaks=1'})
        want=(0,'','') if expected is None else (1,'','CHECK '+expected+'\n')
        if (result.returncode,result.stdout,result.stderr)!=want:
            raise RuntimeError(f'{name}: {result.returncode}\n{result.stdout}{result.stderr}')
        print('PASS '+name,flush=True)
    (output/'manifest.json').write_text(json.dumps({'scope':__doc__,'inputs':hashes,
        'sanitize':args.sanitize,'variants':[v[0] for v in variants]},indent=2)+'\n')


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--sanitize',action='store_true')
    parser.add_argument('--no-mutations',action='store_true')
    parser.add_argument('--out',type=Path)
    args=parser.parse_args()
    if args.out:run(args,args.out)
    else:
        parent=ROOT/'build/host-tests';parent.mkdir(parents=True,exist_ok=True)
        with tempfile.TemporaryDirectory(prefix='midi-events-',dir=parent)as temp:run(args,Path(temp))


if __name__=='__main__':main()
