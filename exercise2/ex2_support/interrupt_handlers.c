#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "sound.h"

int count = 0;
int amplitude = 2000;
int freq = 440;
const int sampleFreq = 20000;
int scala_counter = 0;
int note_counter = 0;

/* TIMER1 interrupt handler */
void __attribute__ ((interrupt)) TIMER1_IRQHandler() 
{  
  /*
    TODO feed new samples to the DAC
    remember to clear the pending interrupt by writing 1 to TIMER1_IFC
  */
  int note_frequency = scala[scala_counter]; 
  double freqFactor = (double)note_frequency/(double)sampleFreq;
  *TIMER1_IFC |= 0x1;

  *DAC0_CH0DATA = (int) count*amplitude*freqFactor;
  

  if(note_counter == 5000){
	note_counter = 0;
	count = 0;
	scala_counter += 1;
	if (scala_counter > 31)
		scala_counter = 0;
   }else {
	note_counter += 1;
   }	

  if( count == sampleFreq/note_frequency){
		count = 0;
  }else{
		count++;
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
