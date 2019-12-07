#include <stdint.h>
#include <stddef.h>
#include "register_def.h"
//Extra includes hier onder

#include "inc\hw_memmap.h"
#include "inc\hw_gpio.h"
#include "inc\hw_apps_rcm.h"
#include "inc\hw_ocp_shared.h"

static volatile _Bool flag_led_update;

void SysTickHandler()
{
    static int tick_count = 0;
    flag_led_update = tick_count == 1000 ? 1 : 0;
    tick_count = (tick_count+1) % (1000 + 1);
}

int main(void)
{

    /* Init SysTick */
    HWREG(NVIC_ST_CTRL) = 0x00;         // Disable SysTick during setup
    HWREG(NVIC_ST_RELOAD) = 0x13880;    // 80 000 reload value (1000p/s interrupt)
    HWREG(NVIC_ST_CURRENT) = 0x00;      // Clear any flags and set current value to 0
    HWREG(NVIC_ST_CTRL) = 0x07;         // Enable SysTick, Enable interrupt, CLK_SRC = System clock

    /* Init LEDS */
    HWREG(ARCM_BASE + APPS_RCM_O_GPIO_A_CLK_GATING) = 0x01;

    HWREG(OCP_SHARED_BASE + OCP_SHARED_O_GPIO_PAD_CONFIG_9) = 0x60;
    HWREG(OCP_SHARED_BASE + OCP_SHARED_O_GPIO_PAD_CONFIG_10) = 0x60;
    HWREG(OCP_SHARED_BASE + OCP_SHARED_O_GPIO_PAD_CONFIG_11) = 0x60;

    HWREG(GPIOA1_BASE + GPIO_O_GPIO_DIR) = 0x0E;
    HWREG(GPIOA1_BASE + GPIO_O_GPIO_DATA + (0x0E << 2)) = 0x00;

    unsigned int index = 0;

    while(1)
    {
        if(!flag_led_update)
            continue;

        HWREG(GPIOA1_BASE + GPIO_O_GPIO_DATA + (0x0E << 2)) = index;
        index = (index + 1) % 16;
        flag_led_update = 0;
    }

    return 0;
}
