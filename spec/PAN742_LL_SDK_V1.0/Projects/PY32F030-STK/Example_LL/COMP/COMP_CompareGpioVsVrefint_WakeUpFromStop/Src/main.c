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
void SystemClock_Config(void);
static void APP_COMPInit(void);
static void APP_EnterStop(void);
static void APP_CompRccInit(void);

/*******************************************************************************
**功能描述 ：执行函数
**输入参数 ：
**输出参数 ：
*******************************************************************************/
int main(void)
{
  /* 配置系统时钟 */
  SystemClock_Config();

  /* 初始化LED */
  BSP_LED_Init(LED_GREEN);
  
  /* 初始化按键BUTTON */
  BSP_PB_Init(BUTTON_KEY,BUTTON_MODE_GPIO);

  /* 初始化调试串口(printf使用) */
  BSP_USART_Config();	
  
  /* 比较器时钟切换 */
  APP_CompRccInit();

  /* 比较器初始化 */
  APP_COMPInit();
  
  /* LED打开 */
  BSP_LED_On(LED_GREEN);
  
  /* 等待用户按下按钮开始进入STOP模式 */
  while(BSP_PB_GetState(BUTTON_KEY) == 1)
  {}
  
  /* LED关闭 */
  BSP_LED_Off(LED_GREEN);
  
  /* 进入STOP模式 */
  APP_EnterStop();

  while (1)
  {
    BSP_LED_Toggle(LED_GREEN);
    
    LL_mDelay(200); 
  }
}

/*******************************************************************************
**功能描述 ：比较器初始化
**输入参数 ：
**输出参数 ：
*******************************************************************************/
void APP_COMPInit(void)
{
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
  /**比较器1的GPIO配置
  PA1   ------> 比较器1的Plus端输入
  */	
  LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_1,LL_GPIO_MODE_ANALOG);
  
  LL_GPIO_SetPinPull(GPIOA,LL_GPIO_PIN_1,LL_GPIO_PULL_NO);
  
  /* 使能比较器1时钟 */
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_COMP1);

  /* 输入Plus端选择PA1 */
  LL_COMP_SetInputPlus(COMP1,LL_COMP_INPUT_PLUS_IO3);
  
  /* 输入Minus端选择VREFINT */
  LL_COMP_SetInputMinus(COMP1,LL_COMP_INPUT_MINUS_VREFINT);
  
  /* 使能Scaler */
  LL_COMP_EnableScaler(COMP1);
  
  /* 迟滞关闭 */
  LL_COMP_SetInputHysteresis(COMP1,LL_COMP_HYSTERESIS_DISABLE);
  
  /* 极性不反向 */
  LL_COMP_SetOutputPolarity(COMP1,LL_COMP_OUTPUTPOL_NONINVERTED);
  
  /* 功耗模式快速 */
  LL_COMP_SetPowerMode(COMP1, LL_COMP_POWERMODE_HIGHSPEED);

  /* 窗口模式不使能 */
  LL_COMP_SetCommonWindowMode(__LL_COMP_COMMON_INSTANCE(COMP1), LL_COMP_WINDOWMODE_DISABLE);

  __IO uint32_t wait_loop_index = 0;
  wait_loop_index = (LL_COMP_DELAY_VOLTAGE_SCALER_STAB_US * (SystemCoreClock / (1000000 * 2)));
  while(wait_loop_index != 0)
  {
    wait_loop_index--;
  }

  /* 使能上升沿中断 */
  LL_EXTI_EnableRisingTrig(LL_EXTI_LINE_17);

  /* 使能中断 */
  LL_EXTI_EnableIT(LL_EXTI_LINE_17);

  NVIC_SetPriority(ADC_COMP_IRQn, 0);
  NVIC_EnableIRQ(ADC_COMP_IRQn);

  /* 使能比较器1 */
  LL_COMP_Enable(COMP1);
  wait_loop_index = ((LL_COMP_DELAY_STARTUP_US / 10UL) * (SystemCoreClock / (100000UL * 2UL)));
  while(wait_loop_index != 0UL)
  {
    wait_loop_index--;
  }
}

/*******************************************************************************
**功能描述 ：比较器时钟切换
**输入参数 ：
**输出参数 ：
*******************************************************************************/
void APP_CompRccInit()
{
  /* 使能LSI */
  LL_RCC_LSI_Enable();
 
  /* 等待LSI稳定 */
  while(LL_RCC_LSI_IsReady() != 1)
  {
  }
 
  /* 使能PWR时钟 */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* 使能DBP */
  LL_PWR_EnableBkUpAccess();

  /* 设置LSI为低速时钟 */
  LL_RCC_LSCO_SetSource(LL_RCC_LSCO_CLKSOURCE_LSI);

  /* 使能低速时钟 */
  LL_RCC_LSCO_Enable();

  /* 设置COMP的时钟源为LSC */
  LL_RCC_SetCOMPClockSource(LL_RCC_COMP1_CLKSOURCE_LSC);
}

/*******************************************************************************
**功能描述 ：进入Stop模式
**输入参数 ：
**输出参数 ：
*******************************************************************************/
void APP_EnterStop(void)
{
  /* 使能PWR时钟 */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
 
  /* 进入DeepSleep模式 */
  LL_LPM_EnableDeepSleep();

  /* 等待中断指令 */
  __WFI();
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
