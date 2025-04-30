#ifndef _PHY_GPIO_H_
#define _PHY_GPIO_H_

typedef struct
{
  GPIO_TypeDef *port;
  uint8_t pin;
} gpio_obj;

void configure_gpio_pattern(GPIO_TypeDef *port,
                            uint8_t pin,
                            uint32_t mode,
                            uint32_t speed,
                            uint32_t outtype,
                            uint32_t pull,
                            uint32_t alternate);
                            
bool gpio_read_level(GPIO_TypeDef *port, uint8_t pin);
void gpio_write_level(GPIO_TypeDef *port, uint8_t pin, bool lev);
void gpio_invert_level(GPIO_TypeDef *port, uint8_t pin);
#endif
