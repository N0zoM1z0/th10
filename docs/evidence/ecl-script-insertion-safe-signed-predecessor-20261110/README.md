# ECL name table insertion: signed predecessor after the loop guard

The original Japanese TH10 v1.00a SHA-256 is 2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.

The 589-byte EclVmScriptDatabase::AddScriptData at 0x00450220 inserts a named subroutine into the sorted table. During backward movement, the real loop condition index > insertionIndex is checked before the previous-table entry is addressed. Therefore, inside the body index cannot be INT_MIN: there is no smaller representable int for insertionIndex. Signed index - 1 is defined at every executed path. This source change replaces an unnecessary unsigned subtraction and narrowing with simple index - 1, eliminating the implementation-defined conversion from the source without changing the original low-word behavior.

Four pinned VC7.1 SP1 EclVm/Enemy /GL link experiments compared baseline, the simple signed expression, an inlined signed/unsigned alias helper, and an explicit signed cast. Every variant generated the same complete 589-byte AddScriptData contribution with 502/581 comparable ordinary bytes plus two calls in the same positions. The complete 7,020-byte EclVmContext::Run retained 6,997/7,020 original bytes and all 189 original-bound code/data fields; the exact 136-byte FindSubroutine remained unchanged. No new exact-codegen credit is claimed for the 589-byte owner, and no target byte patch or fabricated ABI was used.

Local source/receipt: .analysis/gpt-web/20261110-ecl-previous-table-index.py, build/gpt-web/20261110/ecl-previous-table-index/results.jsonl. Verify with the selected ecl-vm-find-subroutine / ecl-vm-host-run exact units and strict original-bound AddScriptData and Run comparisons; no all-unit cold replay is necessary.
