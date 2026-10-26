# D3D adapter-format hypothesis and terminal operational failure

Status: unadopted source hypothesis; native experiment stopped before any full-owner result. No exactness credit, successful baseline/candidate measurement, canonical guard result, runtime correction or leak claim is established. Further execution is held; the once-only claim is consumed and cannot be restarted.

## Source hypothesis

Maintained GameWindowView::InitD3DRendering uses the pre-CreateDevice displayMode.format as the AdapterFormat argument of the later CheckDeviceFormat call. Pinned authored reference uses the selected presentation back-buffer format at that point. The quarantined edit changes only that argument to presentParameters.backBufferFormat, retaining receiver, adapter0, HAL type, usage0, texture kind, A8R8G8B8 check format, control flow, failure paths and all other source bytes.

Reference: levzzz5154/th10-decomp commit4e4ab0b73eb75f1d3c2e49b39330deabd5fdcf4a, matching2/msvc/src/core/main/d3d_initialize.cpp, Gitblobfbc82c3d54729ae909c2e772919643d7cd8aa20b, content SHA9ead34f1b09d7521b94af6d75a920ffa5ab41f76002548c137feb2ef625d80bc. This is authored reconstruction evidence, not original TU provenance or target truth. No newer c679 source, upstream implementation/control attributes or helper body is copied. REFERENCE_LICENSE.txt retains the pinned root MIT attribution; provenance.json records source/reference hashes.

Ordinary finite witness: desktopR5G6B5(23), fullscreen colorMode0 selectsX8R8G8B8(22), options permit the post-creation probe and HAL creation succeeds. The original source requests23, the hypothesis22. Matching fullscreen/windowed formats provide equal-argument controls. Driver support and successful creation are explicit premises. The mock does not establish real GPU capability responses. Microsoft documents AdapterFormat as adapter display-mode format and presentation parameters as input/output; these API contracts inform the source question, not target execution:
- https://learn.microsoft.com/en-us/windows/win32/api/d3d9/nf-d3d9-idirect3d9-checkdeviceformat
- https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dpresent-parameters
- https://learn.microsoft.com/en-us/windows/win32/api/d3d9/nf-d3d9-idirect3d9-createdevice

Model limitation: fullscreen options&1 in maintained source also sets colorMode=1, whereas the abstract model leaves input color unchanged. That fixture tests probe suppression only, not distinguishing argument behavior or all initialization state. Eight tests cover the stated finite model, forward/inverse source reconstruction and exact patch; they do not execute C++, callbacks, D3D or scripts.

## Current metadata and proposed natural context

Existing current ledger describes1159 code-only target bytes439890..439D16 endingRET, with adjacentCC padding excluded. Target ABI is blank; static method/name is an adjacent-game hypothesis. Maintained Main.hpp declares static zero-argument InitD3DRendering; real WinMain calls it at Main.cpp173. This source identity is not emitted target ABI proof. Prior normalCOFF/O2/GS1051 mismatch was already known and was not repeated.

The attempted isolated pair selected only the existing SoundJoinRealMain recipe: primarySound.cpp, supportMain.cpp, /GL/GS and genuine WinMain@16 linker request, existing unresolved-symbol-data-anchors-v1 diagnostic harness. Required entry _WinMain@16 and separate D3D source-derived symbol were both bound to support-00-Main.ltcg.obj, with full unique same-start nonoverlapping ownership gates. No alternate graph, private bridge, target crop or new control spelling was tried. Diagnostic anchors do not make the image runnable or establish production linkage. Baseline had to complete ownership/postpins/cleanup before candidate could start.

## Actual terminal outcome

Commandaaf359bd32d044dbb8b56d8bb1787572 completed with exit1, source clean and unchanged at4a113613d9f4a04b8b70ff739f2965461d6352c6. During baseline cold_link the supervisor reported accounting_unresolved at monitoring. No baseline owner/artifact measurement was published and no candidate started. Ordinary client-group cleanup and shared sampling completed; final shared residual8, unknown0, exited_unattested1, all live-failure/grace counts0. Returncode before cleanup and worker exception class were null. Exact sanitized result and payload pins are in evidence.json.

The monitor has several accounting failure checks: process-stat length, process-table size/read errors, negative RSS, known-set capacity and waitid identity/status. It saved the generic reason but not the failing predicate or original supervisor frame. The accepted exited_unattested classification does not identify that cause. A retained-state-only diagnostic was considered and closed without execution because its schema cannot discriminate the historical check. Current process snapshots cannot reconstruct the missing evidence. No raw compiler/native logs or target bytes were read for that assessment.

The operation claim remains consumed. No rebuild, rerun, instrumentation-only repeat, candidate continuation, cleanup retry, gate relaxation or new graph is authorized. Successful ordinary cleanup does not resolve missing accounting coverage. The source hypothesis remains unevaluated by a successful native pair; the previously planned175/301 retained-image guards were not run.

## Reproduce source checks only

Use an explicit absolute pristine baseline root whose Main.cpp SHA isfcf7ba4fa5b6af9ce447aa77f2da58eae3c2d53f52b1666b592e9c86b41a40a4 and Main.hpp SHAae49087657d26b2068e9ad73e6994f5fbf1efaebbafed5fad92e508c7938cb03. Run test_candidate.py --baseline-root ROOT with Python; for isolated -I use a fixed runpy launcher inserting only this verified archive directory. Reconstruction reads only those two authored source files and returns bytes in memory. Expected candidate SHA9a9ed9e39a35c026f8b58dea885d6dd86fe314b8c820c5554676c2672431c78e; exact inverse restores pristine baseline. No candidate full source, native artifact, target payload, private operational inventory or execution runner is archived here.
