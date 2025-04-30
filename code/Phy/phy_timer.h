#ifndef __BSP_TIMER_H
#define __BSP_TIMER_H

#define ENABLE_INT() __set_PRIMASK(0)	 /* ʹ��ȫ���ж� */
#define DISABLE_INT() __set_PRIMASK(1) /* ��ֹȫ���ж� */

#define TMR_BASE_HZ 1000

#define TMR_COUNT 4 /* �����ʱ���ĸ��� ����ʱ��ID��Χ 0 - 3) */

/* ��ʱ���ṹ�壬��Ա���������� volatile, ����C�������Ż�ʱ���������� */
typedef enum
{
	TMR_ONCE_MODE = 0, /* һ�ι���ģʽ */
	TMR_AUTO_MODE = 1	 /* �Զ���ʱ����ģʽ */
} TMR_MODE_E;
/* ��ʱ���ṹ�壬��Ա���������� volatile, ����C�������Ż�ʱ���������� */
typedef struct
{
	volatile uint8_t Mode;		 /* ������ģʽ��1���� */
	volatile uint8_t Flag;		 /* ��ʱ�����־  */
	volatile uint32_t Count;	 /* ������ */
	volatile uint32_t PreLoad; /* ������Ԥװֵ */
} SOFT_TMR;

void bsp_timer_init(void);
void bsp_delay_ms(uint32_t n);
void bsp_delay_us(uint32_t n);
void bsp_start_timer(uint8_t id, uint32_t period);
void bsp_start_auto_timer(uint8_t id, uint32_t period);
void bsp_stop_timer(uint8_t _id);
uint8_t bsp_check_timer(uint8_t id);
int32_t bsp_get_run_time(void);
int32_t bsp_check_run_time(int32_t last_timer);

#endif
