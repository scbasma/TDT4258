#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

void setupDAC()
{
  *CMU_HFPERCLKEN0 |= (1 << 17);
  *DAC0_CTRL = 0x50010;
}
