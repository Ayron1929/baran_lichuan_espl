#include "TUM_Draw.h"
#include "TUM_Event.h"
#include "TUM_Ball.h"
#include "TUM_Sound.h"
#include "TUM_Utils.h"
#include "TUM_Print.h"

#include "defines.h"
#include "main.h"
#include "demo_tasks.h"
#include "draw.h"
#include "async_message_queues.h"
#include "async_sockets.h"
#include "buttons.h"
#include "sm.h"
#include "bird.h"
#include "pipes.h"

#include <time.h>
#include <stdbool.h>
#include <stdlib.h>



void pipesInit(void) {

    srand(time(0));

    pipe1.x = SCREEN_WIDTH;
    pipe1.y = -350 + rand() % 310; 

    pipe2.x = SCREEN_WIDTH; 
    pipe2.y = -350 + rand() % 310;

    pipe3.x = SCREEN_WIDTH;
    pipe3.y = -350 + rand() % 310;

}

void pipe1Generate(void){

    if(pipe1.x < -52){

        pipe1.x = SCREEN_WIDTH;
        pipe1.y = 250 + rand() % 300;
    }
    

}

