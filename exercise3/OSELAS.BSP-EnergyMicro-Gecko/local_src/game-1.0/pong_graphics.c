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
#define PONG_HEIGHT 30 //in pixels
#define BALL_RADIUS 5

short *fbp; //framebuffer pointer
int fbfd; //framebuffer file pointer
struct figure {
    int x;
    int y;
};
struct ball_struct{
    int x;
    int y;
    int speed;
    int dirx;
    int diry;
};
struct figure right_pong;
struct figure left_pong;
struct ball_struct ball;
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

    ball.x = (SCREEN_WIDTH/2) - (BALL_RADIUS/2);
    ball.y = (SCREEN_HEIGHT/2) - (BALL_RADIUS/2);
    ball.speed = 5;
    ball.dirx = 1;
    ball.diry = 1;
    int i = 0;
    for( i = ball.y; i < (ball.y+BALL_RADIUS); i++){
        int j = 0;
        for( j = ball.x; j < (ball.x + BALL_RADIUS); j++){
            *(fbp + i*320 +j) = 0xFFF;
        }
    }
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
    
    int future_y_pos = (current_y_pos - by < 0) || ( current_y_pos + PONG_HEIGHT - by > SCREEN_HEIGHT)? current_y_pos : current_y_pos - by;

    int count;
    int adder = (by < 0) ? 1: -1;

    int up_compensation = 0;
    int down_compensation = 0;
    if (adder < 0) {
        up_compensation = PONG_HEIGHT;
    } else {
        down_compensation = PONG_HEIGHT;
    }

    
    //first make the background black again (after moving pong)
    for(count = current_y_pos + up_compensation; count != future_y_pos + up_compensation;  count += adder){

        *(fbp + current_x_pos + count*320) = 0x0;
        *(fbp + current_x_pos-1 + count*320) = 0x0;
    }
    
    //draw pong at correct spot
    for(count = future_y_pos + up_compensation; count != (future_y_pos + adder*down_compensation); count += adder){

        *(fbp + current_x_pos + count*320) = 0xFFFF;
        *(fbp + current_x_pos-1 + count*320) = 0xFFFF;
    }
    struct fb_copyarea rect;
    rect.dy = (by > 0) ? future_y_pos : current_y_pos;
    rect.dx = current_x_pos - 1;
    rect.width = 2;
    rect.height = PONG_HEIGHT  + (by * adder * -1) + 2;
    ioctl(fbfd, 0x4680, &rect); 

    //update right_pong object
    right_pong.y = future_y_pos;
    return 0;

}

int move_left_pong(int by){
    
    int current_x_pos = left_pong.x;
    int current_y_pos = left_pong.y;
    
    int future_y_pos = (current_y_pos - by < 0) || ( current_y_pos + PONG_HEIGHT - by > SCREEN_HEIGHT)? current_y_pos : current_y_pos - by;

    int count;
    int adder = (by < 0) ? 1: -1;

    int up_compensation = 0;
    int down_compensation = 0;
    if (adder < 0) {
        up_compensation = PONG_HEIGHT;
    } else {
        down_compensation = PONG_HEIGHT;
    }

    
    //first make the background black again (after moving pong)
    for(count = current_y_pos + up_compensation; count != future_y_pos + up_compensation;  count += adder){

        *(fbp + current_x_pos + count*320) = 0x0;
        *(fbp + current_x_pos+1 + count*320) = 0x0;
    }
    
    //draw pong at correct spot
    for(count = future_y_pos + up_compensation; count != (future_y_pos + adder*down_compensation); count += adder){

        *(fbp + current_x_pos + count*320) = 0xFFFF;
        *(fbp + current_x_pos+1 + count*320) = 0xFFFF;
    }

    struct fb_copyarea rect;    
    rect.dy = (by > 0) ? future_y_pos : current_y_pos;
    rect.dx = current_x_pos;
    rect.width = 2;
    rect.height = PONG_HEIGHT  + (by * adder * -1) + 2;
    ioctl(fbfd, 0x4680, &rect);
    //update left_pong object
    left_pong.y = future_y_pos;

    return 0;

}

int move_ball(){
    int next_y, next_x;
    if(ball.y + (ball.diry * ball.speed) < 0){
        next_y = ball.speed - ball.y;
        ball.diry = -ball.diry;
    }
    else if (ball.y + (ball.diry * ball.speed) > SCREEN_HEIGHT - BALL_RADIUS ){
        ball.diry = -ball.diry;
        next_y = SCREEN_HEIGHT- BALL_RADIUS - (ball.speed - (SCREEN_HEIGHT - BALL_RADIUS -ball.y));
    }
    else{
        next_y = ball.y + ball.diry*ball.speed;
    }
    if((ball.x + (ball.dirx * ball.speed) < left_pong.x) && (ball.y  > left_pong.y - (ball.x - left_pong.x)*ball.diry - BALL_RADIUS) && (ball.y < (left_pong.y + PONG_HEIGHT) - (ball.x - left_pong.x)*ball.diry)) {
        ball.dirx= -ball.dirx;
        next_x = ball.x;
    }
    else if((ball.x + (ball.dirx * ball.speed) > right_pong.x - BALL_RADIUS) && (ball.y  > right_pong.y - (right_pong.x - ball.x - BALL_RADIUS)*ball.diry - BALL_RADIUS) && (ball.y < (right_pong.y + PONG_HEIGHT) - (right_pong.x -ball.x)*ball.diry)) {
        ball.dirx= -ball.dirx;
        next_x = ball.x;
    }
    else{
        next_x = ball.x + ball.dirx * ball.speed;
    }

    if(next_x < 0 || next_x > SCREEN_WIDTH - BALL_RADIUS){
        next_x = (SCREEN_WIDTH/2) - (BALL_RADIUS/2);
        next_y = (SCREEN_HEIGHT/2) - (BALL_RADIUS/2);
    }

    int i;
    for( i = ball.y; i < (ball.y+BALL_RADIUS); i++){
        int j = 0;
        for( j = ball.x; j < (ball.x + BALL_RADIUS); j++){
            *(fbp + i*320 +j) = 0x0;
        }
    }


    struct fb_copyarea rect;    
    rect.dy = ball.y;
    rect.dx = ball.x;
    rect.width = BALL_RADIUS;
    rect.height = BALL_RADIUS;
    ioctl(fbfd, 0x4680, &rect);

    for( i = next_y; i < (next_y+BALL_RADIUS); i++){
        int j = 0;
        for( j = next_x; j < (next_x + BALL_RADIUS); j++){
            *(fbp + i*320 +j) = 0xFFF;
        }
    }

    ball.y = next_y;
    ball.x = next_x;
  
    rect.dy = ball.y;
    rect.dx = ball.x;
    rect.width = BALL_RADIUS;
    rect.height = BALL_RADIUS;
    ioctl(fbfd, 0x4680, &rect);


    return 0;
}


