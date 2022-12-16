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
#include"sm.h"


TaskHandle_t Game = NULL;
TaskHandle_t DemoTask2 = NULL;
// TaskHandle_t SinglePlayer = NULL;

// Task to just pereodically run the state machine
void vStatesTask(void *pvParameters)
{
	while(1){
		states_run();
		vTaskDelay(pdMS_TO_TICKS(10));
	}
}

void vTaskGame(void *pvParameters)
{
	tumDrawBindThread();
	vDrawBird(); //vDrawInitAnimations()
	vDrawBackground();

    TickType_t xLastFrameTime = xTaskGetTickCount();

	while (1) {
		tumEventFetchEvents(FETCH_EVENT_NONBLOCK |
				    FETCH_EVENT_NO_GL_CHECK);
		xGetButtonInput();


		if (tumEventGetMouseLeft())
		{
			states_set_state(1);
		}

		vDrawBase();
		vDrawmenu();
		vDrawQuit();
		vDrawSpriteAnimations(xLastFrameTime);
		xLastFrameTime = xTaskGetTickCount();
		
		tumDrawUpdateScreen();
		// vCheckMenuMouse();
		
		vTaskDelay(20);
	}
}

void vDemoTask2(void *pvParameters)
{
	while (1) {
		tumEventFetchEvents(FETCH_EVENT_NONBLOCK |
				    FETCH_EVENT_NO_GL_CHECK);		
		xGetButtonInput();
		tumDrawBindThread();
		vDrawBackground();
		vDrawSubmenu();
		vDrawQuit();
		tumDrawUpdateScreen();

		vTaskDelay(20);
	}
}

void vTaskSingle(void *pvParameters)
{
	while (1) {
		tumEventFetchEvents(FETCH_EVENT_NONBLOCK |
				    FETCH_EVENT_NO_GL_CHECK);		
		xGetButtonInput();
		tumDrawBindThread();
		vDrawBackground();
		vDrawStop();
		vDrawQuit();

		tumDrawUpdateScreen();


	}
	
}

int createTasks(void)
{
	xTaskCreate(vTaskGame, "Game", mainGENERIC_STACK_SIZE * 20, NULL,
		    mainGENERIC_PRIORITY + 1, &Game);
	xTaskCreate(vDemoTask2, "DemoTask2", mainGENERIC_STACK_SIZE * 2, NULL,
		    mainGENERIC_PRIORITY + 1, &DemoTask2);
	// xTaskCreate(vTaskSingle, "SinglePlayer", mainGENERIC_STACK_SIZE * 2, NULL,
	// 	    mainGENERIC_PRIORITY + 1, &SinglePlayer);

	vTaskSuspend(Game);
	vTaskSuspend(DemoTask2);
	// vTaskSuspend(SinglePlayer);

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