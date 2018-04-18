#include "ipa_dma_tests.h"
#include "ipa_execute_tests.h"
#include "mchan_tests.h"
#include "context.h"
#include "data.h"
#include <stdio.h>
#define VERBOSE

#define ipa_execute 1
#define test_result 0
#define cpu_execute 0

int MCHAN_DATA_TRANSFER(unsigned int length, char type, char incr, char twd, char ele, char ile, char ble,  unsigned int ext_addr,  unsigned int tcdm_addr, unsigned short int count, unsigned short int stride);
int IPA_LOAD_CONTEXT(unsigned int length, char type, char incr, char twd, char ele, char ile, char ble,  unsigned int ext_addr,  unsigned int tcdm_addr, unsigned short int count, unsigned short int stride);
int IPA_LOAD_CONTEXT_L22GCM();
int DATA_LOAD_L22TCDM();
int IPA_RUN();
int IPA_BAR();
int IPA_TEST();
int CPU_RUN();

int main()
{
  
  volatile int cycles_loadcontext  = 0,cycles_execute = 0,  cycles_cpurun = 0, error_count = 0;
  volatile unsigned int result;
  

  if (get_core_id() == 0)
    {
     
      if(ipa_execute)
	{
	  printf("Data is loading from L2 to TCDM\n");

	  DATA_LOAD_L22TCDM ();

	  printf("Data loading is completed\n");
       
	  printf("Context is loading from L2 to GCM\n");

	  reset_timer();
	  start_timer();

	  IPA_LOAD_CONTEXT_L22GCM();
       
	  stop_timer();

	  cycles_loadcontext = get_time();
       
	  printf("Context loading is completed \t @ %d cycles\n", cycles_loadcontext);

    
	  printf("IPA execution is getting started\n");

	  reset_timer();
	  start_timer();
	  IPA_RUN();
	  stop_timer();
	  cycles_execute = get_time();
       
	  printf("IPA Execution completed \t @ %d cycles \n", cycles_execute);
	  printf("IPA context load + IPA Execution completed \t @ %d cycles \n", cycles_loadcontext + cycles_execute);
	}

      if(test_result)
	error_count = IPA_TEST();

      if(cpu_execute)
	{
	  printf("CPU execution is getting started...\n");
	  reset_timer();
	  start_timer();
	  CPU_RUN();
	  stop_timer();
	  cycles_execute = get_time();
	  printf("IPA Execution completed \t @ %d cycles \n", cycles_execute);
	  reset_timer();
	}
     
     
   
    } 
  return error_count;
  
}

int DATA_LOAD_L22TCDM()
{ 
  
  int len_data = sizeof(input_l2);

  int id_mchan = mchan_alloc();

  mchan_transfer(len_data, RX, INC, LIN, 1, 0, 0, (unsigned int)&input_l2[0], 0, 4, 0);  
       
  mchan_barrier(id_mchan); 

  mchan_free(id_mchan);
     
  return 0;

}

int IPA_LOAD_CONTEXT_L22GCM()
{
  volatile unsigned int len_context = sizeof(context_data), id_dma_ipa = 0;
 
  id_dma_ipa = ipa_mchan_alloc();

  ipa_mchan_transfer(len_context, RX, INC, LIN, 1, 0, 0, (unsigned int)&context_data[0], 0, 4, 0);

  ipa_mchan_barrier(id_dma_ipa);

  ipa_mchan_free(id_dma_ipa);
  
  return 0;
}


int IPA_RUN()
{
  
  int id = 0;

  id = ipa_start_execution();

  ipa_execute_barrier(id);

  free_ipa_execute(id);

  return 0;
}


int IPA_TEST()
{
  int error = 0;
  unsigned int test, read, x = 8;
 
   for (unsigned i = 0; i < 1; i++) {
	 read = *(unsigned int*)(x);
	 if ( output_data[i] != read ){
	   printf("Error!!! Read: %x, Test:%x, Index: %d \n ",read,output_data[i],x);
	   error++;
	 }
	 x+=4;
  }
   printf("Execution succeded with %d error(s)\n", error);
   return error;
}

int CPU_RUN()
{
  volatile unsigned int x1 =  0x00045c5d, x2 = 0x0000001f;
  while(x1 != x2)
    {
      if(x1 > x2)
	{
	  x1 = x1 - x2;
	}
      else
	{
	  x2 = x2 - x1;
	}
    }
 
  return x1;
}
