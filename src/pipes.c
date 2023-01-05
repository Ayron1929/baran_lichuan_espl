#include "TUM_Draw.h"
#include "TUM_Event.h"
#include "TUM_Ball.h"
#include "TUM_Sound.h"
#include "TUM_Utils.h"
#include "TUM_Print.h"

#include "defines.h"
#include "main.h"
#include "demo_tasks.h"
#include "draw.h"
#include "async_message_queues.h"
#include "async_sockets.h"
#include "buttons.h"
#include "state_machine.h"
#include "bird.h"
#include "pipes.h"

#include <time.h>
#include <stdbool.h>
#include <stdlib.h>

//int score = 0;
char showScore[10];

int highscore = 0;


void pipesInit(void) {

    srand(time(0));

    pipe1.x = SCREEN_WIDTH;
    pipe1.y = -350 + rand() % 310;
    pipe1.count = 0; 

    pipe2.x = SCREEN_WIDTH + 200; 
    pipe2.y = -350 + rand() % 310;

    pipe3.x = SCREEN_WIDTH + 400;
    pipe3.y = -350 + rand() % 310;
    
    //pipe4.x = SCREEN_WIDTH + 145 * 3;
    //pipe4.y = -350 + rand() % 310;
}
 

void vCheckCollision(void){ //kinda messy but it works

    if(player1.birdX - 52 < pipe1.x && pipe1.x < player1.birdX + 50){

        if(player1.birdY < pipe1.y + 400 || player1.birdY > pipe1.y + 600 - 36) {

            bCollision = true;
        }
    }
    if(player1.birdX - 52 < pipe2.x && pipe2.x < player1.birdX + 50){

        if(player1.birdY < pipe2.y + 400 || player1.birdY > pipe2.y + 600 - 36) {

            bCollision = true;
        }
    }
    if(player1.birdX - 52 < pipe3.x && pipe3.x < player1.birdX + 50){

        if(player1.birdY < pipe3.y + 400 || player1.birdY > pipe3.y + 600 - 36) {

            bCollision = true;
        }
    }
    if(player1.birdY > SCREEN_HEIGHT - 175 || player1.birdY < 0){
        bCollision = true;
    }
}


void countScore(void) {

    if(bBirdAlive == true) {

        if(pipe1.x == player1.birdX - 26) {
            score++;
        }
        if(pipe2.x == player1.birdX - 26) {
            score++;
        }
        if(pipe3.x == player1.birdX - 26) {
            score++;
        }
    }

}
