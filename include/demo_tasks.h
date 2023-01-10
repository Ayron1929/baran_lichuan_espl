#ifndef __DEMO_TASKS_H__
#define __DEMO_TASKS_H__

#include "FreeRTOS.h"
#include "task.h"
#include <stdbool.h>

extern TaskHandle_t DemoTask1;
extern TaskHandle_t DemoTask2;
extern TaskHandle_t DemoTask3;
extern TaskHandle_t DemoSendTask;
extern TaskHandle_t Game;
extern TaskHandle_t SinglePlayer;
extern TaskHandle_t GameOver;
extern TaskHandle_t CheatMode;
extern TaskHandle_t ViewScores;
extern TaskHandle_t StartSingle;

int createTasks(void);
void deleteTasks(void);
void vStatesTask(void *pvParameters);
void vCheatMode(void *pvParameters);

void vSetHighscore(void);
extern int highscore;
extern bool newHigh;


#endif // __DEMO_TASKS_H__