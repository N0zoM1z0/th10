#!/usr/bin/env python3
"""Finite unordered obligation fixture; not a native body or feasible-path model."""
import hashlib,json,sys
from pathlib import Path
if sys.flags.optimize:raise SystemExit('optimized Python')
FACTS_FILE='facts.json'
FACTS_SHA='6653d41b20552b88bf5a44f67028dae74152fea0f7087197c9f792102fb4322a'

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

def guarded_projection_satisfies(is_zero, inventory, facts):
 """Fixed proved guard membership only; no event order or complete body claim."""
 required=set(facts['mode0_guard_fixed_requirements']['zero' if is_zero else 'nonzero'])
 forbidden={'return_1','return_unresolved'} if is_zero else {'return_4','return_unresolved'}
 return required<=set(inventory) and not forbidden&set(inventory)

def prefix_projection_satisfies(events,facts):
 """Required recognized prefix projection, conditional on reaching direct dispatch."""
 return set(facts['predispatch_analysis']['recognized_prefix_events_required'])<=set(events)

def receiver_projection_valid(obligations):
 return {x['api']:x['argument_origin_proved'] for x in obligations}=={'EnterCriticalSection':True,'LeaveCriticalSection':True}

def mode15_projection_satisfies(case,events):
 """Recognized events only; other real writes/helper effects remain possible."""
 return set(case['required_events'])<=set(events)<=set(case['cfg_possible_events'])

def named_helper_projection_satisfies(matches,helper,events):
 """Membership of one named direct helper only; other effects remain unconstrained."""
 return (helper in events)==matches

def run_tests():
 c=load_contract();checks=0;mutants=0
 facts=json.loads(Path(__file__).with_name(FACTS_FILE).read_bytes())
 prefix=facts['predispatch_analysis'];required=set(prefix['recognized_prefix_events_required'])
 assert all(prefix['fixed_hypotheses'].values()) and len(required)==4
 assert prefix_projection_satisfies(required,facts)
 for event in required:assert not prefix_projection_satisfies(required-{event},facts)
 assert receiver_projection_valid(facts['critical_receiver_analysis']['obligations'])
 for api in ('EnterCriticalSection','LeaveCriticalSection'):
  changed=[dict(x,argument_origin_proved=not x['argument_origin_proved']) if x['api']==api else x for x in facts['critical_receiver_analysis']['obligations']]
  assert not receiver_projection_valid(changed)
 expression=facts['mode10_fixed_expression']
 assert all(expression[k] for k in ('scalar_store_proved','record_expression_proved','same_load_site_proved'))
 assert expression['data_identities']=={'scalar_dword_cell':'0x474c80','record_table_base':'0x474788'}
 partition=facts['mode15_partition'];assert partition['shared_request_load_site_proved']
 mode15_cases=0;mode15_mutants=0
 assert {c['request_snapshot_class'] for c in partition['classes']}=={'request_2','request_7','request_14','other_request_values'}
 for case in partition['classes']:
  assert case['fixed_store_outcome_proved'] and case['normal_return1_proved'] and not case['effects_complete']
  allowed=set(case['cfg_possible_events']);assert mode15_projection_satisfies(case,allowed);mode15_cases+=1
  for event in case['required_events']:
   assert not mode15_projection_satisfies(case,allowed-{event});mode15_mutants+=1
  assert not mode15_projection_satisfies(case,allowed|{'unproved_effect'});mode15_mutants+=1
 optional_cases=0;optional_mutants=0
 optional=facts['optional_helper_partitions']['modes']
 assert {(x['mode_snapshot'],x['request_snapshot_equals']) for x in optional}=={(7,4),(12,4),(11,7),(14,7)}
 for mode in optional:
  helper=mode['named_direct_helper'];assert {x['request_matches'] for x in mode['classes']}=={True,False}
  for case in mode['classes']:
   assert case['named_helper_outcome_proved'] and case['normal_return1_proved']
   events=set(case['direct_helper_events_cfg_possible']);matches=case['request_matches']
   assert named_helper_projection_satisfies(matches,helper,events);optional_cases+=1
   changed=events^{helper};assert not named_helper_projection_satisfies(matches,helper,changed);optional_mutants+=1
 mode4=facts['mode4_partition'];assert mode4['range_guard_mandatory'] and mode4['complement_includes_zero_high_bit_and_all_out_of_range']
 expected={'request_1_or_2':{'call_helper_0x42cd50'},'request_7':{'call_helper_0x418150','call_helper_0x42cd50'},'request_14':{'call_helper_0x40b9d0','call_helper_0x42cd50'},'other_uint32_requests':set()}
 mode4_cases=0;mode4_mutants=0
 for case in mode4['classes']:
  allowed=set(case['direct_helper_events_cfg_possible']);required=set(case['direct_helper_events_required'])
  assert allowed==required==expected[case['request_snapshot_class']] and case['normal_return1_proved'] and not case['effects_complete'];mode4_cases+=1
  for event in required:
   assert not required<=allowed-{event};mode4_mutants+=1
  assert not (allowed|{'unproved_helper'})<=allowed;mode4_mutants+=1
 identity=facts['snapshot_identity_analysis']['identity_obligations']
 assert identity=={'normal_copy_uses_distinct_current_load':True,'previous_copy_reuses_equality_request':False,'selector_reuses_equality_current':False}
 order=facts['fixed_source_precedence']['obligations'];prefix_order=facts['prefix_load_order']['obligations']
 assert len(order)==42 and len(prefix_order)==5 and all(row[3]=='proved_when_both_occur' for row in order+prefix_order)
 pure=facts['mode10_pure_transform'];assert pure['proved_conditional'] and pure['recognized_families']==['fixed_left_shift'] and pure['unclassified_transform_count']==1
 guard=facts['mode0_guard_obligations']
 for key in ('single_41fd00_call_site','unique_guard','guard_required_after_mode0_dispatch','zero_result_failure_obligations_proved','nonzero_result_normal_return_obligations_proved'):assert guard[key] is True
 guard_cases=0;guard_mutants=0
 for is_zero in (True,False):
  required=set(facts['mode0_guard_fixed_requirements']['zero' if is_zero else 'nonzero'])
  assert guarded_projection_satisfies(is_zero,required,facts);guard_cases+=1
  for event in required:
   assert not guarded_projection_satisfies(is_zero,required-{event},facts);guard_mutants+=1
  for forbidden in ('return_1','return_unresolved') if is_zero else ('return_4','return_unresolved'):
   assert not guarded_projection_satisfies(is_zero,required|{forbidden},facts);guard_mutants+=1
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
 print(json.dumps({'status':'PASS','finite_inventory_and_count_cases':checks,'projection_schema_and_membership_mutants_killed':mutants,'guard_projection_cases':guard_cases,'guard_projection_mutants_killed':guard_mutants,'prefix_and_receiver_projection_cases':2,'prefix_and_receiver_projection_mutants_killed':6,'mode15_projection_cases':mode15_cases,'mode15_projection_mutants_killed':mode15_mutants,'optional_helper_projection_cases':optional_cases,'optional_helper_membership_mutants_killed':optional_mutants,'mode4_projection_cases':mode4_cases,'mode4_membership_mutants_killed':mode4_mutants,'fixed_precedence_records_checked':42,'prefix_load_order_records_checked':5,'snapshot_and_mode10_classification_checks':4,'native_or_body_acceptance':False,'feasibility_or_order_acceptance':False}))
if __name__=='__main__':run_tests()
