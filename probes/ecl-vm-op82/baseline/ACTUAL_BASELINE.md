# Actual unchanged-source ECL baseline

Completed parent-run command feeeb3abe6ee47d7a38fc05aedbb57dd on 2026-10-18, using reviewed v2 runner cb83691809cd57f4e6157987fc81dfd5194b5a426c6fa70cb018e4add3b07755 and ready manifest 3756f6c56689cdba2ea9c66c598bacb98aadc8280ce3e2e3732a8416ff56081e.

- Complete strict same-start PDB Run owner: 7020 bytes, matching target extent 7020. All required attribution gates passed. Owner SHA256 98f613dc9838da5b5879304e255c32409b8a0dbb9141f24a7c6d1cd66e42ce39.
- Canonical host-run 146/146 and spawn-thread 142/142 guards exact under their complete linkage normalization. Both raw_equal=false; do not call them raw-identical. Both used image 7ceaba074b38dc02c58cdbfecf7a730aec77876f7cd416b6bfde1e2cb87b84dd.
- baseline_valid=true, one cold_link call, worker elapsed 7.827 seconds. Production clean and all postrun pins valid; HEAD remained a4edda20e434cd40ee99ea3b6b68673a80c3e3be. Process group cleanup confirmed.
- One CPU, nice increment 10, CPU 120s, worker wall 150s, outer 240s, unlimited inherited address space. No native program execution.

The earlier v1 command 8007e34c6f9c4e15874fd5513fd9b371 failed before claim/build. Parent diagnosis 519a6bc0c4384102a157444910781d8c found imported pinned module 5.0.6 but unrelated installed distribution metadata 4.0.2, with no run directory/phase/receipt. V2 corrected only version-source selection; no package/environment change. The prior packet/failure receipt remains retained.

Run owner byte exactness was not tested. Equal extent, owner hash and exact neighboring guards do not establish full-owner equality or grant percentage credit. This is diagnostic baseline evidence, with acceptance_authority=none. No candidate overlay/build or production promotion is authorized by this result; those require their separate parent gate. No native payload leaves the remote environment.

Evidence: remote-baseline-receipt.json and ACTUAL_BASELINE_RESULT.json. Frozen runner, manifest, invocation and existing delivery manifest remain unchanged.
