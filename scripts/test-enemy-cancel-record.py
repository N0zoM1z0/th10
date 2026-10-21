#!/usr/bin/env python3
"""Exercise the maintained cancel-record body with logical live host objects."""
from pathlib import Path
import argparse
import hashlib
import json
import re
import resource
import subprocess

ROOT = Path(__file__).resolve().parents[1]
parser = argparse.ArgumentParser()
parser.add_argument("--source", type=Path, default=ROOT / "src/EnemyEclDispatcher.cpp")
parser.add_argument("--out", type=Path, default=ROOT / "build/host-tests/enemy-cancel-record")
parser.add_argument("--mutations", action="store_true")
args = parser.parse_args()
text = args.source.read_text()
start = re.search(r"(?m)^__declspec\(noinline\) unsigned int EnemyCancelBulletRecord\(", text)
assert start
opening = text.index("{", start.start())
depth = 0
for end in range(opening, len(text)):
    depth += (text[end] == "{") - (text[end] == "}")
    if depth == 0:
        break
body = text[start.start():end + 1]
body = body.replace("unsigned char *bullet", "Bullet *bullet")
adaptations = {
    r"reinterpret_cast<EnemyBulletPositionView *>(bullet + 0x3b4)": "&bullet->position",
    "reinterpret_cast<const AnmFloat3View *>(\n                        bullet + 0x3b4)": "&bullet->position",
    "reinterpret_cast<short *>(bullet + 0x446)": "&bullet->state",
    "reinterpret_cast<short *>(bullet + 0x30c)": "&bullet->animation",
    "reinterpret_cast<int *>(bullet + 0x438)": "&bullet->script",
    "reinterpret_cast<unsigned int *>(bullet + 0x408)": "&bullet->timer.flags",
    "reinterpret_cast<int *>(bullet + 0x3fc)": "&bullet->timer.current",
    "reinterpret_cast<int *>(bullet + 0x3f8)": "&bullet->timer.previous",
    "reinterpret_cast<float *>(bullet + 0x400)": "&bullet->timer.subframe",
    "reinterpret_cast<float **>(bullet + 0x404)": "&bullet->timer.scale",
    "reinterpret_cast<unsigned int *>(bullet)": "&bullet->flags",
}
for old, new in adaptations.items():
    assert old in body, old
    body = body.replace(old, "(" + new + ")")
assert "bullet +" not in body and "reinterpret_cast" not in body

header = r'''
#include <cassert>
#include <cstdio>
#include <vector>
#define __declspec(x)
struct EnemyBulletPositionView { float x,y,z; int IsOutsidePlayfield(float,float); };
using AnmFloat3View = EnemyBulletPositionView;
struct Timer { int previous,current;float subframe;float *scale;unsigned flags; };
struct Bullet { short state,animation;unsigned flags;int script;EnemyBulletPositionView position;Timer timer; };
struct AnmVmIdView {unsigned value;};
struct AnmLoadedView { int identity;AnmVmIdView CreateVmAtWorldVariant0(int,const AnmFloat3View*); };
struct EnemyPrimaryResourceOwnerView {AnmLoadedView *primaryEnemyResource;};
static AnmLoadedView resourceA{1},resourceB{2};
static EnemyPrimaryResourceOwnerView ownerA{&resourceA},ownerB{&resourceB};
static EnemyPrimaryResourceOwnerView *g_EnemyPrimaryResourceOwner;
static float g_AnmGameSpeed=1.0f,otherScale=0.5f;
static Bullet *active;static short originalState,originalAnimation;
static int outside;static bool changeEffect;static std::vector<int> events;
int EnemyBulletPositionView::IsOutsidePlayfield(float xMargin,float yMargin){
 assert(this==&active->position&&xMargin==8.0f&&yMargin==8.0f);
 assert(active->state==originalState&&active->animation==originalAnimation);
 assert(events.empty());events.push_back(1);return outside;
}
AnmVmIdView AnmLoadedView::CreateVmAtWorldVariant0(int script,const AnmFloat3View *position){
 assert(this==g_EnemyPrimaryResourceOwner->primaryEnemyResource);
 assert(position==&active->position&&script==active->script&&script>=0);
 assert(active->state==3&&active->animation==1&&events==std::vector<int>{1});
 events.push_back(2);
 if(changeEffect){active->timer.flags^=1u;active->timer.scale=&otherScale;active->timer.current=91;active->timer.previous=92;active->timer.subframe=93.0f;}
 return {0x12345678};
}
'''
tail = r'''
int main(){int scenarios=0;
 for(short state:{short(-1),short(0),short(1),short(2),short(3),short(4)})
 for(int off:{0,1})for(int script:{-1,0,9})for(unsigned flags:{0u,1u,0x20u,0x21u})for(int change:{0,1}){
  Bullet bullet{state,7,0x40u,script,{11,12,13},{31,32,33.0f,&otherScale,flags}};
  active=&bullet;originalState=state;originalAnimation=7;outside=off;changeEffect=change;events.clear();
  g_EnemyPrimaryResourceOwner=(scenarios&1)?&ownerA:&ownerB;
  unsigned result=EnemyCancelBulletRecord(&bullet);const bool valid=state==1||state==2;
  const bool effect=valid&&!off&&script>=0;
  assert(result==unsigned(valid));std::vector<int> expected;
  if(valid)expected.push_back(1);if(effect)expected.push_back(2);assert(events==expected);
  assert(bullet.state==(valid?3:state)&&bullet.animation==(valid?1:7));
  assert(bullet.flags==(0x40u|(valid&&off?8u:0u))&&bullet.script==script);
  assert(bullet.position.x==11&&bullet.position.y==12&&bullet.position.z==13);
  if(valid&&!off){
   unsigned before=flags^unsigned(effect&&change);bool initialize=(before&1u)==0;
   assert(bullet.timer.flags==(before|1u));
   assert(bullet.timer.previous==-1&&bullet.timer.current==0&&bullet.timer.subframe==0.0f);
   assert(bullet.timer.scale==(initialize?&g_AnmGameSpeed:&otherScale));
  }else{
   assert(bullet.timer.flags==flags&&bullet.timer.previous==31&&bullet.timer.current==32);
   assert(bullet.timer.subframe==33.0f&&bullet.timer.scale==&otherScale);
  }
  ++scenarios;
 }
 assert(scenarios==288);std::printf("%d complete cancel-record host scenarios passed\n",scenarios);
}
'''
variants = {"positive": body}
expected_assertions = {}
if args.mutations:
    replacements = {
        "wrong-state-predicate": ("state != 2 && state != 1", "state != 2 || state != 1"),
        "wrong-transition-state": ("&bullet->state) = 3", "&bullet->state) = 2"),
        "wrong-animation-state": ("&bullet->animation) = 1", "&bullet->animation) = 0"),
        "skip-script-zero": ("effectScript >= 0", "effectScript > 0"),
        "wrong-resource": ("g_EnemyPrimaryResourceOwner->primaryEnemyResource->", "resourceA."),
        "lose-timer-flags": ("timerFlagValue |= 1u", "timerFlagValue = 1u"),
        "always-initialize-timer": ("(timerFlagValue & 1u) == 0", "true"),
        "wrong-removal-bit": ("&bullet->flags) |= 8", "&bullet->flags) |= 4"),
    }
    for label, (old, new) in replacements.items():
        assert old in body, old
        variants[label] = body.replace(old, new)
    expected_assertions = {
        "wrong-state-predicate": "result==unsigned(valid)",
        "wrong-transition-state": "bullet.state==(valid?3:state)&&bullet.animation==(valid?1:7)",
        "wrong-animation-state": "bullet.state==(valid?3:state)&&bullet.animation==(valid?1:7)",
        "skip-script-zero": "events==expected",
        "wrong-resource": "this==g_EnemyPrimaryResourceOwner->primaryEnemyResource",
        "lose-timer-flags": "bullet.timer.flags==(before|1u)",
        "always-initialize-timer": "bullet.timer.scale==(initialize?&g_AnmGameSpeed:&otherScale)",
        "wrong-removal-bit": "bullet.flags==(0x40u|(valid&&off?8u:0u))&&bullet.script==script",
    }
args.out.mkdir(parents=True, exist_ok=True)
results = []
for label, code in variants.items():
    program = header + code + tail
    source, binary = args.out / (label + ".cpp"), args.out / label
    source.write_text(program)
    compiled = subprocess.run([
        "c++", "-std=c++17", "-O1", "-g", "-ffp-contract=off",
        "-fsanitize=address,undefined", "-fno-omit-frame-pointer", "-fno-pie", "-no-pie",
        str(source), "-o", str(binary),
    ], capture_output=True, text=True)
    assert compiled.returncode == 0, compiled.stderr
    tested = subprocess.run([str(binary)], capture_output=True, text=True,
        preexec_fn=lambda: resource.setrlimit(resource.RLIMIT_CORE, (0, 0)))
    log = tested.stdout + tested.stderr
    (args.out / (label + ".log")).write_text(log)
    assert (tested.returncode == 0) == (label == "positive"), (label, tested.returncode, log)
    if label != "positive":
        expected = "Assertion `" + expected_assertions[label] + "' failed."
        assert expected in log, (label, expected, log)
        assert "ERROR: AddressSanitizer" not in log and "runtime error:" not in log
    results.append({"control": label, "result": "passed" if label == "positive" else "rejected",
        "expected_assertion": expected_assertions.get(label),
        "returncode": tested.returncode, "program_sha256": hashlib.sha256(program.encode()).hexdigest(),
        "log_sha256": hashlib.sha256(log.encode()).hexdigest()})
    print(label, results[-1]["result"], tested.stdout.strip(), flush=True)
(args.out / "receipt.json").write_text(json.dumps({
    "scope": "Complete maintained body projected to logical live host fields and mutable service sinks; no target layout, i386 ABI or original/native-game runtime claim",
    "source_sha256": hashlib.sha256(args.source.read_bytes()).hexdigest(),
    "producer_sha256": hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
    "scenarios": 288, "strict_mutants": len(variants) - 1,
    "adaptations": adaptations, "sanitizers": "address,undefined,leaks", "results": results,
}, indent=2) + "\n")
