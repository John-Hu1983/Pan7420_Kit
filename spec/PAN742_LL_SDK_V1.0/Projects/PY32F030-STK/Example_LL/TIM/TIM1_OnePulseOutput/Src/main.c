/**
  ******************************************************************************
  * @file    main.c
  * @author  MCU Application Team
  * @Version V1.0.0
  * @Date    2022-8-19
  * @brief   main function
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "py32f030xx_ll_Start_Kit.h"

/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void APP_ConfigTIM1OnePulse(void);
static void APP_ConfigTIM1Channel(void);

/*******************************************************************************
**功能描述 ：执行函数
**输入参数 ：
**输出参数 ：
*******************************************************************************/
int main(void)
{
  /*配置系统时钟*/
  SystemClock_Config();
  
  /*使能TIM1，GPIOA时钟*/
  LL_APB1_GRP2_EnableClock(RCC_APBENR2_TIM1EN);
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
  
  /*配置并开启TIM1单脉冲模式*/
  APP_ConfigTIM1OnePulse();
  
  while (1)
  {
  }
}

/*******************************************************************************
**功能描述 ：TIM1单脉冲模式通道配置函数
**输入参数 ：
**输出参数 ：
*******************************************************************************/
void APP_ConfigTIM1Channel(void)
{
  /*通道2配置*/
  /*配置有效边沿为上升沿*/
  LL_TIM_IC_SetPolarity(TIM1,LL_TIM_CHANNEL_CH2,LL_TIM_IC_POLARITY_RISING);
  /*配置通道为输入模式*/
  LL_TIM_IC_SetActiveInput(TIM1,LL_TIM_CHANNEL_CH2,LL_TIM_ACTIVEINPUT_DIRECTTI);
  
  /*通道2输入映射PA9*/
  LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_9,LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_8_15(GPIOA,LL_GPIO_PIN_9,LL_GPIO_AF_2);

  /*使能通道2*/
  LL_TIM_CC_EnableChannel(TIM1,LL_TIM_CHANNEL_CH2);
  
  /*通道1配置*/
  /*配置输出极性为高有效*/
  LL_TIM_OC_SetPolarity(TIM1,LL_TIM_CHANNEL_CH1,LL_TIM_OCPOLARITY_HIGH);
  /*配置空闲极性为低*/
  LL_TIM_OC_SetIdleState(TIM1,LL_TIM_CHANNEL_CH1,LL_TIM_OCIDLESTATE_LOW);
  /*设置比较值：200*/
  LL_TIM_OC_SetCompareCH1(TIM1,200);
  /*配置通道为PWM2模式*/
  LL_TIM_OC_SetMode(TIM1,LL_TIM_CHANNEL_CH1,LL_TIM_OCMODE_PWM2);
  
  /*输出映射PA8*/
  LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_8,LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_8_15(GPIOA,LL_GPIO_PIN_8,LL_GPIO_AF_2);

  /*使能通道1*/
  LL_TIM_CC_EnableChannel(TIM1,LL_TIM_CHANNEL_CH1);
  
}

/*******************************************************************************
**功能描述 ：TIM1单脉冲模式配置函数
**输入参数 ：
**输出参数 ：
*******************************************************************************/
void APP_ConfigTIM1OnePulse(void)
{
  /*CK_INT 1分频*/
  LL_TIM_SetClockDivision(TIM1,LL_TIM_CLOCKDIVISION_DIV1);
  /*向上计数模式*/
  LL_TIM_SetCounterMode(TIM1,LL_TIM_COUNTERMODE_UP);
  /*重装载值1000*/
  LL_TIM_SetAutoReload(TIM1,1000-1);
  /*CK_CNT 预分频值：800*/
  LL_TIM_SetPrescaler(TIM1,800-1);

  /*设置TIM1为从模式触发模式*/
  LL_TIM_SetSlaveMode(TIM1,LL_TIM_SLAVEMODE_TRIGGER);
  
  /*设置输入触发源为TI2FP2*/
  LL_TIM_SetTriggerInput(TIM1,LL_TIM_TS_TI2FP2);
  
  /*触发/输出通道配置*/
  APP_ConfigTIM1Channel();
  
  /*使能单脉冲模式*/
  LL_TIM_SetOnePulseMode(TIM1,LL_TIM_ONEPULSEMODE_SINGLE);
  
  /*关闭自动输出*/
  LL_TIM_DisableAutomaticOutput(TIM1);
  /*主输出使能*/
  LL_TIM_EnableAllOutputs(TIM1);
  
  /*开启TIM1计数器*/
  LL_TIM_EnableCounter(TIM1);
}

/*******************************************************************************
**功能描述 ：系统时钟配置函数
**输入参数 ：
**输出参数 ：
*******************************************************************************/
void SystemClock_Config(void)
{
  /* 使能HSI */
  LL_RCC_HSI_Enable();
  while(LL_RCC_HSI_IsReady() != 1)
  {
  }

  /* 设置 AHB 分频*/
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

  /* 配置HSISYS作为系统时钟源 */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSISYS);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSISYS)
  {
  }

  /* 设置 APB1 分频*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_Init1msTick(8000000);
  
  /* 更新系统时钟全局变量SystemCoreClock(也可以通过调用SystemCoreClockUpdate函数更新) */
  LL_SetSystemCoreClock(8000000);
}

/*******************************************************************************
**功能描述 ：错误执行函数
**输入参数 ：
**输出参数 ：
*******************************************************************************/
void Error_Handler(void)
{
  while(1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
/*******************************************************************************
**功能描述 ：输出产生断言错误的源文件名及行号
**输入参数 ：file：源文件名指针
**输入参数 ：line：发生断言错误的行号
**输出参数 ：
*******************************************************************************/
void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT Puya *****END OF FILE****/
