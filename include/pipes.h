#ifndef __PIPES_H__
#define __PIPES_H__

struct pipes { 
    
    int x;
    int y;
    float fPipeVelocity;
};

struct pipes pipe1;
struct pipes pipe2;
struct pipes pipe3;

void pipesInit(void);
void pipe1Generate(void);

#endif
