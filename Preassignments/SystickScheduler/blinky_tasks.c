#include "register_def.h"

#include "inc\hw_memmap.h"
#include "inc\hw_gpio.h"
#include "inc\hw_apps_rcm.h"
#include "inc\hw_ocp_shared.h"

#include "blinky_tasks.h"


void blinky_red(void)
{
    HWREG(GPIOA1_BASE + GPIO_O_GPIO_DATA + (0x02 << 2)) ^= 0x02;
}

void blinky_yellow(void)
{
    HWREG(GPIOA1_BASE + GPIO_O_GPIO_DATA + (0x04 << 2)) ^= 0x04;
}

void blinky_green(void)
{
    HWREG(GPIOA1_BASE + GPIO_O_GPIO_DATA + (0x08 << 2)) ^= 0x08;
}


