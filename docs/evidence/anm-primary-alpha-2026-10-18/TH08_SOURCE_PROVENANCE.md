# TH08 primary alpha interpolation: source-only provenance

Retrieved 2026-10-18 UTC using the connected GitHub read-only tools. Repository: N0zoM1z0/th08. The main branch metadata resolved to commit 08c7f00f81e35db8206d917b14f47883930beef6 (2026-10-15T01:11:22Z), tree f08387792a2aa2030c54211848ee8b8096c38a0e. Every source file below was then fetched explicitly at that commit, rather than relying on mutable main.

Commit: https://github.com/N0zoM1z0/th08/commit/08c7f00f81e35db8206d917b14f47883930beef6

## Findings

The authored C++ declaration and definition are void AnmVm::StartColor1AlphaInterpolation(i32 duration, i32 mode, i32 initial, i32 final). The definition is in src/PlayerBomb.cpp, lines 102–110, annotated by the repository as TH08 0x40ed50. All formals resolve to int through typedef i32. This annotation is recorded as source metadata only, not independently validated against any binary.

The storage is narrower: interpModes is a u8 array, and color1Initial/color1Final are ZunColor unions whose .a member is u8; u8 is typedef unsigned char. Consequently mode, initial and final undergo implicit unsigned-byte conversion at the helper's assignments (lines 107–109), not in its formal parameter list. Duration is assigned to a ZunTimer member; the timer arrays are declared as ZunTimer, not integer alpha endpoint storage.

TH08 opcode 15 (AnmOpcode_AlphaTimeLinear) and opcode 34 (AnmOpcode_AlphaTime) directly initialize the same storage. They do not call this helper in the inspected switch. Linear assigns raw instruction->intArgs[0] to color1Final.a (line 320); generic assigns GET_INT_VAR(2) to color1Final.a (518) and raw intArgs[1] to the mode byte (515). Both capture initial from color1.a. GET_INT_VAR selects an i32 GetIntVar result or an i32 raw argument. Thus these opcode paths also narrow at storage assignment. No explicit byte cast is needed in this TH08 source because the destination is already unsigned char.

## Scope and limits

This establishes the current, immutable TH08 reconstruction source pattern only. It is newly recovered reference evidence, not the provenance or source of the historical TH10 integer-parameter trial. The parent-provided TH10 knowledge says that earlier trial encountered Wine link crashes and was reverted, with its reference revision/candidate path pruned. Nothing here recovers the discarded candidate or proves TH10's original ABI.

The parent-provided current TH10 context differs materially: StartPrimaryAlphaInterpolation (0x442300, 130-byte target) has int duration and unsigned-char mode/initial/final formals, while its endpoint storage is int. Callers 55/58 narrow final values before entry. Simply importing TH08's int formals without preserving TH10's existing byte normalization would change semantics. This note does not propose or apply an edit or build experiment.

Only repository metadata and authored C++ source were used. No binaries, assembly, disassembly, decompiled traces, repository code execution, builds, external writes, or publication were performed. GitHub code-search discovery also returned incidental repository status/config excerpts; these were not used as evidence for binary equivalence.

## Immutable source evidence

### src/PlayerBomb.cpp
Git blob SHA-1: d266fcfad0eb3075b2af5b2ebdc7a6a07faa3961

https://github.com/N0zoM1z0/th08/blob/08c7f00f81e35db8206d917b14f47883930beef6/src/PlayerBomb.cpp#L102-L110

```cpp
102: // FUNCTION: th08 0x40ed50
103: void AnmVm::StartColor1AlphaInterpolation(i32 duration, i32 mode, i32 initial, i32 final)
104: {
105:     this->interpCurrentTimers[AnmInterp_Alpha1] = 0;
106:     this->interpEndTimers[AnmInterp_Alpha1] = duration;
107:     this->interpModes[AnmInterp_Alpha1] = mode;
108:     this->color1Initial.a = initial;
109:     this->color1Final.a = final;
110: }
```

### src/AnmManager.hpp
Git blob SHA-1: 5d93c69d4297432f6eb831cd7de7767b88c151e1

https://github.com/N0zoM1z0/th08/blob/08c7f00f81e35db8206d917b14f47883930beef6/src/AnmManager.hpp#L330-L341

```cpp
330: struct AnmRawInstr
331: {
332:     i16 opcode;
333:     u16 instructionSize;
334:     i16 time;
335:     u16 varMask;
336:     union {
337:         i32 intArgs[ANM_MAX_ARGS];
338:         f32 floatArgs[ANM_MAX_ARGS];
339:         u8 byteArgs[ANM_MAX_ARGS * sizeof(i32)];
340:     };
341: };
```

https://github.com/N0zoM1z0/th08/blob/08c7f00f81e35db8206d917b14f47883930beef6/src/AnmManager.hpp#L371-L375

```cpp
371:     ZunTimer currentTimeInScript;
372:     ZunTimer waitTimer;
373:     ZunTimer interpCurrentTimers[AnmInterp_Last];
374:     ZunTimer interpEndTimers[AnmInterp_Last];
375:     u8 interpModes[AnmInterp_Last];
```

https://github.com/N0zoM1z0/th08/blob/08c7f00f81e35db8206d917b14f47883930beef6/src/AnmManager.hpp#L453-L456

```cpp
453:     ZunColor color1Initial;
454:     ZunColor color1Final;
455:     ZunColor color2Initial;
456:     ZunColor color2Final;
```

https://github.com/N0zoM1z0/th08/blob/08c7f00f81e35db8206d917b14f47883930beef6/src/AnmManager.hpp#L471-L479

```cpp
471:     i32 UpdatePulsingRadialTrail();
472:     void StartPositionInterpolation(i32 duration, i32 mode, Float3 *initial, Float3 *final);
473:     void StartColor1RgbInterpolation(i32 duration, i32 mode, u32 initial, u32 final);
474:     void StartColor1AlphaInterpolation(i32 duration, i32 mode, i32 initial, i32 final);
475:     void StartScaleInterpolation(i32 duration, i32 mode, Float2 *initial, Float2 *final);
476: 
477:     f32 GetFloatVar(f32 varId);
478:     i32 GetIntVar(i32 varId);
479:     f32 *GetFloatVarPtr(f32 *varPtr, u16 varMask, u32 variableNumber);
```

### src/AnmManager.cpp
Git blob SHA-1: b186010b9650877028c262962663192f2d278006

https://github.com/N0zoM1z0/th08/blob/08c7f00f81e35db8206d917b14f47883930beef6/src/AnmManager.cpp#L215-L217

```cpp
215: #define GET_INT_VAR(argNumber)                                                                                         \
216:     ((instruction->varMask & (1 << argNumber)) ? vm->GetIntVar(instruction->intArgs[argNumber])                        \
217:                                                : instruction->intArgs[argNumber])
```

https://github.com/N0zoM1z0/th08/blob/08c7f00f81e35db8206d917b14f47883930beef6/src/AnmManager.cpp#L318-L325

```cpp
318:         case AnmOpcode_AlphaTimeLinear:
319:             vm->color1Initial.a = vm->color1.a;
320:             vm->color1Final.a = instruction->intArgs[0];
321: 
322:             vm->interpCurrentTimers[AnmInterp_Alpha1] = 0;
323:             vm->interpEndTimers[AnmInterp_Alpha1] = GET_INT_VAR(1);
324:             vm->interpModes[AnmInterp_Alpha1] = AnmInterpMode_Linear;
325:             break;
```

https://github.com/N0zoM1z0/th08/blob/08c7f00f81e35db8206d917b14f47883930beef6/src/AnmManager.cpp#L512-L519

```cpp
512:         case AnmOpcode_AlphaTime:
513:             vm->interpCurrentTimers[AnmInterp_Alpha1] = 0;
514:             vm->interpEndTimers[AnmInterp_Alpha1] = GET_INT_VAR(0);
515:             vm->interpModes[AnmInterp_Alpha1] = instruction->intArgs[1];
516: 
517:             vm->color1Initial.a = vm->color1.a;
518:             vm->color1Final.a = GET_INT_VAR(2);
519:             break;
```

### src/ZunColor.hpp
Git blob SHA-1: 690ae2babbc2fe895b8df12e7376b2d25cdee723

https://github.com/N0zoM1z0/th08/blob/08c7f00f81e35db8206d917b14f47883930beef6/src/ZunColor.hpp#L20-L29

```cpp
20: union ZunColor {
21:     u32 d3dColor;
22:     struct
23:     {
24:         u8 b;
25:         u8 g;
26:         u8 r;
27:         u8 a;
28:     };
29: };
```

### src/inttypes.hpp
Git blob SHA-1: 7a05f1fd568731150d9b59ccf316aab926b31a22

https://github.com/N0zoM1z0/th08/blob/08c7f00f81e35db8206d917b14f47883930beef6/src/inttypes.hpp#L3-L8

```cpp
3: typedef char i8;
4: typedef unsigned char u8;
5: typedef short i16;
6: typedef unsigned short u16;
7: typedef int i32;
8: typedef unsigned int u32;
```

