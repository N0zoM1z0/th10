#!/usr/bin/env python3
"""Project the actual ASCII viewport transitions with explicit service roles.

Checks publication, flush-time selection, camera/device reloads and final flags.
Camera internals and target layouts/ABI are outside this host-only projection.
"""

from pathlib import Path
import argparse
import hashlib
import json
import os
import subprocess
import tempfile


ROOT = Path(__file__).resolve().parents[1]


def compound(text, start):
    opening = text.index("{", start)
    depth = 0
    for index in range(opening, len(text)):
        if text[index] == "{":
            depth += 1
        elif text[index] == "}":
            depth -= 1
            if not depth:
                return text[start:index + 1]
    raise ValueError("unclosed source block")


def replace(text, before, after, count):
    if text.count(before) != count:
        raise ValueError(f"source mutation expected {count} occurrences of {before!r}")
    return text.replace(before, after)


def run(args, output):
    output.mkdir(parents=True, exist_ok=True)
    source = ROOT / "src/AnmManager.cpp"
    text = source.read_text()
    macro_start = text.index("#define CONFIGURE_ASCII_VIEWPORT(viewportIndex)")
    macro_end = text.index("\n    } while (0)", macro_start) + len("\n    } while (0)")
    macro = text[macro_start:macro_end] + "\n"
    functions = []
    for markers in [
        ["int __stdcall AsciiManagerDrawStrings(AsciiManagerView *manager)"],
        ["int __stdcall AsciiManagerDrawGuiStrings(AsciiManagerView *manager)",
         "int AsciiManagerView::DrawGuiStrings()"],
    ]:
        found = [marker for marker in markers if text.count(marker) == 1]
        if len(found) != 1:
            raise ValueError("ambiguous ASCII renderer source")
        body = compound(text, text.index(found[0]))
        blocks = []
        for condition in ["if (currentViewport != string->viewportIndex)",
                          "if (currentViewport != 0)"]:
            if body.count(condition) != 1:
                raise ValueError("ambiguous viewport transition")
            blocks.append(compound(body, body.index(condition)))
        functions.extend(blocks)

    variants = [("positive", macro, functions, None)]
    swapped = replace(macro, "&main->backgroundCamera", "&main->temporaryCamera", 1)
    swapped = replace(swapped, "&main->gameplayCamera", "&main->backgroundCamera", 1)
    swapped = replace(swapped, "&main->temporaryCamera", "&main->gameplayCamera", 1)
    variants.append(("swapped-cameras", swapped, functions, "camera-selection"))
    variants.append(("unpublished-camera", replace(macro,
        "main->activeViewport = reinterpret_cast<AnmViewportOwnerView *>(camera);",
        "/* publication omitted */", 2), functions, "camera-publication"))
    variants.append(("cached-viewport", replace(macro,
        "&main->activeViewport->viewport",
        "&reinterpret_cast<AnmViewportOwnerView *>(camera)->viewport", 2),
        functions, "viewport-reload"))
    cached = replace(macro, "StageConfigureCamera(camera);",
        "D3d9DeviceView *device = g_Direct3DDevice; StageConfigureCamera(camera);", 2)
    cached = replace(cached, "g_Direct3DDevice->vtable->SetViewport(",
                     "device->vtable->SetViewport(", 2)
    cached = replace(cached, "g_Direct3DDevice, &main", "device, &main", 2)
    variants.append(("cached-device", cached, functions, "device-reload"))
    early = macro
    for value, camera in [(0, "backgroundCamera"), (1, "gameplayCamera")]:
        early = replace(early, f"main->viewportConfigured = {value};", "", 1)
        early = replace(early, f"StageCameraStateView *camera = &main->{camera};",
            f"main->viewportConfigured = {value}; StageCameraStateView *camera = &main->{camera};", 1)
    variants.append(("early-flag", early, functions, "flag-order"))
    inverted = replace(macro, "viewportConfigured = 0;", "viewportConfigured = 2;", 1)
    inverted = replace(inverted, "viewportConfigured = 1;", "viewportConfigured = 0;", 1)
    inverted = replace(inverted, "viewportConfigured = 2;", "viewportConfigured = 1;", 1)
    variants.append(("inverted-flag", inverted, functions, "final-flag"))
    stale = list(functions)
    for index in [0, 2]:
        stale[index] = replace(stale[index],
            "currentViewport = string->viewportIndex;\n            g_AnmRenderManagerView->FlushVertexBuffer();",
            "g_AnmRenderManagerView->FlushVertexBuffer();\n            currentViewport = string->viewportIndex;", 1)
    variants.append(("late-selection", macro, stale, "camera-selection"))
    variants.append(("missing-camera", replace(macro, "StageConfigureCamera(camera);",
        "/* camera call omitted */", 2), functions, "call-order"))

    prefix = r'''
#include <climits>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
struct AnmViewportView { int identity; };
struct AnmViewportOwnerView { AnmViewportView viewport; };
// The camera role exposes only the viewport, not a target memory layout.
using StageCameraStateView = AnmViewportOwnerView;
struct MainSupervisorAnmPrefixView {
    StageCameraStateView backgroundCamera, gameplayCamera;
    AnmViewportOwnerView *activeViewport;
    int viewportConfigured;
};
MainSupervisorAnmPrefixView g_MainSupervisorView;
struct D3d9DeviceView;
struct Vtable { void (*SetViewport)(D3d9DeviceView *, const AnmViewportView *); };
struct D3d9DeviceView { Vtable *vtable; };
D3d9DeviceView *g_Direct3DDevice;
struct Record { int viewportIndex; };
int phase;
bool mutate;
Record *record;
StageCameraStateView *expectedCamera;
AnmViewportOwnerView replacement;
D3d9DeviceView firstDevice, secondDevice;
void check(bool condition,const char *message) {
    if(!condition) throw std::runtime_error(message);
}
struct Renderer {
    void FlushVertexBuffer() {
        check(phase==0,"call-order"); phase=1;
        record->viewportIndex=record->viewportIndex==0?1:0;
    }
} renderer;
Renderer *g_AnmRenderManagerView=&renderer;
void StageConfigureCamera(StageCameraStateView *camera) {
    check(phase==1,"call-order");
    check(camera==expectedCamera,"camera-selection");
    check(g_MainSupervisorView.activeViewport==camera,"camera-publication");
    check(g_MainSupervisorView.viewportConfigured==77,"flag-order"); phase=2;
    if(mutate) {
        g_MainSupervisorView.activeViewport=&replacement;
        g_MainSupervisorView.viewportConfigured=99;
        g_Direct3DDevice=&secondDevice;
    }
}
void setViewport(D3d9DeviceView *device,const AnmViewportView *viewport) {
    check(phase==2,"call-order");
    check(device==(mutate?&secondDevice:&firstDevice),"device-reload");
    check(viewport==(mutate?&replacement.viewport:&expectedCamera->viewport),"viewport-reload");
    check(g_MainSupervisorView.viewportConfigured==(mutate?99:77),"flag-order"); phase=3;
}
'''
    chunks = [prefix]
    for index, (_, code, blocks, _) in enumerate(variants):
        chunks.append(f"namespace variant{index} {{\n" + code)
        for renderer_index in [0, 1]:
            transition, tail = blocks[renderer_index * 2:renderer_index * 2 + 2]
            chunks.append(f"int transition{renderer_index}(int currentViewport,Record *string) {{\n"
                          + transition + "\nreturn currentViewport;\n}\n")
            chunks.append(f"void tail{renderer_index}(int currentViewport) {{\n" + tail + "\n}\n")
        chunks.append("#undef CONFIGURE_ASCII_VIEWPORT\n}\n")
    chunks.append(r'''
void cases(int (*transition)(int,Record *),void (*tail)(int)) {
    for(int selected: {0,1,-1,2,INT_MIN,INT_MAX})
    for(int mode: {0,1,2}) for(bool callbacks: {false,true}) {
        Record entry{selected}; record=&entry; mutate=callbacks; phase=0;
        g_MainSupervisorView.activeViewport=&replacement;
        g_MainSupervisorView.viewportConfigured=77;
        Vtable table{setViewport}; firstDevice.vtable=secondDevice.vtable=&table;
        g_Direct3DDevice=&firstDevice;
        bool changes=mode==0 || (mode==2 && selected!=0);
        expectedCamera=(mode==2 || selected==0)?&g_MainSupervisorView.gameplayCamera:
                                                 &g_MainSupervisorView.backgroundCamera;
        if(mode==2) tail(selected);
        else check(transition(mode==0?42:selected,&entry)==selected,"selected-snapshot");
        check(phase==(changes?3:0),"call-order");
        check(g_MainSupervisorView.viewportConfigured==(changes?(mode==2||selected==0?1:0):77),"final-flag");
    }
}
int main(int argc,char **argv) {try {
''')
    for index, (name, _, _, _) in enumerate(variants):
        chunks.append(f'if(argc==2 && std::strcmp(argv[1],"{name}")==0) {{ '
                      f'cases(variant{index}::transition0,variant{index}::tail0); '
                      f'cases(variant{index}::transition1,variant{index}::tail1); '
                      'std::cout<<"72 scenarios passed\\n";return 0;}\n')
    chunks.append('return 3;}catch(const std::exception& e){std::cerr<<e.what()<<"\\n";return 2;}}\n')
    cpp = output / "viewport.cpp"
    cpp.write_text("".join(chunks))
    executable = output / "viewport"
    command = ["g++", "-std=c++17", "-O1", "-g", str(cpp), "-o", str(executable)]
    if args.sanitize:
        command[1:1] = ["-fsanitize=address,undefined", "-fno-omit-frame-pointer", "-fno-pie", "-no-pie"]
    subprocess.run(command, check=True)
    environment = {**os.environ, "ASAN_OPTIONS": "detect_leaks=1:halt_on_error=1",
                   "UBSAN_OPTIONS": "halt_on_error=1"}
    results = {}
    for name, _, _, message in variants:
        result = subprocess.run([str(executable), name], env=environment,
                                capture_output=True, text=True)
        expected = 0 if message is None else 2
        if result.returncode != expected or (message and result.stderr.strip() != message):
            raise RuntimeError(f"{name}: unexpected exit/message: {result.returncode}, {result.stderr}")
        results[name] = {"exit": result.returncode, "predicate": message}
    receipt = {"scope": __doc__, "source_sha256": hashlib.sha256(source.read_bytes()).hexdigest(),
               "scenarios": 72, "functional_mutants": len(variants) - 1,
               "sanitize": args.sanitize, "results": results}
    (output / "receipt.json").write_text(json.dumps(receipt, indent=2) + "\n")
    print("ASCII viewport projection: 72 scenarios and 8 strict functional mutants passed")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--out", type=Path)
    parser.add_argument("--sanitize", action="store_true")
    args = parser.parse_args()
    if args.out:
        run(args, args.out.resolve())
    else:
        (ROOT / "build").mkdir(exist_ok=True)
        with tempfile.TemporaryDirectory(prefix="host-ascii-viewport-", dir=ROOT / "build") as output:
            run(args, Path(output))


if __name__ == "__main__":
    main()
