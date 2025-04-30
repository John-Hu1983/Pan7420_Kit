#include "../Inc/includes.h"

task_t APPS[] = {
    {0, 0, task_idle_process},
    {5, 5, task_OLED_screen},
    {10, 10, task_leds_display},
};
const u16 TASK_SIZE = sizeof(APPS) / sizeof(APPS[0]);

/*
 * argument in  :   none
 * argument out :
 * description  :
 */
void task_idle_process(osvar_t ms)
{
}

/*
 * argument in  :   none
 * argument out :
 * description  :
 */
void Callback_os_timer(void)
{
  u16 inx;
  for (inx = 0; inx < TASK_SIZE; inx++)
  {
    if (APPS[inx].counter)
    {
      APPS[inx].counter--;
    }
  }
}

/*
 * argument in  :   none
 * argument out :
 * description  :
 */
void open_app_process(void)
{
  static u16 tick_weak = 0;
  u16 inx;
  while (1)
  {
    if (is_systick_opened())
    {
      tick_weak = 0;
    }
    else
    {
      delay_us(1);
      if (++tick_weak >= 1000)
      {
        tick_weak = 0;
        cb_systick_1ms();
      }
    }

    for (inx = 0; inx < TASK_SIZE; inx++)
    {
      if (APPS[inx].counter == 0)
      {
        APPS[inx].counter = APPS[inx].set;
        APPS[inx].func(APPS[inx].counter);
      }
    }
  }
}
