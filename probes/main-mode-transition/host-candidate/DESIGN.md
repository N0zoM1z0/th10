# Independent transition candidate

This host-only C++ candidate is assembled from the finite target obligations, without copying the public reference body. Its abstract hook interfaces deliberately expose mutable context and globals so tests can exercise arbitrary opaque memory mutation. They are not native argument or calling-convention claims. In particular, field 768 and the computed record address are raw uint32 values, not inferred pointers.

## Established construction choices

- Equality uses the initial two observations. Enter can change both fields; the previous-field copy and current-mode dispatch therefore read them again after Enter.
- Every mode uses its independently observed request value. Mode 11 captures that request before setting field 39C to zero.
- Mode 4 calls 42CD50 only for request snapshots 1, 2, 7 or 14. This follows the target and corrects the specific default-branch public-reference hypothesis.
- Mode 10 uses a fresh scalar observation after 418150, and computes the raw 32-bit modular record expression from that same value.
- The normal tail rereads current state, copies it to field 38C, calls Leave, then decrements the byte's post-Leave value. Failure return 4 does not invent a Leave call.

## Pending before semantic acceptance

All 42 fixed source-order pairs now pass the target check (actual cdf65acecace450daa337fd9ce092825). They constrain the optional helper versus variant store in mode 7, variant/publication stores before successor hooks, mode 15 cleanup before the exit store, and the mode 0 result store before failure/normal continuation hooks. These specific ordering choices are supported conditionally when both selected events occur; this is not a complete source-order proof.

Actual 089ae948a8a14785a0ef168e54b6cffe proves the fresh current load precedes the previous/fade/depth stores, and the fresh previous-request load precedes depth/fade stores. Both snapshots now occur immediately after Enter and before the prefix writes. Their relative read order is not claimed; these are independent ordinary field reads with no hook between them. No commutativity assumption across a store is needed.

Actual 690ab76987ed463ea94b74f247ad4ea5 classifies the remaining mode 10 instruction category as one fixed full32 left shift in the record-scale expression. Its register and EFLAGS writes are included. This does not prove flags dead, authorize eliminating effects, or establish full source equivalence. Complete effects and stack/frame restoration remain open. Helper argument types, runtime critical-section ownership and natural native ABI compatibility remain separate gates.

Normal-model premises: valid, distinct ordinary fields; no asynchronous mutation outside the explicitly modeled hooks; normal hook returns; raw uint32 modular arithmetic and uint8 wrap. No layout offsets are implemented by this host struct. No native build is authorized by this packet.

## Reproduction and evidence

Run `python run_tests.py` with Python 3 and g++ available. It compiles only this authored host model in a temporary directory. The fixture covers 666 scenarios, including post-Enter mode/request changes, zero/nonzero creation results, equality returns, uint32 boundary values, arbitrary mutations by each selected helper, and a Leave mutation that forces the decrement to use its new byte value. Twelve independently compiled source mutations are rejected. The call oracle is an independently authored finite obligation model; it is not native trace data or a complete semantic-equivalence oracle.

One important mutation consequence is retained: in mode 11, the variant-zero store precedes helper 418150. That helper may overwrite the variant before 4180E0 observes it. The model must not restore zero after the helper simply because the earlier store was required.

Evidence is the frozen actual snapshot-identity result b5a1a97b70394ce0a2569706956059b2 and the 42-pair precedence result cdf65acecace450daa337fd9ce092825, together with the committed finite partitions checkpoint at cc60c9d667ca26f368bff9b0882c251f2199f52e. The manifest records receipt hashes without embedding their full payloads.

The local `.gitattributes` fixes LF checkout endings for these two authored host C++ files only. Manifest and delivery identities remain raw byte hashes; the repository-wide production C++ policy is unchanged.
