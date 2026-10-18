// Independent host semantic candidate. No target ABI/layout or body-equivalence claim.
// Opaque hooks may mutate every Context and Globals field. Interface shapes below
// are host-model hypotheses, not recovered native helper declarations.
#include <cstdint>

namespace transition_candidate {
struct Context {
    std::uint32_t field38c{}, field390{}, field394{}, field39c{};
    std::uint32_t field768{}, field780{};
    std::uint8_t field6f9{};
};
struct Globals {
    std::uint32_t data474c80{}, data474c7c{}, data477848{}, data491c00{};
};
struct Hooks {
    virtual ~Hooks() = default;
    virtual void enter(Context&, Globals&) = 0;
    virtual void leave(Context&, Globals&) = 0;
    virtual std::uint32_t call41fd00(Context&, Globals&) = 0;
    virtual void call4203f0(Context&, Globals&) = 0;
    virtual void call421c50(Context&, Globals&) = 0;
    virtual void call42cd50(Context&, Globals&) = 0;
    virtual void call418150(Context&, Globals&) = 0;
    virtual void call40b9d0(Context&, Globals&) = 0;
    virtual void call42cdb0(Context&, Globals&) = 0;
    virtual void call4180e0(Context&, Globals&) = 0;
    virtual void call40b940(Context&, Globals&) = 0;
};

int transition(Context& state, Globals& globals, Hooks& hooks) {
    const auto equalityRequest = state.field38c;
    const auto equalityCurrent = state.field390;
    if (equalityRequest == equalityCurrent) return 1;

    hooks.enter(state, globals);
    const auto dispatchCurrent = state.field390; // Fresh before the prefix stores.
    const auto previousRequest = state.field38c; // Independent fresh post-Enter read.
    state.field6f9 = static_cast<std::uint8_t>(state.field6f9 + 1u);
    state.field394 = previousRequest;
    state.field780 = 0xff000000u;
    switch (dispatchCurrent) {
    case 0: {
        state.field390 = 1;
        const auto result = hooks.call41fd00(state, globals);
        state.field768 = result; // Unconstrained machine-value model, not pointer inference.
        if (result == 0) {
            state.field390 = 3;
            hooks.call4203f0(state, globals);
            hooks.call421c50(state, globals);
            return 4;
        }
        break;
    }
    case 3:
        hooks.call4203f0(state, globals);
        hooks.call421c50(state, globals);
        return 4;
    case 4: {
        const auto request = state.field38c;
        switch (request) {
        case 1: case 2:
            hooks.call42cd50(state, globals);
            break;
        case 7:
            hooks.call418150(state, globals);
            hooks.call42cd50(state, globals);
            break;
        case 14:
            hooks.call40b9d0(state, globals);
            hooks.call42cd50(state, globals);
            break;
        default: break;
        }
        break;
    }
    case 7:
        if (state.field38c == 4) hooks.call42cdb0(state, globals);
        state.field39c = 1;
        hooks.call4180e0(state, globals);
        break;
    case 10: {
        hooks.call418150(state, globals);
        state.field39c = 1;
        state.field390 = 7;
        const auto scalar = globals.data474c80;
        globals.data474c7c = scalar;
        globals.data477848 = 0x474788u + 0x30u * scalar;
        hooks.call4180e0(state, globals);
        break;
    }
    case 11: {
        const auto request = state.field38c;
        state.field39c = 0;
        if (request == 7) hooks.call418150(state, globals);
        state.field390 = 7;
        hooks.call4180e0(state, globals);
        break;
    }
    case 12:
        if (state.field38c == 4) hooks.call42cdb0(state, globals);
        state.field390 = 7;
        state.field39c = 1;
        hooks.call4180e0(state, globals);
        break;
    case 13:
        hooks.call418150(state, globals);
        state.field39c = 1;
        state.field390 = 7;
        hooks.call4180e0(state, globals);
        break;
    case 14:
        if (state.field38c == 7) hooks.call418150(state, globals);
        hooks.call40b940(state, globals);
        break;
    case 15: {
        const auto request = state.field38c;
        if (request == 2 || request == 7 || request == 14) {
            if (request == 7) hooks.call418150(state, globals);
            if (request == 14) hooks.call40b9d0(state, globals);
            state.field390 = 4;
            globals.data491c00 = 3;
            hooks.call42cd50(state, globals);
        }
        break;
    }
    default: break;
    }
    state.field38c = state.field390; // Fresh after all mode-specific hooks.
    hooks.leave(state, globals);
    state.field6f9 = static_cast<std::uint8_t>(state.field6f9 - 1u);
    return 1;
}
} // namespace transition_candidate
