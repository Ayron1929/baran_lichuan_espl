#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>
#include <stdbool.h>
#include <SDL2/SDL_scancode.h>

#include "TUM_Ball.h"
#include "TUM_Draw.h"
#include "TUM_Event.h"
#include "TUM_Sound.h"
#include "TUM_Utils.h"
#include "TUM_Font.h"
#include "TUM_Print.h"
#include "TUM_FreeRTOS_Utils.h"

#include "demo_tasks.h"
#include "buttons.h"
#include "defines.h"
#include "AsyncIO.h"
#include "sm.h"
#include "game_menu.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"



SemaphoreHandle_t DrawSignal = NULL;

static TaskHandle_t GameMenuHandle = NULL;


static TaskHandle_t BufferSwap = NULL;

void vSwapBuffers(void *pvParameters)
{
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	const TickType_t frameratePeriod = 20;

	while (1) {
		tumDrawUpdateScreen();
		tumEventFetchEvents(FETCH_EVENT_BLOCK);
		xSemaphoreGive(DrawSignal);
		vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(frameratePeriod));
	}
}

int main(int argc, char *argv[])
{
    char *bin_folder_path = tumUtilGetBinFolderPath(argv[0]);

    printf("Initializing: ");

    if (tumDrawInit(bin_folder_path))
    {
        PRINT_ERROR("Failed to initialize drawing");
        goto err_init_drawing;
    }

    if (tumEventInit())
    {
        PRINT_ERROR("Failed to initialize events");
        goto err_init_events;
    }

    if (tumSoundInit(bin_folder_path))
    {
        PRINT_ERROR("Failed to initialize audio");
        goto err_init_audio;
    }

    if (buttonsInit())
    {
        PRINT_ERROR("Failed to create buttons lock");
        goto err_buttons_lock;
    }

    DrawSignal = xSemaphoreCreateBinary();

	xTaskCreate(vSwapBuffers, "BufferSwapTask", mainGENERIC_STACK_SIZE * 2,
		    NULL, configMAX_PRIORITIES, &BufferSwap);


    xTaskCreate(GameMenu, "Game Menu", mainGENERIC_STACK_SIZE * 2, NULL, configMAX_PRIORITIES - 1, &GameMenuHandle);
    xTaskCreate(vStatesTask, "States", mainGENERIC_STACK_SIZE * 2, NULL, configMAX_PRIORITIES - 1, NULL);


    if (createTasks())
    {
        PRINT_ERROR("Failed to create tasks");
        goto err_demotask;
    }

    states_add(NULL, EnterStartMenu, NULL, ExitStartMenu, 0, "START_MENU");
    states_add(NULL, EnterSettingMenu, NULL, ExitSettingMenu, 1, "SETTING_MENU");
    states_add(NULL, EnterSingleStart, RunSingleStart, ExitSingleStart, 2, "SINGLE_PLAYER");
    states_add(NULL, EnterGameOver, NULL, ExitGameOver, 3, "Game Over");
    states_add(NULL, EnterCheatMode, NULL, ExitCheatMode, 4, "Cheat Mode");
    states_add(NULL, EnterViewScores, NULL, ExitViewScores, 5, "View Scores");

    states_init();
    states_run();
    

    vTaskStartScheduler();
    tumFUtilPrintTaskStateList();

    return EXIT_SUCCESS;

err_demotask:
    vSemaphoreDelete(buttons.lock);
err_buttons_lock:
    tumSoundExit();
err_init_audio:
    tumEventExit();
err_init_events:
    tumDrawExit();
err_init_drawing:
    return EXIT_FAILURE;
}

// cppcheck-suppress unusedFunction
__attribute__((unused)) void vMainQueueSendPassed(void)
{
    /* This is just an example implementation of the "queue send" trace hook. */
}

// cppcheck-suppress unusedFunction
__attribute__((unused)) void vApplicationIdleHook(void)
{
#ifdef __GCC_POSIX__
    struct timespec xTimeToSleep, xTimeSlept;
    /* Makes the process more agreeable when using the Posix simulator. */
    xTimeToSleep.tv_sec = 1;
    xTimeToSleep.tv_nsec = 0;
    nanosleep(&xTimeToSleep, &xTimeSlept);
#endif
}
