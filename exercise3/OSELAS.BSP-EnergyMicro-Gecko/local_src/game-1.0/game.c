#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <unistd.h>
#include "pong_graphics.h"
#include <signal.h>
#include <stdbool.h>

//FILE* device;
int device;
bool buttons_pressed[8];
void gamepad_interrupt_handler(int);
bool isPressed(int key);
int gamepad_init();
void refresh();

int main(int argc, char *argv[])
{
	initialize_board();
	gamepad_init();
	initialize_board();
	while(true){
	    refresh();
	    usleep(16667);
	}
	exit(EXIT_SUCCESS);
	
	return 0;
}

int gamepad_init(){
	device = open("/dev/gamepad",O_RDONLY);
	if(device == -1){
		printf("Error: Unable to open device\n");
		return -1;
	}
	if(signal(SIGIO, &gamepad_interrupt_handler) == SIG_ERR){
		printf("Error: Unable to register signal handler for gamepad interrupts\n");
		return -1;
	}
	if(fcntl(device, F_SETOWN, getpid()) == -1){
		printf("Error: Unable to set pid as owner\n");
		return -1;
	}
	long oflags = fcntl(device, F_GETFL);
	if(fcntl(device, F_SETFL, oflags | FASYNC) == -1){
		printf("Error: unable to set async flag\n");
		return -1;
	}
	return 0;
}

void gamepad_interrupt_handler(int signo){
	int i;
	for(i = 0; i <8; i++){
		buttons_pressed[i] = isPressed(i);
	}
}


void refresh(){
    int i;
    for(i = 0; i < 8; i++){
        if (buttons_pressed[i]) { 
            if(i == 5){
            	move_right_pong(10);
            }
            if(i == 7){
            	move_right_pong(-10);
            }
            if(i == 1){
            	move_left_pong(10);
            }
            if(i == 3){
            	move_left_pong(-10);
            }
           }
    }
    move_ball();
}

bool isPressed(int key){
	char buttons;
	read(device, &buttons, 1);
	if( !( (buttons >> key) % 2))
		return true;
	else
		return false;
}
