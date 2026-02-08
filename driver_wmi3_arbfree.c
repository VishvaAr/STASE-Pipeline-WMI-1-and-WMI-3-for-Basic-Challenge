#include <stdint.h>
#include <stddef.h>
#include <stdlib.h> // Needed for malloc/free mocks if strictly standard

#ifdef __KLEE__
  #include "klee/klee.h"
  // We don't strictly need check_memory_access for ArbFree, 
  // because KLEE crashes automatically on invalid free.
#else
  #define klee_make_symbolic(a,b,c) ((void)0)
  #define klee_assume(x) ((void)0)
#endif

#include "metalogin.h"

// Functions we need for WMI 3
void init_system(void);
int inventory_remove_by_obj(long inventory_obj);

// Global session
extern session g_session;

int main(void) {
  init_system();

  // --- WMI 3: ARBITRARY FREE ATTACK SIMULATION ---

  // 1. The Target: A variable on the STACK. 
  // In a secure program, you should NEVER be able to free this.
  long stack_target = 0xCAFEBABE; 
  
  // 2. The Setup: We simulate the "Corruption" step.
  // We manually point the inventory list to our stack variable.
  // In the real exploit, you'd do this via a buffer overflow. 
  // Here, we just "assume" the attacker has achieved this state.
  g_session.inventory = (item *)&stack_target;

#ifdef __KLEE__
  // 3. The Constraint: 
  // The code acts on 'cur->inventory_obj'. Since 'cur' is our stack_target,
  // the 'inventory_obj' IS the value of stack_target (0xCAFEBABE).
  // We simply pass that ID to the remove function.
  long id_to_remove = stack_target; 
#else
  long id_to_remove = 0xCAFEBABE;
#endif

  // 4. The Trigger
  // This function will:
  //   a. Walk the list (finding our stack pointer)
  //   b. Match the ID (fake_item->inventory_obj == id_to_remove)
  //   c. Call free(fake_item) <--- THIS IS free(&stack_target)
  inventory_remove_by_obj(id_to_remove);

  // If we get here, the bug was NOT found. 
  // KLEE should report an error BEFORE this line ("free of alloca" or similar).
  return 0;
}
