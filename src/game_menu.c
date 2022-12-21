#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"

#include "buttons.h"
#include "main.h"
#include "demo_tasks.h"
#include "TUM_Draw.h"
#include "TUM_Event.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sm.h"
#include "game_menu.h"


void EnterStartMenu(void* enter_start_menu)
{
    vTaskResume(Game);

    printf("Enter Start Menu\n");
    printf("Run Start Menu\n");
}

void ExitStartMenu(void* exit_start_menu)
{
    vTaskSuspend(Game);
    printf("Exit Start Menu\n");
}

void EnterSettingMenu(void *enter_setting_menu)
{
    vTaskResume(DemoTask2);

    printf("Enter Setting Menu\n");
    printf("Run Setting Menu\n");
}

void ExitSettingMenu(void* exit_setting_menu)
{
    vTaskSuspend(DemoTask2);
    printf("Exit Setting Menu\n");
}

void EnterSingleStart(void *enter_single_start)
{
    vTaskResume(SinglePlayer);
    printf("Enter Single Player\n");
    printf("Run Single Player\n");
}

void ExitSingleStart(void *eixt_single_start)
{
    vTaskSuspend(SinglePlayer);
    printf("Exit Single Player\n");
}

void EnterGameOver(void *enter_game_over)
{
    vTaskResume(GameOver);
    printf("Enter Game Over\n");
    printf("Run Game Over\n");
}

void ExitGameOver(void *exit_game_over)
{
    vTaskSuspend(GameOver);
    printf("Exit Game Over\n");
}

void EnterCheatMode(void *enter_cheat_mode)
{
    vTaskResume(CheatMode);
    printf("Enter Cheat Mode\n");
    printf("Run Cheat Mode\n");
}

void ExitCheatMode(void *exit_cheat_mode)
{
    vTaskSuspend(CheatMode);
    printf("Exit Cheat Mode\n");
}

void EnterViewScores(void * enter_view_scores)
{
    vTaskResume(ViewScores);
    printf("Enter View Scores\n");
    printf("Run View Scores\n");
}

void ExitViewScores(void *exit_view_scores)
{
    vTaskSuspend(ViewScores);
    printf("Exit View Scores\n");
}

void GameMenu(void* pvParameters)
{


    while(1)
    {

        vTaskDelay(10);
    }

}




