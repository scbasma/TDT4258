#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <err.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define SCREEN_SIZE SCREEN_WIDTH*SCREEN_HEIGHT

struct pong {
    int x;
    int y;
    int height;
    int width;
};

struct pong right_pong;
struct pong left_pong; 
char *fbp; //framebuffer pointer
int fbfd; //framebuffer file pointer

int initialize_board(){
    fbfd = 0;
    *fbp = 0;

    fbfd = open("/dev/fb0", O_RDWR);

    if(!fbfd){
        return -1;
    }

    fbp = (char *) mmap(0, SCREEN_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);

    if( fbp == MAP_FAILED){
        return -1;
    }

    struct fb_copyarea rect;
    rect.dy = 0;
    rect.dx = 0;
    rect.width = SCREEN_WIDTH;
    rect.height = SCREEN_HEIGHT;
    ioctl(fbfd, 0x4680, &rect); 

    return 1;
}

int move_right_pong(int by){
    return 0;
}

int move_left_pong(int by){
    return 0;
}

int move_ball(int by){
    return 0;
}



