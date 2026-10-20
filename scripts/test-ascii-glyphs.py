#!/usr/bin/env python3
"""Project actual per-record ASCII setup and glyph loops against target traces.

Typed host roles observe glyph positions, dimensions, dirty flags, shadow color
and rounded/non-rounded selection. Queue traversal, viewport changes, renderer
internals, i386 layout/ABI and original/native-product execution are excluded.
Only exact integer-valued geometry and a GUI unordered-comparison control are
used; this is not an x87 precision or control-word proof.
"""

from pathlib import Path
import argparse
import hashlib
import json
import os
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]


def extract(text, marker):
    if text.count(marker) != 1:
        raise ValueError("ambiguous renderer definition")
    start = text.index(marker)
    prepare = text.index("        manager->primaryVm014.position = string->position;", start)
    viewport = text.index("        if (currentViewport != string->viewportIndex)", prepare)
    cursor = text.index("        text = reinterpret_cast<unsigned char *>(string->text);", viewport)
    loop = text.index("        while (*text != '\\0')", cursor)
    opening = text.index("{", loop)
    depth = 0
    for index in range(opening, len(text)):
        if text[index] == "{":
            depth += 1
        elif text[index] == "}":
            depth -= 1
            if depth == 0:
                return text[prepare:viewport], text[cursor:index + 1]
    raise ValueError("unclosed glyph loop")


def change(text, before, after, count=1):
    if text.count(before) != count:
        raise ValueError(f"mutation expected {count} occurrences: {before!r}")
    return text.replace(before, after)


def run(args, output):
    output.mkdir(parents=True, exist_ok=True)
    source = ROOT / "src/AnmManager.cpp"
    text = source.read_text()
    regular = extract(text, "int __stdcall AsciiManagerDrawStrings(AsciiManagerView *manager)")
    gui = extract(text, "int __stdcall AsciiManagerDrawGuiStrings(AsciiManagerView *manager)")
    variants = [("positive", regular, gui, None)]
    spacing = "                manager->primaryVm014.position.x += spaceWidth;\n            }"
    outside = "            }\n            manager->primaryVm014.position.x += spaceWidth;"
    variants.append(("newline-spacing", (regular[0], change(regular[1], spacing, outside)),
                     (gui[0], change(gui[1], spacing, outside)), "glyph-position"))
    variants.append(("missing-glyph-dirty", (regular[0], change(regular[1],
        "manager->primaryVm014.flags35C |= 8u;", "/* dirty write omitted */")),
        gui, "glyph-dirty"))
    variants.append(("gui-draw-mode", regular, (gui[0], change(gui[1],
        "manager->primaryVm014.scaleX == 1.0f", "manager->primaryVm014.scaleX != 1.0f")),
        "draw-mode"))

    prefix = r'''
#include <cmath>
#include <cstring>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>
struct AnmFloat3View {float x,y,z;};
struct AnmSpriteView {float width,height;};
struct Color {unsigned int value;};
struct AnmVmView {
    AnmFloat3View position;
    float scaleX,scaleY,spriteWidth,spriteHeight;
    unsigned int flags35C;
    AnmSpriteView *loadedSprite;
    Color primaryColor;
};
struct Loaded {AnmSpriteView *sprites;};
struct AsciiManagerView {AnmVmView primaryVm014;Loaded *asciiAnm;int spaceWidth;};
struct AsciiManagerStringView {
    char text[64];AnmFloat3View position;float scaleX,scaleY;
    int smallFont,drawShadow;unsigned int color;
};
struct Event {char mode;float x,y,width,height;unsigned int color;int sprite;bool dirty;};
std::vector<Event> observed;
AnmSpriteView *spriteBase;
struct Renderer {
    int draw(AnmVmView *vm,char mode) {
        observed.push_back({mode,vm->position.x,vm->position.y,vm->spriteWidth,
            vm->spriteHeight,vm->primaryColor.value,int(vm->loadedSprite-spriteBase),
            (vm->flags35C&8)!=0});
        // Service role: expose whether each glyph republishes its dirty bit.
        // This does not assert that the original renderer always clears it.
        vm->flags35C &= ~8u;
        return 0;
    }
    int DrawNoRotation(AnmVmView *vm) {return draw(vm,'R');}
    int DrawNoRotationNoRound(AnmVmView *vm) {return draw(vm,'N');}
} renderer;
Renderer *g_AnmRenderManagerView=&renderer;
void check(bool value,const char *message) {if(!value)throw std::runtime_error(message);}
bool equal(float a,float b) {return a==b || (std::isnan(a)&&std::isnan(b));}
'''
    chunks = [prefix]
    for index, (_, normal, gui_body, _) in enumerate(variants):
        chunks.append(f"namespace variant{index} {{\n")
        for name, (setup, loop) in [("regular", normal), ("gui", gui_body)]:
            chunks.append(f"void {name}(AsciiManagerView *manager,AsciiManagerStringView *string) {{\n"
                          "float spaceWidth=0,lineHeight=0;unsigned char *text;\n"
                          + setup + loop + "\n}\n")
        chunks.append("}\n")
    chunks.append(r'''
int scenarios=0;
void scenario(void (*execute)(AsciiManagerView *,AsciiManagerStringView *),bool gui,
              const char *text,int smallFont,int shadow,float scaleX,float scaleY) {
    AnmSpriteView sprites[256];
    for(int i=0;i<256;++i)sprites[i]={float(4+i%11),float(8+i%7)};
    spriteBase=sprites;Loaded loaded{sprites};
    AsciiManagerStringView entry{};std::strcpy(entry.text,text);
    entry.position={10,20,3};entry.scaleX=scaleX;entry.scaleY=scaleY;
    entry.smallFont=smallFont;entry.drawShadow=shadow;entry.color=0xc8123456u;
    AsciiManagerView manager{};manager.asciiAnm=&loaded;manager.spaceWidth=8;
    manager.primaryVm014.spriteWidth=111;manager.primaryVm014.spriteHeight=222;
    manager.primaryVm014.flags35C=0x140001u;
    manager.primaryVm014.loadedSprite=sprites;observed.clear();
    execute(&manager,&entry);
    // Reference trace from the target's three character paths:
    // newline resets/advances Y only; space advances X without drawing;
    // glyph emits optional shadow then foreground and advances X.
    float x=10,y=20;
    float width=!gui&&smallFont==1?7.0f:8.0f*scaleX;
    float height=!gui&&smallFont==1?9.0f:14.0f;
    std::vector<Event> expected;bool first=true;
    for(unsigned char c: std::string(text)) {
        if(c==10) {x=10;y+=height*scaleY;continue;}
        if(c==32) {x+=width;continue;}
        int sprite=(gui?0:smallFont*0x62)+int(c)-32;
        float glyphWidth=gui?111:sprites[sprite].width;
        float glyphHeight=gui?222:sprites[sprite].height;
        if(!gui&&shadow)expected.push_back({'R',x+2,y+2,glyphWidth,glyphHeight,
            (entry.color>>25)<<24,sprite,true});
        expected.push_back({gui&&scaleX!=1.0f?'N':'R',x,y,glyphWidth,glyphHeight,
            entry.color,sprite,gui?first:!shadow});
        first=false;x+=width;
    }
    check(observed.size()==expected.size(),"glyph-count");
    for(unsigned i=0;i<expected.size();++i) {
        const Event&a=observed[i],&b=expected[i];
        check(a.mode==b.mode,"draw-mode");
        check(equal(a.x,b.x)&&equal(a.y,b.y),"glyph-position");
        check(a.width==b.width&&a.height==b.height,"glyph-size");
        check(a.color==b.color,"glyph-color");
        check(a.sprite==b.sprite,"glyph-index");
        check(a.dirty==b.dirty,"glyph-dirty");
    }
    check(equal(manager.primaryVm014.position.x,x)&&equal(manager.primaryVm014.position.y,y),"glyph-position");
    ++scenarios;
}
void cases(void (*regular)(AsciiManagerView *,AsciiManagerStringView *),
           void (*gui)(AsciiManagerView *,AsciiManagerStringView *)) {
    for(const char *text: {"","A","AB","A B","\nA","A\nB","\n\nB","A \n B","A\n"}) {
        for(int font: {0,1})for(int shadow:{0,1})for(float x:{1.0f,2.0f})for(float y:{1.0f,2.0f})
            scenario(regular,false,text,font,shadow,x,y);
        for(float x:{1.0f,2.0f,std::numeric_limits<float>::quiet_NaN()})for(float y:{1.0f,2.0f})
            scenario(gui,true,text,0,0,x,y);
    }
}
int main(int argc,char **argv) {try {
    static_assert(sizeof(unsigned int)==4,"32-bit packed color role");
    unsigned int order=0x11223344;
    check(*reinterpret_cast<unsigned char*>(&order)==0x44,"fixture-endianness");
''')
    for index, (name, _, _, _) in enumerate(variants):
        chunks.append(f'if(argc==2&&std::strcmp(argv[1],"{name}")==0) {{'
                      f'cases(variant{index}::regular,variant{index}::gui);'
                      'check(scenarios==198,"scenario-count");return 0;}\n')
    chunks.append('return 3;}catch(const std::exception&e){std::cerr<<e.what()<<"\\n";return 2;}}\n')
    cpp = output / "glyphs.cpp"
    cpp.write_text("".join(chunks))
    executable = output / "glyphs"
    command = ["g++", "-std=c++17", "-O1", "-g", str(cpp), "-o", str(executable)]
    if args.sanitize:
        command[1:1] = ["-fsanitize=address,undefined", "-fno-omit-frame-pointer", "-fno-pie", "-no-pie"]
    subprocess.run(command, check=True)
    environment = {**os.environ, "ASAN_OPTIONS": "detect_leaks=1:halt_on_error=1",
                   "UBSAN_OPTIONS": "halt_on_error=1"}
    results = {}
    for name, _, _, predicate in variants:
        result = subprocess.run([str(executable), name], env=environment, capture_output=True, text=True)
        if result.returncode != (0 if predicate is None else 2) or (predicate and result.stderr.strip() != predicate):
            raise RuntimeError(f"{name}: unexpected exit/predicate: {result.returncode}, {result.stderr}")
        results[name] = {"exit": result.returncode, "predicate": predicate}
    receipt = {"scope": __doc__, "source_sha256": hashlib.sha256(source.read_bytes()).hexdigest(),
               "scenarios": 198, "strict_mutants": len(variants)-1,
               "sanitize": args.sanitize, "results": results}
    (output / "receipt.json").write_text(json.dumps(receipt, indent=2) + "\n")
    print("ASCII glyph projection: 198 scenarios and 3 strict functional mutants passed")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--out", type=Path)
    parser.add_argument("--sanitize", action="store_true")
    args = parser.parse_args()
    if args.out:
        run(args, args.out.resolve())
    else:
        (ROOT / "build").mkdir(exist_ok=True)
        with tempfile.TemporaryDirectory(prefix="host-ascii-glyphs-", dir=ROOT / "build") as directory:
            run(args, Path(directory))


if __name__ == "__main__":
    main()
