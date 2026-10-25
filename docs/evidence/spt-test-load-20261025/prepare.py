from pathlib import Path
import shutil,json,hashlib
root=Path.cwd(); p=root/'.analysis/spt-test-load-20261025'; dest=p/'candidate'; assert not dest.exists();dest.mkdir()
for src in (root/'src').iterdir():
 if src.is_file():shutil.copy2(src,dest/src.name)
f=dest/'Enemy.cpp'; b=f.read_bytes();start=b.index(b'// TH10\'s 0x1098-byte Enemy ECL resource');end=b.index(b'struct EnemyEclResourceView :',start)
part=b[start:end]
(dest/'EnemyEclResourceBase.hpp').write_bytes(b'#pragma once\r\n#include "EclVm.hpp"\r\n#include <stdlib.h>\r\n#include <string.h>\r\n'+part)
f.write_bytes(b[:start]+b'#include "EnemyEclResourceBase.hpp"\r\n\r\n'+b[end:])
f=dest/'SptTest.hpp';s=f.read_text().replace('struct EclVmScriptDatabase;','struct EclVmScriptDatabase;\nstruct EnemyEclHostBaseView;').replace('    int Draw();','    EnemyEclHostBaseView *activeVm;\n    unsigned char *scriptData;\n    int LoadScript();\n    int Draw();').replace('int __fastcall SptTestDrawCallback', 'extern SptTestView *g_SptTestView;\nint SptTestLoadCallback();\nint __fastcall SptTestDrawCallback');f.write_text(s)
f=dest/'SptTest.cpp';s=f.read_text();s+='''
#include "Enemy.hpp"
#include "EnemyEclResourceBase.hpp"
#include "FileSystem.hpp"
#include <stdio.h>
#include <stdlib.h>

// Complete isolated proposal for 0x42BE20; retain observed failures/null paths.
int SptTestView::LoadScript()
{
    char path[260];
    sprintf(path, "%s", scriptNames[files.current]);
    if (activeVm != NULL)
    {
        delete activeVm;
        activeVm = NULL;
    }
    activeVm = NULL;
    if (scriptData != NULL)
    {
        free(scriptData);
        scriptData = NULL;
    }
    scriptData = FileSystem::OpenFile(path, NULL, 0);
    scriptDatabase = new EnemyEclResourceBaseView;
    scriptDatabase->AddScriptData(scriptData);
    entries.count = scriptDatabase->subroutineCount;
    int selected = 0;
    if (entries.count != 0 && selected >= entries.count)
        selected = entries.count - 1;
    entries.current = selected;
    state = 1;
    return 0;
}

int SptTestLoadCallback()
{
    g_SptTestView->LoadScript();
    return 0;
}
''';f.write_text(s)
(p/'proposal.json').write_text(json.dumps({'source_checkpoint':'cb894e071b053cc3df5257ffcb8203c15cb6718d','model':'Existing concrete database class moved byte-preserved; real global callback; actual filesystem support for natural private ABI; scalar delete active host; full state/load/clamp failures preserved.','prediction':'First complete load261 and callback16 compare; Draw531 and ASCII100 guards retained.','falsifier':'Any extent/field/local/body or required guard mismatch rejects exactness.','runtime':'Unknown; memset erases vptr, deliberately not repaired.'},indent=2)+'\n')
print('prepared isolated candidate; concrete class bytes',len(part))
