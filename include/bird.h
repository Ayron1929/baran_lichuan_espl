#ifndef __BIRD_H__
#define __BIRD_H__

struct bird {
	int birdPosition;
	float birdVelocity;
	float gravity;

} player1;

extern bool bBirdAlive;
extern bool bCollison;
void birdFreeFall(void);
void vBirdMovement(void);
void vBirdStatus(struct bird player1);
void vCheckCollision(struct bird player1);

#endif