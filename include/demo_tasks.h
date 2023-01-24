#ifndef __DEMO_TASKS_H__
#define __DEMO_TASKS_H__

#include "FreeRTOS.h"
#include "task.h"

extern TaskHandle_t Settings;
extern TaskHandle_t DemoSendTask;
extern TaskHandle_t Game;
extern TaskHandle_t SinglePlayer;
extern TaskHandle_t GameOver;
extern TaskHandle_t CheatMode;
extern TaskHandle_t ViewScores;
extern TaskHandle_t StartSingle;
extern TaskHandle_t StartCheats;
extern TaskHandle_t PauseMode;

void vStatesTask(void *pvParameters);
void vTaskGame(void *pvParameters);
void vTaskSettings(void *pvParameters);
void vCheatMode(void *pvParameters);
void vEnterCheats(void *pvParameters);
void vPauseMode(void *pvParameters);
void vTaskSingle(void *pvParameters);
void vTaskStartSingle(void *pvParameters);
void vTaskGameOver(void *pvParameters);
void vViewScores(void *pvParameters);

int createTasks(void);
void deleteTasks(void);


#endif // __DEMO_TASKS_H__