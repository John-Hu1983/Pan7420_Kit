#include "../Inc/includes.h"

const gpio_obj LED1 = {
    .port = GPIOB,
    .pin = 0,
};
const gpio_obj LED2 = {
    .port = GPIOB,
    .pin = 1,
};

/*
 * @description	:
 * @param		:
 * @return		:
 */
void init_leds_object(void)
{
  configure_gpio_pattern(LED1.port, LED1.pin,
                         LL_GPIO_MODE_OUTPUT,
                         LL_GPIO_SPEED_FREQ_HIGH,
                         LL_GPIO_OUTPUT_OPENDRAIN,
                         LL_GPIO_PULL_NO,
                         LL_GPIO_AF_0);

  configure_gpio_pattern(LED2.port, LED2.pin,
                         LL_GPIO_MODE_OUTPUT,
                         LL_GPIO_SPEED_FREQ_HIGH,
                         LL_GPIO_OUTPUT_OPENDRAIN,
                         LL_GPIO_PULL_NO,
                         LL_GPIO_AF_0);
}

/*
 * @description	:
 * @param		:
 * @return		:
 */
void control_leds_condition(u8 cn, bool onoff)
{
  gpio_obj *io;

  io = (cn == 0) ? (gpio_obj *)&LED1 : (gpio_obj *)&LED2;
  gpio_write_level(io->port, io->pin, onoff);
}