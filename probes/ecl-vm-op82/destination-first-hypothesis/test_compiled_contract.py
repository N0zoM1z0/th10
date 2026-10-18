"""Control test only; optimized-Python path must not reach compiler/source reads."""
import hashlib
from pathlib import Path
import subprocess
import sys
import unittest

ROOT=Path(__file__).parent
class Controls(unittest.TestCase):
 def test_optimized_python_cli_refused(self):
  result_path=ROOT/'compiled-contract-result.json'
  before=result_path.read_bytes() if result_path.exists() else None
  result=subprocess.run([sys.executable,'-O',str(ROOT/'compiled_contract.py')],capture_output=True,text=True,timeout=2)
  self.assertEqual(result.returncode,1);self.assertEqual(result.stdout,'');self.assertEqual(result.stderr.strip(),'OPTIMIZED_PYTHON_REJECTED')
  after=result_path.read_bytes() if result_path.exists() else None
  self.assertEqual(after,before)
 def test_optimized_import_execute_refused_before_source_read(self):
  program='''import importlib.util
from pathlib import Path
spec=importlib.util.spec_from_file_location("host_contract",%r)
m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m)
class ForbiddenSource:
 def read_bytes(self):raise SystemExit("SOURCE_READ_REACHED")
m.BASE=ForbiddenSource()
try:m.execute()
except RuntimeError as e:
 if str(e)!="OPTIMIZED_PYTHON_REJECTED":raise
 print("REJECTED_BEFORE_READ")
else:raise SystemExit("OPTIMIZED_EXECUTE_ALLOWED")
'''%str(ROOT/'compiled_contract.py')
  result=subprocess.run([sys.executable,'-O','-c',program],capture_output=True,text=True,timeout=2)
  self.assertEqual(result.returncode,0);self.assertEqual(result.stdout.strip(),'REJECTED_BEFORE_READ');self.assertEqual(result.stderr,'')

if __name__=='__main__':unittest.main()
