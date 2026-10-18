# V2 dependency identity correction

V1 start 8007e34c6f9c4e15874fd5513fd9b371 failed before claim or build. Parent diagnosis 519a6bc0c4384102a157444910781d8c found imported pinned Capstone5.0.6 in .tools/capstone but unrelated installed distribution metadata4.0.2. No run directory, phase or receipt existed. V1 packet is preserved unchanged.

V2 changes only dependency_gate to inspect the actual imported capstone.__version__, retaining full pinned component verification before cold_link. No package install or environment change. Tests reproduce the mismatch and verify all four component identities through the exact pinned helper using synthetic non-executable files; component drift and wrong imported versions fail.

Parent authorizes one corrected unchanged-source baseline after independent review and exact readiness verification. New persistent packet directory ends packet-20261018-v2 and fresh run ends20261018-a002. No candidate or overlay authority.
