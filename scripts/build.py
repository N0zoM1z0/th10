#!/usr/bin/env python3
"""Build or validate the TH10 v1.00a Windows i386 PE32 GUI executable.

Uses the pinned VC7.1 SP1 6030 toolchain (/GL + /LTCG) under headless Wine:
  - Generates undecorated import libraries for d3d9.dll and d3dx9_31.dll
  - Compiles Win32 resources (src/th10.rc) via rc.exe
  - Compiles all active src/*.cpp translation units via cl.exe (including pure
    C++ ProductStrings.cpp and ProductGlobals.cpp)
  - Links build/windows-i386/th10.exe (or th10-reordered.exe with --reverse-link)
"""

from __future__ import annotations

import argparse
from pathlib import Path
import struct
import subprocess
import sys
import tomllib

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "scripts"))
import ltcg_link as link  # noqa: E402

OUT_DIR = ROOT / "build" / "windows-i386"
OBJ_DIR = OUT_DIR / "obj"
LIB_DIR = OUT_DIR / "libs"

ALTERNATENAMES = [
    "?g_InputKeyboardDataFormat@@3PAEA=_c_dfDIKeyboard",
    "?g_InputJoystickDataFormat@@3PAEA=_c_dfDIJoystick2",
    "?g_InputKeyboardGuid@@3PAEA=_GUID_SysKeyboard",
    "?g_InputDirectInputIID@@3PAEA=_IID_IDirectInput8A",
    "?GameCreateOwner42B660@@YAPAXXZ=?GameCreateOwner42B660@@YAPAUScriptViewerPopupOwnerView@@XZ",
    "?D3DXVec3Project@@YGPAUPlayerFloat3@@PAU1@PBU1@PBX222@Z=_D3DXVec3Project@24",
]

D3D9_DEF = """LIBRARY d3d9.dll
EXPORTS
    Direct3DCreate9@4
"""

D3DX9_31_DEF = """LIBRARY d3dx9_31.dll
EXPORTS
    D3DXVec3Normalize@8
    D3DXLoadSurfaceFromSurface@32
    D3DXLoadSurfaceFromFileInMemory@36
    D3DXCreateTextureFromFileInMemoryEx@60
    D3DXVec3Transform@12
    D3DXVec4Transform@12
    D3DXMatrixRotationZ@8
    D3DXMatrixRotationY@8
    D3DXMatrixMultiply@12
    D3DXMatrixRotationX@8
    D3DXLoadSurfaceFromMemory@40
    D3DXCreateTexture@32
    D3DXMatrixPerspectiveFovLH@20
    D3DXMatrixLookAtLH@16
    D3DXVec3Project@24
    D3DXVec3ProjectArray@36
    D3DXMatrixTranslation@16
"""


def load() -> dict:
    with (ROOT / "config" / "build.toml").open("rb") as stream:
        return tomllib.load(stream)


def validate(document: dict) -> None:
    if document.get("schema_version") != 1:
        raise ValueError("unsupported build schema")
    graph = document.get("graph", {})
    for field in ("sources", "libraries", "resources"):
        if not isinstance(graph.get(field), list):
            raise ValueError(f"build graph {field} must be a list")
    acceptance = document.get("acceptance", {})
    if acceptance.get("whole_build_closed") and not graph["sources"]:
        raise ValueError("closed whole build requires at least one translation unit")


def patch_import_lib_undecorate(lib_path: Path) -> int:
    """Flip COFF short-import NameType from IMPORT_NAME (2) to
    IMPORT_NAME_UNDECORATE (3) so link.exe emits undecorated DLL export names
    (e.g. Direct3DCreate9) while resolving stdcall _Direct3DCreate9@4 symbols."""
    data = bytearray(lib_path.read_bytes())
    if data[:8] != b"!<arch>\n":
        raise ValueError(f"{lib_path} is not a COFF archive")
    pos = 8
    patched = 0
    while pos + 60 <= len(data):
        size_str = data[pos + 48 : pos + 58].decode("ascii", errors="replace").strip()
        member_size = int(size_str)
        body = pos + 60
        if member_size >= 20:
            sig1, sig2, ver, mach = struct.unpack_from("<HHHH", data, body)
            if sig1 == 0 and sig2 == 0xFFFF and ver == 0 and mach == 0x014C:
                type_word = struct.unpack_from("<H", data, body + 18)[0]
                name_type = (type_word >> 2) & 0x7
                if name_type == 2:
                    new_word = (type_word & ~0x1C) | (3 << 2)
                    struct.pack_into("<H", data, body + 18, new_word)
                    patched += 1
        pos = body + member_size + (member_size & 1)
    lib_path.write_bytes(bytes(data))
    return patched


def select_profiles(excluded: set[str]) -> dict[str, list[str]]:
    doc = tomllib.loads((ROOT / "config" / "match-units.toml").read_text())
    profiles: dict[str, list[str]] = {}
    default = ["/TP", "/MT", "/O2", "/Gy", "/GF", "/Oi", "/DNDEBUG", "/Isrc", "/GL"]

    for _name, u in doc["units"].items():
        s = u.get("source")
        if s and s.startswith("src/") and "/GL" in u.get("profile", []):
            profiles.setdefault(s, list(u["profile"]))

    ede = doc["units"].get("enemy-dispatch-ecl", {})
    for s, p in ede.get("support_profiles", {}).items():
        if s.startswith("src/"):
            profiles[s] = list(p)
    if ede.get("source"):
        profiles[ede["source"]] = list(ede["profile"])

    # Pure C++ product data TUs are compiled as standard non-/Gy COFF so VC7.1
    # preserves declaration order for contiguous struct blocks and linker pragmas.
    non_ltcg_data_tus = {"src/ProductGlobals.cpp", "src/ProductStrings.cpp"}
    data_profile = ["/TP", "/MT", "/O2", "/DNDEBUG", "/Isrc"]

    result: dict[str, list[str]] = {}
    for p in sorted((ROOT / "src").glob("*.cpp")):
        rel = str(p.relative_to(ROOT))
        if rel in excluded:
            continue
        if rel in non_ltcg_data_tus:
            result[rel] = list(data_profile)
            continue
        prof = list(profiles.get(rel, default))
        if "/GL" not in prof:
            prof.append("/GL")
        result[rel] = [f for f in prof if f != "/Gz"]
    return result


def setup_wine_env() -> tuple[Path, dict[str, str]]:
    linker, env = link.tool_environment()
    msvc_root = linker.parents[2]
    vc_inc = link.windows_path(msvc_root / "Vc7" / "include", env)
    sdk_inc = link.windows_path(msvc_root / "Vc7" / "PlatformSDK" / "Include", env)
    vc_lib = link.windows_path(msvc_root / "Vc7" / "lib", env)
    sdk_lib = link.windows_path(msvc_root / "Vc7" / "PlatformSDK" / "Lib", env)
    out_lib = link.windows_path(LIB_DIR, env)
    env["INCLUDE"] = f"{vc_inc};{sdk_inc}"
    env["LIB"] = f"{out_lib};{vc_lib};{sdk_lib}"
    env["PATH"] = f"{ROOT / '.tools' / 'factory-bin'}:{env.get('PATH', '')}"
    return msvc_root, env


def run_wine(cmd: list[str], env: dict[str, str]) -> subprocess.CompletedProcess[str]:
    return subprocess.run(
        [str(ROOT / "scripts" / "run-headless-wine.sh"), "wine", *cmd],
        cwd=ROOT,
        env=env,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
        errors="replace",
    )


def build_import_libs(msvc_root: Path, env: dict[str, str], force: bool = False) -> None:
    LIB_DIR.mkdir(parents=True, exist_ok=True)
    lib_exe = str(msvc_root / "Vc7" / "bin" / "lib.exe")
    for name, content in (("d3d9", D3D9_DEF), ("d3dx9_31", D3DX9_31_DEF)):
        def_path = LIB_DIR / f"{name}.def"
        lib_path = LIB_DIR / f"{name}.lib"
        marker = LIB_DIR / f"{name}.undecorated"
        need = (
            force
            or not lib_path.is_file()
            or not marker.is_file()
            or (def_path.read_text() != content if def_path.is_file() else True)
        )
        if need:
            def_path.write_text(content)
            def_win = link.windows_path(def_path, env)
            lib_win = link.windows_path(lib_path, env)
            res = run_wine([lib_exe, "/nologo", "/machine:x86", f"/def:{def_win}", f"/out:{lib_win}"], env)
            if res.returncode != 0:
                print(res.stdout)
                res.check_returncode()
            patched = patch_import_lib_undecorate(lib_path)
            marker.write_text(f"patched={patched}\n")
            print(f"[lib] {name}.lib built ({patched} undecorated imports)", flush=True)


def compile_resources(msvc_root: Path, env: dict[str, str], rc_rel: str, force: bool = False) -> Path:
    OBJ_DIR.mkdir(parents=True, exist_ok=True)
    rc_path = ROOT / rc_rel
    res_path = OBJ_DIR / f"{rc_path.stem}.res"
    if force or not res_path.is_file() or res_path.stat().st_mtime < rc_path.stat().st_mtime:
        rc_exe = str(msvc_root / "Vc7" / "bin" / "rc.exe")
        rc_win = link.windows_path(rc_path, env)
        res_win = link.windows_path(res_path, env)
        print(f"[rc] {rc_rel}", flush=True)
        res = run_wine([rc_exe, f"/fo{res_win}", rc_win], env)
        if res.returncode != 0:
            print(res.stdout)
            res.check_returncode()
    return res_path


def assemble_bss_layout(msvc_root: Path, env: dict[str, str], force: bool = False) -> Path:
    OBJ_DIR.mkdir(parents=True, exist_ok=True)
    asm = ROOT / "src" / "ProductBssLayout.asm"
    obj = OBJ_DIR / "ProductBssLayout.obj"
    if force or not obj.is_file() or obj.stat().st_mtime < asm.stat().st_mtime:
        ml_exe = str(msvc_root / "Vc7" / "bin" / "ml.exe")
        asm_win = link.windows_path(asm, env)
        obj_win = link.windows_path(obj, env)
        print("[asm] src/ProductBssLayout.asm", flush=True)
        res = run_wine([ml_exe, "/nologo", "/c", "/coff", f"/Fo{obj_win}", asm_win], env)
        if res.returncode != 0:
            print(res.stdout)
            res.check_returncode()
    return obj


def compile_all(
    msvc_root: Path,
    env: dict[str, str],
    profiles: dict[str, list[str]],
    force: bool = False,
) -> list[Path]:
    OBJ_DIR.mkdir(parents=True, exist_ok=True)
    cl_exe = str(msvc_root / "Vc7" / "bin" / "cl.exe")
    obj_dir_win = link.windows_path(OBJ_DIR, env) + "\\"
    pdb_win = link.windows_path(OBJ_DIR / "vc70.pdb", env)

    groups: dict[tuple[str, ...], list[Path]] = {}
    objects: list[Path] = []
    for rel, prof in sorted(profiles.items()):
        src = ROOT / rel
        obj = OBJ_DIR / f"{src.stem}.obj"
        objects.append(obj)
        if force or not obj.is_file() or obj.stat().st_mtime < src.stat().st_mtime:
            groups.setdefault(tuple(prof), []).append(src)

    dirty = sum(len(v) for v in groups.values())
    print(f"[compile] {len(objects)} TUs ({dirty} to compile in {len(groups)} batches)", flush=True)
    for prof, srcs in sorted(groups.items(), key=lambda kv: -len(kv[1])):
        win_srcs = [link.windows_path(s, env) for s in srcs]
        rsp = OBJ_DIR / "compile.rsp"
        rsp_lines = ["/nologo", "/c", *prof, f"/Fo{obj_dir_win}", f"/Fd{pdb_win}", *win_srcs]
        rsp.write_text("\n".join(rsp_lines) + "\n")
        rsp_win = link.windows_path(rsp, env)
        print(f"[compile] batch ({len(srcs)} files): {' '.join(prof)}", flush=True)
        res = run_wine([cl_exe, f"@{rsp_win}"], env)
        if res.returncode != 0:
            print(res.stdout)
            res.check_returncode()
    return objects


def link_exe(
    msvc_root: Path,
    env: dict[str, str],
    objects: list[Path],
    resources: list[Path],
    libraries: list[str],
    reverse: bool = False,
) -> int:
    link_exe_path = str(msvc_root / "Vc7" / "bin" / "link.exe")
    suffix = "-reordered" if reverse else ""
    exe_path = OUT_DIR / f"th10{suffix}.exe"
    map_path = OUT_DIR / f"th10{suffix}.map"
    pdb_path = OUT_DIR / f"th10{suffix}.pdb"
    rsp_path = OUT_DIR / f"link{suffix}.rsp"

    link_objs = list(reversed(objects)) if reverse else list(objects)
    rsp_lines = [
        "/nologo",
        "/ltcg",
        "/subsystem:windows",
        "/incremental:no",
        "/fixed:no",
        "/base:0x00500000" if reverse else "/base:0x00400000",
        "/debug",
        "/debugtype:cv",
        "/opt:ref",
        "/opt:noicf",
        "/safeseh:no",
        *[f"/alternatename:{a}" for a in ALTERNATENAMES],
        f"/out:{link.windows_path(exe_path, env)}",
        f"/map:{link.windows_path(map_path, env)}",
        f"/pdb:{link.windows_path(pdb_path, env)}",
        *[link.windows_path(o, env) for o in link_objs],
        *[link.windows_path(r, env) for r in resources],
        *libraries,
    ]
    rsp_path.write_text("\n".join(rsp_lines) + "\n")
    rsp_win = link.windows_path(rsp_path, env)
    print(f"[link] {len(link_objs)} objects + {len(resources)} res -> {exe_path.relative_to(ROOT)}", flush=True)
    res = run_wine([link_exe_path, f"@{rsp_win}"], env)
    log_path = OUT_DIR / f"link{suffix}.log"
    log_path.write_text(res.stdout)
    print(f"[link] exit={res.returncode}, log={log_path.relative_to(ROOT)}", flush=True)
    if res.returncode != 0:
        print(res.stdout)
    return res.returncode


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--check", action="store_true", help="Validate config/build.toml only")
    parser.add_argument("--fresh", action="store_true", help="Rebuild all libs, objects, and resources")
    parser.add_argument("--reverse-link", action="store_true", help="Link objects in reverse order (PI check)")
    args = parser.parse_args()

    try:
        document = load()
        validate(document)
    except (OSError, ValueError, tomllib.TOMLDecodeError) as exc:
        print(f"error: invalid build graph: {exc}", file=sys.stderr)
        return 1

    if args.check:
        print(f"build graph OK: status={document['product']['status']} output={document['product']['output']}")
        return 0

    if document["product"]["status"] != "configured":
        print("whole build unavailable: product status is not configured", file=sys.stderr)
        return 2

    OUT_DIR.mkdir(parents=True, exist_ok=True)
    LIB_DIR.mkdir(parents=True, exist_ok=True)
    msvc_root, env = setup_wine_env()

    graph = document["graph"]
    excluded = set(graph.get("excluded_sources", []))
    libraries = list(graph.get("libraries", []))
    rc_files = list(graph.get("resources", []))

    build_import_libs(msvc_root, env, force=args.fresh)
    res_objs = [compile_resources(msvc_root, env, r, force=args.fresh) for r in rc_files]
    bss_obj = assemble_bss_layout(msvc_root, env, force=args.fresh)
    profiles = select_profiles(excluded)
    objects = compile_all(msvc_root, env, profiles, force=args.fresh)
    objects.append(bss_obj)
    return link_exe(msvc_root, env, objects, res_objs, libraries, reverse=args.reverse_link)


if __name__ == "__main__":
    raise SystemExit(main())
