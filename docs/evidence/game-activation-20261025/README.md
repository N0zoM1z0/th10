# Game activation: complete isolated pilot, 2026-10-25

Status: **parked; structural rejection, no source/exact credit**.
Baseline38cf50e4829020b9547fa0730ba34a1fc98cc98a. Reviewed authored owners
405860..4059EC (397 bytes) and405500..405550 (81 bytes) remain source-absent.

## Early complete comparison

| Model | Activation / target397 | Helper / target81 | Result |
|---|---:|---:|---|
| First complete ordinary-stdcall model | 456 | 89 | structural rejection |
| Target-grounded flags capture before bonus clear | 456 | 89 | whole-body raw identical to first |

The first comparison preceded broader tests. Both models retain complete
owners, not target-sized fragments. Target activation has17 address fields,
12 local branches and4 direct calls; candidate has19 fields,12 branches and6
calls. Helper has no fields and one branch in both target and candidate.
`first-inventory.json`, `capture-inventory.json`, and `target-fields.json`
retain complete inventories. Target lacks usable PE base relocations, so the
raw decoder alone reports only4 REL32 fields: the independent explicit target
operand-role inventory supplies all13 DIR32 fields. This limitation is not
silently treated as complete normalization. Rank is bound to the actual map
public g_GameScoreState418280+58=4182D8, corresponding to474C40+58=474C98;
nearest-anchor labels are not identities.

The actual world creator ABI, including its hidden result in the dead incoming
owner argument slot, emerges naturally. Both target and candidate call it at
offset203. No fake output slot or forced register convention was used. The
known creator remains116/118 nonexact and its parked flags/layer panels were
not reopened. The activation instead inlines power display into three SetSprite
calls where target calls the real79-byte helper. The standalone helper remains
exact in both images. No noinline, fake ABI, profile or entry-point matrix was
used to force that call.

## Recovered source model

- Extend the existing PlayerUpdateGate28 view: timer14, active28, VM ID2C,
  position30, radius3C, value40 and mode44. Preserve the actual global and both
  calls from maintained PlayerUpdateCallbackBody; ordinary stdcall returns
  -1 when already active and0 on activation, cleaning one stack argument.
- Lazy timer initialization followed by reset; capture/copy Player drawPosition,
  set radius32 and value4. Script441 when `(flags378C bit0 && signed spellId
  in93..96) || spellId==109`; otherwise400 or407 by statistics index. The entire
  script-selection branch was omitted by Ghidra and recovered from raw target.
- Actual bullet ANM world creator, then reread position.x for sound38; read
  signed16 power only after sound and call the actual shared power-display
  declaration, moved without duplication from ItemManager.cpp to a header.
- Recapture EnemyGameState after power display, call helper81, subtract128
  from rank and clamp[-1024,1024], then reread captured panel flags/timer for mode.
- Helper acts only when signed timer.current>=60, clears bonus and bit1 in
  flags378C and frontVms0,1,3,4,5,6,7; index2 intentionally remains untouched.
- Second model captures flags before clearing bonus, matching target def-use
  order. The prediction of earlier bonus store/helper81 was falsified by whole
  raw-byte equality. It does not establish an observable nonvolatile-field bug.

Independent read-only review approved semantics, storage identity, ordinary ABI,
shared declarations, all17 target fields/12 local branches and complete guards.
Both complete owners remain nonexact; no fragment is admitted.

## Graph, guards, execution

Five coherent copied TUs: Player.cpp as actual PlayerUpdateCallback root,
ItemManager.cpp, AnmManager.cpp, AnmVmCreate.cpp and EnemyEclDispatcher.cpp.
Pinned TH10 VC7.1 `/GL`; dispatcher retains its established `/GS` override.
No unresolved-symbol data anchor is represented as a runnable implementation.
These diagnostic images do not establish product closure or runtime semantics.

Five complete same-image guards pass **486 bytes/all9 fields** in each image:
sound149/all3, power display79/all3, FindVm73/all0, AddVm118/all0 and Player
angle67/all3. Frozen proposal/support/header/toolchain inputs and protected nine
D3D files were verified unchanged. Original artifacts are retained under
`.analysis/gpt-dots/game-activation-20261025` and the two
`build/gpt-dots/game-activation-{first,capture}` directories; hashes are in
`artifact-hashes.json`. `neutrality.json` checks actual PDB-owned raw extents,
not only assembly text. Prior proofs are untouched.

- Preflight target/toolchain-execute/tracking/status: exit0,
  repository-command:1da9b7c096614aeba8623c00a972dd32,08:45:47 UTC.
- Native Ghidra check08:45:04 passed target:th10-main,
  factory-native-command; bindingdb1dc1f87be5ed970941be139ad7ed4a8ede593b336111211310389b252428d1.
- First native: exit0, repository-command:305b7cfdf61e48f8a1f50f38ef2639d7.
- Flags-capture native: exit0, repository-command:c871219c9b6e44c4af1a4dbdb9e3765e.
- Both native jobs serialized, one CPU/nice10, bounded300 seconds; durable logs
  and terminal exit files retained. No timeout or duplicate native run.
- Initial reusable-patch preparation failed on ItemManager's CRLF text,
  repository-command:8abac5e59f5c4baeac01a7ad242dddfe. No native run followed.
  The corrected runner normalizes only the three edited existing files, matching
  the original text-mode proposal creation; both patches then reconstructed all
  four edited/new proposal files byte-identically. Failed scratch copies remain
  separate, never relabeled as successful native evidence.

## Reproduction and stopping decision

Use a new output path, one CPU at nice10, bounded native execution and retained
log/exit, for example:

    timeout 300s taskset -c "$CPU" nice -n 10 scripts/repo-python \
      docs/evidence/game-activation-20261025/reproduce.py \
      --trial capture --output-dir build/gpt-dots/game-activation-reproduction

`--trial first` selects the original; `--prepare-only` validates the source
materialization without compiling. The runner verifies target and all112
maintained source/toolchain inputs, applies the retained patch only to an
isolated copy and records complete linked PE/PDB contributions. The retained
patches are proposals, not canonical source admission.

Park unsupported scheduling/register/inline/profile panels. Reopen only with
independent target/source-context evidence that predicts a full-owner change.
No maintained source, recipe, ledger, denominator, metric, runtime or phase-gate
change follows. Prior full CI is not relabeled as a new pass for this packet.

Final docs-only checks pass: toolchain declarations, tracking, regenerated
progress (no metric diff) and `git diff --check`,
repository-command:068631e84a224286b71b55a8a7c1e5d1. Final verification preserves
all285 original artifacts and both123-file frozen native input sets, including
protected9. No full CI was rerun for this evidence-only checkpoint.
