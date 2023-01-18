#ifndef __PIPES_H__
#define __PIPES_H__

struct pipes { 
    
    int x;
    int y;
    float fPipeVelocity;
    int count;
};

extern struct pipes pipe1;
extern struct pipes pipe2;
extern struct pipes pipe3;

extern int score;
void pipesInit(void);
void vCheckCollision(void);

void countScore(void);

#endif
