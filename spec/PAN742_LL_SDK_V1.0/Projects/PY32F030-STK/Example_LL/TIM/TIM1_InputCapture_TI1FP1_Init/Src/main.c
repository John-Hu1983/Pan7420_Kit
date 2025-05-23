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
static void APP_ConfigInputCapture(void);
void APP_InputcaptureCallback(void);

/*******************************************************************************
**功能描述 ：执行函数
**输入参数 ：
**输出参数 ：
*******************************************************************************/
int main(void)
{
  /*使能TIM1时钟*/
  LL_APB1_GRP2_EnableClock(RCC_APBENR2_TIM1EN);
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);

  /*配置系统时钟*/
  SystemClock_Config();
  
  /*初始化LED*/
  BSP_LED_Init(LED3);
  
  /*配置捕获通道TI1*/
  APP_ConfigInputCapture();
  
  /*配置并开启TIM1计数模式*/
  APP_ConfigTIM1Count();
  
  while (1)
  {
  }
}

/*******************************************************************************
**功能描述 ：配置TIM1输入捕获通道
**输入参数 ：
**输出参数 ：
*******************************************************************************/
void APP_ConfigInputCapture(void)
{
  LL_TIM_IC_InitTypeDef InputCaptureInit ={0};
  LL_GPIO_InitTypeDef TIM1CH1MapInit= {0};
  
  /*配置PA3为捕获输入引脚*/
  TIM1CH1MapInit.Pin        = LL_GPIO_PIN_3;
  TIM1CH1MapInit.Mode       = LL_GPIO_MODE_ALTERNATE;
  TIM1CH1MapInit.Alternate  = LL_GPIO_AF_13;
  
  LL_GPIO_Init(GPIOA,&TIM1CH1MapInit);
  
  /*配置捕获通道*/
  InputCaptureInit.ICActiveInput  = LL_TIM_ACTIVEINPUT_DIRECTTI;
  InputCaptureInit.ICPrescaler    = LL_TIM_ICPSC_DIV1;
  InputCaptureInit.ICPolarity     = LL_TIM_IC_POLARITY_RISING;
  InputCaptureInit.ICFilter       = LL_TIM_IC_FILTER_FDIV1;
  
  LL_TIM_IC_Init(TIM1,LL_TIM_CHANNEL_CH1,&InputCaptureInit);
  
  /*使能通道1捕获中断*/
  LL_TIM_EnableIT_CC1(TIM1);
  
  /*使能TIM1捕获中断请求*/
  NVIC_EnableIRQ(TIM1_CC_IRQn);
  NVIC_SetPriority(TIM1_CC_IRQn,0);
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
  ** 输入时钟：    8000000 
  ** 计数模式：    向上计数
  ** 时钟预分频：  8000 
  ** 自动重装载值：1000
  ** 重复计数值：  0
  ************************************************/
  TIM1CountInit.ClockDivision       = LL_TIM_CLOCKDIVISION_DIV1;
  TIM1CountInit.CounterMode         = LL_TIM_COUNTERMODE_UP;
  TIM1CountInit.Prescaler           = 8000-1;
  TIM1CountInit.Autoreload          = 1000-1;
  TIM1CountInit.RepetitionCounter   = 0;
  
  /*初始化TIM1*/
  LL_TIM_Init(TIM1,&TIM1CountInit);
  
  /*使能TIM1计数器*/
  LL_TIM_EnableCounter(TIM1);
}

/*******************************************************************************
**功能描述 ：TIM输入捕获中断回调函数
**输入参数 ：
**输出参数 ：
*******************************************************************************/
void APP_InputcaptureCallback(void)
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
