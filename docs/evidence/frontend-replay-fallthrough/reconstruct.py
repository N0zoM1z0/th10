"""Pure byte reconstruction after explicit-root, pinned source reads. No writes on import."""
from pathlib import Path
import hashlib

CPP_SHA256 = 'c26c9c1c00b3caa98a01fc210efdbb898fe6e8cace5a4d84a8072cd3f8a90d5d'
HPP_SHA256 = '8b4e0e13312bce0b9bab71e16daff79cfa402d55a61e55cca53bd359edc7832b'
FUNCTION = b'// TH10_FRONTEND_FUNCTION: 0x004315C0 FrontEndControllerView::UpdateReplay\r\n'
OLD = (b'        controller->replayListOffset = 0;\r\n'
       b'        break;\r\n'
       b'    }\r\n\r\n'
       b'    case FRONT_END_REPLAY_OPENING:\r\n'
       b'        if (controller->stateTimer.current > 6)\r\n')
REMOVED = b'        break;\r\n'
NEW = OLD.replace(REMOVED, b'', 1)

def sha256(data):
    return hashlib.sha256(data).hexdigest()

def reconstruct(source_root):
    """Return candidate bytes; source_root must explicitly name an absolute root."""
    root = Path(source_root)
    if not root.is_absolute():
        raise ValueError('An explicit absolute source root is required')
    cpp = (root / 'src/FrontEnd.cpp').read_bytes()
    hpp = (root / 'src/FrontEnd.hpp').read_bytes()
    if (sha256(cpp), sha256(hpp)) != (CPP_SHA256, HPP_SHA256):
        raise ValueError('Baseline source pin mismatch')
    if cpp.count(FUNCTION) != 1 or cpp.count(OLD) != 1:
        raise ValueError('Ambiguous function or edit context')
    start = cpp.index(FUNCTION)
    end = cpp.find(b'// TH10_FRONTEND_FUNCTION:', start + len(FUNCTION))
    if end < 0:
        end = len(cpp)
    if not start < cpp.index(OLD) < end:
        raise ValueError('Edit context is outside UpdateReplay')
    candidate = cpp.replace(OLD, NEW, 1)
    if candidate.count(NEW) != 1 or candidate.replace(NEW, OLD, 1) != cpp:
        raise ValueError('Inverse reconstruction failed')
    return {'src/FrontEnd.cpp': candidate, 'src/FrontEnd.hpp': hpp}
