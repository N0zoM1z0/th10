# TH10 DirectInput initialization thread @0x0043BA90 — 83-byte exact

Original Japanese 1.00a body 0x0043BA90..0x0043BAE2 (83 bytes, RET) is
reconstructed in the maintained `InputDeviceInitThread` source. This is a
real thread entry referenced from the runtime setup code, not a fabricated
callback. It clears the 0x200 and 0x400 input presence bits of the genuine
`g_FrontEndSupervisorFlags` at 0x00491FF4, calls the real
`InputInitializeDevices` owner at 0x0043B8D0 on the original main supervisor
at 0x00491C28, then refreshes the same two bits from the actual keyboard
pointer at 0x00491C38 and joystick pointer at 0x00491C3C using the target's
XOR/AND schedule. Its EAX return is the final flags word. Original name,
physical TU and full peripheral runtime behavior remain unproved.

The previously built three-source VC7.1 `/GL /GS /EHs` graph emitted 85
bytes, with an extra EBX save/restore pair. That diagnostic **predated** the
real `InputDeviceOwnerView::ReleaseDirectInput8` reconstruction in the 439-byte
initialization owner. With the actual maintained `InputDirectInputInit.cpp`
primary source and the unchanged `InputDirectInputDeviceEnum.cpp` and
`InputDirectInputRange.cpp` support sources, the compiler now naturally
produces the entire **83-byte** native thread body (55/55 ordinary bytes),
including the correct private cleanup register schedule. Nothing was patched
in the original executable or forced via inline assembly.

The unit `input-initialize-device-thread-0043ba90` uses
`InputDeviceInitThread` as its actual pinned VC7.1 SP1 link entry. Two
independent zero-reuse cold-compiled PE images yield different image hashes
and PDB GUIDs; the full PDB-owned 83-byte function passes unchanged strict
comparison both times. Seven target-bound fields: three references to the
0x491FF4 flags, supervisor 0x491C28, keyboard 0x491C38, joystick 0x491C3C,
and the real 0x43B8D0 initializer call. The generated comparisons are saved
privately as `.analysis/gpt-web/20261103/input-device-thread-cold{1,2}.json`.

**Exactness is limited to the 83-byte thread.** The 439-byte
`InputInitializeDevices` still differs in the native graph (around 409/439),
so its complete owner is NOT exact. Its 21-byte COM release helper and the
52-/98-byte enumeration callbacks remain independent exact functions. We
run only those four affected unit cold replays, without a full historical
cold replay or whole-game runtime/product claim.
