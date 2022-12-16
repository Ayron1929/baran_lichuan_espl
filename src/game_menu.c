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
}

void RunStartMenu(void *run_start_menu)
{
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
}

void RunSettingMenu(void* run_setting_menu)
{
    printf("Run Setting Menu\n");
}

void EnterSingleStart(void *enter_single_start)
{
    vTaskResume(SinglePlayer);
}



void GameMenu(void* pvParameters)
{


    while(1)
    {

        vTaskDelay(10);
    }

}




