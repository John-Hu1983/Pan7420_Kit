#include "../Inc/includes.h"

LL_UTILS_ClkInitTypeDef UTILS_ClkInitStruct;

/*
 * @description	:   about 0.1us
 * @param		:
 * @return		:
 */
void __delay_pan742(uint32_t cycle)
{
    __asm volatile(
        "loop: \n"
        "subs %0, %0, #1 \n"
        "bcs loop \n"
        "bx lr \n"
        : "+r"(cycle) // Input/output operand
        :
        : "cc" // Clobbers condition flags
    );
}

/*
 * @description	:
 * @param		:
 * @return		:
 */
void SystemClock_Config(void)
{
    // LL_RCC_HSI_Enable();
    // LL_RCC_HSI_SetCalibFreq(LL_RCC_HSICALIBRATION_24MHz);
    // while (LL_RCC_HSI_IsReady() != 1)
    // {
    // }

    // LL_PLL_ConfigSystemClock_HSI(&UTILS_ClkInitStruct);
    // LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    // LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
    // while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
    // {
    // }

    // LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
    // LL_SetSystemCoreClock(48000000);

    /* 使能HSI */
    LL_RCC_HSI_Enable();
    while (LL_RCC_HSI_IsReady() != 1)
    {
    }

    /* 设置 AHB 分频*/
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

    /* 配置HSISYS作为系统时钟源 */
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSISYS);
    while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSISYS)
    {
    }

    /* 设置 APB1 分频*/
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
    LL_Init1msTick(8000000);

    /* 更新系统时钟全局变量SystemCoreClock(也可以通过调用SystemCoreClockUpdate函数更新) */
    LL_SetSystemCoreClock(8000000);
}

/*
 * @description	:
 * @param		:
 * @return		:
 */
void init_board_global(void)
{
    SystemClock_Config();
    open_systick_irq();
}

void Error_Handler(void)
{
    while (1)
    {
    }
}
