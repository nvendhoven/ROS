#include <stdint.h>
#include <stddef.h>
#include "register_def.h"
//Extra includes hier onder

#include <gpio.h>
#include <pin.h>
#include <prcm.h>
#include <systick.h>

typedef enum {
    STATE_RED,
    STATE_YELLOW,
    STATE_GREEN
} state_t;

static volatile state_t state;
static volatile int tick;

void SysTickHandler()
{
    tick++;
}

void next_state(state_t newState, int delayInSeconds)
{
    while((tick/1000) <= delayInSeconds)
        PRCMSleepEnter();

    tick = 0;
    state = newState;
}

int main(void)
{
    SysTickDisable();
    SysTickPeriodSet(80000);
    SysTickIntRegister(SysTickHandler);
    SysTickIntEnable();
    SysTickEnable();

    PRCMPeripheralClkEnable(PRCM_GPIOA1 ,PRCM_RUN_MODE_CLK);

    PinTypeGPIO(PIN_64, PIN_STRENGTH_2MA, false);   /* Red LED is push-pull */
    PinTypeGPIO(PIN_01, PIN_STRENGTH_2MA, false);   /* Yellow LED is push-pull */
    PinTypeGPIO(PIN_02, PIN_STRENGTH_2MA, false);   /* Green LED is push-pull */

    GPIODirModeSet(GPIOA1_BASE, GPIO_PIN_2, GPIO_DIR_MODE_OUT);
    GPIODirModeSet(GPIOA1_BASE, GPIO_PIN_3, GPIO_DIR_MODE_OUT);
    GPIODirModeSet(GPIOA1_BASE, GPIO_PIN_1, GPIO_DIR_MODE_OUT);

    GPIOPinWrite(GPIOA1_BASE, GPIO_PIN_2, ~GPIO_PIN_2);  /* Turn yellow LED off */
    GPIOPinWrite(GPIOA1_BASE, GPIO_PIN_3, ~GPIO_PIN_3);  /* Turn green LED off */
    GPIOPinWrite(GPIOA1_BASE, GPIO_PIN_1, ~GPIO_PIN_1);  /* Turn red LED off */

    state = STATE_RED;

    while(1)
    {
        switch(state)
        {
        case STATE_RED:
            GPIOPinWrite(GPIOA1_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x02);
            next_state(STATE_YELLOW, 5);
            break;
        case STATE_YELLOW:
            GPIOPinWrite(GPIOA1_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x04);
            next_state(STATE_GREEN, 1);
            break;
        case STATE_GREEN:
            GPIOPinWrite(GPIOA1_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x08);
            next_state(STATE_RED, 4);
            break;
        }

    }

    return 0;
}
