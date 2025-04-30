#include "../Inc/includes.h"

static event_state schedule = {
    .words = 0,
};

led_ctr lamps[2];

/*
 * @description	:
 * @param		:
 * @return		:
 */
void set_led_patt(u8 cn, LED_Patt_t pat, u16 argu)
{
    led_ctr *led;

    led = (cn == 0) ? &lamps[0] : &lamps[1];
    if (led->patt == pat && led->argument == argu)
    {
        return;
    }
    memset((void *)led, 0, sizeof(led));
    led->patt = pat;
    led->argument = argu;
}

/*
 * @description	:
 * @param		:
 * @return		:
 */
void perform_led_display(u8 cn, osvar_t ms)
{
    led_ctr *led;

    led = (cn == 0) ? &lamps[0] : &lamps[1];
    led->ms += ms;
    switch (led->step)
    {
    case 0:
        if (led->patt == led_patt_on)
        {
            led->step = 10;
        }
        else if (led->patt == led_patt_flicker)
        {
            led->step = 20;
        }
        else
        {
            led->step = 0xff;
        }
        break;

    case 10:
        control_leds_condition(cn, LED_Lev_on);
        break;

    case 20:
        if (led->argument == 0)
        {
            led->step = 0xff;
        }
        else if (led->argument == 0xffff)
        {
            led->ms = 0;
            led->step++;
        }
        else
        {
            led->ms = 0;
            led->argument--;
            led->step++;
        }
        break;

    case 21:
        control_leds_condition(cn, LED_Lev_on);
        if (led->ms >= 200)
        {
            led->ms = 0;
            led->step++;
        }
        break;

    case 22:
        control_leds_condition(cn, LED_Lev_off);
        if (led->ms >= 200)
        {
            led->ms = 0;
            led->step = 20;
        }
        break;

    default:
        control_leds_condition(cn, LED_Lev_off);
        break;
    }
}

/*
 * @description	:
 * @param		:
 * @return		:
 */
enum
{
    LED_step_init = 0,
    LED_step_loop,
};
void task_leds_display(osvar_t ms)
{
    schedule.bits.debounce += ms;
    switch (schedule.bits.step)
    {
    case LED_step_init:
        memset((void *)lamps, 0, sizeof(lamps));
        init_leds_object();

        set_led_patt(0, led_patt_on, 0xffff);
        set_led_patt(1, led_patt_flicker, 0xffff);

        schedule.bits.step = LED_step_loop;
        break;

    case LED_step_loop:
        perform_led_display(0, ms);
        perform_led_display(1, ms);
        break;
    }
}
