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
	printf("Hello World!\n");
//	int fp = open("/dev/gamepad", O_RDWR);
//	int byte;
//	while(1){
//	
//	ssize_t size = read(fp, &byte, 4);
//	printf("Read byte %d\n", byte);
//	}	
//	if(    initialize_board() != -1){
//		printf("Initialize board not -1!\n");
//	}else {
//		printf("initialize board equals -1\n");
//	}
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
//int createPong(int x, int y){
//
//	int fbfd = 0;
//	short *fbp = 0;	
//
//	fbfd = open("/dev/fb0", O_RDWR);
//	
//	if(!fbfd) {
//		printf("Error: cannot open framebuffer device");
//		exit(1);
//	}
//	printf("Framebuffer device sucessfully openend");
//	
//
//	
//	int screensize = 320*240;
//	fbp = (short *) mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
//
//	if((int) fbp == -1) {
//		printf("Error: failed to map framebuffer device to memory.");
//		exit(4);
//	}
//	printf("The framebuffer device was mapped to memory successfully");
//
//	int x_pos = x;
//	int y_pos = y;
//	int count;
//	for (count = y_pos; count < (y_pos+20); count++){
//		
//		*(fbp + x_pos + count*320) = 0xFFFF;
//		*(fbp + x_pos-1 + count*320) = 0xFFFF;
//	}
//	struct fb_copyarea rect; 
//	rect.dy = 0;
//	rect.dx = 0;
//	rect.width = 320;
//	rect.height = 240;
//	ioctl(fbfd, 0x4680, &rect);
//
//	munmap(fbp, screensize);
//	close(fbfd);
//	return 0;
//}
//int testFramebuffer(){
//
//	int fbfd = 0;
//	short *fbp = 0;	
//
//	fbfd = open("/dev/fb0", O_RDWR);
//	
//	if(!fbfd) {
//		printf("Error: cannot open framebuffer device");
//		exit(1);
//	}
//	printf("Framebuffer device sucessfully openend");
//	
//
//	
//	int screensize = 320*240;
//	fbp = (short *) mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
//
//	if((int) fbp == -1) {
//		printf("Error: failed to map framebuffer device to memory.");
//		exit(4);
//	}
//	printf("The framebuffer device was mapped to memory successfully");
//
//	int middle_vert = 320/2;
//	int count;
//	for (count = 0; count < 240*320; count++){
//		*(fbp + count) = 0x0;
//	}
//	for (count = 0; count < 240; count++){
//		
//		*(fbp + middle_vert + count*320) = 0xFFFF;
//	}
//	struct fb_copyarea rect; 
//	rect.dy = 0;
//	rect.dx = 0;
//	rect.width = 320;
//	rect.height = 240;
//	ioctl(fbfd, 0x4680, &rect);
//
//	munmap(fbp, screensize);
//	close(fbfd);
//	return 0;
//}
