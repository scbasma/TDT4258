#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <unistd.h>
#include "pong_graphics.h"

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
    initialize_board();
	exit(EXIT_SUCCESS);

	return 0;
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
