#ifndef IPA_H
#define IPA_H

#include "pulp.h"

#define TX 0
#define RX 1
#define INC 1
#define FIX 0
#define LIN 0
#define TWD 1

#define IPA_COMMAND_QUEUE (ARCHI_DMAIPA_ADDR + PLP_DMA_QUEUE_OFFSET)
#define IPA_STATUS_REGISTER (ARCHI_DMAIPA_ADDR + PLP_DMA_STATUS_OFFSET)


static inline int ipa_mchan_alloc(){
  return *(volatile int*) IPA_COMMAND_QUEUE;
}


static inline void ipa_mchan_transfer(unsigned int len, char type, char incr, char twd, char ele, char ile, char ble, unsigned int ext_addr, unsigned int tcdm_addr, unsigned short int count, unsigned short int stride)
{
 *(volatile int*) IPA_COMMAND_QUEUE = len | (type<<PLP_DMA_TYPE_BIT) | ( incr<<PLP_DMA_INCR_BIT) | (twd <<PLP_DMA_2D_BIT) | (ele<<PLP_DMA_ELE_BIT) | (ile <<PLP_DMA_ILE_BIT) | (ble <<PLP_DMA_BLE_BIT);
 *(volatile int*) IPA_COMMAND_QUEUE = tcdm_addr;
 *(volatile int*) IPA_COMMAND_QUEUE = ext_addr;
 
 if (twd == 1) *(volatile int*) IPA_COMMAND_QUEUE = count | (stride << 16);
}

static inline void ipa_mchan_barrier(int id) {
  while(((*(volatile int*)(IPA_STATUS_REGISTER)) >> id ) & 0x1 ) {
    
 }
}

static inline void ipa_mchan_free(int id) {
  *(volatile int*) IPA_STATUS_REGISTER = 0x1 << id;
}

#endif
