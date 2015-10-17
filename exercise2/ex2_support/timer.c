#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/* function to setup the timer */
void setupTimer(uint16_t period)
{
  /*
    TODO enable and set up the timer
    
    1. Enable clock to timer by setting bit 6 in CMU_HFPERCLKEN0
    2. Write the period to register TIMER1_TOP
    3. Enable timer interrupt generation by writing 1 to TIMER1_IEN
    4. Start the timer by writing 1 to TIMER1_CMD
    
    This will cause a timer interrupt to be generated every (period) cycles. Remember to configure the NVIC as well, otherwise the interrupt handler will not be invoked.
  */  
//  *CMU_HFPERCLKEN0 |= (0x1 << 6);
  *CMU_OSCENCMD |= (0x1 << 8);
  while((*CMU_STATUS & (0x1 << 9))){}
  *CMU_HFCORECLKEN0 |= (0x1 << 4);
  *CMU_LFACLKEN0 |= CMU2_LFACLKEN0_LE_TIMER;
  *CMU_LFCLKSEL = 0x2;
  *LE_TIMER_IEN = 0x0;
//  while((*CMU_SYNCBUSY)){}
  *LE_TIMER_COMP0 |= 0x0;
  while((*CMU_SYNCBUSY)){}
  *LE_TIMER_CTRL |= (0x1 << 9);
  while((*CMU_SYNCBUSY)){}
  *LE_TIMER_CMD = 0x1;
//  while((*CMU_SYNCBUSY)){}
//  *TIMER1_TOP = 350;
//  *TIMER1_IEN |= 0x1;
//  *TIMER1_CMD |= 0x1;
}


