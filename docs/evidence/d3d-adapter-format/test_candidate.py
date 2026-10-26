import unittest,difflib,argparse,sys
from pathlib import Path
from unittest.mock import patch
import reconstruct as r
from model import run
ROOT=None;HERE=Path(__file__).parent
class Tests(unittest.TestCase):
 def probes(self,x):return [e[1] for e in x['trace'] if e[0]=='probe']
 def test_source(self):
  old=(ROOT/'src/Main.cpp').read_bytes();out=r.reconstruct(ROOT);new=out['src/Main.cpp']
  self.assertEqual(r.sha(new),'9a9ed9e39a35c026f8b58dea885d6dd86fe314b8c820c5554676c2672431c78e');self.assertEqual(new.replace(r.NEW,r.OLD,1),old)
  self.assertEqual(new,old.replace(r.OLD,r.NEW,1));self.assertEqual((new.count(b'\n'),new.count(b'\r\n')),(old.count(b'\n'),old.count(b'\r\n')))
  self.assertEqual(r.sha(out['src/Main.hpp']),r.HPP)
  self.assertEqual(new.count(b'->CheckDeviceFormat('),old.count(b'->CheckDeviceFormat('))
  self.assertEqual((HERE/'candidate.patch').read_bytes(),b''.join(difflib.diff_bytes(difflib.unified_diff,old.splitlines(True),new.splitlines(True),fromfile=b'a/src/Main.cpp',tofile=b'b/src/Main.cpp',n=3)))
 def test_reject_and_import(self):
  with self.assertRaises(ValueError):r.reconstruct('relative')
  with patch.object(Path,'read_bytes',return_value=b'bad'):
   with self.assertRaises(ValueError):r.reconstruct(ROOT)
  for n in ('reconstruct','model'):
   s=(HERE/(n+'.py')).read_text()
   with patch('builtins.open',side_effect=AssertionError()),patch.object(Path,'read_bytes',side_effect=AssertionError()):exec(compile(s,n,'exec'),{'__name__':'isolated'})
 def test_fullscreen_witness_and_trace(self):
  a=run(23,False,0,0,[True]);b=run(23,False,0,0,[True],proposed=True)
  self.assertEqual(self.probes(a),[('receiver',0,1,23,0,3,21)]);self.assertEqual(self.probes(b),[('receiver',0,1,22,0,3,21)])
  self.assertEqual([e for e in a['trace'] if e[0]!='probe'],[e for e in b['trace'] if e[0]!='probe'])
  names=[e[0] for e in b['trace']];self.assertLess(names.index('create'),names.index('publish'));self.assertLess(names.index('publish'),names.index('matrices'));self.assertLess(names.index('caps'),names.index('probe'));self.assertLess(names.index('probe'),names.index('reset'))
 def test_matching_windowed_controls(self):
  for desktop,windowed,color in ((22,False,0),(23,False,1),(22,True,1),(23,True,0)):
   self.assertEqual(run(desktop,windowed,color,0,[True]),run(desktop,windowed,color,0,[True],proposed=True))
 def test_color_modes(self):
  a=run(23,False,-1,0,[True],proposed=True);self.assertEqual(a['color'],0);self.assertEqual(self.probes(a)[0][3],22)
  a=run(22,False,1,0,[True],proposed=True);self.assertEqual(self.probes(a)[0][3],23)
 def test_software_hal_and_suppression(self):
  a=run(23,False,0,0,[False,True],proposed=True);self.assertEqual(len(self.probes(a)),1);self.assertEqual(len([e for e in a['trace'] if e[0]=='create']),2)
  for options,results in ((0,[False,False,True]),(2,[True]),(1,[True])):
   self.assertEqual(self.probes(run(23,False,0,options,results,proposed=True)),[])
 def test_probe_effects(self):
  for result in (0,-1):
   a=run(23,False,0,0x100,[True],probe_result=result,proposed=True,flags=0x84)
   self.assertEqual(a['flags']&0x80,0x80);self.assertEqual(a['options']&0x100,0x100)
   self.assertEqual(bool(a['flags']&4),result==0);self.assertEqual(bool(a['options']&1),result!=0)
   self.assertEqual(('log','texture_warning') in a['trace'],result!=0)
 def test_fallback_and_terminal_order(self):
  for no_vsync in (False,True):
   a=run(23,False,0,0,[False,False,False,True],proposed=True,no_vsync=no_vsync)
   creates=[e for e in a['trace'] if e[0]=='create'];self.assertEqual({e[3] for e in creates},{22});self.assertEqual(creates[-1][4:],(0,1,3) if no_vsync else (0,1,1))
  a=run(23,False,0,0,[False]*6,proposed=True);self.assertEqual(a['trace'][-2:],[('fatal',),('release_interface',)]);self.assertEqual(self.probes(a),[]);self.assertEqual(a['return_value'],1)
if __name__=='__main__':
 parser=argparse.ArgumentParser();parser.add_argument('--baseline-root',required=True);args=parser.parse_args();ROOT=Path(args.baseline_root)
 if not ROOT.is_absolute():parser.error('absolute baseline root required')
 unittest.main(argv=[sys.argv[0]])
