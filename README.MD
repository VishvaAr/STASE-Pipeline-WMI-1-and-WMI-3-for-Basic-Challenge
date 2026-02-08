Task: Use KLEE symbolic execution to detect a complex Arbitrary Free (Write-What-Where) vulnerability in a C codebase (metalogin.c).

This project builds upon the STASE pipeline workflow originally designed to detect WMI-1 (Use-After-Free) stale references. We adapted the driver, stubs, and build process to model a more severe exploitation scenario: WMI-3 (Arbitrary Free).
