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
#include "buttons.h"
#include "bird.h"
#include "pipes.h"

#include <stdbool.h>

 
bool bBirdAlive;
bool bCollision; //true if collision occured

struct bird player1;
struct bird *b1 = &player1;

int getBirdY(){
	int ret = 0;
	if(xSemaphoreTake(player1.lock, portMAX_DELAY) == pdTRUE){
		ret = player1.y;
		xSemaphoreGive(player1.lock);
	}
	return ret;
}


int getScore(){
	int ret = 0;
	if(xSemaphoreTake(player1.lock, portMAX_DELAY) == pdTRUE){
		ret = player1.score;
		xSemaphoreGive(player1.lock);
	}
	return ret;
}


void incrementScore(struct bird *ptr, int a){
	if(xSemaphoreTake(ptr->lock, portMAX_DELAY) == pdTRUE){
		ptr->score += a;
		xSemaphoreGive(player1.lock);
	}
}

void birdInit(void){

	player1.lock = xSemaphoreCreateMutex();
	player1.y = SCREEN_HEIGHT / 2;
	player1.velocity = 0.0f;
	player1.score = 0;

}

void vBirdStatus(void)
{
	if(bCollision == true) {
		bBirdAlive = false;
	}
	else {
		bBirdAlive = true;
	}
}


void vBirdJump(void){
	if(xSemaphoreTake(player1.lock, portMAX_DELAY) == pdTRUE){

		player1.velocity = 0.0f;
		player1.velocity += -GRAVITY * 9.5f;
		player1.y += player1.velocity;
		xSemaphoreGive(player1.lock);
	}
}

void vBirdFall(void){
	if(xSemaphoreTake(player1.lock, portMAX_DELAY) == pdTRUE){
		player1.velocity += GRAVITY;
		player1.y += player1.velocity;
		xSemaphoreGive(player1.lock);
	}
}

void vBirdFreeFall(void){
	if(xSemaphoreTake(player1.lock, portMAX_DELAY) == pdTRUE){
		
		if(player1.y <= SCREEN_HEIGHT - 185) 
		{
			player1.y += GRAVITY * 12;
		}
		else {
			player1.y = SCREEN_HEIGHT - BASE_HEIGHT ;
		}
		xSemaphoreGive(player1.lock);
	}
}


void vBirdMovement(void)
{
	if (bBirdAlive == true) {
		if(tumEventGetMouseLeft()){ // Gotta add delay

			tumSoundPlaySample(a3); //wing sound
			vBirdJump();

		} else {
			vBirdFall();
		}
	}
	//if collision occurs fall down
	else{ 
		vBirdFreeFall();
	}
	
}

void vBirdReset(void) {

		player1.y = SCREEN_HEIGHT / 2;
		player1.velocity = 0.0f;

		player1.score = 0;

		bCollision = false;
		pipesInit();

}

void countScore(void)
{

    if (bBirdAlive == true)
    {

        if (getPipeX(pipe1) == BIRD_X - PIPE_WIDTH / 2)
        {
            tumSoundPlaySample(a4);
            incrementScore(b1, 1);
        }
        if (getPipeX(pipe2) == BIRD_X - PIPE_WIDTH / 2)
        {
            tumSoundPlaySample(a4);
            incrementScore(b1, 1);
        }
        if (getPipeX(pipe3) == BIRD_X - PIPE_WIDTH / 2)
        {
            tumSoundPlaySample(a4);
            incrementScore(b1, 1);
        }
    }
}