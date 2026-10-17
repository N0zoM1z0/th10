"""Synthetic tests; imports mock instruction definitions, not target data."""
import sys
if sys.flags.optimize != 0:
    raise SystemExit('Optimized Python is not permitted for assertion-based synthetic controls.')
from synthetic_routing_fixture import dec,jumps,sel,I,R,C,M,copy
from audit_ecl_control_dependencies import analyze_dependencies as analyze
r=analyze(dec,jumps,sel)
assert r['CALL']['comparison_constant']==0
assert r['CALL']['equal_successor']['category']=='pre_dispatch_code'
assert r['CALL']['unequal_successor']['return_constant']==-1
assert r['DEFAULT']['comparison_constant']==-1
assert r['DEFAULT']['equal_successor']['return_constant']==0
assert r['DEFAULT']['unequal_successor']['category']=='common_nop_entry'
assert r['common_advance']['strict_straight_line_dependency']['established']
def mutate(address,ins):
 d=copy.deepcopy(dec);idx=next(n for n,i in enumerate(d['Run']) if i.address==address);d['Run'][idx]=ins;return analyze(d,jumps,sel)
assert mutate(43,I(43,'test',R('eax'),C(-1)))['DEFAULT']['status']=='UNRESOLVED'
assert mutate(44,I(44,'jne',C(95)))['DEFAULT']['unequal_successor']['return_constant']==0
assert not mutate(61,I(61,'movsx',R('ecx'),M('eax',6,2)))['common_advance']['strict_straight_line_dependency']['established']
assert not mutate(60,I(60,'mov',R('eax'),M('esi',4,1)))['common_advance']['strict_straight_line_dependency']['established']
assert not mutate(63,I(63,'mov',M('esi',4,1),R('eax')))['common_advance']['strict_straight_line_dependency']['established']
assert mutate(95,I(95,'mov',M('esi'),C(0)))['DEFAULT']['equal_successor']['category']=='UNRESOLVED'
assert mutate(95,I(95,'call',C(1000)))['DEFAULT']['equal_successor']['category']=='UNRESOLVED'
# Add instructions between call/comparison/branch to exercise true dependency guards.
def insert_after(address,ins):
 d=copy.deepcopy(dec);idx=next(n for n,i in enumerate(d['Run']) if i.address==address)
 d['Run'][idx].size=0.5;ins.address=address+0.5;ins.size=0.5;d['Run'].insert(idx+1,ins)
 return analyze(d,jumps,sel)
assert insert_after(42,I(0,'mov',R('eax'),C(1)))['DEFAULT']['status']=='UNRESOLVED'
assert insert_after(42,I(0,'mov',R('al'),C(1)))['DEFAULT']['status']=='UNRESOLVED'
assert insert_after(43,I(0,'add',R('edi'),C(1)))['DEFAULT']['status']=='UNRESOLVED'
d=copy.deepcopy(dec)
for n,i in enumerate(d['Run']):
 if i.address==60:d['Run'][n]=I(60,'mov',R('eax'),M('ecx',4))
 if i.address==63:d['Run'][n]=I(63,'mov',M('ecx',4),R('eax'))
assert not analyze(d,jumps,sel)['common_advance']['strict_straight_line_dependency']['established']
fs=M('esi',4);fs.mem.segment='fs'
assert not mutate(63,I(63,'mov',fs,R('eax')))['common_advance']['strict_straight_line_dependency']['established']
d=copy.deepcopy(dec)
for n,i in enumerate(d['Run']):
 if i.address==62:d['Run'][n]=I(62,'add',R('ecx'),R('eax'))
 if i.address==63:d['Run'][n]=I(63,'mov',M('esi',4),R('ecx'))
assert analyze(d,jumps,sel)['common_advance']['strict_straight_line_dependency']['established']
# Pure post-call pointer reload still rejoins predispatch and never asserts time gate identity.
d=copy.deepcopy(dec)
for n,i in enumerate(d['Run']):
 if i.address==16:d['Run'][n]=I(16,'mov',R('eax'),M('esi',4))
d['Run'].append(I(17,'jmp',C(2)))
r=analyze(d,jumps,sel)
assert r['CALL']['equal_successor']['category']=='pre_dispatch_code'
assert r['CALL']['equal_successor']['gpr_reload_count']==1
# Follow only bounded unconditional jump prefixes, reject cycles.
d=copy.deepcopy(dec);d['Run'].extend([I(70,'jmp',C(60)),I(71,'jmp',C(71))]);j=list(jumps);j[0]=70
assert analyze(d,j,sel)['common_advance']['strict_straight_line_dependency']['established']
j[0]=71
assert analyze(d,j,sel)['common_advance']['strict_straight_line_dependency']['established']==False
# Noncontiguous copies preserve dependency; unsupported writes/calls fail closed.
assert insert_after(61,I(0,'mov',R('edi'),C(9)))['common_advance']['strict_straight_line_dependency']['established']
assert not insert_after(61,I(0,'mov',M('edi'),C(9)))['common_advance']['strict_straight_line_dependency']['established']
assert not insert_after(61,I(0,'call',C(1000)))['common_advance']['strict_straight_line_dependency']['established']
assert not insert_after(61,I(0,'mov',R('eax'),C(9)))['common_advance']['strict_straight_line_dependency']['established']
assert not insert_after(61,I(0,'mov',R('al'),C(9)))['common_advance']['strict_straight_line_dependency']['established']
assert insert_after(61,I(0,'fild',M('edi')))['common_advance']['strict_straight_line_dependency']['failure_category']=='unsupported_floating_operation'
# Different symbolic slot expressions are never assumed equal or nonaliasing.
assert not mutate(63,I(63,'mov',M('edi',4),R('eax')))['common_advance']['strict_straight_line_dependency']['established']
assert not insert_after(61,I(0,'mov',R('cr0'),C(9)))['common_advance']['strict_straight_line_dependency']['established']
assert not insert_after(61,I(0,'mov',R('edi'),R('dr0')))['common_advance']['strict_straight_line_dependency']['established']
print('synthetic_dependency_tests: PASS (inherited routing/shape controls + 9 dependency controls)')
