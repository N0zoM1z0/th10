# TH10 full Enemy ECL dispatcher: recover actual motion and world-position owners

The original Japanese TH10 v1.00a ECL opcode interpreter is the complete
14,416-byte EnemyRuntimeView::DispatchEclInstruction at 0x0040E770,
from the SHA-256 verified original target
2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.
This C++ source recovery is not a claim that the complete body is exact.

Previously the MOVE ADD_OFFSET_VELOCITY/ADD_BASE_VELOCITY opcodes wrote
two float words through integer-pointer additions at +0x0C/+0x10 to the
selected offset/base EnemyMotionView. The actual existing 0x2C-byte native
EnemyMotionView has velocity.x/y at exactly these offsets. Both writes now
use those typed members, preserving selection, original operand order and
the -999999.0f sentinel guards.

The COPY_PLAYER_TO_BASE opcode and the shared COPY_PLAYER_TO_OFFSET path
also previously copied from an integer +0x1068 offset of the manager's
special enemy full object. The actual EnemyFullObjectView has an embedded
0x14DC-byte EnemyRuntimeView after its 0x103C-byte ECL host prefix;
worldMotion.position is runtime +0x2C, hence full enemy +0x1068.
The base-motion assignment now uses that complete typed three-float
member. The offset-motion path retains three independent **bit-wise**
32-bit stores and reads the corresponding full object position x/y/z
words; it does not turn them into new floating-point arithmetic or
replace the three target stores by an unverified memcpy.

## Pinned compiler outcome, limits and rejected controls

Under the actual 11-source Enemy update MSVC 7.1 SP1 /GL graph, all
changed code was physically compiled and linked. The complete selected
PDB contribution is **14,352 bytes**, compared with the previous **14,348**
candidate and the original **14,416-byte** target. Both native images
have 716 decoded linked fields, but their offset geometry **differs**.
Consequently no candidate/target bytes are counted exact, no unsupported
field normalization is applied, and no new canonical match is added.
See native-result.json for current source SHA-256 and retained PE/MAP/PDB.

Four isolated real-source compiler controls explored typed full-position
copy, typed three-coordinate bit reads, typed movement velocity, and typed
manager slot checks separately. Each emitted the same 14,352-byte owner
with shifted field locations. Two further variants preserved target
source semantics but kept 14,352 with shifted geometry. These results
are not confused with complete target equality or an exact admission.

For exploratory *non-acceptance* triage only, a coarse normalized
Capstone instruction-sequence alignment against the entire original body
found 6,467 aligned candidate instruction bytes in the 14,348-byte
baseline versus 6,533 in the 14,352-byte typed source variants.
This token alignment is not a precise byte comparator, is sensitive to
heuristic matching, and must not be included in exact counts.

The source change is a genuine owner-layout and opcode-meaning recovery,
not proof of historical C++ spelling, game-product/runtime correctness
or portability. Scoped exact helper replays and the seven repository
consistency checks accompany this evidence; no all-unit cold replay.
