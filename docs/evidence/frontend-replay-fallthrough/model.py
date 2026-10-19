"""Abstract dispatch witness only: no native code, file APIs, or full initialization.

Events symbolize source operations. The injected callback is an adversarial model
input, not a claim that Win32/file APIs mutate the timer. Other states are opaque.
"""
from dataclasses import dataclass, field

@dataclass
class State:
    phase: str
    timer: int
    events: list = field(default_factory=list)
    offset: int = 123


def initialize(s, mutate, vm_missing, user_count):
    if not 0 <= user_count <= 25:
        raise ValueError('user_count must be 0..25; zero models invalid search')
    e = s.events
    e.extend(['cursor.count=25', 'cursor.SetCurrent', 'saved_cursor=0', 'ResolveVm'])
    if vm_missing:
        e.extend(['CreateVm:5e', 'CreateVmVariant0:8'])
    e.extend(['CreateVm:65', 'SetScreenState:OPENING'])
    s.phase, s.timer = 'OPENING', 0
    e.append('clear_replay_owners')
    e.extend('numbered_load:%d' % n for n in range(1, 26))
    e.extend(['mkdir:replay', 'chdir:replay', 'FindFirstFile'])
    for n in range(user_count):
        e.extend(['chdir:parent', 'user_load:%d' % (25 + n), 'chdir:replay'])
        if n < 24:
            e.append('FindNextFile')
    e.extend(['FindClose', 'chdir:parent'])
    if mutate is not None:
        mutate(s)  # Explicit mock boundary, after external initialization events.
    s.offset = 0
    e.append('replayListOffset=0')


def step(phase, timer, fallthrough, mutate=None, vm_missing=False, user_count=0):
    s = State(phase, timer)
    if phase == 'INITIALIZE':
        initialize(s, mutate, vm_missing, user_count)
        if not fallthrough:
            return s
        phase = 'OPENING'
    if phase == 'OPENING':
        s.events.append('opening_predicate')
        if s.timer > 6:
            s.events.append('SetScreenState:SELECT_FILE')
            s.phase, s.timer = 'SELECT_FILE', 0
    else:
        # Non-initial/opening semantics are deliberately uninterpreted.
        s.events.append('unchanged_dispatch:' + phase)
    return s
