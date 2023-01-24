#ifndef __BIRD_H__
#define __BIRD_H__

#include <stdbool.h>

struct bird
{
	SemaphoreHandle_t lock;
	int y;
	float velocity;
	int score;
	int highscore;	
};

extern struct bird player1;

extern bool bBirdAlive;
extern bool bCollision;

int getBirdY();
int getScore();
int getHighscore();
void vSetHighscore(void);
void birdInit(void);
void vBirdMovement(void);
void vBirdStatus(void);
void vBirdReset(void);
void countScore(void);

#endif