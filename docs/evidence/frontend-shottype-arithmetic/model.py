"""Finite-domain arithmetic/capture witness, not native behavior or helper ABI."""
def index(character, selection, proposed):
    if type(character) is not int or character not in (0, 1):
        raise ValueError('Premise: character is integer 0 or 1')
    if type(selection) is not int or selection not in (0, 1, 2):
        raise ValueError('Premise: selection is integer 0, 1 or 2')
    base = character * 3 + 0x3f
    value = base * 2 + selection if proposed else base + selection
    assert -32768 <= value <= 32767  # Explicit short conversion preserves value here.
    return value

def confirm(character, selection, proposed, state_callback=None, sound_callback=None):
    state = {'character': character, 'selection': selection}
    root = character + 0x96  # Captured at maintained handler entry.
    argument = index(character, selection, proposed)  # Captured before callbacks.
    events = [('capture_root', root), ('capture_short_argument', argument)]
    events.append(('SetScreenState', 'CONFIRMED'))
    if state_callback is not None:
        state_callback(state)
    events.append(('FrontEndPlaySound', 'SELECT'))
    if sound_callback is not None:
        sound_callback(state)
    events.append(('InterruptChildVm', root, argument, 6))
    return state, events
