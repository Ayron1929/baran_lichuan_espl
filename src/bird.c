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
#include "sm.h"
#include "bird.h"
#include <stdbool.h>

bool bBirdAlive;
bool bCollision; //true if collision occured
bool bInScreen; //true if bird is in screen

void vBirdStatus(struct bird player)
{
	//return true --> Bird is Alive, false --> is dead.
	//Gotta add pipe collision
	if (player.birdPosition >= 583 ||
	    player.birdPosition < -3) {
		bBirdAlive = false;
	} else {
		bBirdAlive = true;
	}
}

void vCheckCollision(struct bird player1) {

	//check top and bottom collision
	if (player1.birdPosition >= 583 || player1.birdPosition < -3) {
		bCollision = true;
	} else {
		bCollision = false;
	}
	//check pipe collisions

}

void birdFreeFall(void)
{
}

void vBirdMovement(void)
{
	if (bBirdAlive == true) {
		if(buttons.buttons[KEYCODE(SPACE)]){ // Gotta add delay

			player1.birdVelocity = 0.0f;
			player1.birdVelocity += -player1.gravity * 10.0f;
			player1.birdPosition += player1.birdVelocity;

		} else {
			player1.birdVelocity += player1.gravity;
			player1.birdPosition += player1.birdVelocity;
		}
	}
}


//(tumEventGetMouseLeft() == true)