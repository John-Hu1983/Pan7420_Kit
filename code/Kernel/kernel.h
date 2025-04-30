#ifndef _KERNEL_H_
#define _KERNEL_H_

typedef union
{
  u16 words;
  struct
  {
    unsigned debounce : 8;
    unsigned step : 8;
  } bits;
} event_state;

typedef struct
{
  osvar_t counter;
  const osvar_t set;
  void (*func)(osvar_t);
} task_t;

void task_idle_process(osvar_t ms);
void Callback_os_timer(void);
void open_app_process(void);
#endif
