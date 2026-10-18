# Source/domain and ABI limits

## Device layout and mapper

The compatible native declaration chooses two contiguous IDirectInputDevice8A*
cells: keyboard at Main+0x10 and controller at Main+0x14, with IDirectInput8A* at
+0x0C. This is not recovery of an original array extent. The opaque region +0x18
through +0x47 remains opaque. Successful factory/CreateDevice/EnumDevices lifecycle
evidence establishes the typed cell roles conditionally; it does not establish
runtime initialization success. Device-layout closure certificate SHA-256 is
`12374f4286b4656ed3f6f0b162f2157b283a6583284df6ff677cbe4a93131c26`.

DirectInput admits rawController == 0 only, as supplied by the proved keyboard-owner
call. Address arithmetic for other indices does not prove C++ in-bounds access.
The selected device member is reloaded at each Poll/Acquire/GetDeviceState call;
no extra cached-pointer stability assumption is introduced. WinMM instead requires
a valid initialized JOYCAPSA element indexed by rawController. The API device ID
is its boolean nonzero value, but capability indexing is the raw value. Neither
JOYCAPSA nor input-record allocation count is established. bindingSet must select
a valid initialized input record. Button binding domains are negative or 0..31
for WinMM and negative or 0..127 for DirectInput. Out-of-domain inputs are not
clamped or certified. SDK assertions preserve 32-bit arithmetic, 404-byte JOYCAPSA,
52-byte JOYINFOEX and 272-byte DIJOYSTATE2 layouts. Signed-short thresholds at
Main+0x136/+0x138 are widened before comparisons. DirectInput version is 0x0800,
with explicit A interfaces independent of UNICODE.

## Keyboard and state

The key high-bit predicates belong inside UpdateInput, not artificial independent
mapping helpers. Win32 GetKeyboardState's return does not select a failure branch;
defined API output is a precondition for the deliberately uninitialized buffer.
DirectInput nonzero GetDeviceState status triggers Acquire. Mapper is called with
rawController 0. The record stride is 0x6A; field widths/offsets and 16 counters
are checked. Unknown spans remain actual untouched data, not code padding.

## Callback and unresolved helpers

The callback reads the low byte at context+0x3CC. Its high bit and zero +0x63C
jointly cause the global Main+0x390 store of 3. Calls to 0x421E00, UpdateInput(0),
and ServicePreloadedAnms are unconditional and ordered; the ANM pointer is loaded
after the first two calls. A nonzero service result returns 4. Otherwise a fresh
+0x648 read returns 4 for 2, 1 for other nonzero values, or the unchanged full-width
0x4218D0 result for zero. Normal return, preserved nonvolatile registers, valid
memory, private stack and no asynchronous mutation remain collector assumptions.

The Chain `int __fastcall(void*)` callback/context binding assumes normal
registration lifecycle. The 131-byte registration callee's complete stack/list
semantics remain open. The source-absent 225-byte 0x421E00 helper has a minimal
ordinary void() hypothesis, not a proved signature. The source-absent 572-byte
0x4218D0 helper has an ordinary int(MainSupervisorView*) hypothesis; the actual
target private context transport is EAX, which this declaration does not certify.
Observed register values alone do not establish additional parameters. No adapter
or invented helper implementation is supplied.

Full byte comparisons, relocation acceptance, unresolved helper ABI closure and
old/new affected collateral-owner comparison remain required before promotion.
The chosen source/TU/signature hypotheses are measured baselines, not certification
of original declarations, runtime behavior or compiler acceptance beyond the
specific historical diagnostic receipts.
