#!/usr/bin/env python3
"""Compile complete maintained types under GNU i386 for layout/source diagnostics.

No target VC7.1 ABI/codegen claim: attribute macros and two declaration-only C
headers are supplied to avoid host multilib libc dependency. Every production
size/offset assertion stays active; no game declaration is replaced or removed.
"""
from pathlib import Path
import argparse
import subprocess
import tempfile

def require(ok, text):
    if not ok:
        raise RuntimeError(text)

def main():
    ap=argparse.ArgumentParser()
    ap.add_argument('--root',type=Path,default=Path(__file__).resolve().parents[1])
    args=ap.parse_args()
    root=args.root
    with tempfile.TemporaryDirectory(prefix='gui-layout-i386-') as temp:
        temp=Path(temp)
        include=temp/'include'; include.mkdir()
        (include/'stddef.h').write_text('#pragma once\ntypedef __SIZE_TYPE__ size_t;\n#define NULL 0\n#define offsetof(T, m) __builtin_offsetof(T, m)\n')
        (include/'string.h').write_text('#pragma once\n#include <stddef.h>\nextern "C" void *memset(void *, int, size_t);\n')
        (include/'compiler.hpp').write_text('#define __cdecl __attribute__((cdecl))\n#define __stdcall __attribute__((stdcall))\n#define __fastcall __attribute__((fastcall))\n#define __thiscall __attribute__((thiscall))\n#define __declspec(x)\n#define __forceinline inline\n')
        # Prefer proposal files, fall back only to byte-verified snapshots of
        # unchanged production headers when running this local proposal packet.
        paths=['-I'+str(root/'src')]
        if (root/'baseline/src').exists(): paths += ['-I'+str(root/'baseline/src')]
        flags=['g++','-m32','-std=c++98','-fms-extensions','-fsyntax-only',
               '-nostdinc','-I'+str(include),*paths,
               '-include',str(include/'compiler.hpp'),'-Werror','-Wno-invalid-offsetof']
        for filename in ['GuiDraw.cpp','AnmRectangle.cpp']:
            command=flags+[str(root/'src'/filename)]
            result=subprocess.run(command,capture_output=True,text=True,timeout=60)
            require(result.returncode==0 and not result.stdout and not result.stderr,
                filename+' full-header GNU i386 diagnostic failed:\n'+result.stdout+result.stderr)
            print(filename+': complete headers and target layout assertions passed (GNU i386 only)')

if __name__=='__main__':
    main()
