# Complete Player main timer operand candidate — 2026-10-30

The complete `PlayerUpdateCallbackBody` owner at `425730..426357` now emits
**3108/3112 bytes, NONEXACT**, with112 native address fields versus110 before.
Two scaled decrements retain multiplication by the observed read-only unit
operand. There is no new exact owner or byte credit, source mapping, boundary
change or data-owner claim.

## Target evidence and source limits

The operand at470AFC is float1.0, word3F800000, in `.rdata` with section flags
40000040, without write permission. Both scaled decrement paths load the timer
scale through ECX and multiply by this operand: target instructions at owner
offsets6E0/6E2 and760/762 are `FLD [ECX]` / `FMUL [470AFC]`.
The two fast decrements and two other timer increments also read470AFC.

The maintained source gives this read-only operand a provisional external
`const float` view and passes it to an ordinary private timer-step helper.
The helper uses both real operands and fully inlines under the existing /GL
graph. The public `PlayerTimerView::Subtract(float)` remains unchanged.
No forced inline, register declaration, padding or compiler-profile change is
used. The numeric view is not a source definition: original global versus
compiler-literal-pool ownership, spelling, type ancestry and TU remain unknown.
The diagnostic anchor image does not supply a production data owner or runtime
product. Treat this as a source hypothesis expressing the observed read.

The recovered native four references to this view correspond to the two fast
decrements and two slow-path products. The native slow paths still load the
unit operand first and multiply by `[ECX]`, opposite to target operand order.
Having112 fields is not proof of target bindings or exactness.

## Complete comparison and remaining differences

Target ownership includes3089 code bytes,3 final alignment bytes and the20-byte
five-state table at+0xC14. The candidate has3086 code bytes,2 alignment bytes
and the same20-byte table at+0xC10. All five table destinations retain the exact
target owner-relative offsets:21/234/379/5F5/366. The table base remains four
bytes early. Target and candidate each contain92 direct local branches; the
receipt fixes every native destination independently and grants no waiver for
differences from target.

The early option-count load still moves ahead of the draw-Y store. The effect
loop still uses timer-subframe storage as its EDI base, rather than the target
motion base. This changes operand displacements and the polar helper receiver
setup. Slow decrement multiplication order, two later floating operand orders
and the final alignment also differ. The actual script initializer remains
197/203 with its unresolved private interface, and state2 remains786/796.
The main candidate has no matches.csv row or canonical exact unit.

Integer-count, literal-reference and coordinate-value helpers leave3096/110.
Using the read-only operand recovers3108/112. Reversing multiplication spelling
is neutral; nested scaling, coordinate-pointer and effect-lifetime helpers do
not close the owner. A derived dual cursor restores3112 physical bytes but
changes prefix registers and loop addressing; it is rejected. Passing the new
operand by value adds another floating-order difference and is also restored.
Do not repeat these controls without a distinguishing new observation.

## Reproduction

Only the affected canonical units are selected; compiler/linker jobs remain
serial, CPU0, nice10 and bounded, with pinned VC7.1 profiles:

```sh
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --check
timeout 600 taskset -c 0 nice -n 10 scripts/repo-python scripts/replay-exact-units.py --unit player-calculate-damage-to-target --unit player-auxiliary-damage-at-position --unit player-update-callback --json
```

Damage1170, auxiliary202 and the registered callback7 pass1379 bytes/all35
fields. The shared seventeen-input linked graph rebuilds every object without
reuse. Its main candidate reproduces all3108 owned bytes,112 resolved field
identities,92 local branches, RET4 and all five table entries from the final
warm source. Hit624/all21 and movement1616/all54 receipts remain unchanged;
ten other native candidate identities are preserved, without target credit.

After locating the seventeen-input artifact directory in the replay report:

```sh
scripts/repo-python docs/evidence/player-main-timer-context-20261030/verify-native.py --artifact-dir build/gpt-6.1-sol/player-main-timer-context-20261030/final-cold-1
```

The verifier checks complete native reproduction, including final alignment
and dispatch data, and independently checks the target operand's payload and
read-only section. It is not an exact-match comparator. Source/graph pins and
cold results are in `results.json`; private compiler artifacts and logs stay
under `build/` and `.analysis/`. Historical source-pinned receipts remain
historical; use this checkpoint's pins for the current main candidate.

Authored exact remains100640/275817 across493 owners. Product/runtime closure,
original data/TU ownership and portability remain open.
