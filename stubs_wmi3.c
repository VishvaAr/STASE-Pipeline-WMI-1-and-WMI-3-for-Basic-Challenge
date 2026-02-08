#include <stdlib.h>

// --- Math Stubs ---
// We stub these because we aren't linking against libm (-lm)
// and metalogin.c uses them for the "start location" math.
double sin(double x) { return 0.0; }
double cos(double x) { return 0.0; }
long long llround(double x) { return (long long)x; }

// --- Project Stubs ---
// metalogin.c calls this, so we must provide a definition.
void print_card(void) {
    // No-op for symbolic execution
}

/* NOTE: We do NOT define printf, stdin, malloc, strlen, etc.
   The --libc=uclibc flag in KLEE provides these automatically.
*/
