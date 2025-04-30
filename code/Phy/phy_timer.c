#include "../Inc/includes.h"

/* ��2��ȫ�ֱ���ת���� bsp_DelayMS() ���� */
static volatile uint32_t s_ui_delay_count = 0;
static volatile uint8_t s_uc_timeout_flag = 0;

/* ���������ʱ���ṹ����� */
static SOFT_TMR s_t_tmr[TMR_COUNT] = {0};

/*
	ȫ������ʱ�䣬��λ1ms
	����Ա�ʾ 24.85�죬�����Ĳ�Ʒ��������ʱ�䳬�������������뿼���������
*/
__IO int32_t g_i_run_time = 0;

static __IO uint8_t g_uc_enable_systick_isr = 0; /* �ȴ�������ʼ�� */

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

	/*����UPDATA�ж�����*/
	NVIC_EnableIRQ(TIM14_IRQn);
}

/*
 * @description		: ����systick�жϣ�����ʼ�������ʱ������
 * @param        	: ��
 * @return			: ��
 */
void bsp_timer_init(void)
{
	uint8_t i;

	/* �������е������ʱ�� */
	for (i = 0; i < TMR_COUNT; i++)
	{
		s_t_tmr[i].Count = 0;
		s_t_tmr[i].PreLoad = 0;
		s_t_tmr[i].Flag = 0;
		s_t_tmr[i].Mode = TMR_ONCE_MODE; /* ȱʡ��1���Թ���ģʽ */
	}

	// SysTick_Config(SystemCoreClock / 1000);

	__bsp_timer_init();

	g_uc_enable_systick_isr = 1; /* 1��ʾִ��systick�ж� */
}

/*
 * @description	: SysTick�жϷ������ÿ��1ms����1��
 * @param 		: ��
 * @return		: ��
 */
void SysTick_ISR(void)
{
	static uint8_t s_count = 0;
	uint8_t i;

	/* ÿ��1ms����1�� �������� bsp_DelayMS�� */
	if (s_ui_delay_count > 0)
	{
		if (--s_ui_delay_count == 0)
		{
			s_uc_timeout_flag = 1;
		}
	}

	/* ÿ��1ms���������ʱ���ļ��������м�һ���� */
	for (i = 0; i < TMR_COUNT; i++)
	{
		bsp_soft_timer_dec(&s_t_tmr[i]);
	}

	/* ȫ������ʱ��ÿ1ms��1 */
	g_i_run_time++;
	if (g_i_run_time == 0x7FFFFFFF) /* ��������� int32_t ���ͣ������Ϊ 0x7FFFFFFF */
	{
		g_i_run_time = 0;
	}

	// bsp_run_per_1ms();		/* ÿ��1ms����һ�δ˺������˺����� bsp.c */

	if (++s_count >= 10)
	{
		s_count = 0;

		// bsp_run_per_10ms();	/* ÿ��10ms����һ�δ˺������˺����� bsp.c */
	}
}
/*
 * @description	: ÿ��1ms�����ж�ʱ��������1�����뱻SysTick_ISR�����Ե��á�
 * @param - tmr	: ��ʱ������ָ��
 * @return		: ��
 */
static void bsp_soft_timer_dec(SOFT_TMR *tmr)
{
	if (tmr->Count > 0)
	{
		/* �����ʱ����������1�����ö�ʱ�������־ */
		if (--tmr->Count == 0)
		{
			tmr->Flag = 1;

			/* ������Զ�ģʽ�����Զ���װ������ */
			if (tmr->Mode == TMR_AUTO_MODE)
			{
				tmr->Count = tmr->PreLoad;
			}
		}
	}
}

/*
 * @description		: ����һ����ʱ���������ö�ʱ���ڡ�
 * @param - _id	    : ��ʱ��ID��ֵ��0,TMR_COUNT-1�����û���������ά����ʱ��ID���Ա��ⶨʱ��ID��ͻ
 *          _period : ��ʱ���ڣ���λ1ms
 * @return			: ��
 */
void bsp_start_timer(uint8_t _id, uint32_t _period)
{
	if (_id >= TMR_COUNT)
	{
		while (1)
			; /* �����쳣�������ȴ����Ź���λ */
	}

	DISABLE_INT(); /* ���ж� */

	s_t_tmr[_id].Count = _period;			 /* ʵʱ��������ֵ */
	s_t_tmr[_id].PreLoad = _period;		 /* �������Զ���װֵ�����Զ�ģʽ������ */
	s_t_tmr[_id].Flag = 0;						 /* ��ʱʱ�䵽��־ */
	s_t_tmr[_id].Mode = TMR_ONCE_MODE; /* 1���Թ���ģʽ */

	ENABLE_INT(); /* ���ж� */
}
/*
 * @description		: ����һ���Զ���ʱ���������ö�ʱ���ڡ�
 * @param - _id	    : ��ʱ��ID��ֵ��0,TMR_COUNT-1�����û���������ά����ʱ��ID���Ա��ⶨʱ��ID��ͻ
 *          _period : ��ʱ���ڣ���λ10ms
 * @return			: ��
 */
void bsp_start_auto_timer(uint8_t _id, uint32_t _period)
{
	if (_id >= TMR_COUNT)
	{
		while (1)
			; /* �����쳣�������ȴ����Ź���λ */
	}

	DISABLE_INT(); /* ���ж� */

	s_t_tmr[_id].Count = _period;			 /* ʵʱ��������ֵ */
	s_t_tmr[_id].PreLoad = _period;		 /* �������Զ���װֵ�����Զ�ģʽ������ */
	s_t_tmr[_id].Flag = 0;						 /* ��ʱʱ�䵽��־ */
	s_t_tmr[_id].Mode = TMR_AUTO_MODE; /* �Զ�����ģʽ */

	ENABLE_INT(); /* ���ж� */
}
/*
 * @description		: ֹͣһ����ʱ��
 * @param - _id	    : ��ʱ��ID��ֵ��0,TMR_COUNT-1�����û���������ά����ʱ��ID���Ա��ⶨʱ��ID��ͻ
 * @return			: ��
 */
void bsp_stop_timer(uint8_t _id)
{
	if (_id >= TMR_COUNT)
	{
		while (1)
			; /* �����쳣�������ȴ����Ź���λ */
	}

	DISABLE_INT(); /* ���ж� */

	s_t_tmr[_id].Count = 0;						 /* ʵʱ��������ֵ */
	s_t_tmr[_id].Flag = 0;						 /* ��ʱʱ�䵽��־ */
	s_t_tmr[_id].Mode = TMR_ONCE_MODE; /* �Զ�����ģʽ */

	ENABLE_INT(); /* ���ж� */
}
/*
 * @description		: ��ⶨʱ���Ƿ�ʱ
 * @param - _id	    : ��ʱ��ID��ֵ��0,TMR_COUNT-1�����û���������ά����ʱ��ID���Ա��ⶨʱ��ID��ͻ
 *          _period : ��ʱ���ڣ���λ1ms
 * @return			: 0����ʱδ����1����ʱ��
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
 * @description		: ��ȡCPU����ʱ�䣬��λ1ms������Ա�ʾ 24.85��
 * @param			: ��
 * @return			: cpu����ʱ�䣬��λ1ms
 */
int32_t bsp_get_run_time(void)
{
	int32_t runtime;

	DISABLE_INT(); /* ���ж� */

	runtime = g_i_run_time; /* ���������Systick�ж��б���д�������Ҫ���жϽ��б��� */

	ENABLE_INT(); /* ���ж� */

	return runtime;
}
/*
 * @description			: ���㵱ǰ����ʱ��͸���ʱ��֮��Ĳ�ֵ
 * @param - _LastTIime	: �ϸ�ʱ��
 * @return				: ��ǰʱ��͹�ȥʱ��Ĳ�ֵ����λ1ms
 */
int32_t bsp_check_run_Time(int32_t _LastTime)
{
	int32_t now_time;
	int32_t time_diff;

	DISABLE_INT(); /* ���ж� */

	now_time = g_i_run_time; /* ���������Systick�ж��б���д�������Ҫ���жϽ��б��� */

	ENABLE_INT(); /* ���ж� */

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
 * @description	: ϵͳ��શ�ʱ���жϷ������
 * @param 		: ��
 * @return		: ��
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

/***************************** ����΢���� www.panchip.com *********************************/
