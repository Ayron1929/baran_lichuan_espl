#ifndef __DEMO_TASKS_H__
#define __DEMO_TASKS_H__

#include "FreeRTOS.h"
#include "task.h"
#include <stdbool.h>

extern TaskHandle_t Game;

int createTasks(void);
void deleteTasks(void);

#endif // __DEMO_TASKS_H__