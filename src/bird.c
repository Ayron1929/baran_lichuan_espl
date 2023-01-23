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

void incrementScore(void){
	if(xSemaphoreTake(player1.lock, portMAX_DELAY) == pdTRUE){
		player1.score++;
		xSemaphoreGive(player1.lock);
	}
}

void decrementScore(void){
	if(xSemaphoreTake(player1.lock, portMAX_DELAY) == pdTRUE){
		player1.score--;
		xSemaphoreGive(player1.lock);
	}
}

int getHighscore(){
	int ret = 0;
	if(xSemaphoreTake(player1.lock, portMAX_DELAY) == pdTRUE){
		ret = player1.highscore;
		xSemaphoreGive(player1.lock);
	}
	return ret;
}

void setHighscore(int score){
	if(xSemaphoreTake(player1.lock, portMAX_DELAY) == pdTRUE){
		player1.highscore = score;
		xSemaphoreGive(player1.lock);
	}
}

void birdInit(void){

	player1.lock = xSemaphoreCreateMutex();
	player1.y = SCREEN_HEIGHT / 2;
	player1.velocity = 0.0f;

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


void vBirdMovement(void)
{	

	if (bBirdAlive == true) {
		if(tumEventGetMouseLeft() == true){ // Gotta add delay

			tumSoundPlaySample(a3); //wing sound
			player1.velocity = 0.0f;
			player1.velocity += -GRAVITY * 9.5f;
			player1.y += player1.velocity;
			

		} else {
			player1.velocity += GRAVITY;
			player1.y += player1.velocity;
		}
	}
	
	//if collision occurs fall down
	else{ 
		if(player1.y <= SCREEN_HEIGHT - 185) {
			
			player1.y += GRAVITY * 12;
		}
		else {
			player1.y = SCREEN_HEIGHT - 175;
		}
	}
	
}

void vBirdReset(void) {

	if(xSemaphoreTake(player1.lock, portMAX_DELAY) == pdTRUE){


		player1.y = SCREEN_HEIGHT / 2;
		player1.velocity = 0.0f;
		player1.score = 0;

		bCollision = false;
		pipesInit();

		xSemaphoreGive(player1.lock);
	}

}

void countScore(void)
{
	if(xSemaphoreTake(player1.lock, portMAX_DELAY) == pdTRUE){
		if (bBirdAlive == true)
		{

			if (getPipeX(pipe1) == BIRD_X - 26)
			{
				tumSoundPlaySample(a4);
				incrementScore();
			}
			if (getPipeX(pipe2) == BIRD_X - 26)
			{
				tumSoundPlaySample(a4);
				incrementScore();
			}
			if (getPipeX(pipe3) == BIRD_X - 26)
			{
				tumSoundPlaySample(a4);
				incrementScore();
			}
		}
		xSemaphoreGive(player1.lock);
	}
}