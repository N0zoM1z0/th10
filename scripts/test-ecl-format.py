#!/usr/bin/env python3
"""Replay the actual FORMAT arm with bounded packets and operand-service roles.

This is a host source projection, not execution of the original/native product.
It checks argument consumption and live instruction reloads. It does not model
the operand services, malformed packets, allocation failure or i386/x87 ABI.
"""
from pathlib import Path
import argparse
import hashlib
import json
import os
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]


def extract(text, start, end, *, unique_start=True):
    starts = text.count(start)
    if not starts or (unique_start and starts != 1) or text.count(end) != 1:
        raise ValueError(f"ambiguous source projection: {start!r}")
    return text[text.index(start):text.index(end)]


def run(args, out):
    out.mkdir(parents=True, exist_ok=True)
    source = ROOT / "src/EclVm.cpp"
    header = ROOT / "src/EclVm.hpp"
    text = source.read_text()
    arm = extract(text, "            case ECL_VM_EVALUATE_FORMAT_OPERANDS:",
                  "            default:\n                if (host->DispatchEclInstruction() == -1)")
    # Compile the whole trace overload group used by the actual FORMAT arm.
    sink = extract(text, "static void EclDebugTrace(",
                   "int EclVmContext::Run(float timeDelta)", unique_start=False)
    instruction = extract(header.read_text(), "struct EclVmInstruction\n",
                          "class EclVmHost;")
    variants = [("positive", arm)]
    mutations = [
        ("flag-index", "argumentIndex + 1", "argumentIndex", 2),
        ("metadata-stride", "argumentIndex * 8", "argumentIndex * 4", 1),
        ("value-stride", "argumentIndex * 2", "argumentIndex", 2),
        ("float-tag-g", "argumentType == 'g'", "argumentType == 'z'", 1),
        ("percent-consumes", "case '%':", "case '%':\n                        ++argumentIndex;", 1),
        ("unknown-consumes", "case 'd':", "case 'x':\n                    case 'd':", 1),
        ("cursor-step", "cursor = percent + 2;", "cursor = percent + 1;", 1),
        ("cached-instruction", "instruction", "current", 4),
    ]
    for name, before, after, count in mutations:
        actual = arm.count(before)
        if actual != count:
            raise ValueError(f"mutation {name}: expected {count} sites, found {actual}")
        variants.append((name, arm.replace(before, after)))

    prefix = r'''
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <initializer_list>
#include <functional>
'''+instruction+r'''
static_assert(offsetof(EclVmInstruction, operands)==0x10,"operand base");
static_assert(offsetof(EclVmInstruction, size)==6,"instruction size");
static int OperandInt(const EclVmInstruction *p, unsigned i) {
    int result; std::memcpy(&result,p->operands+4*i,4); return result;
}
struct Event { char type; unsigned flag; uint32_t bits; };
static uint32_t bits(float value) {
    uint32_t result; std::memcpy(&result,&value,4); return result;
}
static uint32_t word(int value) { return static_cast<uint32_t>(value); }
struct Packet {
    alignas(16) unsigned char bytes[1024]{};
    EclVmInstruction *get() { return reinterpret_cast<EclVmInstruction *>(bytes); }
    Packet(const char *format, int length,
           std::initializer_list<char> types, std::initializer_list<uint32_t> values) {
        if(length%4 || length<static_cast<int>(std::strlen(format)+1)
           || types.size()!=values.size()) throw std::runtime_error("fixture packet");
        std::memcpy(bytes+0x10,&length,4);
        std::strcpy(reinterpret_cast<char *>(bytes)+0x14,format);
        auto value=values.begin(); unsigned i=0;
        for(char type: types) {
            bytes[0x14+length+8*i]=type;
            std::memcpy(bytes+0x18+length+8*i,&*value,4);
            ++value; ++i;
        }
    }
};
struct Role {
    EclVmInstruction *instruction;
    std::vector<Event> events;
    std::function<void()> afterFirst;
    void record(char type,unsigned flag,uint32_t value) {
        events.push_back({type,flag,value});
        if(events.size()==1 && afterFirst) afterFirst();
    }
    int ReadIntValue(unsigned flag,int value) {
        record('i',flag,word(value)); return value;
    }
    static float ReadFloatValue(unsigned flag,Role *context,float value) {
        context->record('f',flag,bits(value)); return value;
    }
};
static void expect(const Role& role,std::initializer_list<Event> expected) {
    if(role.events.size()!=expected.size()) throw std::runtime_error("event count");
    unsigned i=0;
    for(auto event: expected) {
        auto actual=role.events[i++];
        if(actual.type!=event.type || actual.flag!=event.flag || actual.bits!=event.bits)
            throw std::runtime_error("operand identity/order/value");
    }
}
enum { ECL_VM_EVALUATE_FORMAT_OPERANDS=30 };
'''
    suffix = r'''
template<class Context> void cases() {
    {
        Packet p("ordinary text",16,{},{}); Context c; c.instruction=p.get();
        c.evaluate(p.get()); expect(c,{});
    }
    {
        Packet p("%%d %x %q %%",16,{'i'},{word(19)}); Context c;
        c.instruction=p.get(); c.evaluate(p.get()); expect(c,{});
    }
    for(const char *format: {"%d %f %d", "%f %d %f", "%% %d %x %f %q %d", "%%%d%f%d"}) {
        Packet p(format,32,{'i','f','g'},{word(-17),bits(2.5f),bits(-1.25f)});
        Context c; c.instruction=p.get(); c.evaluate(p.get());
        expect(c,{{'i',1,word(-17)},{'f',2,bits(2.5f)},{'f',3,bits(-1.25f)}});
    }
    {
        Packet p("%d%f%d",16,{'z','i','?'},{word(0),word(2147483647),word(-2147483647-1)});
        Context c; c.instruction=p.get(); c.evaluate(p.get());
        expect(c,{{'i',1,word(0)},{'i',2,word(2147483647)},{'i',3,word(-2147483647-1)}});
    }
    {
        Packet p("%d%f",16,{'f','g'},{0x7fc12345u,0x7f800000u});
        Context c; c.instruction=p.get(); c.evaluate(p.get());
        expect(c,{{'f',1,0x7fc12345u},{'f',2,0x7f800000u}});
    }
    for(bool floatFirst: {false,true}) {
        Packet a("%d%f",16,{floatFirst?'f':'i','i'},{floatFirst?bits(3.0f):word(17),word(99)});
        Packet b("ignored %d%d%d",32,{'i','f','i'},{word(900),bits(-7.5f),word(901)});
        Context c; c.instruction=a.get(); c.afterFirst=[&]{c.instruction=b.get();};
        c.evaluate(a.get());
        if(floatFirst) expect(c,{{'f',1,bits(3.0f)},{'f',2,bits(-7.5f)}});
        else expect(c,{{'i',1,word(17)},{'f',2,bits(-7.5f)}});
        if(c.instruction!=b.get()) throw std::runtime_error("instruction publication");
    }
}
'''
    chunks = [prefix]
    for index, (_, body) in enumerate(variants):
        chunks.append(f"namespace variant{index} {{\n"+sink+
                      "struct Context: Role { void evaluate(EclVmInstruction *current) {\n"
                      "switch(ECL_VM_EVALUATE_FORMAT_OPERANDS) {\n"+body+"}\n} };\n}\n")
    chunks.append(suffix)
    chunks.append('int main(int argc,char **argv) { try {\n')
    for index, (name, _) in enumerate(variants):
        chunks.append(f'if(argc==2 && std::strcmp(argv[1],"{name}")==0) {{ cases<variant{index}::Context>(); return 0; }}\n')
    chunks.append('return 3; } catch(const std::exception& e) { std::cerr<<e.what()<<"\\n"; return 2; } }\n')
    cpp=out/"format.cpp"; cpp.write_text("".join(chunks)); exe=out/"format"
    command=["g++","-std=c++17","-O1","-g","-fno-strict-aliasing",str(cpp),"-o",str(exe)]
    if args.sanitize:
        command[1:1]=["-fsanitize=address,undefined","-fno-omit-frame-pointer","-fno-pie","-no-pie"]
    subprocess.run(command,check=True)
    environment={**os.environ,"ASAN_OPTIONS":"detect_leaks=1:halt_on_error=1","UBSAN_OPTIONS":"halt_on_error=1"}
    results={}
    for name,_ in variants:
        result=subprocess.run([str(exe),name],env=environment,capture_output=True,text=True)
        expected=0 if name=="positive" else 2
        if result.returncode!=expected:
            raise RuntimeError(f"{name}: expected exit {expected}, got {result.returncode}: {result.stderr}")
        results[name]=result.returncode
    report={"scope":__doc__,"source_sha256":{str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in [source,header]},
            "scenarios":10,"strict_functional_mutants":len(mutations),"sanitize":args.sanitize,"results":results}
    (out/"receipt.json").write_text(json.dumps(report,indent=2)+"\n")
    print("ECL FORMAT source projection: 10 scenarios and 8 functional mutants passed"+ (" (ASan/UBSan/leaks)" if args.sanitize else ""))


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output-dir",type=Path)
    parser.add_argument("--sanitize",action="store_true")
    args=parser.parse_args()
    if args.output_dir:
        run(args,args.output_dir.resolve())
    else:
        build_root=ROOT/"build"
        build_root.mkdir(exist_ok=True)
        with tempfile.TemporaryDirectory(prefix="host-ecl-format-",dir=build_root) as directory:
            run(args,Path(directory))


if __name__=="__main__":
    main()
