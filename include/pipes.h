#ifndef __PIPES_H__
#define __PIPES_H__

struct pipes { 
    
    SemaphoreHandle_t lock;
    int x;
    int y;
    //int velocity;
};

extern struct pipes pipe1;
extern struct pipes pipe2;
extern struct pipes pipe3;

extern struct pipes *p1;
extern struct pipes *p2;
extern struct pipes *p3;

int getPipeX(struct pipes pipe);
int getPipeY(struct pipes pipe);
void pipesInit(void);
void vPipesShift(struct pipes *pipe);
void vPipesReset(struct pipes *pipe);
void vCheckCollision(void);

void countScore(void);

#endif
