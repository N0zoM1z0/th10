from pathlib import Path
import hashlib
root=Path.cwd()
source=root/'src/GameManager.cpp'
assert hashlib.sha256(source.read_bytes()).hexdigest() == '272f7522ead73a5e446b4580350bcf612b675abdb00f13113aefbb54b5764b42', 'source checkpoint changed'
destination=root/'.analysis/gpt-dots/replay-controller-cleanup-20261025/candidate'
destination.mkdir(parents=True,exist_ok=True)
out=destination/'GameManager.cpp'
expected=source.read_text()+(root/'docs/evidence/replay-controller-cleanup-20261025/controller-proposal.cpp.inc').read_text()
if out.exists():assert out.read_text()==expected, 'preserve different proposal'
else:out.write_text(expected)
