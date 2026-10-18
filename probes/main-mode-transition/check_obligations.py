#!/usr/bin/env python3
"""Finite unordered obligation fixture; not a native body or feasible-path model."""
import hashlib,json,sys
from pathlib import Path
if sys.flags.optimize:raise SystemExit('optimized Python')
FACTS_FILE='facts.json'
FACTS_SHA='159a5e12ca08a3426bd6cad48f2bc7832937d938bd2daf68ea0de00727394410'

def load_contract():
 raw=Path(__file__).with_name(FACTS_FILE).read_bytes()
 assert hashlib.sha256(raw).hexdigest()==FACTS_SHA
 packet=json.loads(raw)['case_effect_analysis'];vocabulary=packet['event_vocabulary'];result={}
 for case in packet['cases']:
  allowed=frozenset(vocabulary[n] for n in case['cfg_possible_events'])
  required=frozenset(vocabulary[n] for n in case['required_on_all_modeled_normal_continuations'])
  assert required<=allowed and case['effects_complete'] is False
  for mode in case['mode_snapshot_values']:
   assert mode not in result
   result[mode]=(required,allowed)
 assert set(result)==set(range(16))
 return result

def recognized_projection_satisfies(mode,inventory,contract):
 """Recognized-vocabulary projection only. Extra real effects may exist outside this vocabulary. Passing proves no order, feasibility or complete effects."""
 required,allowed=contract[mode]
 return required<=set(inventory)<=allowed

def direct_critical_count_satisfies(return_value,enter_count,leave_count):
 """Counts direct572 API calls only; no lock argument/ownership claim."""
 if return_value==1:return (enter_count,leave_count) in ((0,0),(1,1))
 if return_value==4:return (enter_count,leave_count)==(1,0)
 return False

def run_tests():
 c=load_contract();checks=0;mutants=0
 for mode,(required,allowed) in c.items():
  assert recognized_projection_satisfies(mode,allowed,c);checks+=1
  for event in required:
   assert not recognized_projection_satisfies(mode,allowed-{event},c);mutants+=1
  assert not recognized_projection_satisfies(mode,allowed|{'unproved_effect'},c);mutants+=1
 assert c[3][0]==frozenset(('call_helper_0x4203f0','call_helper_0x421c50','return_4'))
 assert 'call_LeaveCriticalSection' not in c[3][1]
 assert 'store_context_0x768_opaque_EAX_result_0x41fd00' in c[0][0]
 assert 'store_context_0x39c_constant_0x0' in c[11][0]
 for mode in (7,10,12,13):assert 'store_context_0x39c_constant_0x1' in c[mode][0]
 for mode in (1,2,5,6,8,9):assert c[mode]==c[1]
 for result in (1,4):
  for enter in range(3):
   for leave in range(3):
    expected=(result,enter,leave) in ((1,0,0),(1,1,1),(4,1,0))
    assert direct_critical_count_satisfies(result,enter,leave)==expected;checks+=1
 assert not direct_critical_count_satisfies(4,1,1) # Do not insert a speculative unlock fix.
 print(json.dumps({'status':'PASS','finite_inventory_and_count_cases':checks,'projection_schema_and_membership_mutants_killed':mutants,'native_or_body_acceptance':False,'feasibility_or_order_acceptance':False}))
if __name__=='__main__':run_tests()
