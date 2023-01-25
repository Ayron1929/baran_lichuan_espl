#ifndef __PIPES_H__
#define __PIPES_H__

struct pipes { 
    
    SemaphoreHandle_t lock;
    int x;
    int y;

};

extern struct pipes pipe1;
extern struct pipes pipe2;
extern struct pipes pipe3;

int getPipeX(struct pipes pipe);
int getPipeY(struct pipes pipe);
void vPipesShift(struct pipes *ptr);
void vPipesReset(struct pipes *ptr);
void vPipeMovement(void);
void pipesInit(void);
void vCheckCollision(void);



#endif
