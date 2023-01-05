#ifndef __BIRD_H__
#define __BIRD_H__

#include "pipes.h"

struct bird {
	int birdY;
	int birdX;
	float birdVelocity;
	float gravity;

} player1;

extern bool bBirdAlive;
extern bool bCollision;

void birdInit(void);
void birdFreeFall(void);
void vBirdMovement(void);
void vBirdStatus(void);

#endif