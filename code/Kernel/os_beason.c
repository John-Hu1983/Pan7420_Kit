#include "../Inc/includes.h"

volatile struct soft_t *tim_head = NULL;
kernel_var rt = {0};
/**
 * @brief
 * @note
 * @param
 * @retval
 */
void init_soft_link(void)
{
  tim_head = NULL;
}

/*
 * Argument in  :
 * Argument out :
 * Description  :
 */
#define SYS_CLOCK_HZ 48000000
#define TICKS_PER_MS (SYS_CLOCK_HZ / 1000)
void open_systick_irq(void)
{
  // Set reload register
  SysTick->LOAD = (TICKS_PER_MS & SysTick_LOAD_RELOAD_Msk) - 1;

  // Set the priority of the SysTick interrupt (optional)
  NVIC_SetPriority(SysTick_IRQn, (1 << __NVIC_PRIO_BITS) - 1);

  // Reset the SysTick counter value
  SysTick->VAL = 0;

  // Enable SysTick, SysTick interrupt, and select processor clock
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

/*
 * @description	:
 * @param		:
 * @return		:
 */
bool is_systick_opened(void)
{
  return (SysTick->CTRL & SysTick_CTRL_TICKINT_Msk) ? true : false;
}

/*
 * Argument in  :
 * Argument out :
 * Description  :
 */
void close_systick_irq(void)
{
  SysTick->VAL = 0;
  SysTick->CTRL = 0;
}

/*
 * argument in	:	none
 * argument out	:	none
 * descrition	:
 */
void create_soft_timer(struct soft_t *node, Timer_Pat_t pat)
{
  node->timer.member.pat = pat;
  node->timer.member.ms = 0;
  if (tim_head == NULL)
  {
    tim_head = node;
    tim_head->next = NULL;
  }
  else
  {
    node->next = tim_head->next;
    tim_head->next = node;
  }
}

/*
 * argument in	:	none
 * argument out	:	none
 * descrition	:
 */
void cb_soft_timer(void)
{
  struct soft_t *node;
  node = (struct soft_t *)tim_head;

  while (node != NULL)
  {
    if (node->timer.member.pat == Soft_Tp_Increase)
    {
      if (node->timer.member.ms < SOFTTIME_COUNT_MAX)
      {
        node->timer.member.ms++;
      }
    }
    else
    {
      if (node->timer.member.ms > SOFTTIME_COUNT_MIN)
      {
        node->timer.member.ms--;
      }
    }

    node = node->next;
  }
}

/*
 * Argument in  :
 * Argument out :
 * Description  : 1ms
 */
void cb_systick_1ms(void)
{
  rt.bits.ms_tick++;
  if (rt.bits.ms_tick >= 1)
  {
    rt.bits.ms_tick = 0;

    if (rt.bits.ms_counter)
    {
      rt.bits.ms_counter--;
    }

    Callback_os_timer();
    cb_soft_timer();
  }
}

/*
 * Argument in  :
 * Argument out :
 * Description  :
 */
void delay_us(u32 us)
{
  u32 i;
  for (i = 0; i < us; i++)
  {
    WatchdogClear();
    __delay_pan742(100);
  }
}

/*
 * Argument in  :
 * Argument out :
 * Description  :
 */
void delay_ms(u16 ms)
{
  if (is_systick_opened())
  {
    rt.bits.ms_counter = ms;
    while (rt.bits.ms_counter)
    {
      WatchdogClear();
    }
  }
  else
  {
    delay_us(1000 * ms);
  }
}
