#include <stdint.h>
#include <stddef.h>
#include "register_def.h"
//Extra includes hier onder

#include <gpio.h>
#include <pin.h>
#include <prcm.h>
#include <systick.h>

static volatile _Bool flag_led_update;

void SysTickHandler()
{
    static int tick_count = 0;
    flag_led_update = tick_count == 1000 ? 1 : 0;
    tick_count = (tick_count+1) % (1000 + 1);
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

    unsigned int switcher = 0;

    while(1)
    {
        if(flag_led_update)
        {
            GPIOPinWrite(GPIOA1_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, switcher);
            switcher = (switcher + 1) % 16;
            flag_led_update = 0;
        }

       // __asm("     WFI"); assignment 2.4
        PRCMSleepEnter();
    }

    return 0;
}
