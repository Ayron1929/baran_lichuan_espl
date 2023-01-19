#ifndef __BIRD_H__
#define __BIRD_H__

#include <stdbool.h>

struct bird
{
	SemaphoreHandle_t lock;
	int birdY;
	int birdX;
	float birdVelocity;
	float gravity;
};

extern struct bird player1;

extern bool bBirdAlive;
extern bool bCollision;

int getBirdX();
void birdInit(void);
void vBirdMovement(void);
void vBirdStatus(void);
void vBirdReset(void);

#endif