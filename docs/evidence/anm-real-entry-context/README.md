# Real ANM link-entry contexts

2026-10-23, startingf9b1b7a. Three link-only contexts select existing maintained
ANM entries instead of baseline DispatchEclInstruction. All ten game objects,
two harness inputs, source, headers, profiles and object order remain unchanged.
PE entry RVA, map identity and complete PDB owner are independently validated.

| Selected entry | Full guards | Entry caller difference |
| --- | --- | --- |
| UpdatePrimaryVms |37/38;4741/all124|149 bytes,148 matching; +6 linkedF9 versus targetF8|
| UpdateSecondaryVms |37/38;4767/all124|123 bytes,122 matching; +3 linkedF9 versus targetF8|
| ExecuteScript |38/38;4890/all127|Both additional149/123 callers exact|

Fresh target/Ghidra review confirms primary448860 and secondary448900 call the
executor43EE30. Both full caller contracts are added to the existing36 guards;
no existing contract is dropped. In all three contexts, the three complete large
bodies are raw-identical to the baseline, not merely equal in size or aggregate
scores. ANM remains9964 and7541/7995/90of92/gap2; ECL7020/6993/all189 with the
full171-byte callee exact; Enemy14336/14416. Eight bounded child paths also pass
per image. Entry selection does not repair this ten-object graph's residuals;
none is adopted or treated as original production context evidence.

Every link runs serially at nice10; no source is recompiled. The original
Japanese v1.00a disk target, executing pinned toolchain, tracking/status and
readOnly/noanalysis Ghidra attestations pass. Ghidra uses rollback, maxcpu1 and
heap768m, preserving all412289 mapped bytes with the canonical complete hash.
No full cold, full CI or runtime/product equality is claimed.

[Evidence](evidence.json) binds reused input hashes, actual entries, complete peer
checks, both rejected caller bytes and all removed artifacts. Private producer,
contexts, Ghidra observations and full reports remain under
.analysis/gpt-6.1-sol/anm-real-entry-context-20261023. All48 new carrier files were
hash-verified and removed; the byte count is in the inventory. Maintained source,
canonical outputs, prior useful failures and nine unrelated D3D files remain
unchanged. No new exact credit. Rotate to another reviewed large owner rather
than repeating these link-entry choices.
