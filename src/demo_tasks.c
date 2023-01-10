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
#include "buttons.h"
#include "bird.h"
#include "pipes.h"

TaskHandle_t Game = NULL;
TaskHandle_t Settings = NULL;
TaskHandle_t SinglePlayer = NULL;
TaskHandle_t GameOver = NULL;
TaskHandle_t CheatMode = NULL;
TaskHandle_t ViewScores = NULL;
TaskHandle_t StartSingle = NULL;
TaskHandle_t StartCheats = NULL;
TaskHandle_t PauseMode = NULL;

int pause = 0;

// Task to just pereodically run the state machine
void vStatesTask(void *pvParameters)
{
	while (1)
	{
		states_run();
		vTaskDelay(pdMS_TO_TICKS(10));
	}
}

void vTaskGame(void *pvParameters)
{
	vDrawBase();
	vDrawBird();
	birdInit();
	pipesInit();

	tumDrawBindThread();
	
	TickType_t xLastFrameTime = xTaskGetTickCount();

	while (1)
	{
		if (DrawSignal)
			if (xSemaphoreTake(DrawSignal, portMAX_DELAY) ==
				pdTRUE)
			{
				tumEventFetchEvents(FETCH_EVENT_NONBLOCK |
									FETCH_EVENT_NO_GL_CHECK);
				xGetButtonInput();
				

				if (xSemaphoreTake(buttons.lock, 0) == pdTRUE)
				{

					if (tumEventGetMouseLeft() && vCheckMenuMouse())
						states_set_state(1);
					xSemaphoreGive(buttons.lock);
				}

				
				vDrawBackground();
				
				
				
				
				
				vDrawSpriteAnimations(xLastFrameTime);
				xLastFrameTime = xTaskGetTickCount();
				vDrawmenu();
				vDrawFlappyBird();
			}

		// vTaskDelay(20);
	}
}

void vTaskSettings(void *pvParameters)
{
	while (1)
	{
		if (DrawSignal)
			if (xSemaphoreTake(DrawSignal, portMAX_DELAY) ==
				pdTRUE)
			{
				tumEventFetchEvents(FETCH_EVENT_NONBLOCK |
									FETCH_EVENT_NO_GL_CHECK);
				// xGetButtonInput();
				// tumDrawBindThread();
				vDrawBackground();
				vDrawSubmenu();

				if (xSemaphoreTake(buttons.lock, 0) == pdTRUE)
				{

					if (tumEventGetMouseLeft() && vCheckSingle())
						states_set_state(2);
					if (tumEventGetMouseLeft() && vCheckCheatMode())
						states_set_state(4);
					if (tumEventGetMouseLeft() && vCheckViewScores())
						states_set_state(5);

					xSemaphoreGive(buttons.lock);
				}

				// vTaskDelay(20);
			}
	}
}

void vTaskSingle(void *pvParameters)
{


	
	TickType_t xLastFrameTime = xTaskGetTickCount();

	while (1)
	{
		if (DrawSignal)
			if (xSemaphoreTake(DrawSignal, portMAX_DELAY) ==
				pdTRUE)
			{
				tumEventFetchEvents(FETCH_EVENT_NONBLOCK |
									FETCH_EVENT_NO_GL_CHECK);
				xGetButtonInput();

				// if (xSemaphoreTake(buttons.lock, 0) == pdTRUE)
				// {

				// 	if (tumEventGetMouseLeft() && vCheckSingleTest())
				// 		states_set_state(3);

				// 	xSemaphoreGive(buttons.lock);
				// }

				vDrawBackground();
				
				vDrawPipes();
				vDrawScore();
				vCheckCollision();
				vBirdStatus();
				vBirdMovement();
				vDrawSpriteAnimations(xLastFrameTime);
				xLastFrameTime = xTaskGetTickCount();
				vDrawStop();

				if (xSemaphoreTake(buttons.lock, 0) == pdTRUE)
				{

					if (player1.birdY == (SCREEN_HEIGHT - 175))
					{
						// vTaskDelay(500);
						states_set_state(3);

					}
					
					if (buttons.buttons[KEYCODE(P)]) pause ++;
					if((pause % 2) == 1) states_set_state(6);
					


					
				

					xSemaphoreGive(buttons.lock);
				}
			}



	}
}

void vTaskStartSingle(void *pvParameters) {

	vDrawBase();
	vDrawBird();
	birdInit();
	pipesInit();
	

	while (1)
	{
		if (DrawSignal)
			if (xSemaphoreTake(DrawSignal, portMAX_DELAY) ==
				pdTRUE)
			{
				tumEventFetchEvents(FETCH_EVENT_NONBLOCK |
									FETCH_EVENT_NO_GL_CHECK);
				xGetButtonInput();
			
				vDrawBackground();
				vDrawStartSingle();

			}
	}

}

void vTaskGameOver(void *pvParameters)
{
	tumDrawBindThread();
	TickType_t xLastFrameTime = xTaskGetTickCount();

	while (1)
	{
		if (DrawSignal)
			if (xSemaphoreTake(DrawSignal, portMAX_DELAY) ==
				pdTRUE)
			{
				tumEventFetchEvents(FETCH_EVENT_NONBLOCK |
									FETCH_EVENT_NO_GL_CHECK);
				xGetButtonInput();
				
				vDrawBackground();

				vDrawPipes();
				vDrawScore();
				vDrawSpriteAnimations(xLastFrameTime);
				xLastFrameTime = xTaskGetTickCount();
				vDrawGameOver();
				// vShowScores();

				if (xSemaphoreTake(buttons.lock, 0) == pdTRUE)
				{

					if (tumEventGetMouseLeft() && vCheckReplay())
					{
						vBirdReset();
						states_set_state(2);
					}
						
					if (tumEventGetMouseLeft() && vCheckGameOverBack())
					{
						vBirdReset();
						states_set_state(1);
					}
						

					xSemaphoreGive(buttons.lock);
				}

				// vTaskDelay(20);
			}
	}

	
}

void vPauseMode(void *pvParameters)
{
	tumDrawBindThread();
	TickType_t xLastFrameTime = xTaskGetTickCount();
	

	while (1)
	{
		if (DrawSignal)
			if (xSemaphoreTake(DrawSignal, portMAX_DELAY) ==
				pdTRUE)
			{
				tumEventFetchEvents(FETCH_EVENT_NONBLOCK |
									FETCH_EVENT_NO_GL_CHECK);
				xGetButtonInput();
				
				vDrawBackground();
				
				
				vDrawPipes();
				vDrawScore();
				vDrawSpriteAnimations(xLastFrameTime);
				xLastFrameTime = xTaskGetTickCount();
				vDrawStop();

				if (xSemaphoreTake(buttons.lock, 0) == pdTRUE)
				{

					if (buttons.buttons[KEYCODE(P)]) pause ++;
					if((pause % 2) == 0) states_set_state(2);
					
						

					xSemaphoreGive(buttons.lock);
				}

		

				// vTaskDelay(20);
			}
	}
}

void vEnterCheats(void *pvParameters)
{
	
	while (1)
	{
		if (DrawSignal)
			if (xSemaphoreTake(DrawSignal, portMAX_DELAY) ==
				pdTRUE)
			{
				tumEventFetchEvents(FETCH_EVENT_NONBLOCK |
									FETCH_EVENT_NO_GL_CHECK);
				xGetButtonInput();
			
				vDrawBackground();
				vDrawCheatMode();

			}
	}
}

void vCheatMode(void *pvParameters)
{
	vDrawBird();
	vDrawBase();
	pipesInit();
	birdInit();
	TickType_t xLastFrameTime = xTaskGetTickCount();

	while (1)
		if (DrawSignal)
			if (xSemaphoreTake(DrawSignal, portMAX_DELAY) ==
				pdTRUE)
			{
				tumEventFetchEvents(FETCH_EVENT_NONBLOCK |
									FETCH_EVENT_NO_GL_CHECK);
				xGetButtonInput();

				vDrawBackground();

				
				
				
				vDrawPipes();
				vDrawScore();

				vBirdStatus();
				vBirdMovement();
				vDrawSpriteAnimations(xLastFrameTime);
				xLastFrameTime = xTaskGetTickCount();
				vDrawQuit();

				if (xSemaphoreTake(buttons.lock, 0) == pdTRUE)
				{
					// if we do it with escape, gotta add a text that says "esc to go back"
					if (buttons.buttons[KEYCODE(ESCAPE)]) // tumEventGetMouseLeft() && vCheckCheatModeBack()
					{
						vBirdReset();
						states_set_state(1);
					}
						

					xSemaphoreGive(buttons.lock);
				}
				




				// vTaskDelay(20);
			}
}

void vViewScores(void *pvParameters)
{
	while (1)
	{
		if (DrawSignal)
			if (xSemaphoreTake(DrawSignal, portMAX_DELAY) ==
				pdTRUE)
			{
				tumEventFetchEvents(FETCH_EVENT_NONBLOCK |
									FETCH_EVENT_NO_GL_CHECK);
				xGetButtonInput();

				vDrawBackground();

				vDrawCheatMode();

				if (xSemaphoreTake(buttons.lock, 0) == pdTRUE)
				{

					if (tumEventGetMouseLeft() && vCheckCheatModeBack())
						states_set_state(1);

					xSemaphoreGive(buttons.lock);
				}

				//vTaskDelay(20);
			}
	}
}

int createTasks(void)
{
	xTaskCreate(vTaskGame, "Game", mainGENERIC_STACK_SIZE * 20, NULL,
				mainGENERIC_PRIORITY + 4, &Game);
	xTaskCreate(vTaskSettings, "DemoTask2", mainGENERIC_STACK_SIZE * 2, NULL,
				mainGENERIC_PRIORITY + 1, &Settings);
	xTaskCreate(vTaskSingle, "SinglePlayer", mainGENERIC_STACK_SIZE * 2, NULL,
				mainGENERIC_PRIORITY + 4, &SinglePlayer);
	xTaskCreate(vTaskGameOver, "Game OVer", mainGENERIC_STACK_SIZE * 2, NULL,
				mainGENERIC_PRIORITY + 1, &GameOver);
	xTaskCreate(vCheatMode, "Cheat Mode", mainGENERIC_STACK_SIZE * 2, NULL,
				mainGENERIC_PRIORITY + 1, &CheatMode);
	xTaskCreate(vViewScores, "View Scores", mainGENERIC_STACK_SIZE * 2, NULL,
				mainGENERIC_PRIORITY + 1, &ViewScores);
	xTaskCreate(vEnterCheats, "Start Cheats", mainGENERIC_STACK_SIZE * 2, NULL,
				mainGENERIC_PRIORITY + 1, &StartCheats);
	xTaskCreate(vPauseMode, "Pause Mode", mainGENERIC_STACK_SIZE * 2, NULL,
				mainGENERIC_PRIORITY + 1, &PauseMode);

	xTaskCreate(vTaskStartSingle, "Start Single", mainGENERIC_STACK_SIZE * 2, NULL, mainGENERIC_PRIORITY +1,&StartSingle);

	vTaskSuspend(Game);
	vTaskSuspend(Settings);
	vTaskSuspend(SinglePlayer);
	vTaskSuspend(GameOver);
	vTaskSuspend(CheatMode);
	vTaskSuspend(ViewScores);
	vTaskSuspend(StartSingle);
	vTaskSuspend(StartCheats);
	vTaskSuspend(PauseMode);

	return 0;
}

void deleteTasks(void)
{
	if (Game)
	{
		vTaskDelete(Game);
	}
	if (Settings)
	{
		vTaskDelete(Settings);
	}
	if (SinglePlayer)
	{
		vTaskDelete(SinglePlayer);
	}
	if (GameOver)
	{
		vTaskDelete(GameOver);
	}
	if (CheatMode)
	{
		vTaskDelete(CheatMode);
	}
	if (ViewScores)
	{
		vTaskDelete(ViewScores);
	}
	if (StartSingle)
	{
		vTaskDelete(StartSingle);
	}
	if(StartCheats)
	{
		vTaskDelete(StartCheats);
	}
	if(PauseMode) vTaskDelete(PauseMode);
}