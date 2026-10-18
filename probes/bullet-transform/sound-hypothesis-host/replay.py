#!/usr/bin/env python3
"""Replay frozen A007 host tests without adopting or executing a native packet."""
import argparse
import hashlib
import json
import pathlib
import subprocess
import sys
import tempfile

ROOT = pathlib.Path(__file__).resolve().parent
PREFIX = 'probes/bullet-transform/native-experiment/'
OVERLAY = '824eb6a3a14fe140168ed1146f4aff7ece5bbc914639934ae35850f0ecdacccc'
FROZEN = {
    'run.py': '2ede42d8373eabda23c545099f267611a0be5f0cd2574fac011ee4306c0e0fcd',
    'harness.cpp': 'f4faf55000a8ec80520ef18f49bc380e7cdf32eb3dbe3a3a6bdf5b8bc9d32448',
    'README.md': '312d26760a7071ab9f58468a13e7e93e50ff87bb0917c11318d2bb187fb0de43',
    'manifest.json': '55435723ced8b38502d0e0e3bd2a7ff57cdd09a7828117723c185dafbb77dc1d',
    '.gitattributes': 'df29ed0dab6de040a422667cdfd75f283b430e8e170491b5404f9caf27071b62',
}
INPUTS = {
    PREFIX + 'a003/run.py': '265f014afd96ff452b4d025d65481554daa26f2ec81e60051103ee2b8b3d5c46',
    PREFIX + 'a003/manifest.proposed.json': 'a064b5422d3ef131d138523db44991a6e581447b6cd65066bfa0f90b65fe8bce',
    PREFIX + 'a003/src/BulletTransform.cpp': 'da1d560622c704618397099410ec680d08349f891dc2a87cc3d46bcea653af3f',
    PREFIX + 'recovery-history/reconstruct.py': 'beb77a37b328983924f441b9a4c97c3f41e413b78ad5279efdf9bdb1318ffa79',
    PREFIX + 'recovery-history/recipes.json': '27301494b01940d637745bd8458ba967ad8eabb03663e34a95188f30eadd3b53',
}
HEADERS = {
    'BulletManager.hpp': '3eb4b14e48456c4cec49a851e7c4ede0b7c38ef71c40a36ccd5321da622701c1',
    'EnemySoundQueue.hpp': '1fdc23149fca106bd00413a7acbdb3bc39849364a1ff0807e41c66d1365574e8',
}


def require(condition, message):
    if not condition:
        raise ValueError(message)


def read_pinned(root, relative, expected, canonical_lf=False):
    path = root
    for part in pathlib.Path(relative).parts:
        require(part not in ('..', '.', ''), 'invalid input path')
        path = path / part
        require(not path.is_symlink(), 'symlink input: ' + relative)
    require(path.is_file(), 'missing input: ' + relative)
    raw = path.read_bytes()
    checked = raw.replace(b'\r\n', b'\n') if canonical_lf else raw
    require(not canonical_lf or b'\r' not in checked, 'bare CR in header: ' + relative)
    require(hashlib.sha256(checked).hexdigest() == expected, 'input hash mismatch: ' + relative)
    return raw


def prepare_inputs(repo, archive=ROOT):
    frozen = {name: read_pinned(archive, name, pin) for name, pin in FROZEN.items()}
    inputs = {name: read_pinned(repo, name, pin) for name, pin in INPUTS.items()}
    headers = {name: read_pinned(repo, 'src/' + name, pin, True) for name, pin in HEADERS.items()}
    namespace = {'__name__': '_pinned_archival_reconstruction'}
    name = PREFIX + 'recovery-history/reconstruct.py'
    # Execute only the hash-verified offline reconstruction utility, never a
    # historical native runner. Its __main__ CLI branch is deliberately skipped.
    exec(compile(inputs[name], name, 'exec'), namespace)
    files = namespace['reconstruct'](
        inputs[PREFIX + 'a003/run.py'],
        inputs[PREFIX + 'a003/manifest.proposed.json'],
        inputs[PREFIX + 'a003/src/BulletTransform.cpp'],
        json.loads(inputs[PREFIX + 'recovery-history/recipes.json']), 'a007')
    require(set(files) == {'run.py', 'manifest.proposed.json', 'src/BulletTransform.cpp'}, 'unexpected reconstructed files')
    overlay = files['src/BulletTransform.cpp']
    require(hashlib.sha256(overlay).hexdigest() == OVERLAY, 'reconstructed overlay mismatch')
    # The reconstructed native runner and manifest stay in memory and are not
    # written, imported, or executed. Only the hypothesis source is materialized.
    return frozen, {'BulletTransform.cpp': overlay, **headers}


def run_replay(repo, archive=ROOT):
    repo, archive = pathlib.Path(repo).resolve(), pathlib.Path(archive).resolve()
    frozen, sources = prepare_inputs(repo, archive)
    temporary_parent = pathlib.Path(tempfile.gettempdir()).resolve()
    require(not temporary_parent.is_relative_to(repo), 'temporary directory is inside repository')
    require(not temporary_parent.is_relative_to(archive), 'temporary directory is inside archive')
    with tempfile.TemporaryDirectory(prefix='th10-sound-host-', dir=temporary_parent) as temp:
        work = pathlib.Path(temp)
        host, source = work / 'host', work / 'source'
        host.mkdir()
        source.mkdir()
        for name, raw in frozen.items():
            (host / name).write_bytes(raw)
        for name, raw in sources.items():
            (source / name).write_bytes(raw)
        command = [sys.executable, '-B']
        if sys.flags.optimize:
            command.append('-O')
        command += [str(host / 'run.py'), '--source-dir', str(source), '--mutations']
        subprocess.run(command, cwd=work, check=True, timeout=1900)
    return {'status': 'PASS', 'scope': 'frozen host baseline and 52 semantic mutants',
            'overlay_sha256': OVERLAY, 'production_source_adopted': False,
            'native_build_or_target_read': False}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--repo', type=pathlib.Path, default=ROOT.parents[2])
    parser.add_argument('--mutations', action='store_true', required=True,
                        help='Run the baseline and all 52 frozen mutation controls')
    args = parser.parse_args()
    print(json.dumps(run_replay(args.repo), sort_keys=True))


if __name__ == '__main__':
    try:
        main()
    except (ValueError, OSError, subprocess.SubprocessError) as exc:
        print('ERROR:', exc, file=sys.stderr)
        sys.exit(1)
