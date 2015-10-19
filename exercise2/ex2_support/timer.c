#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/* function to setup the timer */
void setupTimer()
{ 
  *CMU_OSCENCMD |= (0x1 << 8);
  while((*CMU_STATUS & (0x1 << 9))){}
  *CMU_HFCORECLKEN0 |= (0x1 << 4);
  *CMU_LFACLKEN0 |= CMU2_LFACLKEN0_LE_TIMER;
  *CMU_LFCLKSEL = 0x2;
  while((*CMU_SYNCBUSY)){}
  *LE_TIMER_CTRL |= (0x1 << 9);
  while((*CMU_SYNCBUSY)){}
  *LE_TIMER_CMD = 0x1;
}


