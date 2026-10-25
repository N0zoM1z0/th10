from pathlib import Path
import shutil,json,subprocess
p=Path('.analysis/spt-test-update-20261025');d=p/'candidate';assert not d.exists();d.mkdir()
baseline='c7e3f6e487f03b725bb799e6e9b1f9230c111cb2'
for name in subprocess.check_output(['git','ls-tree','-r','--name-only',baseline,'src']).decode().splitlines():
 (d/Path(name).name).write_bytes(subprocess.check_output(['git','show',baseline+':'+name]))
(p/'manifest.json').write_text(json.dumps({'state':'isolated-first-model','head':'c7e3f6e487f03b725bb799e6e9b1f9230c111cb2','scope':'update926 and actual callback5; natural factory89; no canonical changes','prediction':'complete state semantics with actual definitions may recover private ABI naturally','limitations':['LoadScript261 and callback16 declared only','original source/TU identity unknown','no runtime claim'],'artifacts':[{'path':str(d),'class':'session-scratch','producer':'prepare.py','cleanup':'retain unresolved model until checkpoint'}]},indent=2)+'\n')
f=d/'SptTest.hpp';s=f.read_text().replace('unsigned char unknown2D0[8];','int value2D0;\n    unsigned int flags2D4;').replace('    int Draw();','    int Draw();\n    int Update();').replace('// Actual registered five-byte callback42C580 and its update body42BFC0\n// remain unimplemented dependencies; the callback has no authored credit.','// Actual callback, original authored/outlined origin indeterminate.');f.write_text(s)
viewer=(d/'ScriptViewer.cpp').read_text();state0=viewer[viewer.index('    case 0:\n        ClearScriptNames();'):viewer.index('\nint ScriptViewerView::Draw()')];state0=state0.replace('value680','value2D0');helpers=viewer[viewer.index('static inline void SelectScriptViewerEntry'):viewer.index('int ScriptViewerView::Update()')]
s='''
#include "Enemy.hpp"
#include "InputState.hpp"
// The real loader callback42BE10 ignores the context; declared dependency only.
extern int SptTestLoadCallback();
EnemyEclHostBaseView *SptTestCreateVm(EclVmScriptDatabase *database, const char *name);
'''+helpers+'''
int SptTestView::Update()
{
    WIN32_FIND_DATAA find;
    switch (state)
    {
    case 4:
        reinterpret_cast<EclVmHost *>(activeVm)->Run(1.0f);
        if (!activeVm || !activeVm->activeEclContext->currentInstruction)
        {
            if (activeVm) delete activeVm;
            activeVm = NULL;
            state = 1;
        }
        break;
    case 1:
        RememberScriptViewerCursor(&menu);
        if (g_FrontEndInput.IsActiveInline(0x20)) menu.Move(1);
        if (g_FrontEndInput.IsActiveInline(0x10)) menu.Move(-1);
        switch (menu.current)
        {
        case 2:
            if (g_FrontEndInput.pressed & 0x1001)
                g_MainSupervisorView.SetNextGameMode(3);
            break;
        case 1:
            if (scriptDatabase)
            {
                RememberScriptViewerCursor(&entries);
                if (static_cast<unsigned short>(g_FrontEndInput.IsRepeated(0x80))) entries.Move(1);
                if (static_cast<unsigned short>(g_FrontEndInput.IsRepeated(0x40))) entries.Move(-1);
                if (g_FrontEndInput.mask008 & 0x1001)
                {
                    if (activeVm)
                    {
                        delete activeVm;
                        activeVm = NULL;
                    }
                    activeVm = SptTestCreateVm(scriptDatabase, scriptDatabase->subroutines[entries.current].name);
                    state = 4;
                }
            }
            break;
        case 0:
            RememberScriptViewerCursor(&files);
            if (g_FrontEndInput.IsActiveInline(0x80)) files.Move(1);
            if (g_FrontEndInput.IsActiveInline(0x40)) files.Move(-1);
            if (g_FrontEndInput.pressed & 0x1001)
            {
                flags2D4 &= ~2u;
                worker.Start(this, SptTestLoadCallback);
                state = 2;
            }
            break;
        }
        break;
'''+state0+'''
int __fastcall SptTestUpdateCallback(void *context)
{
    return static_cast<SptTestView *>(context)->Update();
}
'''
f=d/'SptTest.cpp';f.write_text(f.read_text()+s)
f=d/'Enemy.cpp';b=f.read_bytes();b+=b'''
// Complete target450500 factory: no ResetEclState, active-context initialization
// or allocation failure repair. Original private ABI must emerge from LTCG.
EnemyEclHostBaseView *SptTestCreateVm(EclVmScriptDatabase *database, const char *name)
{
    EnemyEclHostBaseView *host = new EnemyEclHostBaseView;
    host->scriptDatabase = reinterpret_cast<EnemyEclResourceView *>(database);
    host->activeEclContext->currentInstruction =
        reinterpret_cast<EnemyEclInstructionView *>(database->FindSubroutine(name));
    host->activeEclContext->value00 = 0;
    return host;
}
''';f.write_bytes(b)
print('complete isolated state owner and natural factory prepared')
