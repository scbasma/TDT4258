#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <unistd.h>
#include "pong_graphics.h"
FILE* device;


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
	if(    initialize_board() != -1){
		printf("Initialize board not -1!\n");
	}else {
		printf("initialize board equals -1\n");
	}
	exit(EXIT_SUCCESS);

	return 0;
}

int gamepad_init(){
	device = fopen("/dev/gamepad","rb");
	if(!device){
		printf("Error: Unable to open device\n");
		return -1;
	}
	if(signal(SIGIO, &gamepad_interrupt_handler) == SIG_ERR){
		printf("Error: Unable to register signal handler for gamepad interrupts\n");
		return -1;
	}
	if(fcntl(fileno(device), F_SETOWN, getpid()) == -1){
		printd("Error: Unable to set pid as owner\n");
		return -1;
	}
	long oflags = fcntl(fileno(device), F_GETFL);
	if(fcntl(fileno(device), F_SETFL, oflags | FASYNC) == -1){
		printf("Error: unable to set async flag\n");
		return -1;
	}
	return 0;
}

void gamepad_interrupt_handler(int signo){
	printf("Received interrupt\n");
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
