#ifndef __DEMO_TASKS_H__
#define __DEMO_TASKS_H__

#include "FreeRTOS.h"
#include "task.h"

extern TaskHandle_t Game;
extern TaskHandle_t DemoTask2;


int createTasks(void);
void deleteTasks(void);

#endif // __DEMO_TASKS_H__