#ifndef _APP_LEDS_H_
#define _APP_LEDS_H_

typedef enum
{
  LED_Lev_on = 0,
  LED_Lev_off,
} LED_LEV_t;

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
  u16 ms;
  u8 step;
} led_ctr;

void task_leds_display(osvar_t ms);
void set_led_patt(u8 cn, LED_Patt_t pat, u16 argu);
#endif
