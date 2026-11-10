# Bullet transform timers and update count: native DWORD increments

Japanese TH10 v1.00a target SHA-256: 2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.

Six BulletRuntimeView transform code paths still computed their state.timer.current increment as unsigned 32-bit addition followed by a potentially implementation-defined signed-int conversion. Native i386 instead advances the low DWORD, including at INT_MAX. The new C++ increments the existing signed timer word through the corresponding unsigned int lvalue. Signed thresholds, floating-point subframe handling, and playback logic remain unchanged.

BulletManagerView::UpdateBullets resets activeBulletCount to zero and visits exactly 2,000 records, incrementing it at most once per visited record. Thus its count never reaches INT_MAX during an actual invocation. An ordinary defined signed ++activeBulletCount replaces an unnecessary unsigned-add-then-narrow expression, retaining exactly the target codegen.

Pinned MSVC 7.1 SP1 /GL /GS baseline and combined source variants were compiled into three genuine graphs: the four-TU BulletRuntimeDeactivate, one-TU BulletManagerUpdateCallback and seven-TU BulletHorizontalWrap. Complete accepted owner sizes, original bytes and normalized bindings are unchanged: UpdateDeceleration 164/164 (7 fields), UpdateVectorAcceleration 305/305 (10 fields), SpawnBulletPattern 194/194 (6 fields), UpdateBullets 249/249 (6 fields), and update callback 40/40 (2 fields). The incomplete UpdateAbsoluteDirectionChange remains a full 335-byte diagnostic candidate with 301/303 ordinary bytes, eight candidate-discovered fields and the same +0xD7,+0xDD register mismatches; no exact credit is claimed for it.

Trial: .analysis/gpt-web/20261110-bullet-timer-native-words.py; results: build/gpt-web/20261110/bullet-timer-word/results.jsonl. Rebuild only affected accepted Bullet transform and manager units. No full cold replay, target byte patches or original source identity claim.
