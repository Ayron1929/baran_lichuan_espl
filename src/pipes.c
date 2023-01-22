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
#include "bird.h"
#include "pipes.h"

#include <time.h>
#include <stdbool.h>
#include <stdlib.h>

struct pipes pipe1, pipe2, pipe3;

int getPipeX(struct pipes pipe){
    int ret = 0;
	if(xSemaphoreTake(pipe.lock, portMAX_DELAY) == pdTRUE){
		ret = pipe.x;
		xSemaphoreGive(pipe.lock);
	}
	return ret;
}

int getPipeY(struct pipes pipe){
    int ret = 0;
	if(xSemaphoreTake(pipe.lock, portMAX_DELAY) == pdTRUE){
		ret = pipe.y;
		xSemaphoreGive(pipe.lock);
	}
	return ret;
}

int vPipesShift(struct pipes pipe){
    if(xSemaphoreTake(pipe.lock, portMAX_DELAY) == pdTRUE){

		pipe.x -= 2;
		xSemaphoreGive(pipe.lock);
    }
    return 0;
}

void pipesInit(void) {

    //Offsets of the y values decided according to the pipe image size and the vertical gap
    //For the x values 200 px offset was decided to keep the pipe distance constant 

    srand(time(0));

    pipe1.lock = xSemaphoreCreateMutex();
    pipe1.x = SCREEN_WIDTH;
    pipe1.y = -350 + rand() % 310;
    // pipe1.velocity = 2;

    pipe2.lock = xSemaphoreCreateMutex();
    pipe2.x = SCREEN_WIDTH + 200; 
    pipe2.y = -350 + rand() % 310;
    // pipe2.velocity = 2;

    pipe3.lock = xSemaphoreCreateMutex();
    pipe3.x = SCREEN_WIDTH + 400;
    pipe3.y = -350 + rand() % 310;
    // pipe3.velocity = 2;
    
}
 

void vCheckCollision(void){ //kinda messy but it works

    if(BIRD_X - 52 < getPipeX(pipe1) && getPipeX(pipe1) < BIRD_X + 50){

        if(getBirdY() < getPipeY(pipe1) + 400 || getBirdY() > getPipeY(pipe1) + 600 - 36) {

            tumSoundPlaySample(b3); // hit sound
            bCollision = true;
        }
    }
    if(BIRD_X - 52 < getPipeX(pipe2) && getPipeX(pipe2) < BIRD_X + 50){

        if(getBirdY() < getPipeY(pipe2) + 400 || getBirdY() > getPipeY(pipe2) + 600 - 36) {

            tumSoundPlaySample(b3); // hit sound
            bCollision = true;
        }
    }
    if(BIRD_X - 52 < getPipeX(pipe3) && getPipeX(pipe3) < BIRD_X + 50){

        if(getBirdY() < getPipeY(pipe3) + 400 || getBirdY() > getPipeY(pipe3) + 600 - 36) {

            tumSoundPlaySample(b3); // hit sound
            bCollision = true;
        }
    }
    if(getBirdY() > SCREEN_HEIGHT - 175 || getBirdY() < 0){
        tumSoundPlaySample(a5);
        bCollision = true;
    }
}


void countScore(void) {

    if(bBirdAlive == true) {

        if(pipe1.x == BIRD_X - 26) {
            tumSoundPlaySample(a4);
            score++;
        }
        if(pipe2.x == BIRD_X - 26) {
            tumSoundPlaySample(a4);
            score++;
        }
        if(pipe3.x == BIRD_X - 26) {
            tumSoundPlaySample(a4);
            score++;
        }
    }

}
