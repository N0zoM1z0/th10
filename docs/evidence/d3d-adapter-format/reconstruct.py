"""Pinned explicit-root reconstruction, in memory; no import-time I/O."""
from pathlib import Path
import hashlib
CPP='fcf7ba4fa5b6af9ce447aa77f2da58eae3c2d53f52b1666b592e9c86b41a40a4'
HPP='ae49087657d26b2068e9ad73e6994f5fbf1efaebbafed5fad92e508c7938cb03'
MARKER=b'// TH10_MAIN_FUNCTION: 0x00439890 GameWindowView::InitD3DRendering\n'
OLD=b'                MAIN_D3D_DEVICE_HAL, displayMode.format, 0,\n'
NEW=b'                MAIN_D3D_DEVICE_HAL, presentParameters.backBufferFormat, 0,\n'
def sha(b):return hashlib.sha256(b).hexdigest()
def reconstruct(root):
 root=Path(root)
 if not root.is_absolute():raise ValueError('Absolute baseline root required')
 cpp=(root/'src/Main.cpp').read_bytes();hpp=(root/'src/Main.hpp').read_bytes()
 if (sha(cpp),sha(hpp))!=(CPP,HPP):raise ValueError('Baseline pins')
 begin=cpp.index(MARKER);end=cpp.index(b'// TH10_MAIN_FUNCTION:',begin+len(MARKER))
 if cpp.count(OLD)!=1 or not begin<cpp.index(OLD)<end:raise ValueError('Unique owner context')
 new=cpp.replace(OLD,NEW,1)
 if new.count(NEW)!=1 or new.replace(NEW,OLD,1)!=cpp:raise ValueError('Inverse')
 return {'src/Main.cpp':new,'src/Main.hpp':hpp}
