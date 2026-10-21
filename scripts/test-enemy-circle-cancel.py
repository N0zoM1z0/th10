#!/usr/bin/env python3
"""Check the maintained circle-cancel body with live logical host records."""
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
parser.add_argument("--out", type=Path, default=ROOT / "build/host-tests/enemy-circle-cancel")
parser.add_argument("--mutations", action="store_true")
args = parser.parse_args()
text = args.source.read_text()
start = re.search(r"(?m)^__declspec\(noinline\) unsigned int EnemyCancelManagerView::CancelBulletPattern\(", text)
assert start
opening = text.index("{", start.start())
depth = 0
for end in range(opening, len(text)):
    depth += (text[end] == "{") - (text[end] == "}")
    if not depth:
        break
body = text[start.start():end + 1]
value_type = text[text.index("struct EnemyCircleDisplacementView : PlayerFloat3"):start.start()]
adaptations = {
    "reinterpret_cast<BulletRuntimeView *>(bulletStorage)": "bulletStorage",
    "reinterpret_cast<unsigned char *>(bullet)": "bullet",
    "reinterpret_cast<EnemyBulletPositionView *>(&bullet->position)": "&bullet->position",
    "reinterpret_cast<const AnmFloat3View *>(bulletPosition)": "bulletPosition",
}
for old, new in adaptations.items():
    assert old in body, old
    body = body.replace(old, new)
assert "reinterpret_cast" not in body

header = r'''
#include <cassert>
#include <cstdio>
#include <limits>
#include <utility>
#include <vector>
#define __declspec(x)
struct EnemyBulletPositionView {float x,y,z;int IsOutsidePlayfield(float,float);};
using PlayerFloat3=EnemyBulletPositionView;using AnmFloat3View=PlayerFloat3;
struct BulletRuntimeView {unsigned short state;int cancelBehavior;PlayerFloat3 position;float collisionWidth;bool outside;int index;};
struct EnemyCancelManagerView {BulletRuntimeView bulletStorage[2001];unsigned CancelBulletPattern(const PlayerFloat3*,float,int,int);};
struct ItemManagerView {int identity;int Spawn(const AnmFloat3View*,int,unsigned,float,float);};
static ItemManagerView itemA{1},itemB{2};static ItemManagerView*g_ItemManager;
static EnemyCancelManagerView*active;static BulletRuntimeView*lastCancelled;
static bool mutate;static int filterFlag;static std::vector<std::pair<int,int>>trace;
unsigned EnemyCancelBulletRecord(BulletRuntimeView*row){
 assert(row->index<2000);assert(row==&active->bulletStorage[row->index]);
 assert(row->state!=0&&row->state!=3);assert(!filterFlag||row->cancelBehavior==0);
 trace.emplace_back(1,row->index);lastCancelled=row;
 if(row->state!=1&&row->state!=2)return 0;
 row->state=3;
 if(mutate){row->position.x+=32.0f;row->position.z=123.0f;g_ItemManager=&itemB;}
 return 1;
}
int EnemyBulletPositionView::IsOutsidePlayfield(float xMargin,float yMargin){
 assert(lastCancelled&&this==&lastCancelled->position&&xMargin==2.0f&&yMargin==2.0f);
 assert(trace.back()==std::make_pair(1,lastCancelled->index));
 trace.emplace_back(2,lastCancelled->index);return lastCancelled->outside;
}
int ItemManagerView::Spawn(const AnmFloat3View*position,int kind,unsigned color,float angle,float speed){
 assert(this==g_ItemManager);assert(position==&lastCancelled->position);
 assert(kind==8&&color==0xffffffffu&&angle==-1.5707964f&&speed==0.6f);
 assert(trace.back()==std::make_pair(2,lastCancelled->index));trace.emplace_back(3,lastCancelled->index);return 0;
}
'''
tail = r'''
struct Geometry {float x,y,z,width,radius,px,py,pz;bool hit;};
int main(){
 const float nan=std::numeric_limits<float>::quiet_NaN(),inf=std::numeric_limits<float>::infinity();
 const Geometry geometry[]={
  {0,0,1000,2,0,0,0,0,true},{3,4,1000,2,4,0,0,0,true},
  {3,4,0,2,3,0,0,0,false},{3,4,0,0,-5,0,0,0,true},
  {nan,0,0,2,0,0,0,0,true},{0,nan,0,2,0,0,0,0,true},
  {0,0,0,nan,0,0,0,0,true},{0,0,0,2,nan,0,0,0,true},
  {9,0,nan,2,0,0,0,0,false},{0,0,nan,2,0,0,0,0,true},
  {inf,0,0,2,0,0,0,0,false},{0,0,0,2,inf,0,0,0,true},
  {inf,0,0,2,inf,0,0,0,true},{inf,0,0,2,0,inf,0,0,true},
  {0,0,0,inf,-inf,0,0,0,true}
 };
 EnemyCancelManagerView manager{};active=&manager;int scenarios=0;
 for(const auto&g:geometry)for(unsigned short state:{0,1,2,3,7,65535})
 for(int flags:{0,1})for(int countdown:{-1,0,2})for(int mode:{-1,0,1})
 for(int outside:{0,1})for(int change:{0,1}){
  for(int i=0;i<2001;++i){manager.bulletStorage[i]=BulletRuntimeView{};manager.bulletStorage[i].index=i;}
  const PlayerFloat3 point{g.px,g.py,g.pz};
  for(int i:{0,17,1999})manager.bulletStorage[i]={state,countdown,{g.x,g.y,g.z},g.width,bool(outside),i};
  manager.bulletStorage[2000]={1,0,point,0,false,2000};
  g_ItemManager=&itemA;lastCancelled=nullptr;mutate=change;filterFlag=flags;trace.clear();
  assert(manager.CancelBulletPattern(&point,g.radius,mode,flags)==0);
  const bool eligible=state!=0&&state!=3&&(!flags||countdown==0);const bool hit=eligible&&g.hit;
  std::vector<std::pair<int,int>>expected;
  if(hit)for(int i:{0,17,1999}){expected.emplace_back(1,i);expected.emplace_back(2,i);if(!outside&&mode)expected.emplace_back(3,i);}
  assert(trace==expected);
  for(int i:{0,17,1999})assert(manager.bulletStorage[i].state==((hit&&(state==1||state==2))?3:state));
  assert(manager.bulletStorage[2000].state==1);
  assert(g_ItemManager==((hit&&change&&(state==1||state==2))?&itemB:&itemA));
  ++scenarios;
 }
 assert(scenarios==6480);std::printf("%d complete circle-cancel host scenarios passed\n",scenarios);
}
'''
variants = {"positive": body}
expected_assertions = {}
if args.mutations:
    comparison = "displacement.x * displacement.x +\n                displacement.y * displacement.y"
    changes = {
        "ordered-only-comparison": (f"!({comparison} > effectiveRadius * effectiveRadius)", f"{comparison} <= effectiveRadius * effectiveRadius", "trace==expected"),
        "wrong-state-filter": ("state == 0 || state == 3", "state == 0 && state == 3", "row->state!=0&&row->state!=3"),
        "skip-countdown-filter": ("flags != 0 && bullet->cancelBehavior != 0", "false", "!filterFlag||row->cancelBehavior==0"),
        "use-z-distance": (comparison, comparison + " + displacement.z * displacement.z", "trace==expected"),
        "short-pool": ("remaining = 2000", "remaining = 1999", "trace==expected"),
        "oversized-pool": ("remaining = 2000", "remaining = 2001", "row->index<2000"),
        "mode-before-bounds": ("bulletPosition->IsOutsidePlayfield(2.0f, 2.0f) == 0 &&\n                mode != 0", "mode != 0 &&\n                bulletPosition->IsOutsidePlayfield(2.0f, 2.0f) == 0", "trace==expected"),
        "gate-on-cancel-return": ("EnemyCancelBulletRecord(bullet);", "if (EnemyCancelBulletRecord(bullet) == 0) continue;", "trace==expected"),
        "wrong-item-receiver": ("g_ItemManager->Spawn", "itemA.Spawn", "this==g_ItemManager"),
        "wrong-item-kind": ("8, -1, -1.5707964f, 0.6f", "9, -1, -1.5707964f, 0.6f", "kind==8&&color==0xffffffffu&&angle==-1.5707964f&&speed==0.6f"),
    }
    for label, (old, new, assertion) in changes.items():
        assert body.count(old) == 1, (label, old)
        variants[label] = body.replace(old, new)
        expected_assertions[label] = assertion
args.out.mkdir(parents=True, exist_ok=True)
results = []
for label, code in variants.items():
    program = header + value_type + code + tail
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
        assert "Assertion `" + expected_assertions[label] + "' failed." in log, (label, log)
        assert "ERROR: AddressSanitizer" not in log and "runtime error:" not in log
    results.append({"control": label, "result": "passed" if label == "positive" else "rejected",
        "expected_assertion": expected_assertions.get(label), "returncode": tested.returncode,
        "program_sha256": hashlib.sha256(program.encode()).hexdigest(),
        "log_sha256": hashlib.sha256(log.encode()).hexdigest()})
    print(label, results[-1]["result"], tested.stdout.strip(), flush=True)
(args.out / "receipt.json").write_text(json.dumps({
    "scope": "Complete maintained body and value constructor with live logical host records, exact finite witnesses, NaN/infinity cases and mutable service sinks; no target layout, i386/x87 precision or original/native-game runtime claim",
    "source_sha256": hashlib.sha256(args.source.read_bytes()).hexdigest(),
    "producer_sha256": hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
    "scenarios": 6480, "strict_mutants": len(variants) - 1, "adaptations": adaptations,
    "sanitizers": "address,undefined,leaks", "results": results,
}, indent=2) + "\n")
