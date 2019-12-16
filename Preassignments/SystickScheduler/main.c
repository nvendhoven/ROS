#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "register_def.h"
//Extra includes hier onder

#include "inc\hw_memmap.h"
#include "inc\hw_gpio.h"
#include "inc\hw_apps_rcm.h"
#include "inc\hw_ocp_shared.h"

#include "blinky_tasks.h"

#define MS_TO_TICKS(ms)   (ms * 1)

typedef enum
{
    STATE_READY = 0,
    STATE_RUNNING,
    STATE_WAITING
} OS_state;

typedef struct
{
    void (*task_function)(void);
    uint32_t tick_period;
    uint32_t ticks_used;
    OS_state state;
} OS_tcb;

/* Global variables*/
OS_tcb tasks[8];
size_t amount_tasks = 0;

void SystickHandler()
{
    for(size_t idx = 0; idx != amount_tasks; idx++)
    {
        tasks[idx].ticks_used -= 1;
        if(tasks[idx].ticks_used == 0)
        {
            if(tasks[idx].state == STATE_READY)
                tasks[idx].state = STATE_WAITING;
            if(tasks[idx].state == STATE_WAITING)
                tasks[idx].state = STATE_READY;
            tasks[idx].ticks_used = tasks[idx].tick_period;
        }
    }
}

void OS_add_task(void (*task_function)(void), uint32_t tick_period, uint32_t ticks_init_delay)
{
    tasks[amount_tasks].task_function = task_function;
    tasks[amount_tasks].tick_period = tick_period;
    tasks[amount_tasks].ticks_used = ticks_init_delay;
    tasks[amount_tasks++].state = STATE_WAITING;
}

void OS_run_ready_tasks()
{
    for(size_t idx = 0; idx != amount_tasks; idx++)
        if(tasks[idx].state == STATE_READY)
        {
            (*tasks[idx].task_function)();
            tasks[idx].state = STATE_WAITING;
        }
}

void delay_1sec(void)
{
    __asm("    PUSH {r4-r12,lr}");

    __asm("    LDR r4, [pc, #12]");

    __asm("    MOV r5, pc");
    __asm("    NOP");

    __asm("    SUBS r4, #1");   /* 1 instruction cycle */
    __asm("    ITE NEQ");       /* 1 instruction cycle */

    __asm("    MOV pc, r5");    /* 1 + P instructions (where P is between 1 and 3 depending on pipeline refill) */


    __asm("    POP {r4-r12,pc}");
    __asm("    .word    5000000");
}

int main(void)
{

    HWREG(ARCM_BASE + APPS_RCM_O_GPIO_A_CLK_GATING) = 0x01;

    HWREG(OCP_SHARED_BASE + OCP_SHARED_O_GPIO_PAD_CONFIG_9) = 0x60;
    HWREG(OCP_SHARED_BASE + OCP_SHARED_O_GPIO_PAD_CONFIG_10) = 0x60;
    HWREG(OCP_SHARED_BASE + OCP_SHARED_O_GPIO_PAD_CONFIG_11) = 0x60;

    HWREG(GPIOA1_BASE + GPIO_O_GPIO_DIR) = 0x0E;
    HWREG(GPIOA1_BASE + GPIO_O_GPIO_DATA + (0x0E << 2)) = 0x00;

    HWREG(NVIC_ST_CTRL) = 0x00;         /* Disable SysTick during setup */
    HWREG(NVIC_ST_RELOAD) = 0x13880;    /* 80 000 reload value (1000p/s interrupt) */
    HWREG(NVIC_ST_CURRENT) = 0x00;      /* Clear any flags and set current value to 0 */
    HWREG(NVIC_ST_CTRL) = 0x07;         /* Enable SysTick, Enable interrupt, CLK_SRC = System clock */

    OS_add_task(blinky_red, RED_PERIOD, 100);
    OS_add_task(blinky_yellow, YELLOW_PERIOD, 200);
    OS_add_task(blinky_green, GREEN_PERIOD, 300);

    while(1)
        OS_run_ready_tasks();

    return 0;
}
