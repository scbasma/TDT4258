#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/* function to set up GPIO mode and interrupts*/
void setupGPIO()
{
  *CMU_HFPERCLKEN0 |= (1 << 13);

  *GPIO_PC_MODEL |= 0x33333333;	
  *GPIO_PC_DOUT |= 0xff;	
  *GPIO_EXTIPSELL |= 0x22222222;
  *GPIO_EXTIFALL |= 0xff;
  *GPIO_EXTIRISE |= 0xff;
  *GPIO_IEN |= 0xff;
  
  *GPIO_PA_CTRL |= 0x2;
  *GPIO_PA_MODEH |= 0x55555555;

}
