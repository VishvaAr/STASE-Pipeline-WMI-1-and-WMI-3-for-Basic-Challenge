#!/usr/bin/env bash
set -euxo pipefail

# --- CONFIGURATION ---
# Force Clang 13 to match your KLEE version
CLANG="clang-13"
LINK="llvm-link-13"

# Flags: -disable-O0-optnone is required for Clang 13+
CFLAGS=(-I. -O0 -g -emit-llvm -Xclang -disable-O0-optnone)

echo "[*] Compiling Driver (driver_wmi3_arbfree.c)..."
$CLANG "${CFLAGS[@]}" -c driver_wmi3_arbfree.c -o driver.bc

echo "[*] Compiling WMI-3 Stubs (stubs_wmi3.c)..."
$CLANG "${CFLAGS[@]}" -c stubs_wmi3.c -o stubs_wmi3.bc

echo "[*] Compiling Target (metalogin.c)..."
$CLANG "${CFLAGS[@]}" -DKLEE_DRIVER_BUILD -c ./metalogin.c -o metalogin.bc

echo "[*] Linking modules..."
# Note: We link stubs_wmi3.bc here
$LINK driver.bc stubs_wmi3.bc metalogin.bc -o wmi3_arbfree.bc

echo "[OK] Built wmi3_arbfree.bc using Clang 13 and clean stubs."
