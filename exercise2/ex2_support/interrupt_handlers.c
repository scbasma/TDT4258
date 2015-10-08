#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

int count = 0;
int amplitude = 2000;


/* TIMER1 interrupt handler */
void __attribute__ ((interrupt)) TIMER1_IRQHandler() 
{  
  /*
    TODO feed new samples to the DAC
    remember to clear the pending interrupt by writing 1 to TIMER1_IFC
  */ 
  *TIMER1_IFC |= 0x1;
   

  if( count == 1){
	count = 0;
	*DAC0_CH0DATA = amplitude;
  }else{
	count = 1;
	*DAC0_CH0DATA = 0;
  }
}

/* GPIO even pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler() 
{
    /* TODO handle button pressed event, remember to clear pending interrupt */
  if ((*GPIO_PC_DIN & (1 << 7)) > 0 )
	amplitude += 100;
  if ((*GPIO_PC_DIN & (1 << 5)) > 0 )
	amplitude -= 100;
  *GPIO_IFC = 0xff;
  
}

/* GPIO odd pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler() 
{
    /* TODO handle button pressed event, remember to clear pending interrupt */
  *GPIO_IFC = 0xff;
  if ((*GPIO_PC_DIN & (1 << 7)) > 0 )
	amplitude += 100;
  if ((*GPIO_PC_DIN & (1 << 5)) > 0 )
	amplitude -= 100;
  *GPIO_IFC = 0xff;
}
