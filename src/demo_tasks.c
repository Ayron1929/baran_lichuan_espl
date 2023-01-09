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
TaskHandle_t DemoTask2 = NULL;
TaskHandle_t SinglePlayer = NULL;
TaskHandle_t GameOver = NULL;
TaskHandle_t CheatMode = NULL;
TaskHandle_t ViewScores = NULL;
TaskHandle_t StartSingle = NULL;

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
				vDrawLogo();
				vDrawMenu();
			}

		// vTaskDelay(20);
	}
}

void vDemoTask2(void *pvParameters)
{
	while (1)
	{
		if (DrawSignal)
			if (xSemaphoreTake(DrawSignal, portMAX_DELAY) ==
				pdTRUE)
			{
				tumEventFetchEvents(FETCH_EVENT_NONBLOCK |
									FETCH_EVENT_NO_GL_CHECK);

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
	vDrawBird();
	vDrawBase();
	pipesInit();
	//birdInit();
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

				if (xSemaphoreTake(buttons.lock, 0) == pdTRUE)
				{

					if (player1.birdY == SCREEN_HEIGHT - 175){

						vTaskDelay(250);
						states_set_state(3);
						vTaskSuspend(SinglePlayer);
					}

					xSemaphoreGive(buttons.lock);
				}


			}
	}
}

void vTaskStartSingle(void *pvParameters)
{

	vDrawBase();
	birdInit();
	vBirdReset();
	vDrawBird();

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
				vDrawStartSingle();
				vBirdStatus();
				vDrawScore();
				vDrawSpriteAnimations(xLastFrameTime);
				xLastFrameTime = xTaskGetTickCount();
			}
	}
}

void vTaskGameOver(void *pvParameters)
{	
	vDrawBase();

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
				vDrawGameOver();
				vDrawScore();
				vDrawSpriteAnimations(xLastFrameTime);
				xLastFrameTime = xTaskGetTickCount();

				if (xSemaphoreTake(buttons.lock, 0) == pdTRUE)
				{

					if (buttons.buttons[KEYCODE(ESCAPE)])//tumEventGetMouseLeft() && vCheckReplay())
						states_set_state(2);
						//vBirdReset();
					if (tumEventGetMouseLeft() && vCheckGameOverBack())
						states_set_state(1);

					xSemaphoreGive(buttons.lock);
				}

				// vTaskDelay(20);
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

				vDrawCheatMode();

				vDrawPipes();
				vDrawScore();

				vBirdStatus();
				vBirdMovement();
				vDrawSpriteAnimations(xLastFrameTime);
				xLastFrameTime = xTaskGetTickCount();

				if (xSemaphoreTake(buttons.lock, 0) == pdTRUE)
				{
					// if we do it with escape, gotta add a text that says "esc to go back"
					if (buttons.buttons[KEYCODE(ESCAPE)]) //tumEventGetMouseLeft() && vCheckCheatModeBack()
						states_set_state(1);

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

				// vTaskDelay(20);
			}
	}
}


int createTasks(void)
{
	xTaskCreate(vTaskGame, "Game", mainGENERIC_STACK_SIZE * 5, NULL,
				mainGENERIC_PRIORITY + 1, &Game);
	xTaskCreate(vDemoTask2, "DemoTask2", mainGENERIC_STACK_SIZE * 5, NULL,
				mainGENERIC_PRIORITY + 2, &DemoTask2);
	xTaskCreate(vTaskSingle, "SinglePlayer", mainGENERIC_STACK_SIZE * 5, NULL,
				mainGENERIC_PRIORITY + 4, &SinglePlayer);
	xTaskCreate(vTaskGameOver, "Game Over", mainGENERIC_STACK_SIZE * 5, NULL,
				mainGENERIC_PRIORITY + 4, &GameOver);
	xTaskCreate(vCheatMode, "Cheat Mode", mainGENERIC_STACK_SIZE * 5, NULL,
				mainGENERIC_PRIORITY + 4, &CheatMode);
	xTaskCreate(vViewScores, "View Scores", mainGENERIC_STACK_SIZE * 5, NULL,
				mainGENERIC_PRIORITY + 1, &ViewScores);

	xTaskCreate(vTaskStartSingle, "Start Single", mainGENERIC_STACK_SIZE * 5, NULL, mainGENERIC_PRIORITY + 3, &StartSingle);

	vTaskSuspend(Game);
	vTaskSuspend(DemoTask2);
	vTaskSuspend(SinglePlayer);
	vTaskSuspend(GameOver);
	vTaskSuspend(CheatMode);
	vTaskSuspend(ViewScores);
	vTaskSuspend(StartSingle);

	return 0;
}

void deleteTasks(void)
{
	if (Game)
	{
		vTaskDelete(Game);
	}
	if (DemoTask2)
	{
		vTaskDelete(DemoTask2);
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
}