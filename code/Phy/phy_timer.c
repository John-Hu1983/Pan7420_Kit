#include "../Inc/includes.h"

/* 这2个全局变量转用于 bsp_DelayMS() 函数 */
static volatile uint32_t s_ui_delay_count = 0;
static volatile uint8_t s_uc_timeout_flag = 0;

/* 定于软件定时器结构体变量 */
static SOFT_TMR s_t_tmr[TMR_COUNT] = {0};

/*
	全局运行时间，单位1ms
	最长可以表示 24.85天，如果你的产品连续运行时间超过这个数，则必须考虑溢出问题
*/
__IO int32_t g_i_run_time = 0;

static __IO uint8_t g_uc_enable_systick_isr = 0; /* 等待变量初始化 */

static void bsp_soft_timer_dec(SOFT_TMR *_tmr);

LL_TIM_InitTypeDef sTimxHandle = {0};

static void __bsp_timer_init(void)
{
	uint32_t uwPrescalerValue;
	uwPrescalerValue = (uint32_t)(LL_RCC_GetCOMPClockFreq(LL_RCC_COMP1_CLKSOURCE_PCLK1) / TMR_BASE_HZ) - 1;

	LL_APB1_GRP2_EnableClock(RCC_APBENR2_TIM14EN);

	sTimxHandle.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
	sTimxHandle.CounterMode = LL_TIM_COUNTERMODE_UP;
	sTimxHandle.Prescaler = uwPrescalerValue;
	sTimxHandle.Autoreload = 1;
	sTimxHandle.RepetitionCounter = 0;

	if (LL_TIM_Init(TIM14, &sTimxHandle) != SUCCESS)
	{
		/* Initialization Error */
		// Error_Handler();
	}

	LL_TIM_EnableIT_UPDATE(TIM14);
	LL_TIM_EnableCounter(TIM14);

	/*开启UPDATA中断请求*/
	NVIC_EnableIRQ(TIM14_IRQn);
}

/*
 * @description		: 配置systick中断，并初始化软件定时器变量
 * @param        	: 无
 * @return			: 无
 */
void bsp_timer_init(void)
{
	uint8_t i;

	/* 清零所有的软件定时器 */
	for (i = 0; i < TMR_COUNT; i++)
	{
		s_t_tmr[i].Count = 0;
		s_t_tmr[i].PreLoad = 0;
		s_t_tmr[i].Flag = 0;
		s_t_tmr[i].Mode = TMR_ONCE_MODE; /* 缺省是1次性工作模式 */
	}

	// SysTick_Config(SystemCoreClock / 1000);

	__bsp_timer_init();

	g_uc_enable_systick_isr = 1; /* 1表示执行systick中断 */
}

/*
 * @description	: SysTick中断服务程序，每隔1ms进入1次
 * @param 		: 无
 * @return		: 无
 */
void SysTick_ISR(void)
{
	static uint8_t s_count = 0;
	uint8_t i;

	/* 每隔1ms进来1次 （仅用于 bsp_DelayMS） */
	if (s_ui_delay_count > 0)
	{
		if (--s_ui_delay_count == 0)
		{
			s_uc_timeout_flag = 1;
		}
	}

	/* 每隔1ms，对软件定时器的计数器进行减一操作 */
	for (i = 0; i < TMR_COUNT; i++)
	{
		bsp_soft_timer_dec(&s_t_tmr[i]);
	}

	/* 全局运行时间每1ms增1 */
	g_i_run_time++;
	if (g_i_run_time == 0x7FFFFFFF) /* 这个变量是 int32_t 类型，最大数为 0x7FFFFFFF */
	{
		g_i_run_time = 0;
	}

	// bsp_run_per_1ms();		/* 每隔1ms调用一次此函数，此函数在 bsp.c */

	if (++s_count >= 10)
	{
		s_count = 0;

		// bsp_run_per_10ms();	/* 每隔10ms调用一次此函数，此函数在 bsp.c */
	}
}
/*
 * @description	: 每隔1ms对所有定时器变量减1。必须被SysTick_ISR周期性调用。
 * @param - tmr	: 定时器变量指针
 * @return		: 无
 */
static void bsp_soft_timer_dec(SOFT_TMR *tmr)
{
	if (tmr->Count > 0)
	{
		/* 如果定时器变量减到1则设置定时器到达标志 */
		if (--tmr->Count == 0)
		{
			tmr->Flag = 1;

			/* 如果是自动模式，则自动重装计数器 */
			if (tmr->Mode == TMR_AUTO_MODE)
			{
				tmr->Count = tmr->PreLoad;
			}
		}
	}
}

/*
 * @description		: 启动一个定时器，并设置定时周期。
 * @param - _id	    : 定时器ID，值域【0,TMR_COUNT-1】。用户必须自行维护定时器ID，以避免定时器ID冲突
 *          _period : 定时周期，单位1ms
 * @return			: 无
 */
void bsp_start_timer(uint8_t _id, uint32_t _period)
{
	if (_id >= TMR_COUNT)
	{
		while (1)
			; /* 参数异常，死机等待看门狗复位 */
	}

	DISABLE_INT(); /* 关中断 */

	s_t_tmr[_id].Count = _period;			 /* 实时计数器初值 */
	s_t_tmr[_id].PreLoad = _period;		 /* 计数器自动重装值，仅自动模式起作用 */
	s_t_tmr[_id].Flag = 0;						 /* 定时时间到标志 */
	s_t_tmr[_id].Mode = TMR_ONCE_MODE; /* 1次性工作模式 */

	ENABLE_INT(); /* 开中断 */
}
/*
 * @description		: 启动一个自动定时器，并设置定时周期。
 * @param - _id	    : 定时器ID，值域【0,TMR_COUNT-1】。用户必须自行维护定时器ID，以避免定时器ID冲突
 *          _period : 定时周期，单位10ms
 * @return			: 无
 */
void bsp_start_auto_timer(uint8_t _id, uint32_t _period)
{
	if (_id >= TMR_COUNT)
	{
		while (1)
			; /* 参数异常，死机等待看门狗复位 */
	}

	DISABLE_INT(); /* 关中断 */

	s_t_tmr[_id].Count = _period;			 /* 实时计数器初值 */
	s_t_tmr[_id].PreLoad = _period;		 /* 计数器自动重装值，仅自动模式起作用 */
	s_t_tmr[_id].Flag = 0;						 /* 定时时间到标志 */
	s_t_tmr[_id].Mode = TMR_AUTO_MODE; /* 自动工作模式 */

	ENABLE_INT(); /* 开中断 */
}
/*
 * @description		: 停止一个定时器
 * @param - _id	    : 定时器ID，值域【0,TMR_COUNT-1】。用户必须自行维护定时器ID，以避免定时器ID冲突
 * @return			: 无
 */
void bsp_stop_timer(uint8_t _id)
{
	if (_id >= TMR_COUNT)
	{
		while (1)
			; /* 参数异常，死机等待看门狗复位 */
	}

	DISABLE_INT(); /* 关中断 */

	s_t_tmr[_id].Count = 0;						 /* 实时计数器初值 */
	s_t_tmr[_id].Flag = 0;						 /* 定时时间到标志 */
	s_t_tmr[_id].Mode = TMR_ONCE_MODE; /* 自动工作模式 */

	ENABLE_INT(); /* 开中断 */
}
/*
 * @description		: 检测定时器是否超时
 * @param - _id	    : 定时器ID，值域【0,TMR_COUNT-1】。用户必须自行维护定时器ID，以避免定时器ID冲突
 *          _period : 定时周期，单位1ms
 * @return			: 0：定时未到，1：定时到
 */
uint8_t bsp_check_timer(uint8_t _id)
{
	if (_id >= TMR_COUNT)
	{
		return 0;
	}

	if (s_t_tmr[_id].Flag == 1)
	{
		s_t_tmr[_id].Flag = 0;
		return 1;
	}
	else
	{
		return 0;
	}
}
/*
 * @description		: 获取CPU运行时间，单位1ms。最长可以表示 24.85天
 * @param			: 无
 * @return			: cpu运行时间，单位1ms
 */
int32_t bsp_get_run_time(void)
{
	int32_t runtime;

	DISABLE_INT(); /* 关中断 */

	runtime = g_i_run_time; /* 这个变量在Systick中断中被改写，因此需要关中断进行保护 */

	ENABLE_INT(); /* 开中断 */

	return runtime;
}
/*
 * @description			: 计算当前运行时间和给定时刻之间的差值
 * @param - _LastTIime	: 上个时刻
 * @return				: 当前时间和过去时间的差值，单位1ms
 */
int32_t bsp_check_run_Time(int32_t _LastTime)
{
	int32_t now_time;
	int32_t time_diff;

	DISABLE_INT(); /* 关中断 */

	now_time = g_i_run_time; /* 这个变量在Systick中断中被改写，因此需要关中断进行保护 */

	ENABLE_INT(); /* 开中断 */

	if (now_time >= _LastTime)
	{
		time_diff = now_time - _LastTime;
	}
	else
	{
		time_diff = 0x7FFFFFFF - _LastTime + now_time;
	}

	return time_diff;
}

/*
 * @description	: 系统嘀嗒定时器中断服务程序
 * @param 		: 无
 * @return		: 无
 */
void TIM14_UpdateCallback(void)
{
	// BSP_LED_Toggle(LED_GREEN);
	if (g_uc_enable_systick_isr == 0)
	{
		return;
	}

	SysTick_ISR();
}

/***************************** 磐启微电子 www.panchip.com *********************************/
