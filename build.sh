#!/usr/bin/env bash

# ==========================================
# Script Maked with AI !!!!
# ==========================================


set -Eeuo pipefail

# ==========================================
# Configuration
# ==========================================

PROGRAM="mcbe"
SOURCES=("main.c")

CC_LINUX="${CC_LINUX:-gcc}"
CC_WINDOWS="${CC_WINDOWS:-x86_64-w64-mingw32-gcc}"

CFLAGS=(-O2 -DNDEBUG -Wall -Wextra)

DIST="dist"

# ==========================================
# Arguments
# ==========================================

if [[ $# -ne 1 ]]; then
    echo "Usage: $0 VERSION"
    echo "Example: $0 1.0.0"
    exit 1
fi

VERSION="$1"

if [[ ! "$VERSION" =~ ^[a-zA-Z0-9._+-]+$ ]]; then
    echo "ERROR: Invalid version"
    exit 1
fi

# ==========================================
# Check required tools
# ==========================================

for compiler in "$CC_LINUX" "$CC_WINDOWS"; do
    if ! command -v "$compiler" >/dev/null 2>&1; then
        echo "ERROR: Compiler not found: $compiler"
        exit 1
    fi
done

for tool in file tar zip sha256sum; do
    command -v "$tool" >/dev/null 2>&1 || {
        echo "ERROR: Missing required tool: $tool"
        exit 1
    }
done

# ==========================================
# Prepare directories
# ==========================================

mkdir -p "$DIST"

WORK="$(mktemp -d)"
trap 'rm -rf "$WORK"' EXIT

LINUX="$WORK/${PROGRAM}-linux-x86_64"
WINDOWS="$WORK/${PROGRAM}-windows-x86_64"

mkdir -p "$LINUX" "$WINDOWS"

# ==========================================
# Build Linux x86_64
# ==========================================

echo "[1/2] Building Linux x86_64..."

"$CC_LINUX" "${CFLAGS[@]}" \
    "${SOURCES[@]}" \
    -o "$LINUX/$PROGRAM"

# ==========================================
# Build Windows x86_64
# ==========================================

echo "[2/2] Building Windows x86_64..."

"$CC_WINDOWS" "${CFLAGS[@]}" \
    -static -static-libgcc \
    "${SOURCES[@]}" \
    -o "$WINDOWS/${PROGRAM}.exe"

# ==========================================
# Verify binaries
# ==========================================

echo
echo "Verifying executables..."

file "$LINUX/$PROGRAM"
file "$WINDOWS/${PROGRAM}.exe"

# ==========================================
# Package releases
# ==========================================

echo
echo "Packaging releases..."

tar -czf "$DIST/${PROGRAM}-${VERSION}-linux-x86_64.tar.gz" \
    -C "$WORK" "${PROGRAM}-linux-x86_64"

(
    cd "$WORK"
    zip -qr "$OLDPWD/$DIST/${PROGRAM}-${VERSION}-windows-x86_64.zip" \
        "${PROGRAM}-windows-x86_64"
)

# ==========================================
# Generate SHA256 checksums
# ==========================================

(
    cd "$DIST"
    sha256sum \
        "${PROGRAM}-${VERSION}-linux-x86_64.tar.gz" \
        "${PROGRAM}-${VERSION}-windows-x86_64.zip" \
        > SHA256SUMS
)

echo
echo "=========================================="
echo " RELEASE COMPLETED"
echo "=========================================="

ls -lh "$DIST"
