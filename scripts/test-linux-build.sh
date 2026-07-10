#!/usr/bin/env bash

set -euo pipefail

preset="${1:-linux-x64-debug}"

case "$preset" in
    linux-x64-debug|linux-x64-release)
        ;;
    *)
        echo "Unsupported Linux preset: $preset" >&2
        echo "Use linux-x64-debug or linux-x64-release." >&2
        exit 2
        ;;
esac

repo_root="$(
    cd "$(dirname "${BASH_SOURCE[0]}")/.."
    pwd
)"

cd "$repo_root"

if [[ -z "${VCPKG_ROOT:-}" ]]; then
    echo "VCPKG_ROOT is not set." >&2
    exit 3
fi

toolchain_file="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"

if [[ ! -f "$toolchain_file" ]]; then
    echo "vcpkg toolchain was not found: $toolchain_file" >&2
    exit 4
fi

command -v cmake >/dev/null
command -v ninja >/dev/null
command -v c++ >/dev/null

build_dir="$repo_root/out/build/$preset"

if [[ -f "$build_dir/CMakeCache.txt" ]]; then
    cmake -S "$repo_root" -B "$build_dir"
else
    cmake --preset "$preset"
fi

cmake --build --preset "$preset"
ctest --preset "$preset"

echo
echo "Linux build and tests passed: $preset"
