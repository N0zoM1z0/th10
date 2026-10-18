"""Authored semantic models only: no C++ build, target, or native equivalence claim."""
import hashlib
import json
from pathlib import Path
import struct
import unittest

ROOT=Path(__file__).parent
SNAPSHOT=Path('/workspace/shared/gui-opcode20-isolated/candidate/src')
def bits(v):return struct.pack('<f',v)
def f32(v):return struct.unpack('<f',bits(v))[0]

class Cell:
 def __init__(self,value):self.value=f32(value)

class PureEnemy:
 def __init__(self,values):
  self.cells={k:Cell(v) for k,v in zip(range(-9981,-9977),values)}
  self.unrelated={'integer':7,'flags':0x10000,'stack_top':48,'frame_base':16}
 def resolve(self,index):return self.cells[index]
 def read(self,index):return self.cells[index].value

class PureFrame:
 def __init__(self,base,values):
  self.base=base;self.stack_top=base+4*len(values)+4
  self.cells={base+4*i:Cell(v) for i,v in enumerate(values)}
 def resolve(self,offset):return self.cells[self.base+int(offset)]
 def read(self,offset):return self.cells[self.base+int(offset)].value

def destination_first(host,operand,provider):
 destination=host.resolve(operand)
 destination.value=f32(provider(host.read(operand)))

def value_first(host,operand,provider):
 value=provider(host.read(operand))
 destination=host.resolve(operand)
 destination.value=f32(value)

def state(host):
 return ({k:bits(v.value) for k,v in host.cells.items()},
         dict(host.unrelated) if hasattr(host,'unrelated') else (host.base,host.stack_top))

class Contracts(unittest.TestCase):
 def test_all_valid_enemy_destinations_pure_schedule_agreement(self):
  # Provider seams are pure; they do not assert native EnemyWrapAngle arithmetic.
  providers=(lambda v:v,lambda v:-v,lambda v:0.75)
  for operand in range(-9981,-9977):
   for value in (0.0,-0.0,1.25,-2.5,3.0,-3.0):
    for provider in providers:
     with self.subTest(operand=operand,value=value):
      values=[0.25,0.5,0.75,1.0];values[operand+9981]=value
      a=PureEnemy(values);b=PureEnemy(values);before=state(a)
      destination_first(a,operand,provider);value_first(b,operand,provider)
      self.assertEqual(state(a),state(b));self.assertEqual(bits(a.cells[operand].value),bits(provider(value)))
      self.assertEqual(state(a)[1],before[1])
      for k in a.cells:
       if k!=operand:self.assertEqual(bits(a.cells[k].value),before[0][k])
 def test_valid_frame_destinations_pure_schedule_agreement(self):
  for base in (0,12,128,4000):
   for offset in (0,4,8,12):
    for provider in (lambda v:v,lambda v:-v,lambda v:0.75):
     with self.subTest(base=base,offset=offset):
      a=PureFrame(base,[0.0,-0.0,1.5,-2.75]);b=PureFrame(base,[0.0,-0.0,1.5,-2.75]);before=state(a)
      destination_first(a,float(offset),provider);value_first(b,float(offset),provider)
      self.assertEqual(state(a),state(b));self.assertEqual(state(a)[1],before[1])
      for k in a.cells:
       if k!=base+offset:self.assertEqual(bits(a.cells[k].value),before[0][k])
 def test_explicit_abstract_callback_order(self):
  events=[]
  class Observed(PureEnemy):
   def resolve(self,index):events.append('resolve');return super().resolve(index)
   def read(self,index):events.append('read');return super().read(index)
  def provider(v):events.append('wrap');return v
  destination_first(Observed([1,2,3,4]),-9981,provider)
  self.assertEqual(events,['resolve','read','wrap'])
 def test_stateful_abstract_host_not_universally_equivalent(self):
  class Stateful(PureEnemy):
   def resolve(self,index):self.cells[index].value=1.0;return self.cells[index]
  a=Stateful([2,0,0,0]);b=Stateful([2,0,0,0])
  destination_first(a,-9981,lambda v:v);value_first(b,-9981,lambda v:v)
  self.assertEqual(a.cells[-9981].value,1.0);self.assertEqual(b.cells[-9981].value,2.0)
 def test_null_or_invalid_destinations_not_in_contract(self):
  for operand in (-1,-2,-9998,0):
   with self.assertRaises(KeyError):PureEnemy([0,0,0,0]).resolve(operand)

class SourceIsolation(unittest.TestCase):
 def test_exact_single_replacement_and_patch(self):
  import difflib
  m=json.loads((ROOT/'manifest.json').read_text());source=(SNAPSHOT/'EclVm.cpp').read_bytes()
  self.assertEqual(hashlib.sha256(source).hexdigest(),m['base_source_sha256'])
  old=b'            case ECL_VM_NORMALIZE_ANGLE:\n                *ResolveFloat(0) = EnemyWrapAngle(ReadFloat(0));\n                break;'
  new=b'            case ECL_VM_NORMALIZE_ANGLE:\n            {\n                float *destination = ResolveFloat(0);\n                *destination = EnemyWrapAngle(ReadFloat(0));\n                break;\n            }'
  self.assertEqual(source.count(old),1);candidate=source.replace(old,new)
  self.assertEqual(hashlib.sha256(candidate).hexdigest(),m['hypothetical_source_sha256'])
  patch=''.join(difflib.unified_diff(source.decode().splitlines(True),candidate.decode().splitlines(True),fromfile='a/src/EclVm.cpp',tofile='b/src/EclVm.cpp'))
  self.assertEqual((ROOT/'destination-first.patch').read_text(),patch)
  self.assertEqual(hashlib.sha256(patch.encode()).hexdigest(),m['patch_sha256'])
  self.assertEqual(candidate.replace(new,old),source)
 def test_pinned_actual_enemy_pure_resolver_and_reads(self):
  source=(SNAPSHOT/'Enemy.cpp').read_bytes()
  self.assertEqual(hashlib.sha256(source).hexdigest(),'6f1bea5905ae2f9871f8ae5a0703e3b7309d58689c402614a2f14db4e7a45ee5')
  text=source.decode().replace('\r\n','\n')
  block=text.split('float *EnemyFullObjectView::ResolveFloatOperand(int operand)\n{',1)[1].split('\n}',1)[0]
  expected='\n    switch (operand)\n    {\n'+''.join('    case ENEMY_ECL_OPERAND_FLOAT_%d: return &runtime.eclVariables.floats[%d];\n'%(i,i) for i in range(4))+'    default: return NULL;\n    }'
  self.assertEqual(block,expected)
  reader=text.split('float EnemyFullObjectView::ReadFloatOperand(int operand)\n{',1)[1].split('\n}',1)[0]
  for i in range(4):self.assertIn('case ENEMY_ECL_OPERAND_FLOAT_%d:\n        return runtime.eclVariables.floats[%d];'%(i,i),reader)
 def test_vm_header_and_wrap_source_unchanged(self):
  for name,expected in [('EclVm.hpp','a01e19c6bb2659e258444cd57018947fe6f5444522b75d2b5777e13f55ffe551'),('EnemyEclDispatcher.cpp','9fae2594e13b7e399e7a7b27a3117495f4a54afd08907df9dcc668d5b00a1e30')]:
   self.assertEqual(hashlib.sha256((SNAPSHOT/name).read_bytes()).hexdigest(),expected)

if __name__=='__main__':unittest.main()
