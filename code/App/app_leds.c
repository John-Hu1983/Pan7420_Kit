#include "../Inc/includes.h"

static event_state schedule = {
    .words = 0,
};

led_ctr lamps[2];

/*
 * @description	:   about 0.1us
 * @param		:
 * @return		:
 */
enum
{
    LED_step_init = 0,
};
void task_leds_display(osvar_t ms)
{
    schedule.bits.debounce += ms;
    switch (schedule.bits.step)
    {
    case LED_step_init:
        memset((void *)lamps, 0, sizeof(lamps));
        init_leds_object();
        break;
    }
}
