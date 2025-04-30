#include "../Inc/includes.h"

/*
 * @description		:
 * @param        	:   port -- GPIOA GPIOB GPIOC
 *
 *                    pin  -- 0 ~ 31
 *
 *                    mode -- LL_GPIO_MODE_INPUT
 *                            LL_GPIO_MODE_OUTPUT
 *                            LL_GPIO_MODE_ALTERNATE
 *                            LL_GPIO_MODE_ANALOG
 *
 *                    speed --LL_GPIO_SPEED_FREQ_LOW
 *                            LL_GPIO_SPEED_FREQ_MEDIUM
 *                            LL_GPIO_SPEED_FREQ_HIGH
 *                            LL_GPIO_SPEED_FREQ_VERY_HIGH
 *
 *                    outtype LL_GPIO_OUTPUT_PUSHPULL
 *                            LL_GPIO_OUTPUT_OPENDRAIN
 *
 *                    pull -- LL_GPIO_PULL_NO
 *                            LL_GPIO_PULL_UP
 *                            LL_GPIO_PULL_DOWN
 *
 *                    alternate  LL_GPIO_AF0_SWJ ... too much
 * @return			  :
 */
void configure_gpio_pattern(GPIO_TypeDef *port,
                            uint8_t pin,
                            uint32_t mode,
                            uint32_t speed,
                            uint32_t outtype,
                            uint32_t pull,
                            uint32_t alternate)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct;
  if (port == GPIOA)
  {
    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
  }
  else if (port == GPIOB)
  {
    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
  }
  else if (port == GPIOF)
  {
    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOF);
  }
  else
  {
    return;
  }

  GPIO_InitStruct.Pin = 0x1UL << pin;
  GPIO_InitStruct.Mode = mode;
  GPIO_InitStruct.Speed = speed;
  GPIO_InitStruct.OutputType = outtype;
  GPIO_InitStruct.Pull = pull;
  GPIO_InitStruct.Alternate = alternate;
  LL_GPIO_Init(port, &GPIO_InitStruct);
}

/*
 * @description	:
 * @param		:
 * @return		:
 */
bool gpio_read_level(GPIO_TypeDef *port, uint8_t pin)
{
  return (LL_GPIO_ReadInputPort(port) & (0x1UL << pin)) ? true : false;
}

/*
 * @description	:
 * @param		:
 * @return		:
 */
void gpio_write_level(GPIO_TypeDef *port, uint8_t pin, bool lev)
{
  if (lev)
  {
    LL_GPIO_SetOutputPin(port, 0x1UL << pin);
  }
  else
  {
    LL_GPIO_ResetOutputPin(port, 0x1UL << pin);
  }
}

/*
 * @description	:
 * @param		:
 * @return		:
 */
void gpio_invert_level(GPIO_TypeDef *port, uint8_t pin)
{
  LL_GPIO_TogglePin(port, 0x1UL << pin);
}