#ifndef IPA_EXECUTE_H
#define IPA_EXECUTE_H

#include "pulp.h"

#define IPA_EXECUTE (ARCHI_IPA_ADDR)
#define IPA_EXECUTE_STATUS_REGISTER (ARCHI_IPA_ADDR + 4)
#define IPA_EXECUTE_COMMAND_REGISTER (ARCHI_IPA_ADDR + 0)
static inline int ipa_start_execution()
{
  *(volatile int*) IPA_EXECUTE_COMMAND_REGISTER = 2;
  return *(volatile int*) IPA_EXECUTE_COMMAND_REGISTER;
}

static inline void ipa_execute_barrier(int id)
{ 
  
  while(*(volatile int*) IPA_EXECUTE_STATUS_REGISTER != 1) 
    {}
}

static inline void free_ipa_execute(int id)
 {
   *(volatile int*) IPA_EXECUTE_STATUS_REGISTER = 0x1 << id;
}
#endif
