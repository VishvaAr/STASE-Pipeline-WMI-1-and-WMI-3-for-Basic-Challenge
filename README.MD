# Task
Use KLEE symbolic execution to detect a complex Arbitrary Free (Write-What-Where) vulnerability in a C codebase (metalogin.c).

This project builds upon the STASE pipeline workflow originally designed to detect WMI-1 (Use-After-Free) stale references. I adapted the driver, stubs, and build process to model a more severe exploitation scenario: WMI-3 (Arbitrary Free).


## The Vulnerability: WMI-3 (Arbitrary Free)
While WMI-1 (UAF) simply accesses a stale pointer, WMI-3 leverages that stale pointer to trick the memory allocator into freeing an arbitrary address.

## Logic: 
Allocation: An avatar object is created.

Free: The object is freed, but the global pointer (g_session.current_avatar) remains stale.

Heap Reuse (The Setup): The attacker allocates a new object (simulated via set_start_location) that occupies the exact same memory slot as the freed avatar.

Injection: The attacker writes malicious data into this slot. The stale pointer now interprets this data as valid internal pointers (avatar->username).

Trigger: The program calls free() on the stale pointer again. Instead of freeing valid heap memory, it tries to free the address specified by the attacker (stack address or kernel pointer).


## Files:
metalogin.c:The target vulnerable application (Avatar Manager).

driver_wmi3_arbfree.c:Custom KLEE driver. It initializes the system, mocks the heap reuse by injecting a pointer to a stack variable, and triggers the invalid free."

stubs_wmi3.c: Minimal environment stubs. Replaces stubs_demo.c to avoid conflicts with KLEE's uclibc runtime (which is required for malloc/free modeling).

build_wmi3.sh: Build script that compiles the target, driver, and stubs into LLVM bitcode using Clang 13."

run_wmi3.sh: Execution script. Runs KLEE with --libc=uclibc and --posix-runtime to correctly model the allocator state.

## How to Run:

./build_wmi3.sh
./run_wmi.sh

## Expected Output:
KLEE: ERROR: ../metalogin.c:48: memory error: out of bound pointer
This confirms that the application is vulnerable to Arbitrary Free via the stale pointer in clear_avatar().
