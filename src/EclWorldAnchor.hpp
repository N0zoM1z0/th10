#pragma once

struct AnmFloat3View;

// Target 0x00418810, live EAX world position / EAX persistent screen anchor.
// Original C++ source ABI and data storage ownership are not established.
AnmFloat3View *__fastcall PublishEclWorldAnchor(
    const AnmFloat3View *position);
