#!/usr/bin/env python3
"""Authored host-model regression tests; no target execution or equivalence claim."""
import json,subprocess,tempfile,sys
from pathlib import Path
if sys.flags.optimize:raise SystemExit('optimized Python is unsupported')
root=Path(__file__).resolve().parent
source=(root/'transition.cpp').read_text();test=(root/'test_transition.cpp').read_text()
mutants={
 'mode12_scalar_zero':('hooks.call4180e0(state, globals, 1u);','hooks.call4180e0(state, globals, 0u);'),
 'mode11_scalar_zero':('const auto initializationMode = globals.gameManagerInitializationMode;','const auto initializationMode = 0u;'),
 'mode11_reload_after_cleanup':('const auto initializationMode = globals.gameManagerInitializationMode;\n        state.field39c = 0;\n        if (request == 7) hooks.call418150(state, globals);','state.field39c = 0;\n        if (request == 7) hooks.call418150(state, globals);\n        const auto initializationMode = globals.gameManagerInitializationMode;'),
 'stale_dispatch':('const auto dispatchCurrent = state.field390;', 'const auto dispatchCurrent = equalityCurrent;'),
 'stale_previous':('const auto previousRequest = state.field38c;', 'const auto previousRequest = equalityRequest;'),
 'stale_normal_copy':('state.field38c = state.field390;', 'state.field38c = equalityCurrent;'),
 'decrement_before_leave':('hooks.leave(state, globals);\n    state.field6f9 = static_cast<std::uint8_t>(state.field6f9 - 1u);','state.field6f9 = static_cast<std::uint8_t>(state.field6f9 - 1u);\n    hooks.leave(state, globals);'),
 'mode4_default_commit':('        default: break;', '        default: hooks.call42cd50(state, globals); break;'),
 'mode11_variant_after_cleanup':('state.field39c = 0;\n        if (request == 7) hooks.call418150(state, globals);','if (request == 7) hooks.call418150(state, globals);\n        state.field39c = 0;'),
 'mode10_stale_scalar':('hooks.call418150(state, globals);\n        state.field39c = 1;\n        state.field390 = 7;\n        const auto scalar = globals.data474c80;', 'const auto scalar = globals.data474c80;\n        hooks.call418150(state, globals);\n        state.field39c = 1;\n        state.field390 = 7;'),
 'mode7_variant_before_release':('if (state.field38c == 4) hooks.call42cdb0(state, globals);\n        state.field39c = 1;', 'state.field39c = 1;\n        if (state.field38c == 4) hooks.call42cdb0(state, globals);'),
 'mode15_exit_before_cleanup':('if (request == 7) hooks.call418150(state, globals);\n            if (request == 14) hooks.call40b9d0(state, globals);\n            state.field390 = 4;\n            globals.data491c00 = 3;', 'globals.data491c00 = 3;\n            if (request == 7) hooks.call418150(state, globals);\n            if (request == 14) hooks.call40b9d0(state, globals);\n            state.field390 = 4;'),
 'missing_result_store':('state.field768 = result;', '(void)result;'),
 'failure_unlock':('hooks.call421c50(state, globals);\n            return 4;', 'hooks.call421c50(state, globals);\n            hooks.leave(state, globals);\n            return 4;'),
 'mode12_mode_before_release':('if (state.field38c == 4) hooks.call42cdb0(state, globals);\n        state.field390 = 7;', 'state.field390 = 7;\n        if (state.field38c == 4) hooks.call42cdb0(state, globals);'),
}
with tempfile.TemporaryDirectory(prefix='helper572-host-') as td:
 p=Path(td);(p/'test_transition.cpp').write_text(test)
 def execute(body):
  (p/'transition.cpp').write_text(body)
  build=subprocess.run(['g++','-std=c++17','-Wall','-Wextra','-Werror',str(p/'test_transition.cpp'),'-o',str(p/'test')],capture_output=True,text=True,timeout=30)
  if build.returncode:raise RuntimeError(build.stderr)
  return subprocess.run([str(p/'test')],capture_output=True,timeout=10).returncode
 assert execute(source)==0
 for name,(old,new) in mutants.items():
  assert source.count(old)==1,(name,source.count(old))
  assert execute(source.replace(old,new))!=0,name
print(json.dumps({'status':'PASS','host_scenarios':666,'compiled_host_mutants_killed':len(mutants),'native_or_body_equivalence':False}))
