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
#include "state_machine.h"

TaskHandle_t Game = NULL;
TaskHandle_t DemoTask2 = NULL; //test

void vTaskGame(void *pvParameters)
{
	tumDrawBindThread();
	vDrawBird(); //vDrawInitAnimations()

    TickType_t xLastFrameTime = xTaskGetTickCount();

	while (1) {
		tumEventFetchEvents(FETCH_EVENT_NONBLOCK |
				    FETCH_EVENT_NO_GL_CHECK);
		xGetButtonInput();

		vDrawBackground();
		vDrawBase();
		vDrawSpriteAnimations(xLastFrameTime);
		xLastFrameTime = xTaskGetTickCount();
		
		tumDrawUpdateScreen();
		vCheckStateInput();
		
		vTaskDelay(20);
	}
}

void vDemoTask2(void *pvParameters)
{
	while (1) {
		xGetButtonInput();
		printf("Task2\n");
		vCheckStateInput();
		vTaskDelay(1000);
	}
}

int createTasks(void)
{
	xTaskCreate(vTaskGame, "Game", mainGENERIC_STACK_SIZE * 20, NULL,
		    mainGENERIC_PRIORITY + 1, &Game);
	xTaskCreate(vDemoTask2, "DemoTask2", mainGENERIC_STACK_SIZE * 2, NULL,
		    mainGENERIC_PRIORITY + 1, &DemoTask2);

	vTaskSuspend(Game);
	vTaskSuspend(DemoTask2);

	return 0;
}

void deleteTasks(void)
{
	if (Game) {
		vTaskDelete(Game);
	}
	if (DemoTask2) {
		vTaskDelete(DemoTask2);
	}
}