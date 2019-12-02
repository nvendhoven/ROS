/*
 * task.h
 *
 *  Created on: 12 nov. 2013
 *      Author: VersD @ hr.nl
 *
 */

#ifndef TASK_H_
#define TASK_H_

#include <stdint.h>

#define MAX_TASKS 16
#define TASK_MASK MAX_TASKS-1

//What states can our task have?
enum taskState{RUNNING, READY, WAITING};

//The task itself
typedef struct _task{
	int * 			stack; 				//pointer to the stack (on the heap?)

	void(*function)(void);				//function to execute
	enum taskState	state;				//Tasks have a state
	uint8_t 		uiPriority;			//Tasks have a priority
	uint32_t		uiCounter;			//Tasks have a counter for delays

	char			bInitialized;		//After initialization the stack also contains R4-R11
} task;

//Add new task to the taskList
void addTaskToList(void(*function)(void), int stackSize, int priority);

#endif /* TASK_H_ */
