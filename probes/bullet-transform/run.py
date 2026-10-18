#!/usr/bin/env python3
"""Compile the exact authored implementation against instrumented host dependencies."""
import argparse, hashlib, json, pathlib, re, subprocess, tempfile, sys
ROOT=pathlib.Path(__file__).resolve().parent
DEFAULT=ROOT.parents[1]/'src'
HASHES={'BulletTransform.cpp':'72aa46821fdae7f35482a30fcfe8abbd48b075a75489f593d922e9ad2ab131d1','BulletManager.hpp':'3eb4b14e48456c4cec49a851e7c4ede0b7c38ef71c40a36ccd5321da622701c1'}
def require(ok,msg):
    if not ok: raise RuntimeError(msg)
def replace_once(s,old,new):
    require(s.count(old)==1,'mutation anchor not unique: '+old)
    return s.replace(old,new,1)
MUTATIONS={
 'unsigned_color':('static_cast<signed char>((packedPattern >> 8) & 0xff)', 'static_cast<unsigned char>((packedPattern >> 8) & 0xff)'),
 'copy_17':('sizeof(pattern.transforms));', 'sizeof(pattern.transforms) - sizeof(pattern.transforms[0]));'),
 'skip_second_record':('++record;', '/* deliberately retain first record */'),
 'early_spawn_increment':('SpawnBulletPattern(&pattern);\n            ++this->transformIndex;', 'SpawnBulletPattern(&pattern);\n            this->transformIndex = 2;'),
 'drop_fade':('if (fadeParent != 0)', 'if (false)'),
 'despawn_stops':('EnemyCancelBulletRecord(reinterpret_cast<unsigned char *>(this));\n        break;', 'EnemyCancelBulletRecord(reinterpret_cast<unsigned char *>(this));\n        return;'),
 'ungate_skip':('record->allowWhileActive == 0 && this->activeTransformFlags != 0', 'record->allowWhileActive == 0 && this->activeTransformFlags != 0 && kind != BULLET_TRANSFORM_SKIP'),
 'wait_zero':('this->exStates[5].timer.SetCurrent(record->payload.int0);', 'this->exStates[5].timer.SetCurrent(0);'),
 'omit_second_index':('++record;\n            ++this->transformIndex;', '++record;'),
 'bounce_payload':('this->exStates[4].int1 = record->payload.int0;', 'this->exStates[4].int1 = record->payload.int1;'),
 'bounce_counter':('this->exStates[4].int0 = 0;', 'this->exStates[4].int0 = 1;'),
 'bounce_flags':('case BULLET_TRANSFORM_BOUNCE_VARIANT:\n        this->activeTransformFlags |= kind;', 'case BULLET_TRANSFORM_BOUNCE_VARIANT:\n        this->activeTransformFlags = kind;'),
 'cancel_payload':('this->cancelBehavior = record->payload.int0;', 'this->cancelBehavior = record->payload.int1;'),
 'sprite_offset':('g_BulletSpriteScriptBase[record->payload.int0] +\n                record->payload.int1;', 'g_BulletSpriteScriptBase[record->payload.int0];'),
 'sprite_manager':('reinterpret_cast<BulletManagerView *>(g_EnemyBulletManager)->bulletAnm,', 'nullptr,'),
 'vector_sound_index':('            this->exStates[1].value1, this->exStates[1].value0);\n        if (this->transformIndex != 0 && this->transformSound >= 0)', '            this->exStates[1].value1, this->exStates[1].value0);\n        if (this->transformSound >= 0)'),
 'vector_sound_zero':('            this->exStates[1].value1, this->exStates[1].value0);\n        if (this->transformIndex != 0 && this->transformSound >= 0)', '            this->exStates[1].value1, this->exStates[1].value0);\n        if (this->transformIndex != 0 && this->transformSound > 0)'),
 'vector_sound_negative':('            this->exStates[1].value1, this->exStates[1].value0);\n        if (this->transformIndex != 0 && this->transformSound >= 0)', '            this->exStates[1].value1, this->exStates[1].value0);\n        if (this->transformIndex != 0)'),
 'vector_sound_position':('            this->exStates[1].value1, this->exStates[1].value0);\n        if (this->transformIndex != 0 && this->transformSound >= 0)\n            BulletQueueSoundAtPosition(this->transformSound, 0.0f);', '            this->exStates[1].value1, this->exStates[1].value0);\n        if (this->transformIndex != 0 && this->transformSound >= 0)\n            BulletQueueSoundAtPosition(this->transformSound, this->position.x);'),
 'polar_sound_index':('        this->exStates[2].int0 = record->payload.int0;\n        if (this->transformIndex != 0 && this->transformSound >= 0)', '        this->exStates[2].int0 = record->payload.int0;\n        if (this->transformSound >= 0)'),
 'polar_sound_zero':('        this->exStates[2].int0 = record->payload.int0;\n        if (this->transformIndex != 0 && this->transformSound >= 0)', '        this->exStates[2].int0 = record->payload.int0;\n        if (this->transformIndex != 0 && this->transformSound > 0)'),
 'polar_sound_negative':('        this->exStates[2].int0 = record->payload.int0;\n        if (this->transformIndex != 0 && this->transformSound >= 0)', '        this->exStates[2].int0 = record->payload.int0;\n        if (this->transformIndex != 0)'),
 'polar_sound_position':('        this->exStates[2].int0 = record->payload.int0;\n        if (this->transformIndex != 0 && this->transformSound >= 0)\n            BulletQueueSoundAtPosition(this->transformSound, 0.0f);', '        this->exStates[2].int0 = record->payload.int0;\n        if (this->transformIndex != 0 && this->transformSound >= 0)\n            BulletQueueSoundAtPosition(this->transformSound, this->position.x);'),
 'direction_lower_equality':('if (requestedAngle <= -990.0f)\n                this->exStates[3].value1', 'if (requestedAngle < -990.0f)\n                this->exStates[3].value1'),
 'direction_upper_equality':('else if (requestedAngle >= 990.0f)', 'else if (requestedAngle > 990.0f)'),
 'direction_eager_aim':('float requestedAngle = record->payload.float0;', 'float requestedAngle = record->payload.float0;\n            EnemyAngleFromPlayer(g_Player, &this->position);'),
 'vector_nan_aim':('    if (requestedAngle >= 990.0f)\n        return EnemyAngleFromPlayer(g_Player, &bullet->position);\n    return requestedAngle;', '    if (requestedAngle < 990.0f)\n        return requestedAngle;\n    return EnemyAngleFromPlayer(g_Player, &bullet->position);'),
 'direction_nan_aim':('else if (requestedAngle >= 990.0f)', 'else if (!(requestedAngle < 990.0f))'),
 'vector_lower_equality':('if (requestedAngle <= -990.0f)\n        return bullet->angle;', 'if (requestedAngle < -990.0f)\n        return bullet->angle;'),
 'vector_upper_equality':('if (requestedAngle >= 990.0f)\n        return EnemyAngleFromPlayer', 'if (requestedAngle > 990.0f)\n        return EnemyAngleFromPlayer'),
 'vector_eager_aim':('    if (requestedAngle <= -990.0f)\n        return bullet->angle;', '    EnemyAngleFromPlayer(g_Player, &bullet->position);\n    if (requestedAngle <= -990.0f)\n        return bullet->angle;'),
 'ungate_vector':('record->allowWhileActive == 0 && this->activeTransformFlags != 0', 'record->allowWhileActive == 0 && this->activeTransformFlags != 0 && kind != BULLET_TRANSFORM_ACCELERATE_VECTOR'),
}

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--source-dir',type=pathlib.Path,default=DEFAULT);ap.add_argument('--mutations',action='store_true');args=ap.parse_args()
    manifest=json.loads((ROOT/'manifest.json').read_text())
    require(set(manifest)=={'source_identity','sources','artifacts_raw_sha256'}, 'unexpected manifest keys')
    require(set(manifest['artifacts_raw_sha256'])=={'run.py','harness.cpp','README.md','.gitattributes'}, 'unexpected artifact keys')
    require(manifest['source_identity']=='sha256 after CRLF-to-LF only; reject remaining CR', 'unsupported source identity policy')
    require(manifest['sources']==HASHES, 'manifest source pins differ from runner')
    for name,digest in manifest['artifacts_raw_sha256'].items():
        require(hashlib.sha256((ROOT/name).read_bytes()).hexdigest()==digest, 'artifact hash mismatch: '+name)
    source={}
    for name,digest in HASHES.items():
        data=(args.source_dir/name).read_bytes().replace(b'\r\n',b'\n')
        require(b'\r' not in data,'unsupported bare CR in source: '+name)
        require(hashlib.sha256(data).hexdigest()==digest,'input hash mismatch: '+name)
        source[name]=data.decode()
    # Only dependency includes and ABI-only typedef assertions are removed.
    # Every declaration, enum value, and method body otherwise stays verbatim.
    hdr=re.sub(r'^#include "(?:AnmManager|Player)\.hpp"\n','',source['BulletManager.hpp'],flags=re.M)
    hdr,n=re.subn(r'typedef char \w+\[\s*.*?\];','',hdr,flags=re.S)
    require(n==36,'unexpected ABI assertion count: '+str(n))
    hdr=hdr.replace('__stdcall','')
    cpp=replace_once(source['BulletTransform.cpp'],'#include "BulletManager.hpp"','')
    cases={'baseline':cpp}
    if args.mutations:
        for name,(old,new) in MUTATIONS.items():cases[name]=replace_once(cpp,old,new)
    with tempfile.TemporaryDirectory(prefix='build-',dir=ROOT) as tmp:
        d=pathlib.Path(tmp);(d/'extracted_header.hpp').write_text(hdr)
        (d/'harness.cpp').write_text((ROOT/'harness.cpp').read_text())
        for name,body in cases.items():
            (d/'extracted_source.cpp').write_text(body)
            result=subprocess.run(['g++','-std=c++17','-O0','-fno-strict-aliasing','-Wall','-Wextra',str(d/'harness.cpp'),'-o',str(d/'test')],capture_output=True,text=True,timeout=30)
            require(result.returncode==0,name+' COMPILE FAILURE (not a mutation kill):\n'+result.stderr)
            result=subprocess.run([str(d/'test')],capture_output=True,text=True,timeout=5)
            if name=='baseline':require(result.returncode==0,result.stdout+result.stderr)
            else:require(result.returncode==31 and result.stderr.startswith('FAIL: '),name+' survived or failed non-semantically: '+str(result.returncode)+result.stderr)
            print(name+': '+('PASS' if name=='baseline' else 'KILLED by semantic assertion')+' '+result.stderr.strip())
    print('Completed actual-source extraction/compilation; no target-binary equivalence claim.')
if __name__=='__main__':
    try:main()
    except Exception as e:print('ERROR:',e,file=sys.stderr);sys.exit(1)
