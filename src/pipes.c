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
#include "bird.h"
#include "pipes.h"

#include <time.h>
#include <stdbool.h>
#include <stdlib.h>

struct pipes pipe1, pipe2, pipe3;
struct pipes *p1 = &pipe1;
struct pipes *p2 = &pipe2;
struct pipes *p3 = &pipe3;

int getPipeX(struct pipes pipe)
{
    int ret = 0;
    if (xSemaphoreTake(pipe.lock, portMAX_DELAY) == pdTRUE)
    {
        ret = pipe.x;
        xSemaphoreGive(pipe.lock);
    }
    return ret;
}

int getPipeY(struct pipes pipe)
{
    int ret = 0;
    if (xSemaphoreTake(pipe.lock, portMAX_DELAY) == pdTRUE)
    {
        ret = pipe.y;
        xSemaphoreGive(pipe.lock);
    }
    return ret;
}

void vPipesShift(struct pipes *ptr)
{
    if (xSemaphoreTake(ptr->lock, portMAX_DELAY) == pdTRUE)
    {
        ptr->x -= 2;
        xSemaphoreGive(ptr->lock);
    }
}

void vPipesReset(struct pipes *ptr)
{
    if (xSemaphoreTake(ptr->lock, portMAX_DELAY) == pdTRUE)
    {
        ptr->x = SCREEN_WIDTH + 116;
        ptr->y = -350 + rand() % 310;
        xSemaphoreGive(ptr->lock);
    }
}

void pipesInit(void)
{

    // Offsets of the y values decided according to the pipe image size and the vertical gap
    // For the x values 200 px offset was decided to keep the pipe distance constant

    srand(time(0));

    pipe1.lock = xSemaphoreCreateMutex();
    pipe1.x = SCREEN_WIDTH;
    pipe1.y = -350 + rand() % 310;

    pipe2.lock = xSemaphoreCreateMutex();
    pipe2.x = SCREEN_WIDTH + PIPE_OFFSET;
    pipe2.y = -350 + rand() % 310;

    pipe3.lock = xSemaphoreCreateMutex();
    pipe3.x = SCREEN_WIDTH + PIPE_OFFSET*2;
    pipe3.y = -350 + rand() % 310;
}

void vPipeMovement(void)
{
    if (bBirdAlive == true)
    {
        
        vPipesShift(p1);
        vPipesShift(p2);
        vPipesShift(p3);

        if (getPipeX(pipe1) <= -PIPE_WIDTH)
        {
            vPipesReset(p1);
        }
        if (getPipeX(pipe2) <= -PIPE_WIDTH)
        {
            vPipesReset(p2);
        }
        if (getPipeX(pipe3) <= -PIPE_WIDTH)
        {
            vPipesReset(p3);
        }
    }
}

void vCheckCollision(void)
{ //Almost pixel perfect

    if (BIRD_X - PIPE_WIDTH < getPipeX(pipe1) && getPipeX(pipe1) < BIRD_X + PIPE_WIDTH - 2)
    {

        if (getBirdY() < getPipeY(pipe1) + PIPE_HEIGHT || getBirdY() > getPipeY(pipe1) + PIPE_HEIGHT + PIPE_GAP - BIRD_IMAGE_HEIGHT -2)
        {

            bCollision = true;
        }
    }
    if (BIRD_X - PIPE_WIDTH < getPipeX(pipe2) && getPipeX(pipe2) < BIRD_X + PIPE_WIDTH - 2)
    {

        if (getBirdY() < getPipeY(pipe2) + PIPE_HEIGHT || getBirdY() > getPipeY(pipe2) + PIPE_HEIGHT + PIPE_GAP - BIRD_IMAGE_HEIGHT -2)
        {

            bCollision = true;
        }
    }
    if (BIRD_X - PIPE_WIDTH < getPipeX(pipe3) && getPipeX(pipe3) < BIRD_X + PIPE_WIDTH - 2)
    {

        if (getBirdY() < getPipeY(pipe3) + PIPE_HEIGHT || getBirdY() > getPipeY(pipe3) + PIPE_HEIGHT + PIPE_GAP - BIRD_IMAGE_HEIGHT -2)
        {
            bCollision = true;
        }
    }
    if (getBirdY() >= SCREEN_HEIGHT - BASE_HEIGHT || getBirdY() < 0)
    {   
        
        tumSoundPlaySample(b3);
        bCollision = true;
    }
}

void countScore(void)
{

    if (bBirdAlive == true)
    {

        if (getPipeX(pipe1) == BIRD_X - PIPE_WIDTH / 2)
        {
            tumSoundPlaySample(a4);
            player1.score++;
        }
        if (getPipeX(pipe2) == BIRD_X - PIPE_WIDTH / 2)
        {
            tumSoundPlaySample(a4);
            player1.score++;
        }
        if (getPipeX(pipe3) == BIRD_X - PIPE_WIDTH / 2)
        {
            tumSoundPlaySample(a4);
            player1.score++;
        }
    }
}
