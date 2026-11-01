# Historical ANM child91 static-ABI experiment — 2026-11-01

This directory records an intermediate experiment. A static
`__stdcall CreateVmVariant2(AnmLoadedView *, int, unsigned int)` overload
produced a byte-exact 73-byte leaf, but it was not the best source model for the
target: the target function is a non-static `AnmLoadedView` member using
`__stdcall`, with implicit `this` stack-passed before the two explicit args.

The maintained source, canonical match-unit symbol, and current parent binding
now use the member symbol `?CreateVmVariant2@AnmLoadedView@@QAG?AUAnmVmIdView@@HI@Z`.
See [the selected member-ABI proof](../anm-child91-member-stdcall-20261101/README.md).

The old static artifact remains useful as a byte-equivalent ABI experiment, not
as the selected source declaration or symbol identity. Its parent result was
9,962/9,964; the EDX/ECX caller pair remains open.
