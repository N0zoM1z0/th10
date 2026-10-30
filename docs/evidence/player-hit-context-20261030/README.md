# Complete Player collision-hit source candidate — 2026-10-30

`PlayerHandleCollisionHit` at `426CF0..426F62` is now implemented in
`src/Player.cpp`. Its complete native contribution is **624/627, NONEXACT**.
This adds one source-present authored mapping and no exact bytes or owners.
Original identifiers, class/TU ownership and product/runtime closure remain open.

## Target scope and behavior

The verified Japanese v1.00a image decodes completely into141 instructions,
including the unreachable seven-byte LEA alignment at+0xA9. The sole RET4 is
at+0x270; thirteen CC bytes follow. All seven local branches land on decoded
instruction starts. Ten REL32 calls and eleven independently reviewed absolute
fields account for all21 address fields. Whole `.text` E8/E9 scanning finds only
two references into this owner, both calls to its entry:426761 in the316-byte
rectangle collision check and42695B in the471-byte laser contact check. Both
pass one stack Player pointer and rely on RET4. No encoded E8/E9 enters its
interior. This supplements the existing reviewed boundary, not a new extent.

The body sets Player state4, uses screen position `(drawX+224, drawY+16, drawZ)`,
creates one script0x162 VM and32 script0x163 VMs, and reloads the bullet ANM owner
each loop iteration. Each creation allocates a VM, selects layer0 and flag
0x40000000, calls InitializeVm, adds primary-tail variant0 and positions the ID.
It resets updateTimer1 to0, conditionally queues sound4 when GameManager exists
and flag0x200 is clear, sets highlightTimer to6, and initializes draw script0.
At spell timer>=60 it clears the bonus, panel flag2 and seven front-VM flag2
words, retaining slot2. No failure/lifecycle scenarios are simulated.

Actual target call interfaces are manager ESI for AllocateVm; loaded ECX plus
two stack values/RET8 for InitializeVm; manager EDX, VM EBX and returned-ID storage
EAX for AddVmVariant0; manager EDX, position ESI and stack ID/RET4 for SetVmPosition;
sound owner ECX, sound ID EDI and one stack argument/RET4 for QueueSoundSample;
and loaded ECX, VM EAX, script index EBX for InitializeAndExecuteScriptIndex.
The source uses genuine existing methods and the actual MainSoundOwner array.

## Complete comparisons and parked controls

The first complete source already emits624 bytes, all21 native field roles and
seven fixed local branches. Its allocation/creation loop agrees in structure;
entry state-store/FPU scheduling, timer register/constant placement and panel
store order differ. The script initializer remains197/203 with native
ESI/EDX/EAX arguments, unlike target ECX/EAX/EBX. This is an actual ABI mismatch,
not a field-normalization exception.

Adding EnemyLaser.cpp, LaserClear.cpp and LaserType1Clear.cpp with their existing
pinned profiles supplies the four remaining real initializer call sites. With
the new hit body, all27 direct initializer calls are present, but the complete
initializer, movement, state2 and hit candidates remain unchanged. Caller count
alone does not recover the target interface. This twenty-input control is not
adopted as a new recipe or profile.

A PlayerFloat3 aggregate position copy emits627 bytes but adds moves, loses
loop alignment and remains nonexact. A three-argument AnmFloat3View constructor
is byte/field neutral. Both are restored. Typed mode-ID GetVm and a local
ordinary resolver also remain nonexact; their experiments are restored. Do not
repeat these controls without a distinguishing prediction.

## Reproduction and acceptance limits

The retained source uses the existing canonical seventeen-input /GL graph and
unchanged per-source profiles. Native jobs are serial, CPU0, nice10 and bounded.
A fresh canonical replay selects only the affected explicit exact units:

```sh
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --check
timeout 600 taskset -c 0 nice -n 10 scripts/repo-python scripts/replay-exact-units.py --unit player-calculate-damage-to-target --unit player-auxiliary-damage-at-position --unit player-collision-check --json
```

All three selected units pass1688 bytes/all42 fields in a fresh cold replay.
The first two share the seventeen-input graph containing the new real hit body;
the collision unit retains its independent canonical graph. The hit candidate
itself has no canonical exact unit or matches.csv row. A separate strict check
of the actual collision caller in the new seventeen-input image verifies its
complete body and real hit binding. Eight scoped same-image exact checks,
including the real hit caller and relevant existing effect/sound/timer boundaries,
pass2149 bytes/all50 fields. Eleven prior native candidates and the complete
1616-byte movement receipt retain their bytes and fields. The machine receipt records the cold result,
full native624-byte identity, all21 bound field identities, local destinations
and selected existing exact checks. It grants zero target exact credit.

After obtaining the seventeen-input artifact directory from the replay report:

```sh
scripts/repo-python docs/evidence/player-hit-context-20261030/verify-native.py --artifact-dir build/gpt-6.1-sol/player-hit-context-20261030/hit-final-cold-1
```

The verifier requires the complete PDB contribution, every resolved field and
all local branches. It checks reproducibility of a nonexact candidate only;
it never accepts a partial target match. Private binaries, objects, database,
generated decompiler text and experiment logs stay outside tracked evidence.
