#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/* function to set up GPIO mode and interrupts*/
void setupGPIO()
{
  /* TODO set input and output pins for the joystick */
  *CMU_HFPERCLKEN0 |= (1 << 13);

  *GPIO_PC_MODEL |= 0x33333333;	
  *GPIO_PC_DOUT |= 0xff;	

  *GPIO_PA_CTRL |= 0x2;
  *GPIO_PA_MODEH |= 0x55555555;
  /* Example of HW access from C code: turn on joystick LEDs D4-D8
     check efm32gg.h for other useful register definitions
  */
}
