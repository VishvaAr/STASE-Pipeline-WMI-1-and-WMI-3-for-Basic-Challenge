#!/usr/bin/env bash
set -euxo pipefail

# Usage:
#   ./run_wmi3.sh
#
# Make sure you've built wmi3_arbfree.bc first using ./build_wmi3.sh

# --- Configuration ---
BC_FILE="wmi3_arbfree.bc"

# Check if the bitcode exists
if [ ! -f "$BC_FILE" ]; then
    echo "[ERROR] $BC_FILE not found. Did you run ./build_wmi3.sh?"
    exit 1
fi

# --- Run KLEE ---
# We MUST use --libc=uclibc and --posix-runtime here.
# Why? The Arbitrary Free check relies on KLEE's internal allocator 
# to detect that we are freeing a stack address (alloca) or invalid memory.
# Without libc support, KLEE might not model free() correctly.

klee \
    --libc=uclibc \
    --posix-runtime \
    --search=bfs \
    --max-time=30s \
    --exit-on-error-type=Ptr \
    --exit-on-error-type=Assert \
    "$BC_FILE"

echo "[OK] KLEE finished; see klee-last/ for results."
