# Sound LoadSoundSample: recover RIFF remaining-size instruction form (2026-11-02)

The maintained complete SoundPlayerView::LoadSoundSample at original target
0x0043D390..0x0043D63B owns **684 bytes**. It remains **NONEXACT**. The
selected native VC7.1 SP1 /GL EndingCreate caller image, with the real
Sound.cpp support TU, now reproduces **678/684** byte locations after
location normalization of 25 decoded address fields. The previous complete
caller-context diagnostic had **676/684**: this is a two-byte improvement,
not a new exact function or partial match.

## Observed compiler/lifetime issue

Target at +0x11A performs ADD EBP,-0x0C, then pushes the "fmt " tag,
moves EBP to EAX, computes the output-size address into EDI, moves
the data cursor into ECX and calls the actual FindWaveChunk. The old
natural source emitted SUB EBP,0x0C and swapped the scheduling of
the MOV EAX,EBP and LEA EDI,[ESP+0x30] instructions.

The retained source snapshots the remaining scan span once:

    remaining -= 12;
    const unsigned int scanRemaining = remaining;
    unsigned char *formatData =
        FindWaveChunk(data, scanRemaining, "fmt ", &chunkSize);
    // The second, conditional FindWaveChunk also uses scanRemaining.

This remains semantically equivalent to the previous source: no statement
modifies remaining between its decrement and two chunk searches. It is a
natural source-lifetime description rather than register forcing.
In the fresh current-source native cold link, ADD EBP,-12 now
matches all three target bytes. The only remaining six ordinary differences
are at +0x122..+0x127 around EAX move versus EDI address-instruction order.

The local PDB identifies the 684-byte complete source contribution
in support-13-Sound.ltcg.obj; complete Capstone decoding covers 684 bytes.
Exactly 25 native address-field locations are accounted for during this
diagnostic comparison. The attached rejection test requires the six
remaining differences; it is deliberately not a full semantic
target-field binding or exact canonical match recipe. The companion [native-facts.json](native-facts.json) also records the 25
native symbol/target destination observations and full PE/MAP/PDB source
fingerprints, without promoting unproven original symbol identities.

## Experiments and scoped native validation

Five initial complete 684-byte controls (negative addition, unsigned addition,
two's-complement spelling, declaration swap, and movement of decrement
ahead of cursor advance) leave eight or ten mismatches. A signed length
local and explicit chunk-output pointer each leave eight differences.
Only the distinct const scan-length snapshot improves codegen to
six differences. All nonselected variants were restored.

The current native image comes from an actual **17-TU selected cold**
ending-create producer; that existing 99-byte exact unit passes.
The affected Sound+ZWave producer was separately cold-built with only the
sound-process-queues and sound-bgm-player-thread selectors, preserving
**1,641/1,641** previously exact bytes across two units. No full-cohort
replay was run.

Reproduction with current source:

    scripts/repo-python scripts/verify-target.py
    nice -n 10 scripts/repo-python scripts/replay-exact-units.py \
      --unit ending-create --json
    scripts/repo-python docs/evidence/sound-sample-lifetime-20261102/verify-native.py \
      --artifact-dir build/match-linked/Ending
    nice -n 10 scripts/repo-python scripts/replay-exact-units.py \
      --unit sound-process-queues --unit sound-bgm-player-thread --json
    scripts/repo-python scripts/ci.py
    git diff --check

No target bytes, ABI, linker settings, or original game file were modified.
The full game/product, source provenance, runtime and portability remain
independent open questions. No config/matches.csv row is added.
