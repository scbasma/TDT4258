#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "sound.h"

int count[] = {0,0,0,0,0,0,0,0};
int buttonPressed[] = {0,0,0,0,0,0,0,0};
int amplitude = 500;

const int sampleFreq = 32768;
int scala_counter = 0;
int note_counter = 0;
bool upCount[] = {true,true,true,true,true,true,true,true};
bool isPlaying = false;
const int note_frequencies[] = {NOTE_A4, NOTE_E5, NOTE_C5}; 

void __attribute__ ((interrupt)) LETIMER0_IRQHandler()
{
  *LE_TIMER_IFC = (0x1 << 2); //cleared interrupt flag
   
  int sound = 0;
  int note_frequency;
  double freqFactor;
  int i = 0;
  for(i = 2; i <= 6; i+=2){ //synthesizing note for the pressed button
     if(buttonPressed[i] == 1){  	
	note_frequency = note_frequencies[(i/2)-1]; 
	freqFactor = (double)note_frequency/(double)sampleFreq;
	sound += (int) count[i]*amplitude*freqFactor;
	if( count[i] >= sampleFreq/note_frequency/2){
		upCount[i] = false;
	}else if(count[i] <= 0){
		upCount[i] = true;
 	}
   	count[i] = upCount[i] ? count[i] + 1: count[i] - 1;
  }
  }

  if(isPlaying){ //synthesizing melody
	note_frequency = scala[scala_counter]; 
	freqFactor = (double)note_frequency/(double)sampleFreq;
	sound += (int) count[0]*amplitude*freqFactor;

	if(note_counter == 10000){ 
		note_counter = 0;
		scala_counter += 1; //incrementing note pointer
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
  	
	count[0] = upCount[0] ? count[0] + 1: count[0] - 1;
  }
  *DAC0_CH0DATA = sound;
}

/* GPIO even pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler() 
{
  *GPIO_IFC = 0xff;
  if ((*GPIO_PC_DIN & (1 << 0)) == 0 ){ //checking button press on SW1 
	if(isPlaying){
		isPlaying = false;
	}
	else{
		isPlaying = true;
	}
  }
  for(int i = 1; i < 4; i++){ //checking button press on SW3, SW5, SW7
  	if ((*GPIO_PC_DIN & (1 << 2*i)) == 0 ){
   		buttonPressed[2*i] = 1; 
 	}
	else{
		buttonPressed[2*i] = 0;
        }
  }
  bool notePlaying = false;
  for(int i = 0; i < 8; i++){ //checking if a button is pressed
	if(buttonPressed[i] == 1)
		notePlaying = true;
  }
  if(notePlaying || isPlaying){ //enabling LE_TIMER interrupt and turning on DAC if button pressed or music is set to play 
	*LE_TIMER_IEN = 0x1;
        *DAC0_CH0CTRL |= 0x1;
        *DAC0_CH1CTRL |= 0x1;
  }else{ //disabling and turning off if not music is set to play or button is not pressed
        *LE_TIMER_IEN = 0x0;
        *DAC0_CH0CTRL = 0x0;
        *DAC0_CH1CTRL = 0x0;
  }
}

/* GPIO odd pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler()  //volume control on SW6 and SW8
{
  *GPIO_IFC = 0xff; 
  if ((*GPIO_PC_DIN & (1 << 7)) == 0 )
	amplitude -= 100;
  if ((*GPIO_PC_DIN & (1 << 5)) == 0 )
	amplitude += 100;

  *GPIO_PA_DOUT &= ~(0x1 << 15);
}
