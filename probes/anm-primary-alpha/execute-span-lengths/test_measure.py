import os,ast,copy,csv,importlib.util,json,pathlib,subprocess,sys,tempfile,types,unittest
from unittest.mock import patch
import measure as m
P=pathlib.Path;ROOT=P(__file__).parent;SCRIPTS=P(os.environ.get('TH10_SCRIPT_ROOT','/workspace/shared/static-owner-selection/tree/scripts'))
class Tests(unittest.TestCase):
 def tables(self):
  a=[m.TARGET+100+max(0,i-2)*10 for i in range(94)];b=[0x600000+100+max(0,i-2)*10 for i in range(94)]
  return a,b,0x600000,9948,0x600000+9948-376,{}
 def test_size_only_synthetic(self):
  r=m.summarize(*self.tables());self.assertEqual(r['target_distinct_group_count'],92);self.assertEqual(r['equal_sized_physical_span_count'],91);self.assertEqual(r['unequal_sized_physical_span_count'],1);self.assertEqual(r['pre_table_span_size_delta'],-16)
  self.assertEqual([x['opcodes'] for x in r['rows']],sorted(x['opcodes'] for x in r['rows']))
  forbidden=['address','offset','destination','matched','comparable','instruction','trace']
  self.assertFalse(any(x in json.dumps(r) for x in forbidden))
  self.assertLess(len(json.dumps(r,separators=(',',':'))),18000)
 def test_table_bounds_partition_and_order_mutations(self):
  for mutate in [lambda a:a[0].pop(),lambda a:a[1].pop(),lambda a:a[0].__setitem__(0,m.CODE_END+1),lambda a:a[1].__setitem__(0,a[2]-1),lambda a:a[1].__setitem__(0,a[4]),lambda a:a.__setitem__(4,a[4]-1),lambda a:a[1].__setitem__(0,a[1][-1]),lambda a:a[1].__setitem__(3,a[1][4])]:
   args=list(self.tables());mutate(args)
   with self.assertRaises(ValueError):m.summarize(*args)
 def test_bounds_before_native_read(self):
  image=types.SimpleNamespace(read_address=unittest.mock.Mock(return_value=b'x'));bound=m.BoundedImage(image,100,400)
  for address,size in [(99,1),(100,401),(500,1),(100,0),(100,-1),(90,376),(200,376)]:
   with self.assertRaises(ValueError):bound.read_address(address,size)
  image.read_address.assert_not_called();bound.read_address(100,400);image.read_address.assert_called_once_with(100,400)
 def test_manifest_mutations(self):
  v=json.loads((ROOT/'manifest.json').read_text());v['execution_ready']=True;m.gate(v)
  for key,value in [('execution_ready',False),('baseline_run','../other'),('baseline_runner_sha256','a'*64),('baseline_manifest_sha256','b'*64),('scope','structural'),('output_limit_bytes',100000),('acceptance_authority','exact')]:
   a=dict(v);a[key]=value
   with self.assertRaises(ValueError):m.gate(a)
 def test_optimized_python(self):
  r=subprocess.run([sys.executable,'-O',str(ROOT/'measure.py')],capture_output=True,text=True,timeout=10);self.assertEqual(r.returncode,1);self.assertEqual(json.loads(r.stdout)['error_class'],'OptimizedPythonDisabled')
 def test_real_import_registration(self):
  li=m.load('linked_image',SCRIPTS/'linked_image.py');self.assertIs(sys.modules['linked_image'],li)
  m.load('target_identity',SCRIPTS/'target_identity.py');table=m.load('host_anm_table',SCRIPTS/'report-anm-execute-table.py')
  src=P(os.environ.get('TH10_SOURCE_ROOT','/workspace/shared/gui-opcode20-isolated/candidate'))/'src';names=m.source_gate(table,src/'AnmManager.cpp',src/'AnmManager.hpp');self.assertEqual(len(names),94)
 def test_source_coverage_mutations(self):
  base={'ANM_OP_NOP':0,'ANM_OP_INTERRUPT_LABEL':1};base.update({'OP'+str(i):i for i in range(-1,93) if i not in [0,1]});cases=[k for k,v in base.items() if v not in [0,1]]
  for ret in [(base,cases[:-1],[]),(base,cases,['bad'])]:
   table=types.SimpleNamespace(parse_source=lambda *args,ret=ret:ret)
   with self.assertRaises(ValueError):m.source_gate(table,None,None)
 def test_failed_loader_restores(self):
  with tempfile.TemporaryDirectory() as td:
   p=P(td)/'bad.py';p.write_text('raise RuntimeError("PRIVATE")');name='host_bad';old=types.SimpleNamespace();sys.modules[name]=old
   with self.assertRaises(RuntimeError):m.load(name,p)
   self.assertIs(sys.modules[name],old);sys.modules.pop(name)
 def test_map_pdb_identity_mutations(self):
  im=types.SimpleNamespace(timestamp=1,image_base=2,codeview_record=lambda:{'guid':'g','age':3});meta={'timestamp':1,'image_base':2};pdb={'guid':'g','pdb_age':3};m.artifact_identity(im,meta,pdb)
  for key in meta:
   bad=dict(meta);bad[key]+=1
   with self.assertRaises(ValueError):m.artifact_identity(im,bad,pdb)
  for key in pdb:
   bad=dict(pdb);bad[key]='bad'
   with self.assertRaises(ValueError):m.artifact_identity(im,meta,bad)
 def test_target_boundary(self):
  with tempfile.TemporaryDirectory() as td:
   p=P(td)/'f.csv';h='address,size,span_end,proposed_name\n';row='0x43ee30,9964,0x44151b,AnmRenderManagerView::ExecuteScript\n';p.write_text(h+row);m.target_boundary(p)
   for text in ['',row+row,row.replace('9964','9948'),row.replace('44151b','44151c'),row.replace('ExecuteScript','Other')]:
    p.write_text(h+text)
    with self.assertRaises(ValueError):m.target_boundary(p)
 def test_no_build_or_structural_compare(self):
  tree=ast.parse((ROOT/'measure.py').read_text());attrs=[n.attr for n in ast.walk(tree) if isinstance(n,ast.Attribute)]
  self.assertFalse(set(attrs)&{'cold_link','structural_compare','linked_functions','Popen','system','write_bytes','write_text'})
 def test_producer_drift_before_import(self):
  with tempfile.TemporaryDirectory() as td:
   r=P(td);run=r/m.RUN;run.mkdir(parents=True);(run/'runner.py').write_text('wrong');v=json.loads((ROOT/'manifest.json').read_text());v['execution_ready']=True
   with patch.object(m,'load') as load:
    with self.assertRaises(ValueError):m.measure(r,v)
    load.assert_not_called()
if __name__=='__main__':unittest.main()
