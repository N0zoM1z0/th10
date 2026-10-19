import argparse,copy,difflib,importlib.util,unittest
from pathlib import Path
from unittest.mock import patch
import reconstruct as r
import model as m
BASE=None
HERE=Path(__file__).parent
class Tests(unittest.TestCase):
 def test_exact_source_inverse_patch(self):
  old=(BASE/'src/FrontEnd.cpp').read_bytes();out=r.reconstruct(BASE);new=out['src/FrontEnd.cpp']
  self.assertEqual(r.sha(new),'addb9de47649a892f4241cf20b1992b165ce7b91afa0e0097b4c0ffff19fe463')
  self.assertEqual(new.replace(r.NEW,r.OLD,1),old);self.assertEqual(r.sha(old),r.CPP)
  self.assertEqual(new,old.replace(r.OLD,r.NEW,1));self.assertEqual(len(new)-len(old),5)
  self.assertEqual(new.count(b'\r\n'),old.count(b'\r\n'));self.assertEqual(new.count(b'\n'),new.count(b'\r\n'))
  self.assertEqual(out['src/FrontEnd.hpp'],(BASE/'src/FrontEnd.hpp').read_bytes())
  expected=b''.join(difflib.diff_bytes(difflib.unified_diff,old.splitlines(True),new.splitlines(True),fromfile=b'a/src/FrontEnd.cpp',tofile=b'b/src/FrontEnd.cpp',n=3))
  self.assertEqual(expected,(HERE/'candidate.patch').read_bytes())
 def test_root_and_pin_rejection(self):
  with self.assertRaises(ValueError):r.reconstruct('relative')
  with patch.object(Path,'read_bytes',return_value=b'wrong'):
   with self.assertRaises(ValueError):r.reconstruct(BASE)
 def test_import_no_io(self):
  for name in ('reconstruct','model'):
   source=(HERE/(name+'.py')).read_text()
   with patch('builtins.open',side_effect=AssertionError()),patch.object(Path,'read_bytes',side_effect=AssertionError()):exec(compile(source,name,'exec'),{'__name__':'isolated'})
 def node(self,previous=9,successors=()):return dict(previous=previous,successors=successors,pending=0)
 def test_baseline_and_candidate_all_modes_backdrops(self):
  for d in range(5):
   root=0x77+(d>=4)
   for backdrop in (False,True):
    for proposed in (False,True):
     slots={root:11,0x62:22};nodes={11:self.node(),22:self.node()}
     e=m.initialize(d,backdrop,slots,nodes,proposed);chosen=root if proposed else 0x62;handle=11 if proposed else 22
     prefix=[('backdrop_lookup',0x5e)]+([] if backdrop else [('create',0x5e),('create_aux',8,15)])+[('count',4 if d<4 else 1)]
     self.assertEqual(e,prefix+[('lookup',chosen,handle),('request',handle),('clear',chosen),('create',root),('restart',root,3),('interrupt',root,0x11),('create',0x62),('state','opening'),('timer',0)])
     self.assertEqual(nodes[handle]['pending'],1);self.assertEqual(nodes[22 if proposed else 11]['pending'],0)
 def test_null_and_unresolved_clear(self):
  for handle in (0,55):
   slots={0x77:handle};e=[];m.retire(slots,{},0x77,e)
   self.assertEqual(e,[('lookup',0x77,handle),('clear',0x77)]);self.assertEqual(slots[0x77],0)
 def test_live_head_and_nonhead(self):
  for previous in (None,9):
   nodes={1:self.node(previous,(2,3)),2:self.node(),3:self.node()};slots={0x78:1};e=[];m.retire(slots,nodes,0x78,e)
   self.assertEqual([x for x in e if x[0]=='request'],[('request',x) for x in ((1,2,3) if previous is None else (1,))]);self.assertEqual(slots[0x78],0)
 def test_character_return_premise(self):
  for d in (0,4):
   root=0x77+(d>=4);incoming={root:11,0x62:22,0x7d:33,0x63:44}
   self.assertEqual(m.character_return(incoming),('difficulty','initialize',0));self.assertEqual(incoming[root],11)
   for proposed in (False,True):
    slots=incoming.copy();nodes={11:self.node()};m.initialize(d,True,slots,nodes,proposed)
    self.assertEqual(nodes[11]['pending'],int(proposed))
if __name__=='__main__':
 parser=argparse.ArgumentParser();parser.add_argument('--baseline-root',type=Path,required=True);BASE=parser.parse_args().baseline_root
 unittest.main(argv=['test_candidate.py'])
