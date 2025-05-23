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
static void APP_ConfigTIM1Count(void);
static void APP_ConfigTIM1ExternalClock(void);
void APP_UpdateCallback(void);

/*******************************************************************************
**功能描述 ：执行函数
**输入参数 ：
**输出参数 ：
*******************************************************************************/
int main(void)
{
  /*使能TIM1时钟*/
  LL_APB1_GRP2_EnableClock(RCC_APBENR2_TIM1EN);

  /*配置系统时钟*/
  SystemClock_Config();
  
  /*初始化LED*/
  BSP_LED_Init(LED3);
  
  /*配置外部时钟模式1*/
  APP_ConfigTIM1ExternalClock();
  
  /*配置并开启TIM1计数模式*/
  APP_ConfigTIM1Count();
  
  while (1)
  {
  }
}

/*******************************************************************************
**功能描述 ：配置TIM外部时钟输入
**输入参数 ：
**输出参数 ：
*******************************************************************************/
void APP_ConfigTIM1ExternalClock(void)
{
  LL_TIM_IC_InitTypeDef CH1ICInit = {0};
  
  /*配置PA8引脚为TI1输入*/
  LL_GPIO_InitTypeDef ETRGPIOinit={0};
  
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
  
  ETRGPIOinit.Pin        = LL_GPIO_PIN_8;
  ETRGPIOinit.Mode       = LL_GPIO_MODE_ALTERNATE;
  ETRGPIOinit.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  ETRGPIOinit.Pull       = LL_GPIO_PULL_UP;
  ETRGPIOinit.Alternate  = LL_GPIO_AF_2;
  
  LL_GPIO_Init(GPIOA,&ETRGPIOinit);
  
  /*配置TIM1外部时钟源模式1*/
  LL_TIM_SetClockSource(TIM1,LL_TIM_CLOCKSOURCE_EXT_MODE1);
  
  /*配置触发选择为TI1F_ED*/
  LL_TIM_SetTriggerInput(TIM1,LL_TIM_TS_TI1F_ED);
  
  /*配置TI1F_ED*/
  /***********************************************
  ** 输入极性：  上升沿有效 
  ** 输入滤波：  无滤波
  ************************************************/
  CH1ICInit.ICPolarity    = LL_TIM_IC_POLARITY_RISING;
  CH1ICInit.ICFilter      = LL_TIM_IC_FILTER_FDIV1;
  
  LL_TIM_IC_Init(TIM1,LL_TIM_CHANNEL_CH1, &CH1ICInit);
}

/*******************************************************************************
**功能描述 ：配置TIM计数模式
**输入参数 ：
**输出参数 ：
*******************************************************************************/
void APP_ConfigTIM1Count(void)
{
  /*配置TIM1*/
  LL_TIM_InitTypeDef TIM1CountInit = {0};
  
  /***********************************************
  ** 计数模式：    向上计数
  ** 时钟预分频：  1000 
  ** 自动重装载值：500 
  ** 重复计数值：  0
  ************************************************/
  TIM1CountInit.ClockDivision       = LL_TIM_CLOCKDIVISION_DIV1;
  TIM1CountInit.CounterMode         = LL_TIM_COUNTERMODE_UP;
  TIM1CountInit.Prescaler           = 1000-1;
  TIM1CountInit.Autoreload          = 500-1;
  TIM1CountInit.RepetitionCounter   = 0;
  
  /*初始化TIM1*/
  LL_TIM_Init(TIM1,&TIM1CountInit);
  
  /*使能UPDATE中断*/
  LL_TIM_EnableIT_UPDATE(TIM1);
  
  /*使能TIM1计数器*/
  LL_TIM_EnableCounter(TIM1);
  
  /*开启UPDATE中断请求*/
  NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);
  NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn,0);
}

/*******************************************************************************
**功能描述 ：TIM更新中断回调函数
**输入参数 ：
**输出参数 ：
*******************************************************************************/
void APP_UpdateCallback(void)
{
  /*翻转LED*/
  BSP_LED_Toggle(LED3);
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
