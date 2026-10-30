# Score display and extra-life owners — 2026-10-31

Two newly maintained authored owners are complete exact in the declared pinned
VC7.1 `/GL` caller context:

| Owner | Target | Complete bytes | Bound fields | Local branches |
| --- | --- | ---: | ---: | ---: |
| GameUpdateGui | 417040..4172DE | 671 | 36 | 24 |
| GameAddLives | 4188A0..41892C | 141 | 12 | 1 |

Both dense extents include every physical instruction and exit: the updater has
171 instructions and one plain RET; life addition has 42 instructions and two
plain RETs. One/three CC bytes respectively separate their next neighbors.
Existing reviewed boundaries and authored origins are unchanged. This adds 812
exact authored bytes, two source mappings and two canonical units.

## Target behavior and actual interfaces

The updater's sole target caller is GameManagerUpdateBody. It smooths the
GUI's displayed score with a signed difference divided by 32, a maximum proposed
step of 0x8D55E, minimum nonzero proposal of one, a nondecreasing stored step and
a final remaining-distance clamp. It retains both 100000000 digit transitions,
the high-score/leading-digit publications, nine digit pairs, both leading VMs
and the separate normal/Extra extend thresholds. The target contains the flag4
OR/TEST and physically present script0x4A notice branch even though setting that
bit makes the branch unreachable; the complete source and comparison retain it.
Normal threshold storage is five words at474474, Extra three at474488. Neither
external data storage nor product ownership is established by this code proof.

Life addition mutates the supplied state+30. A result above nine is capped and
skips the successful-extend sound and notice. Otherwise it queues sample44/0,
releases/replaces the GUI notice with script0x4B/layer15, then refreshes the life
VM flags from the **global** state+30 on either exit. Both target callers,
GameUpdateGui and ItemManagerUpdateBody, have maintained source. The old
ItemAddLives extern is replaced by GameAddLives with the two actual inputs in
count-first order. Ordinary C++ and LTCG recover EAX state/ECX amount, plain RET;
GameUpdateGui recovers its ESI GUI receiver. Original argument spelling, names,
classes and translation-unit ownership remain unproved.

## Exact source and call boundaries

The first full natural implementations emitted729/671 and194/141 bytes. The
creator had been inlined into both. A shared noinline definition retained calls
but changed its private return ABI and was restored. The final source uses the
repository's existing **local** inline_depth convention around the two genuine
CreateVmVariant0 calls. No shared ANM source/header or compiler profile changes.

Full four-byte ID clearing through memset preserves the target slot-pointer
lifetimes. Using the existing GameScoreStateView for high score and score
restores repeated state loads and constant reuse. The state-first free and
ordinary member controls both left only the updater's two final MOVs reversed;
the count-first interface reproduces their target order while retaining the
same complete141-byte callee. No fake inputs, register declarations, synthesized
returns, forced inline, padding or target bytes are used.

All48 fields bind intended symbols and bases. In particular score reads use
GameScoreState+4 and life-display reads GameScoreState+30. Those addresses happen
to coincide with unrelated four-byte anchors in the diagnostic image; binding
to those accidental neighboring symbols would be incorrect. The recipes use
explicit base/addend descriptors. The complete local branches remain fixed.

## Reproduction and affected checks

The19-input graph keeps the existing genuine InitializeVm entry and each
source's pinned profile. It includes the actual GameManager and Item callers,
real ANM creators/ID manager, SetSprite and sound-queue definitions. The graph
is an exact comparison context, not a runnable product.

```sh
timeout 900 taskset -c 0 nice -n 10 scripts/repo-python scripts/replay-exact-units.py --unit gui-update-score-display --unit game-add-lives --unit item-rank-add --json
timeout 900 taskset -c 0 nice -n 10 scripts/repo-python scripts/replay-exact-units.py --unit gui-update-score-display --unit game-add-lives --json
scripts/repo-python scripts/ci.py
git diff --check
```

The first cold passes854 bytes across the two new owners and existing AddRank42.
In that same image, all eight selected complete owners pass1490 bytes/all57
fields: the two new owners, AddRank, life display77, GameManager callback7,
CreateVmVariant0 73, SetSprite356 and sound queue123. The second independent cold
is recorded in results.json. All compiler jobs are serial CPU0/nice10, bounded,
with retained exit logs. Only affected selectors are replayed. The changed nonexact ItemManager caller
is separately reviewed at2208/2320 bytes in this graph; it receives no exact credit.

The refreshed17-input baseline of main GUI updater414900 remains nonexact at
3836 bytes with four ordinary slow-timer differences; no control source was retained. The ECL
AddScriptData helper/late-inline controls remain547/589 and were also restored.
Neither investigation contributes exact bytes here. Existing unresolved large
owners, original storage/TU ownership, product closure and runtime remain open.

Private source snapshots, logs and manifests remain below
`.analysis/gpt-6.1-sol/gui-score-display-20261030/`. The final canonical producer
is `build/match-linked/GuiScoreDisplay`; frozen independent copies are below the
corresponding private build campaign. results.json records the cold identities,
source pins, complete bindings and scoped comparisons without private binaries.
