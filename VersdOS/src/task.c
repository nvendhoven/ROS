/*
 * Copyright 2015 Daniel Versluis
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. 
 *
 * task.h
 *
 *  Created on: 12 nov. 2013
 *      Author: VersD @ hr.nl
 *
 */
#include <stdlib.h>
#include "task.h"
#include "schedule.h"

/*
 * Current task list implementation
 *
 * Alternate possibility: separate list for each task
 * state
 *
 * */
task taskList[MAX_TASKS];

//currentTask used by the scheduler
extern task * currentTask;


/*
 * Function to add a new task to the taskList
 * Not sorted, prioritized or any of that kind.
 *
 * It simply allocates memory for the new task stack,
 * fills the stack up so that the context switch will
 * successfully pop these registers and start running
 * at the correct address when returning from the ISR
 */
void addTaskToList(void(*function)(void), int stackSize, int priority)
{
	uint8_t i=0;

	//Simply find the next empty spot
	while(taskList[i].function != 0){
		//increment i and roll back at the limit
		i++;
		i &= TASK_MASK;
	}

	//The new and latest task will become the currentTask
	currentTask = &taskList[i];

	currentTask->function = function;

	//Allocate memory... do we wanna use malloc or our own implementation ;-) ?
	currentTask->stack = (int *)malloc(stackSize)+stackSize;

	//now we initialize the stack for when we return the first time from the scheduler
	//This is the 'hardware' stack
	*((currentTask->stack)--) 	= 0x01000000;					//XSPR	nothing special , no fpu etc.
	*((currentTask->stack)--) 	= (int)currentTask->function; 	//set PC to function pointer, cast as int to silence the compiler
	*((currentTask->stack)--) 	= 0xFFFFFFFD;					//LR
	*((currentTask->stack)--) 	= 0x0000000C;					//R12	Initial values used for debugging purposes
	*((currentTask->stack)--) 	= 0x00000003;					//R3
	*((currentTask->stack)--) 	= 0x00000002;					//R2
	*((currentTask->stack)--) 	= 0x00000001;					//R1
	*((currentTask->stack)) 	= 0x00000000;					//R0

	//currentTask->stack-=8;		//lower the stack pointer to simulate the other R4-R11 registers

	//Update the CPU PSP with our new stack pointer
	writePSP(currentTask->stack);

	/*
	 * Initialize the task properties
	 */
	currentTask->state 			= READY;
	currentTask->bInitialized 	= 0;
	currentTask->uiPriority 	= priority;


	return;
}
