#ifndef __BIRD_H__
#define __BIRD_H__

#include <stdbool.h>

struct bird
{
	SemaphoreHandle_t lock;
	int y;
	float velocity;
};

extern struct bird player1;

extern bool bBirdAlive;
extern bool bCollision;

int getBirdY();
void birdInit(void);
void vBirdMovement(void);
void vBirdStatus(void);
void vBirdReset(void);

#endif