#!/usr/bin/env python3
"""Host-only extracted owner-operand and real traversal regression.

No target execution, native build, ABI/layout or byte-exactness claim. Routing
seams alias opaque manager types to a live HostOwner; they test the actual
source operand/cast, not surrounding ECL/collision control flow. Traversal uses
actual shared declarations and full helper bodies, without pointer truncation.
"""
from pathlib import Path
import argparse
import os
import re
import subprocess
import tempfile


def require(ok, message):
    if not ok:
        raise RuntimeError(message)


def balanced(text, anchor, declaration=False):
    require(text.count(anchor) == 1, 'anchor drift: ' + anchor)
    begin = text.index(anchor)
    brace = text.index('{', begin)
    depth, end = 1, brace + 1
    while depth:
        require(end < len(text), 'unbalanced source')
        depth += (text[end] == '{') - (text[end] == '}')
        end += 1
    if declaration:
        require(text[end] == ';', 'missing semicolon')
        end += 1
    return text[begin:end]


def extract(root):
    ecl = (root / 'src/EnemyEclDispatcher.cpp').read_text()
    laser = (root / 'src/EnemyLaser.cpp').read_text()
    header = (root / 'src/EnemyLaser.hpp').read_text()
    game = (root / 'src/GameManager.cpp').read_text()
    # Calls are inventoried independently by boundary kind and source order.
    # Capture full actual owner casts or actual factory owner argument.
    patterns = [
        ('fire', r'EnemyFireLaser\s*\(\s*(g_Enemy(?:Bullet|Laser)Manager)\s*,'),
        ('effect', r'(reinterpret_cast<EnemyEffectWaitManager\s*\*>\s*\(\s*g_Enemy(?:Bullet|Laser)Manager\s*\))'),
        ('resource', r'(reinterpret_cast<EnemyLaserManagerResourceView\s*\*>\s*\(\s*g_Enemy(?:Bullet|Laser)Manager\s*\))')]
    routes = []
    for filename, source, expected in [('EnemyEclDispatcher.cpp', ecl, {'fire':4,'effect':5,'resource':0}),
                                       ('EnemyLaser.cpp', laser, {'fire':6,'effect':0,'resource':4})]:
        found = []
        for kind, pattern in patterns:
            matches = list(re.finditer(pattern, source))
            require(len(matches) == expected[kind], filename + ' ' + kind + ' inventory drift')
            for m in matches:
                found.append((m.start(), kind, m.group(1), filename, source.count('\n', 0, m.start()) + 1))
        routes.extend(sorted(found))
    require(len(routes) == 19, '19-route inventory drift')
    lookups = re.findall(r'EnemyFindLaser\s*\(\s*(g_Enemy(?:Bullet|Laser)Manager)\s*,', ecl)
    require(len(lookups) == 6, 'six lookup caller inventory drift')
    bullet = re.findall(r'(reinterpret_cast<BulletManagerView\s*\*>\s*\(\s*g_EnemyBulletManager\s*\)\s*->\s*SpawnBulletPattern\s*\(reinterpret_cast<BulletSpawnDescriptorView\s*\*>\s*\(\s*iVar26\s*\+\s*0x2c4\s*\)\s*\)\s*;)', ecl)
    require(len(bullet) == 1, 'actual bullet caller drift')
    require('g_GameOwner47781C' not in game, 'old GameManager alias remains')
    game_routes = re.findall(r'(reinterpret_cast<GameChainOwnerView\s*\*>\s*\(\s*g_EnemyLaserManager\s*\))', game)
    require(len(game_routes) == 3, 'GameManager canonical boundary inventory drift')
    require('extern EnemyLaserManagerView *g_EnemyLaserManager;' in header, 'canonical extern absent')
    require('g_EnemyBulletManager' not in laser, 'laser TU retains bullet global')
    declarations = []
    for name in ['EnemyEffectWaitNode', 'EnemyEffectWaitManager']:
        declarations.append(balanced(header, 'struct ' + name + '\n', True))
        require('struct ' + name + '\n' not in ecl, 'duplicate shared declaration')
    anchors = ['static __declspec(noinline) unsigned int EnemyWaitForEffect(',
               '__declspec(noinline) unsigned int EnemyEffectWaitManager::ApplyPatternCancel(',
               'static __declspec(noinline) unsigned int EnemyApplyBulletClear(']
    bodies = [balanced(ecl, anchor) for anchor in anchors]
    return routes, lookups, bullet[0], game_routes, declarations, bodies


def build_source(parts, fixture, mutation=None):
    routes, lookups, bullet, game_routes, declarations, bodies = parts
    checks = []
    for i, (_, kind, expression, filename, line) in enumerate(routes):
        if mutation == 'route_' + str(i):
            expression = expression.replace('g_EnemyLaserManager', 'g_EnemyBulletManager')
        checks.append('CHECK(static_cast<void *>(%s) == &laser, "route_%d");' % (expression, i))
        if kind == 'resource':
            checks.append('CHECK((%s)->effectResource == &laserResource, "resource_%d");' % (expression, i))
    for i, expression in enumerate(lookups + game_routes):
        checks.append('CHECK(static_cast<void *>(%s) == &laser, "existing_route_%d");' % (expression, i))
    checks.append(bullet)
    checks.append('CHECK(bullet.spawns == 1 && laser.spawns == 0 && bullet.last == &descriptor, "actual_bullet_route");')
    body = '\n'.join(bodies)
    if mutation == 'clear_next':
        body = body.replace(bodies[2], bodies[2].replace('node = next;', 'node = node->next;'))
    if mutation == 'pattern_next':
        body = body.replace(bodies[1], bodies[1].replace('node = next;', 'node = node->next;'))
    if mutation == 'wait_skip':
        body = body.replace(bodies[0], bodies[0].replace('node->state != 1 && flag == 0', 'flag == 0'))
    return fixture.replace('@ROUTE_CHECKS@', '\n'.join(checks)).replace('@DECLARATIONS@', '\n'.join(declarations)).replace('@BODIES@', body)


def run_case(work, compiler, source, intended=None, opt="O0", sanitize=False):
    cpp, exe = work / 'fixture.cpp', work / 'fixture'
    cpp.write_text(source)
    flags = ['-O1', '-fsanitize=address,undefined', '-fno-sanitize-recover=all', '-fno-omit-frame-pointer', '-no-pie'] if sanitize else ['-' + opt]
    result = subprocess.run([compiler, '-std=c++17', '-Wall', '-Wextra'] + flags + [str(cpp), '-o', str(exe)], capture_output=True, text=True, timeout=60)
    require(result.returncode == 0, 'compile failure is not a mutation kill:\n' + result.stderr)
    env = dict(os.environ)
    if sanitize:
        # This runner uses ptrace, which LSan cannot operate under. All fixture
        # storage is automatic; retain ASan and UBSan, disable leak checking only.
        env['ASAN_OPTIONS'] = 'detect_leaks=0:halt_on_error=1'
        env['UBSAN_OPTIONS'] = 'halt_on_error=1'
    result = subprocess.run([str(exe)], capture_output=True, text=True, timeout=10, env=env)
    if intended is None:
        require(result.returncode == 0 and result.stdout == 'PASS laser owner fixture\n' and not result.stderr,
                'baseline failure: ' + repr((result.returncode, result.stdout, result.stderr)))
    else:
        require(result.returncode == 1 and not result.stdout and result.stderr == 'LASER_OWNER_CHECK_FAILED: ' + intended + '\n',
                'mutation did not fail ONLY intended CHECK: ' + repr((intended, result.returncode, result.stdout, result.stderr)))


def check_layout(root, work, compiler):
    # Complete real include graph: only C library declarations/compiler spelling
    # are supplied. All production game declarations and assertions stay active.
    include = work / 'include'
    include.mkdir()
    (include / 'stddef.h').write_text('#pragma once\ntypedef __SIZE_TYPE__ size_t;\n#define NULL 0\n#define offsetof(T,m) __builtin_offsetof(T,m)\n')
    (include / 'string.h').write_text('#pragma once\n#include <stddef.h>\nextern "C" void *memset(void *, int, size_t);\n')
    (include / 'compiler.hpp').write_text('#define __cdecl __attribute__((cdecl))\n#define __stdcall __attribute__((stdcall))\n#define __fastcall __attribute__((fastcall))\n#define __thiscall __attribute__((thiscall))\n#define __declspec(x)\n#define __forceinline inline\n')
    path = work / 'layout.cpp'
    path.write_text('#include "EnemyLaser.hpp"\n')
    flags = [compiler, '-m32', '-std=c++98', '-fms-extensions', '-fsyntax-only', '-nostdinc',
             '-I' + str(include), '-I' + str(root / 'src'), '-include', str(include / 'compiler.hpp'),
             '-Werror', '-Wno-invalid-offsetof', str(path)]
    result = subprocess.run(flags, capture_output=True, text=True, timeout=30)
    require(result.returncode == 0 and not result.stdout and not result.stderr,
            'full real-header GNU i386 diagnostic failed:\n' + result.stdout + result.stderr)
    print('PASS complete real EnemyLaser.hpp include graph/layout assertions (GNU i386 diagnostic only; not VC ABI/codegen)')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, default=Path(__file__).resolve().parents[1])
    parser.add_argument('--cxx', default='g++')
    parser.add_argument('--no-mutations', action='store_true')
    parser.add_argument('--opt', choices=['O0', 'O2'], default='O0')
    parser.add_argument('--sanitize', action='store_true', help='ASan/UBSan, O1, no sanitizer recovery')
    args = parser.parse_args()
    parts = extract(args.root)
    fixture = (Path(__file__).parent / 'fixtures/laser-owner-binding.cpp').read_text()
    fixture = fixture.replace('@PLAYER_FLOAT3@', balanced((args.root / 'src/Player.hpp').read_text(), 'struct PlayerFloat3\n', True))
    with tempfile.TemporaryDirectory(prefix='laser-owner-host-') as directory:
        work = Path(directory)
        check_layout(args.root, work, args.cxx)
        run_case(work, args.cxx, build_source(parts, fixture), opt=args.opt, sanitize=args.sanitize)
        mutations = [('route_' + str(i), 'route_' + str(i)) for i in range(19)]
        mutations += [('clear_next', 'clear_visits'), ('pattern_next', 'pattern_total'), ('wait_skip', 'wait_skip')]
        if not args.no_mutations:
            for mutation, diagnostic in mutations:
                run_case(work, args.cxx, build_source(parts, fixture, mutation), diagnostic, args.opt, args.sanitize)
    print('PASS 19 corrected routes, six lookup operands, three GameManager casts, actual bullet call, real wait/clear/pattern traversal')
    if not args.no_mutations:
        print('PASS 22 strict intended-CHECK mutation kills (19 routes, clear/pattern saved-next, wait state skip)')
    for i, (_, kind, _, filename, line) in enumerate(parts[0]):
        print('route_%d: %s:%d (%s)' % (i, filename, line, kind))

if __name__ == '__main__':
    main()
