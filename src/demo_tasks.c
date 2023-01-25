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

int highscore = 0;

// Task to just periodically run the state machine
void vStatesTask(void *pvParameters)
{
	while (1)
	{
		states_run();
		vTaskDelay(pdMS_TO_TICKS(10));
	}
}

// enter game
void vTaskGame(void *pvParameters)
{
	vDrawBase();
	vDrawBird();
	birdInit();
	pipesInit();

	TickType_t xLastFrameTime = xTaskGetTickCount();

	while (1)
	{
		if (DrawSignal)
			if (xSemaphoreTake(DrawSignal, portMAX_DELAY) ==
				pdTRUE)
			{
				tumEventFetchEvents(FETCH_EVENT_NONBLOCK |
									FETCH_EVENT_NO_GL_CHECK);

				if (tumEventGetMouseLeft() && vCheckMenuMouse())
					states_set_state(1);

				vDrawBackground();
				vDrawFlappyBird();
				vDrawSpriteAnimations(xLastFrameTime);
				xLastFrameTime = xTaskGetTickCount();
				vDrawmenu();
			}
	}
}

// enter setting
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

				vDrawBackground();
				vDrawSubmenu();

				if (tumEventGetMouseLeft() && vCheckSingle())
					states_set_state(2);
				if (tumEventGetMouseLeft() && vCheckCheatMode())
					states_set_state(4);
				if (tumEventGetMouseLeft() && vCheckViewScores())
					states_set_state(5);
			}
	}
}

// start playing single mode
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
				
				vDrawBackground();
				vDrawPipes();
				vDrawScore();
				vCheckCollision();
				vBirdStatus();
				vBirdMovement();
				vPipeMovement();
				vDrawSpriteAnimations(xLastFrameTime);
				xLastFrameTime = xTaskGetTickCount();
				vDrawStop();

				if (xSemaphoreTake(buttons.lock, 0) == pdTRUE)
				{
					//when bird touches the ground
					if (getBirdY() == SCREEN_HEIGHT - 175)
					{
						states_set_state(3);
					}

					// stop the game
					if (buttons.buttons[KEYCODE(P)])
						states_set_state(6);

					xSemaphoreGive(buttons.lock);
				}
			}
	}
}

// before playing single mode
void vTaskStartSingle(void *pvParameters)
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
				vDrawStartSingle();
			}
	}
}

// when game over
void vTaskGameOver(void *pvParameters)
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
				
				vDrawBackground();
				vDrawPipes();
				vDrawSpriteAnimations(xLastFrameTime);
				xLastFrameTime = xTaskGetTickCount();
				vDrawGameOver();
				vDrawScoreboard();
				vSetHighscore();
				vDrawMedal();
				

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
			}
	}
}

// when game stops
void vPauseMode(void *pvParameters)
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

				//vDrawBackground();
				//vDrawScore();
				//vDrawStop();

				// continue to play
				if (xSemaphoreTake(buttons.lock, 0) == pdTRUE)
				{

					if (buttons.buttons[KEYCODE(P)])
						states_set_state(2);

					xSemaphoreGive(buttons.lock);
				}
			}
	}
}

void vEnterCheats(void *pvParameters)
{	
	pipesInit();
	birdInit();
	TickType_t last_change = xTaskGetTickCount();
	int debounceDelay = 150; // 150 ms
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

				// check user input for start scores
				if (xSemaphoreTake(buttons.lock, 0) == pdTRUE)
				{

					if (buttons.buttons[KEYCODE(UP)])
					{
						if (xTaskGetTickCount() - last_change >
							debounceDelay)
						{
							incrementScore(b1, 1);
							last_change = xTaskGetTickCount();
						}
					}
					if (buttons.buttons[KEYCODE(DOWN)])
					{
						if (xTaskGetTickCount() - last_change >
							debounceDelay)
						{
							incrementScore(b1, -1);
							last_change = xTaskGetTickCount();
						}
					}

					xSemaphoreGive(buttons.lock);
				}
			}
	}
}


// when playing cheat mode
void vCheatMode(void *pvParameters)
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

				vDrawBackground();
				vDrawPipes();
				vDrawScore();
				vBirdStatus();
				vBirdMovement();
				vPipeMovement();
				vDrawSpriteAnimations(xLastFrameTime);
				xLastFrameTime = xTaskGetTickCount();
				vDrawQuit();

				// quit cheat mode
				if (xSemaphoreTake(buttons.lock, 0) == pdTRUE)
				{
					if (buttons.buttons[KEYCODE(ESCAPE)])
					{
						vBirdReset();
						states_set_state(1);
					}
					xSemaphoreGive(buttons.lock);
				}
			}
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

				vDrawBackground();
				vDrawHighScores();

				if (tumEventGetMouseLeft() && vCheckCheatModeBack())
					states_set_state(1);
			}
	}
}

// renew highest score
void vSetHighscore(void)
{
	if (getScore() >= highscore)
	{
		vDrawNewHigh();
		highscore = getScore();
	}
}

int createTasks(void)
{
	xTaskCreate(vTaskGame, "Game", mainGENERIC_STACK_SIZE * 20, NULL,
				mainGENERIC_PRIORITY + 1, &Game);
	xTaskCreate(vTaskSettings, "DemoTask2", mainGENERIC_STACK_SIZE * 20, NULL,
				mainGENERIC_PRIORITY + 1, &Settings);
	xTaskCreate(vTaskSingle, "SinglePlayer", mainGENERIC_STACK_SIZE * 20, NULL,
				mainGENERIC_PRIORITY + 1, &SinglePlayer);
	xTaskCreate(vTaskGameOver, "Game OVer", mainGENERIC_STACK_SIZE * 20, NULL,
				mainGENERIC_PRIORITY + 1, &GameOver);
	xTaskCreate(vCheatMode, "Cheat Mode", mainGENERIC_STACK_SIZE * 20, NULL,
				mainGENERIC_PRIORITY + 1, &CheatMode);
	xTaskCreate(vViewScores, "View Scores", mainGENERIC_STACK_SIZE * 20, NULL,
				mainGENERIC_PRIORITY + 1, &ViewScores);
	xTaskCreate(vEnterCheats, "Start Cheats", mainGENERIC_STACK_SIZE * 20, NULL,
				mainGENERIC_PRIORITY + 1, &StartCheats);
	xTaskCreate(vPauseMode, "Pause Mode", mainGENERIC_STACK_SIZE * 20, NULL,
				mainGENERIC_PRIORITY + 1, &PauseMode);

	xTaskCreate(vTaskStartSingle, "Start Single", mainGENERIC_STACK_SIZE * 20, NULL, mainGENERIC_PRIORITY + 1, &StartSingle);

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
	if (StartCheats)
	{
		vTaskDelete(StartCheats);
	}
	if (PauseMode)
		vTaskDelete(PauseMode);
}