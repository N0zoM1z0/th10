#!/usr/bin/env bash
set -euo pipefail

repo_root=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
msvc_root=${TH10_MSVC71_ROOT:-$repo_root/.tools/msvc710-sp1}
wine_prefix=${TH10_WINEPREFIX:-${WINEPREFIX:-$repo_root/.tools/wineprefix-msvc710-sp1}}
wine_bin=${WINE:-wine}

if [[ $# -lt 3 ]]; then
  echo "usage: $0 SOURCE OUTPUT.obj MSVC_FLAG..." >&2
  echo "flags are mandatory because TH10 contains both normal COFF and LTCG inputs" >&2
  exit 2
fi

source_path=$1
output_path=$2
shift 2
compiler="$msvc_root/Vc7/bin/cl.exe"

if [[ ! -f "$source_path" ]]; then
  echo "missing probe source: $source_path" >&2
  exit 1
fi
if [[ ! -f "$compiler" ]]; then
  echo "missing pinned VC7.1 SP1 compiler; set TH10_MSVC71_ROOT" >&2
  exit 1
fi

expected_sha256=$(python3 - "$repo_root" <<'PY'
from pathlib import Path
import sys
import tomllib

with (Path(sys.argv[1]) / "config" / "tools.lock.toml").open("rb") as stream:
    print(tomllib.load(stream)["msvc71"]["compiler_sha256"])
PY
)
actual_sha256=$(sha256sum "$compiler" | awk '{print $1}')
if [[ "$actual_sha256" != "$expected_sha256" ]]; then
  echo "pinned VC7.1 SP1 compiler hash mismatch: $actual_sha256" >&2
  exit 1
fi

mkdir -p "$(dirname -- "$output_path")"
wine_env=(env WINEARCH=win32 WINEPREFIX="$wine_prefix" WINEDEBUG=-all WINEDLLOVERRIDES=mscoree,mshtml=)
source_win=$("${wine_env[@]}" winepath -w "$(realpath "$source_path")" | tr -d '\r')
output_win=$("${wine_env[@]}" winepath -w "$(realpath -m "$output_path")" | tr -d '\r')
pdb_path="${output_path%.*}.pdb"
pdb_win=$("${wine_env[@]}" winepath -w "$(realpath -m "$pdb_path")" | tr -d '\r')
vc_include=$("${wine_env[@]}" winepath -w "$msvc_root/Vc7/include" | tr -d '\r')
sdk_include=$("${wine_env[@]}" winepath -w "$msvc_root/Vc7/PlatformSDK/Include" | tr -d '\r')
vc_lib=$("${wine_env[@]}" winepath -w "$msvc_root/Vc7/lib" | tr -d '\r')
sdk_lib=$("${wine_env[@]}" winepath -w "$msvc_root/Vc7/PlatformSDK/Lib" | tr -d '\r')

INCLUDE="$vc_include;$sdk_include" \
LIB="$vc_lib;$sdk_lib" \
"$repo_root/scripts/run-headless-wine.sh" "$wine_bin" "$compiler" \
  /nologo /c "$@" "$source_win" "/Fo$output_win" "/Fd$pdb_win"
