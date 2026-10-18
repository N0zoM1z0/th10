#!/usr/bin/env python3
"""Subprocess checks for replay entry rejection; no native inputs or operations."""
import json
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import unittest
ROOT=Path(__file__).resolve().parent
class ReplaySafety(unittest.TestCase):
    def test_optimized_python_rejected_before_manifest_read(self):
        with tempfile.TemporaryDirectory() as tmp:
            wrapper=Path(tmp)/'REPLAY_TESTS.py'
            shutil.copyfile(ROOT/'REPLAY_TESTS.py',wrapper)
            # No manifest exists, so the fixed optimized-entry message proves
            # refusal occurred before input validation and before side effects.
            result=subprocess.run([sys.executable,'-O',str(wrapper)],capture_output=True,text=True,timeout=10)
            self.assertNotEqual(result.returncode,0)
            self.assertEqual(result.stderr.strip(),'Optimized Python is unsupported; assertions are required.')
            self.assertEqual(result.stdout,'')
            self.assertEqual(sorted(p.name for p in Path(tmp).iterdir()),['REPLAY_TESTS.py'])
    def test_tampered_member_rejected_before_replay(self):
        with tempfile.TemporaryDirectory() as tmp:
            root=Path(tmp);shutil.copyfile(ROOT/'REPLAY_TESTS.py',root/'REPLAY_TESTS.py')
            (root/'test.py').write_text('changed')
            (root/'archive-manifest.json').write_text(json.dumps({'files':{'test.py':'0'*64}}))
            result=subprocess.run([sys.executable,str(root/'REPLAY_TESTS.py')],capture_output=True,text=True,timeout=10)
            self.assertNotEqual(result.returncode,0)
            self.assertEqual(result.stderr.strip(),'Archive manifest validation failed.')
            self.assertEqual(result.stdout,'')
    def test_missing_required_member_rejected(self):
        with tempfile.TemporaryDirectory() as tmp:
            root=Path(tmp)/'archive';shutil.copytree(ROOT,root)
            manifest=json.loads((root/'archive-manifest.json').read_text())
            del manifest['files']['synthetic-fixtures/run.py']
            (root/'synthetic-fixtures/run.py').unlink()
            (root/'archive-manifest.json').write_text(json.dumps(manifest))
            self.reject(root)
    def test_extra_fixture_rejected(self):
        with tempfile.TemporaryDirectory() as tmp:
            root=Path(tmp)/'archive';shutil.copytree(ROOT,root)
            (root/'synthetic-fixtures/unpinned.py').write_text('unexpected dependency')
            self.reject(root)
    def test_symlink_directory_rejected(self):
        with tempfile.TemporaryDirectory() as tmp:
            root=Path(tmp)/'archive';shutil.copytree(ROOT,root)
            (root/'unexpected-link').symlink_to(root/'synthetic-fixtures',target_is_directory=True)
            self.reject(root)
    def reject(self,root):
        result=subprocess.run([sys.executable,str(root/'REPLAY_TESTS.py')],capture_output=True,text=True,timeout=10)
        self.assertNotEqual(result.returncode,0)
        self.assertEqual(result.stderr.strip(),'Archive manifest validation failed.')
        self.assertEqual(result.stdout,'')
if __name__=='__main__':unittest.main()
