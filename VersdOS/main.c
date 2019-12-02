#include <stdint.h>
#include <stddef.h>

/* Example/Board Header files */
#include "register_def.h"
#include "src/task.h"
#include "src/schedule.h"

#define RED_LED   (1<<1)
#define YELLOW_LED (1<<2)
#define GREEN_LED (1<<3)

#define ALL_LEDS (RED_LED | YELLOW_LED | GREEN_LED)

/*------------------------------------DEMO---------------------------------------*/
//unworthy delay function
void delay(unsigned int count)
{
    while(count--);
}

void toggleRed(void)
{
    while(1)
    {
        HWREG(GPIOA1_BASE + (ALL_LEDS<<2)) ^= RED_LED;
        delay(3000000);
    }
}

void toggleYellow(void)
{
    while(1)
    {
        HWREG(GPIOA1_BASE + (ALL_LEDS<<2)) ^= YELLOW_LED;
        delay(1500000);
    }
}

void toggleGreen(void)
{
    while(1)
    {
        HWREG(GPIOA1_BASE + (ALL_LEDS<<2)) ^= GREEN_LED;
        delay(6000000);
    }
}

int main(void)
{
    // Turn on GPIOA1 module
    HWREG(ARCM_BASE + APPS_RCM_O_GPIO_B_CLK_GATING ) |= 1;
    // Configure led pins as outputs
    HWREG(GPIOA1_BASE + GPIO_O_GPIO_DIR) |= ALL_LEDS;

    // Connect GPIO module to actual pins and select current
    HWREG(OCP_SHARED_BASE + OCP_SHARED_O_GPIO_PAD_CONFIG_9) = 1<<5; //2mA driver
    HWREG(OCP_SHARED_BASE + OCP_SHARED_O_GPIO_PAD_CONFIG_10) = 1<<5; //2mA driver
    HWREG(OCP_SHARED_BASE + OCP_SHARED_O_GPIO_PAD_CONFIG_11) = 1<<5; //2mA driver

    // Create tasks. Provide fpointer, stacksize, and priority:
    addTaskToList(toggleRed     , 128, 2);
    addTaskToList(toggleGreen   , 128, 2);
    addTaskToList(toggleYellow, 128, 2);

    // Turn on SysTick, every ms when CPU is running at 80MHz
    HWREG(NVIC_ST_RELOAD) =  80000-1;
    HWREG(NVIC_ST_CTRL) =  7;


    while (1)
    {
        __asm(" wfi"); // Sleep until next SysTick
    }
}
