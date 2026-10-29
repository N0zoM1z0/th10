# Player planar bounds source recovery — 2026-10-30

PlayerFloat3::GetPlanarBounds at **004285F0..00428626** is now complete55-byte
exact in two independent canonical cold links, including all four fields. This
adds55 codegen bytes and one source mapping. **Authored origin remains unknown**,
so authored exact bytes stay96,725/275,729. The large damage owner is still
1172/1170 nonexact. The [results](results.json) retain the bounded proofs.

Fresh target decode confirms55 instruction bytes, RET at+36, four preceding and
nine following CC bytes. The helper calculates minimum X/Y and then maximum X/Y,
using half of each extent. It reads extent/center values again after output
stores, which can alias the inputs. It never accesses Z. Ordinary vector-member
source naturally reproduces private ECX center, ESI minimum, EDX maximum and EAX
extent, with no register/ABI annotation. DIR32 fields at+4/+11/+1F/+2C all bind
__real@3f000000 to470B0C; the target payload is the expected float0.5.

The function was a reviewed raw boundary without a maintained mapping. Neither
this source view nor exact compiler output resolves whether its retained entry
was authored directly or generated/retained by the compiler. The original class,
name, TU and original inline relationship to the damage body are unproved.
Boundary and origin ledgers are unchanged. Source presence and complete codegen
are the new claims; authored-byte, product, runtime and portability closure are
separate.

Both target and shot bound calculations now use the shared member. In the
validated ten-input diagnostic the calls inline to the entire prior damage
candidate:1172 bytes, all1060 normalized ordinary bytes and41 local destinations
are preserved. EnemyRuntimeUpdate retains2412 bytes, all2136 ordinary normalized
bytes and89 local destinations. These comparisons target prior native candidates,
not original TH10 exactness. Damage still has permuted bound homes, a load/add/
store rather than memory-add, rotated Y scheduling and alignment differences.

The complete owner was compared early in the existing warm context. Canonical
acceptance uses the established three-input Player-constructor recipe, preserving
/O2 /GL profiles: Player.cpp, AnmManager.cpp and Gui.cpp, real Player constructor
entry. The first explicit cold selects bounds55/all4, allocator181/all1 and the
interrupt helper65/all2:301 bytes/all7 fields pass. A second independent cold
selects only bounds55/all4. All three bodies also pass strict same-image binding
in the ten-input diagnostic. No historical cohort or comparator test was run;
the comparison mechanism is unchanged. No host failure/sanitizer scenario is
required for this exactness claim.

Five bounded large-owner controls were rejected and restored: separate type3
versus other-shot accumulation branches emit1169 but lose target scheduling;
nesting the VM transition in the hit branch is neutral; whole-vector half-size
arithmetic emits1172 but prematurely computes Y; in-place rotation emits1174
and changes x87 lifetimes; coordinate-first multiplication is neutral. The
shared planar call itself preserves the parent, so it does not fix stack homes.
Do not repeat these controls without new evidence and a falsifiable prediction.

Current warm context and full diagnostic field reports are below
`.analysis/gpt-6.1-sol/player-damage-context-20261030/`. final-context.json pins36
recursive project inputs and ten objects. Revalidate pins before cache reuse.
Hash-reconstructible source controls, native logs and both canonical cold outputs
are retained; superseded experimental builds are disposable.

```sh
scripts/repo-python scripts/replay-exact-units.py --unit player-planar-bounds --unit player-spawn-effect-row --unit player-set-shot-vm-interrupt2 --json
scripts/repo-python scripts/replay-exact-units.py --unit player-planar-bounds --json
scripts/repo-python scripts/ci.py
git diff --check
```
