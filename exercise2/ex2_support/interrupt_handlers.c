#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "sound.h"

int count = 0;
int amplitude = 500;
int freq = 440;
const int sampleFreq = 32768;
int scala_counter = 0;
int note_counter = 0;
bool upCount = true;
bool isPlaying = false;

/* TIMER1 interrupt handler */
void __attribute__ ((interrupt)) TIMER1_IRQHandler() 
{  
  /*
    TODO feed new samples to the DAC
    remember to clear the pending interrupt by writing 1 to TIMER1_IFC
  */
  *TIMER1_IFC |= 0x1;
  *GPIO_PA_DOUT &= ~(0x1 << 11);
}

void __attribute__ ((interrupt)) LETIMER0_IRQHandler()
{
  int note_frequency = scala[scala_counter]; 
  double freqFactor = (double)note_frequency/(double)sampleFreq;
  *LE_TIMER_IFC = (0x1 << 2);

  *DAC0_CH0DATA = (int) count*amplitude*freqFactor;
  

  if(note_counter == 10000){
	note_counter = 0;
//	count = 0;
	scala_counter += 1;
	if (scala_counter > 63)
		scala_counter = 0;
   }else {
	note_counter += 1;
   }	

  if( count >= sampleFreq/note_frequency/2){
	upCount = false;
  }else if(count <= 0){
	upCount = true;
  }
  
  count = upCount ? count + 1: count - 1;
   *GPIO_PA_DOUT &= ~(0x1 << 12);
  
}

/* GPIO even pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler() 
{
  *GPIO_IFC = 0xff;
  if ((*GPIO_PC_DIN & (1 << 0)) == 0 ){
	if(isPlaying){
		*LE_TIMER_IEN = 0x0;
		isPlaying = false;
		*DAC0_CH0CTRL = 0;
		*DAC0_CH1CTRL = 0;
	}
	else{
		*LE_TIMER_IEN = 0x1;
		isPlaying = true;
		*DAC0_CH0CTRL |= 0x1;
		*DAC0_CH1CTRL |= 0x1;
	}
  }
}

/* GPIO odd pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler() 
{
  *GPIO_IFC = 0xff;
  if ((*GPIO_PC_DIN & (1 << 7)) > 0 )
	amplitude += 100;
  if ((*GPIO_PC_DIN & (1 << 5)) > 0 )
	amplitude -= 100;
  *GPIO_IFC = 0xff;
  *GPIO_PA_DOUT &= ~(0x1 << 15);
}
