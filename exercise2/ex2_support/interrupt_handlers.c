#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "sound.h"

int count[] = {0,0,0,0,0,0,0,0};
int buttonPressed[] = {0,0,0,0,0,0,0,0};
int amplitude = 500;
int freq = 440;
const int sampleFreq = 32768;
int scala_counter = 0;
int note_counter = 0;
bool upCount[] = {true,true,true,true,true,true,true,true};
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
  *LE_TIMER_IFC = (0x1 << 2);
  int sound = 0;
  int note_frequency;
  double freqFactor;
  if(buttonPressed[2] == 1){	
	note_frequency = NOTE_A4; 
	freqFactor = (double)note_frequency/(double)sampleFreq;
	sound += (int) count[2]*amplitude*freqFactor;
	if( count[2] >= sampleFreq/note_frequency/2){
		upCount[2] = false;
	}else if(count[2] <= 0){
		upCount[2] = true;
 	}
   	count[2] = upCount[2] ? count[2] + 1: count[2] - 1;
  	*GPIO_PA_DOUT &= ~(0x1 << 11);
  }
  if(buttonPressed[4] == 1){	
	note_frequency = NOTE_E5; 
	freqFactor = (double)note_frequency/(double)sampleFreq;
	sound += (int) count[2]*amplitude*freqFactor;
	if( count[2] >= sampleFreq/note_frequency/2){
		upCount[2] = false;
	}else if(count[2] <= 0){
		upCount[2] = true;
 	}
   	count[2] = upCount[2] ? count[2] + 1: count[2] - 1;
  	*GPIO_PA_DOUT &= ~(0x1 << 11);
  }
  if(buttonPressed[6] == 1){	
	note_frequency = NOTE_C5; 
	freqFactor = (double)note_frequency/(double)sampleFreq;
	sound += (int) count[2]*amplitude*freqFactor;
	if( count[2] >= sampleFreq/note_frequency/2){
		upCount[2] = false;
	}else if(count[2] <= 0){
		upCount[2] = true;
 	}
   	count[2] = upCount[2] ? count[2] + 1: count[2] - 1;
  	*GPIO_PA_DOUT &= ~(0x1 << 11);
  }
  if(isPlaying){
	note_frequency = scala[scala_counter]; 
	freqFactor = (double)note_frequency/(double)sampleFreq;
	sound += (int) count[0]*amplitude*freqFactor;

	if(note_counter == 10000){
		note_counter = 0;
		scala_counter += 1;
		if (scala_counter > 63)
			scala_counter = 0;
	}else {
		note_counter += 1;
	}	

	if( count[0] >= sampleFreq/note_frequency/2){
		upCount[0] = false;
	}else if(count[0] <= 0){
		upCount[0] = true;
	}
  	*GPIO_PA_DOUT &= ~(0x1 << 12);
	count[0] = upCount[0] ? count[0] + 1: count[0] - 1;
  }
  *DAC0_CH0DATA = sound;
}

/* GPIO even pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler() 
{
  *GPIO_IFC = 0xff;
  if ((*GPIO_PC_DIN & (1 << 0)) == 0 ){
	if(isPlaying){
		isPlaying = false;
	}
	else{
		isPlaying = true;
	}
  }
  for(int i = 1; i < 4; i++){
  	if ((*GPIO_PC_DIN & (1 << 2*i)) == 0 ){
   		buttonPressed[2*i] = 1; 
 	}
	else{
		buttonPressed[2*i] = 0;
        }
  }
  bool notePlaying = false;
  for(int i = 0; i < 8; i++){
	if(buttonPressed[i] == 1)
		notePlaying = true;
  }
  if(notePlaying || isPlaying){
	*LE_TIMER_IEN = 0x1;
        *DAC0_CH0CTRL |= 0x1;
        *DAC0_CH1CTRL |= 0x1;
  }else{
        *LE_TIMER_IEN = 0x0;
        *DAC0_CH0CTRL = 0x0;
        *DAC0_CH1CTRL = 0x0;
  }
}

/* GPIO odd pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler() 
{
  *GPIO_IFC = 0xff;
  if ((*GPIO_PC_DIN & (1 << 7)) == 0 )
	amplitude -= 100;
  if ((*GPIO_PC_DIN & (1 << 5)) == 0 )
	amplitude += 100;

  *GPIO_PA_DOUT &= ~(0x1 << 15);
}
