#include <stdint.h>
#include <stddef.h>
#include "register_def.h"
//Extra includes hier onder

#include <gpio.h>
#include <pin.h>
#include <prcm.h>

#define SEC_TO_LOOP(x) ((80000000/16)*x)

int main(void)
{
    PRCMPeripheralClkEnable(PRCM_GPIOA0 | PRCM_GPIOA1 ,PRCM_RUN_MODE_CLK);
    PinTypeGPIO( , 0x60, 0);
    GPIODirModeSet();

    GPIOPinWrite();

    while(1)
    {

    }

    return 0;
}
