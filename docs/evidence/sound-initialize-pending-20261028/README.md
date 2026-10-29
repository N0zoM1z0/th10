# Sound initialization experiment, resumed 2026-10-29

**The pending producer has now been run successfully.** The explicit cbSize
assignment produces755/786 bytes and remains nonexact. It is retained in the
source because raw target43CE4D/43CE92 writes that field twice. Do not apply the
historical patch again to the current source. Subsequent CFG/order controls are
also nonexact and have been restored. The complete primary-format callee260/all3
is now independently proven exact; see the
[continuation proof](../sound-primary-format-20261029/README.md) and its results.

The following records the original stopped state before that continuation.

This is **unverified work**, preserved when automatic goal work stopped after
`fb521c1`. No new match or source credit is claimed. The production source has
been restored to that exact checkpoint; [cbsize.patch](cbsize.patch) preserves
the one-line experiment.

The owner is `SoundPlayerView::Initialize`, target `43CD30..43D041`/786 bytes.
The retained current-source native baseline is 754 bytes. Raw target stores
suggest an explicit `WAVEFORMATEX::cbSize = 0` after its other format fields.
That source hypothesis has not received a complete linked comparison.

The first producer stopped with exit 1 before producing an image: its cache
interceptor attempted to reuse generated `link-anchors.c` as a project input,
raising KeyError. This was a runner failure, not a rejected source model.
The ignored runner now tests membership in its known object map before reuse;
generated inputs use the ordinary compiler. Only Python syntax was checked.

Resume from the repository root:

```sh
git apply docs/evidence/sound-initialize-pending-20261028/cbsize.patch
```

Use the retained `.analysis/gpt-6.1-sol/sound-initialize-20261028/probe.py` with a
new variant name, serially on one CPU at nice10 and with timeout/exit logs. Its
base is the preserved Sound `final-main-current` fifteen-input graph. It
recompiles Sound and reuses 14 hash-verified objects. If another input/header
changes, rebuild the affected inputs instead of weakening the pin check.

The real Main context matters: the two-input diagnostic has an unresolved Log
anchor and a different call ABI; the real context includes GameErrorContext and
emits the target ECX receiver. Equal 754-byte sizes do not make those artifacts
equivalent. Retained target/baseline listings, original failure log/exit and the
repaired runner remain in that active analysis directory.

Another untested lead is the success-first initialization CFG: target cleanup
sits after the successful body. Do not assume either hypothesis is correct;
compare the complete 786-byte owner early, preserving all fields and ABI checks.
