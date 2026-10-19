"""Explicit-root, pure in-memory source reconstruction; no import-time I/O."""
from pathlib import Path
import hashlib
CPP = 'c26c9c1c00b3caa98a01fc210efdbb898fe6e8cace5a4d84a8072cd3f8a90d5d'
HPP = '8b4e0e13312bce0b9bab71e16daff79cfa402d55a61e55cca53bd359edc7832b'
MARKER = b'// TH10_FRONTEND_FUNCTION: 0x00430320 FrontEndControllerView::UpdateDifficulty\r\n'
OLD = (b'        controller->cursor.count = g_ReplayDifficulty < 4 ? 4 : 1;\r\n'
       b'        DeleteVm(controller, 0x62);\r\n'
       b'        CreateVm(controller, rootIndex);\r\n')
NEW = OLD.replace(b'DeleteVm(controller, 0x62)', b'DeleteVm(controller, rootIndex)', 1)
def sha(data):
    return hashlib.sha256(data).hexdigest()
def reconstruct(root):
    root = Path(root)
    if not root.is_absolute():
        raise ValueError('Explicit absolute baseline root required')
    cpp = (root / 'src/FrontEnd.cpp').read_bytes()
    hpp = (root / 'src/FrontEnd.hpp').read_bytes()
    if (sha(cpp), sha(hpp)) != (CPP, HPP):
        raise ValueError('Baseline pin mismatch')
    if cpp.count(OLD) != 1 or cpp.count(MARKER) != 1:
        raise ValueError('Ambiguous edit context')
    begin = cpp.index(MARKER)
    end = cpp.find(b'// TH10_FRONTEND_FUNCTION:', begin + len(MARKER))
    if end < 0:
        end = len(cpp)
    if not begin < cpp.index(OLD) < end:
        raise ValueError('Edit outside UpdateDifficulty')
    candidate = cpp.replace(OLD, NEW, 1)
    if candidate.count(NEW) != 1 or candidate.replace(NEW, OLD, 1) != cpp:
        raise ValueError('Inverse reconstruction failed')
    return {'src/FrontEnd.cpp': candidate, 'src/FrontEnd.hpp': hpp}
