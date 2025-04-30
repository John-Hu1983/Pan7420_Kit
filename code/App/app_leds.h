#ifndef _APP_LEDS_H_
#define _APP_LEDS_H_

typedef enum
{
  led_patt_off = 0,
  led_patt_on,
  led_patt_flicker,
} LED_Patt_t;

typedef struct
{
  LED_Patt_t patt;
  u16 argument;
} led_ctr;

void task_leds_display(osvar_t ms);
#endif
