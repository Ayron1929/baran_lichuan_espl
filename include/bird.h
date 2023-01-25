#ifndef __BIRD_H__
#define __BIRD_H__

#include <stdbool.h>

struct bird
{
	SemaphoreHandle_t lock;
	int y;
	float velocity;
	int score;
};

extern struct bird player1;
extern struct bird *b1;

extern bool bBirdAlive;
extern bool bCollision;

int getBirdY();
int getScore();
void birdInit(void);
void vBirdMovement(void);
void vBirdStatus(void);
void vBirdReset(void);
void incrementScore(struct bird *ptr, int a);
void countScore(void);

#endif