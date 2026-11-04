# TH10 native process launch helper — 201-byte source, nonexact

Original Japanese TH10 v1.00a: `0x00436570..0x00436638`, 201 bytes,
confirmed by the pinned private EXE. Original C++ authorship, entry xrefs and
translation unit remain **unproven**. It is a distinct process-launch routine,
not an owner of the adjacent ResFile memory-stream class.

A fresh read-only Ghidra decompile plus complete x86 disassembly established
that the helper takes the mutable command line in EDX, an ignored ECX input,
then an application name and wait flag on the stack (RET 8). Its 84-byte
stack locals are Win32 `STARTUPINFOA` and `PROCESS_INFORMATION`. The source
in `src/ProcessLaunch.cpp` sets all observed startup fields, including
`CW_USEDEFAULT` window location/size, 80-by-25 console dimensions and
`SW_SHOWDEFAULT`, then calls `CreateProcessA` with `NORMAL_PRIORITY_CLASS`
and no inherited handles, explicit environment or current directory.

On launch failure it returns -1. Without the wait flag it returns 0. With
wait enabled, it polls `GetExitCodeProcess` while the status is
`STILL_ACTIVE` (259), then returns the reported exit code. The original
does **not** close process or thread handles; this source intentionally
preserves that behavior rather than silently inserting cleanup.

The actual pinned VC7.1 SP1 `/GL` build compiles successfully and links a
complete **204-byte PDB owner** versus **201 target bytes**. Target calls
through the CreateProcessA/GetExitCodeProcess IAT are retained, but native
argument-push scheduling differs (notably a late 7-byte application-name
load versus the original early 4-byte load), and loop registers differ.
Four additional natural source controls did not shorten the owner; a fifth
increased it to 215 bytes. **No exact matching, original source ABI/TU,
product closure or runtime correctness is claimed.**

Private reproducible diagnostics:
`.analysis/gpt-web/20261104/probe-process.py`,
`process-variants.py`, `process-variants.json`, and the complete linked
`build/gpt-web/20261104-process-launch/` artifacts. The target is never
copied into Git, and no full historical cold replay was run.
