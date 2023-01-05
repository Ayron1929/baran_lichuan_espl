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
 
bool bBirdAlive;
bool bCollision; //true if collision occured
bool bInScreen; //true if bird is in screen

void birdInit(void){

	player1.birdX = SCREEN_WIDTH / 3 - 50;
	player1.birdY = SCREEN_HEIGHT / 2;
	player1.birdVelocity = 0.0f;
	player1.gravity = 1.0f;
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
		if(buttons.buttons[KEYCODE(SPACE))){ // Gotta add delay

			player1.birdVelocity = 0.0f;
			player1.birdVelocity += -player1.gravity * 10.0f;
			player1.birdY += player1.birdVelocity;

		} else {
			player1.birdVelocity += player1.gravity;
			player1.birdY += player1.birdVelocity;
		}
	}
	
	else{ //if collision occurs fall down
		if(player1.birdY <= SCREEN_HEIGHT - 185) {
			
			player1.birdY += player1.gravity * 12;
		}
		else {
			player1.birdY = SCREEN_HEIGHT - 175;
		}
	}
	
	  
}
