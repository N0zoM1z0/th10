import importlib.util
import os
import pathlib
import shutil
import subprocess
import tempfile
import unittest
from unittest.mock import patch

P = pathlib.Path
HERE = P(__file__).resolve().parent
spec = importlib.util.spec_from_file_location('sound_archive_replay', HERE / 'replay.py')
r = importlib.util.module_from_spec(spec)
spec.loader.exec_module(r)
CONTEXT = P(os.environ.get('SOUND_HOST_ARCHIVE_REPO', HERE.parents[2])).resolve()


class Tests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory(prefix='sound-archive-test-')
        self.work = P(self.temp.name)
        self.repo = self.work / 'repo'
        self.archive = self.work / 'host'
        self.archive.mkdir()
        for name in r.FROZEN:
            shutil.copyfile(HERE / name, self.archive / name)
        for name in [*r.INPUTS, *('src/' + name for name in r.HEADERS)]:
            destination = self.repo / name
            destination.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(CONTEXT / name, destination)

    def tearDown(self):
        self.temp.cleanup()

    def test_exact_overlay_and_frozen_files(self):
        frozen, sources = r.prepare_inputs(self.repo, self.archive)
        self.assertEqual(set(frozen), set(r.FROZEN))
        self.assertEqual(set(sources), {'BulletTransform.cpp', *r.HEADERS})
        self.assertEqual(r.hashlib.sha256(sources['BulletTransform.cpp']).hexdigest(), r.OVERLAY)

    def test_current_production_transform_never_read(self):
        forbidden = self.repo / 'src/BulletTransform.cpp'
        forbidden.write_bytes(b'not the hypothesis')
        read = P.read_bytes
        observed = []

        def checked(path):
            self.assertNotEqual(path, forbidden)
            observed.append(path)
            return read(path)

        with patch.object(P, 'read_bytes', checked):
            r.prepare_inputs(self.repo, self.archive)
        self.assertEqual({p.name for p in observed if p.parent == self.repo / 'src'}, set(r.HEADERS))

    def test_frozen_tamper_rejected(self):
        for name in r.FROZEN:
            path = self.archive / name
            raw = path.read_bytes()
            path.write_bytes(raw + b'x')
            with self.assertRaises(ValueError):
                r.prepare_inputs(self.repo, self.archive)
            path.write_bytes(raw)

    def test_dependency_tamper_rejected(self):
        for name in r.INPUTS:
            path = self.repo / name
            raw = path.read_bytes()
            path.write_bytes(raw + b'x')
            with self.assertRaises(ValueError):
                r.prepare_inputs(self.repo, self.archive)
            path.write_bytes(raw)

    def test_header_tamper_rejected(self):
        for name in r.HEADERS:
            path = self.repo / 'src' / name
            raw = path.read_bytes()
            path.write_bytes(raw + b'x')
            with self.assertRaises(ValueError):
                r.prepare_inputs(self.repo, self.archive)
            path.write_bytes(raw)

    def test_header_crlf_accepted_and_retained(self):
        for name in r.HEADERS:
            path = self.repo / 'src' / name
            path.write_bytes(path.read_bytes().replace(b'\r\n', b'\n').replace(b'\n', b'\r\n'))
        _, sources = r.prepare_inputs(self.repo, self.archive)
        for name in r.HEADERS:
            self.assertEqual(sources[name], (self.repo / 'src' / name).read_bytes())

    def test_bare_cr_rejected(self):
        path = self.repo / 'src/BulletManager.hpp'
        path.write_bytes(path.read_bytes().replace(b'\n', b'\r', 1))
        with self.assertRaises(ValueError):
            r.prepare_inputs(self.repo, self.archive)

    def test_archival_overlay_crlf_rejected(self):
        path = self.repo / (r.PREFIX + 'a003/src/BulletTransform.cpp')
        path.write_bytes(path.read_bytes().replace(b'\n', b'\r\n'))
        with self.assertRaises(ValueError):
            r.prepare_inputs(self.repo, self.archive)

    def test_missing_input_rejected(self):
        (self.repo / 'src/BulletManager.hpp').unlink()
        with self.assertRaises(ValueError):
            r.prepare_inputs(self.repo, self.archive)

    def test_symlink_input_rejected(self):
        source = self.repo / 'src'
        source.rename(self.repo / 'actual-src')
        source.symlink_to(self.repo / 'actual-src', target_is_directory=True)
        with self.assertRaises(ValueError):
            r.prepare_inputs(self.repo, self.archive)

    def test_explicit_host_invocation_and_cleanup(self):
        seen = []

        def fake(command, cwd, check, timeout):
            work = P(cwd)
            seen.append(work)
            self.assertFalse(work.is_relative_to(self.repo))
            self.assertFalse(work.is_relative_to(self.archive))
            self.assertEqual(command[-3:], ['--source-dir', str(work / 'source'), '--mutations'])
            self.assertEqual(command[-4], str(work / 'host/run.py'))
            self.assertTrue(check)
            self.assertEqual(timeout, 1900)
            expected = {'host/' + n for n in r.FROZEN} | {'source/' + n for n in ['BulletTransform.cpp', *r.HEADERS]}
            self.assertEqual({str(p.relative_to(work)) for p in work.rglob('*') if p.is_file()}, expected)
            self.assertEqual((work / 'host/run.py').read_bytes(), (HERE / 'run.py').read_bytes())
            self.assertFalse((work / 'manifest.proposed.json').exists())

        with patch.object(r.subprocess, 'run', fake):
            result = r.run_replay(self.repo, self.archive)
        self.assertEqual(result['status'], 'PASS')
        self.assertEqual(len(seen), 1)
        self.assertFalse(seen[0].exists())

    def test_failed_host_cleans_workspace(self):
        seen = []

        def fail(command, cwd, **kwargs):
            seen.append(P(cwd))
            raise subprocess.CalledProcessError(1, command)

        with patch.object(r.subprocess, 'run', fail):
            with self.assertRaises(subprocess.CalledProcessError):
                r.run_replay(self.repo, self.archive)
        self.assertFalse(seen[0].exists())

    def test_internal_temp_root_rejected(self):
        with patch.object(r.tempfile, 'gettempdir', return_value=str(self.repo)):
            with patch.object(r.subprocess, 'run') as execute:
                with self.assertRaises(ValueError):
                    r.run_replay(self.repo, self.archive)
                execute.assert_not_called()

    def test_default_repository_cli(self):
        location = self.repo / 'probes/bullet-transform/sound-hypothesis-host'
        with patch.object(r, 'ROOT', location), patch.object(r.sys, 'argv', ['replay.py', '--mutations']):
            with patch.object(r, 'run_replay', return_value={}) as replay, patch('builtins.print'):
                r.main()
                replay.assert_called_once_with(self.repo)

    def test_mutations_flag_required(self):
        with patch.object(r.sys, 'argv', ['replay.py']):
            with patch.object(r, 'run_replay') as replay, patch.object(r.sys, 'stderr'):
                with self.assertRaises(SystemExit) as stopped:
                    r.main()
                self.assertEqual(stopped.exception.code, 2)
                replay.assert_not_called()


if __name__ == '__main__':
    unittest.main()
