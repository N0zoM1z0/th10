# Fresh baseline evidence

This v2 fixture was reconstructed after the execution filesystem was replaced. No earlier packet's results or hashes are reused. Its inputs are the fresh clean `/workspace/shared/th10-baseline-50b971f` source snapshot.

Run `./test-gui-message-run-baseline.py --sanitize` or add `--out evidence-unsanitized` without `--sanitize` for a plain host build. Every compiler uses `flock /tmp/gui-message-host-tests.lock nice -n 10 g++`. There are no remote calls, production writes, native/VC7.1 builds or target execution.

The complete unchanged Run body and its whole original helper namespace are retained, with full canonical header fields. The SDK-only windows.h stub does not replace MainSupervisorView. Target layout assertions are omitted; no host/target layout claim is made.

Explicit seams:
- Exactly three timer constructors initialize flags(0): AnmVmTimerView, PlayerTimerView and EnemyInterpolationTimerView. Raw-integer construction validity is not established.
- Only Next() and IntArgument() method bodies in the temporary canonical record header delegate to an external sidecar. Record fields, including payload[1], remain unchanged. Every sidecar refers to live canonical record objects and a live aligned int. The tests initialize only payload[0] and do not fabricate oversized record types or text payloads. Production flexible-record and unconditional integer-read validity remain excluded.
- Message and owner constructors/destructors are explicit post-construction seams. Real message construction is not exercised. All objects have actual C++ lifetimes and every consumed field is assigned.
- Creation records a live resource receiver and returns a live registered VM ID; unrequested outcalls fail immediately. Actual FindVm, SetVmPosition, MarkVmForDeletion, generic SetInterrupt, SetSprite/WithAnm and timer Add remain unchanged.

Expected fresh results are independently selected:
- opcode2: exit 1, empty stdout, exactly CHECK opcode2.ordinary_uses_player_resource
- side0: exit 1, empty stdout, exactly CHECK opcode7.both_lines_use_side0
- side1: exit 1, empty stdout, exactly CHECK opcode8.both_lines_use_side1
- control: exit 0 with empty stdout/stderr

All CHECK lines have one trailing newline. Controls cover stage-six resource selection, END state/no advance/no tick, unknown dispatch and future-time tick-only. The classifier rejects compiler errors, crashes, signals, timeouts, wrong CHECKs and extra output. Native expectations derive from the supplied target decode: ordinary opcode2 reads the player resource, and both text positions use the selected side.

Manifests record current input, driver, fixture and Run hashes and exact outputs. ASan+UBSan uses detect_leaks=0 because this host's ptrace environment does not support LeakSanitizer; no leak claim follows. This is behavioral host evidence only, with no exactness or runtime-closure claim.
