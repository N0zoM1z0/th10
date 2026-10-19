"""Abstract stable-state witness, not runtime execution or lifetime proof."""
def retire(slots, nodes, index, events):
    handle = slots.get(index, 0)
    events.append(('lookup', index, handle))
    vm = nodes.get(handle)
    if vm is not None:
        vm['pending'] = 1
        events.append(('request', handle))
        if vm['previous'] is None:
            for child in vm['successors']:
                nodes[child]['pending'] = 1
                events.append(('request', child))
    slots[index] = 0
    events.append(('clear', index))

def initialize(difficulty, backdrop, slots, nodes, proposed):
    if difficulty not in (0, 1, 2, 3, 4):
        raise ValueError('Ordinary difficulty domain required')
    root = 0x77 + (difficulty >= 4)
    events = [('backdrop_lookup', 0x5e)]
    if not backdrop:
        events += [('create', 0x5e), ('create_aux', 8, 15)]
    events.append(('count', 4 if difficulty < 4 else 1))
    retire(slots, nodes, root if proposed else 0x62, events)
    # Distinct successful created handles; no callback mutation in this model.
    slots[root] = 1000 + root
    events += [('create', root), ('restart', root, 3), ('interrupt', root, 0x11)]
    slots[0x62] = 1098
    events += [('create', 0x62), ('state', 'opening'), ('timer', 0)]
    return events

def character_return(slots):
    """Only source-authored slot effects, assuming no external mutation/death."""
    for index in (0x62, 0x7d, 0x63):
        slots[index] = 0
    return ('difficulty', 'initialize', 0)
