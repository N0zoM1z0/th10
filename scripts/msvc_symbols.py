"""Small helpers for conservative MSVC decorated-symbol handling."""

from __future__ import annotations


def source_name_hint(symbol: str) -> str | None:
    """Return a conservative source spelling for common external MSVC names."""
    if symbol.startswith("??0") or symbol.startswith("??1"):
        separator = symbol.find("@@", 3)
        if separator < 0:
            return None
        scopes = symbol[3:separator].split("@")
        if not scopes or any(not scope for scope in scopes):
            return None
        qualified_class = "::".join(reversed(scopes))
        method = scopes[0] if symbol.startswith("??0") else f"~{scopes[0]}"
        return f"{qualified_class}::{method}"
    if not symbol.startswith("?") or symbol.startswith("??"):
        return None
    separator = symbol.find("@@", 1)
    if separator < 0:
        return None
    components = symbol[1:separator].split("@")
    if not components or any(not component for component in components):
        return None
    function = components[0]
    scopes = list(reversed(components[1:]))
    return "::".join([*scopes, function])
