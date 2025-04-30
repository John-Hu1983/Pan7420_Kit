#ifndef __BSP_TIMER_H
#define __BSP_TIMER_H

#define ENABLE_INT() __set_PRIMASK(0)	 /* 使能全局中断 */
#define DISABLE_INT() __set_PRIMASK(1) /* 禁止全局中断 */

#define TMR_BASE_HZ 1000

#define TMR_COUNT 4 /* 软件定时器的个数 （定时器ID范围 0 - 3) */

/* 定时器结构体，成员变量必须是 volatile, 否则C编译器优化时可能有问题 */
typedef enum
{
	TMR_ONCE_MODE = 0, /* 一次工作模式 */
	TMR_AUTO_MODE = 1	 /* 自动定时工作模式 */
} TMR_MODE_E;
/* 定时器结构体，成员变量必须是 volatile, 否则C编译器优化时可能有问题 */
typedef struct
{
	volatile uint8_t Mode;		 /* 计数器模式，1次性 */
	volatile uint8_t Flag;		 /* 定时到达标志  */
	volatile uint32_t Count;	 /* 计数器 */
	volatile uint32_t PreLoad; /* 计数器预装值 */
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
