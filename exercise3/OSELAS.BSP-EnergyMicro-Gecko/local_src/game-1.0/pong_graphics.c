#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <err.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define SCREEN_SIZE SCREEN_WIDTH*SCREEN_HEIGHT
#define PONG_HEIGHT 20 //in pixels
#define BALL_RADIUS

short *fbp; //framebuffer pointer
int fbfd; //framebuffer file pointer
struct figure {
    int x;
    int y;
};
struct figure right_pong;
struct figure left_pong;
int initialize_figures(){
    //first draw right_pong
   int x_pos = SCREEN_WIDTH - 10; //10 pixels from end of screen
   int y_pos = (SCREEN_HEIGHT / 2) - PONG_HEIGHT / 2; // want to center pong vertically

   int count;
   for (count = y_pos; count < (y_pos + PONG_HEIGHT); count++){
        *(fbp + x_pos + count*320) = 0xFFF;
        *(fbp + x_pos-1 + count*320) = 0xFFF; //two pixels wide
    }
    //initialize global right pong struct
    right_pong.x = x_pos;
    right_pong.y = y_pos;

    //lets draw left pong
    x_pos = 10; //10 pixels from start of screen
    y_pos = (SCREEN_HEIGHT / 2) - PONG_HEIGHT / 2;

    for (count = y_pos; count < (y_pos + PONG_HEIGHT); count++){
        *(fbp + x_pos + count*320) = 0xFFF;
        *(fbp + x_pos+1 + count*320) = 0xFFF;
    }
    //initialize global left pong struct
    left_pong.x = x_pos;
    left_pong.y = y_pos;

    //TODO: draw ball
    //ball is of for simplicity sake just a rectangle

    //lets place it in the middle
    x_pos = SCREEN_WIDTH/2;
    y_pos = SCREEN_HEIGHT/240;
    
    //middle pixel
    *(fbp + x_pos + count*320) = 0xFFF;

    return 0;
}

int initialize_board(){
    fbfd = 0;
    *fbp = 0;

    fbfd = open("/dev/fb0", O_RDWR);

    if(!fbfd){
        return -1;
    }

    fbp = (short *) mmap(0, SCREEN_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);

    struct fb_copyarea rect;
    rect.dy = 0;
    rect.dx = 0;
    rect.width = SCREEN_WIDTH;
    rect.height = SCREEN_HEIGHT;
    ioctl(fbfd, 0x4680, &rect); 
    
    int count;
    for(count = 0; count < 240*320; count++){

	*(fbp + count) = 0x0;
    }
    initialize_figures();
    return 1;
}


int exit_board(){
    munmap(fbp, SCREEN_SIZE);
    close(fbfd);
    return 0;
}

int move_right_pong(int by){

    int current_x_pos = right_pong.x;
    int current_y_pos = right_pong.y;
    
    int future_y_pos = current_y_pos + by;

    int count;
    int adder = (by < 0) ? -1: 1;
    
    //first make the background black again (after moving pong)
    for(count = current_y_pos; count != future_y_pos;  count += adder){

        *(fbp + current_x_pos + count*320) = 0x0;
        *(fbp + current_x_pos-1 + count*320) = 0x0;
    }
    
    //draw pong at correct spot
    for(count = future_y_pos; count != (future_y_pos + adder*PONG_HEIGHT); count += adder){

        *(fbp + current_x_pos + count*320) = 0xFFFF;
        *(fbp + current_x_pos-1 + count*320) = 0xFFFF;
    }
    //update right_pong object
    right_pong.y = future_y_pos;
    return 0;

}

int move_left_pong(int by){
    
    int current_x_pos = left_pong.x;
    int current_y_pos = left_pong.y;
    
    int future_y_pos = current_y_pos + by;

    int count;
    int adder = (by < 0) ? -1: 1;
    
    //first make the background black again (after moving pong)
    for(count = current_y_pos; count != future_y_pos;  count += adder){

        *(fbp + current_x_pos + count*320) = 0x0;
        *(fbp + current_x_pos+1 + count*320) = 0x0;
    }
    
    //draw pong at correct spot
    for(count = future_y_pos; count != (future_y_pos + adder*PONG_HEIGHT); count += adder){

        *(fbp + current_x_pos + count*320) = 0xFFFF;
        *(fbp + current_x_pos+1 + count*320) = 0xFFFF;
    }
    //update left_pong object
    left_pong.y = future_y_pos;

    return 0;

}

int move_ball(int by){
    return 0;
}


