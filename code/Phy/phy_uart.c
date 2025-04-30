#include "../Inc/includes.h"

LL_USART_InitTypeDef USART_InitStruct;

#define UARTx USART1
#define BAUDRATE 115200
#define UARTx_TX_PORT GPIOB
#define UARTx_TX_PIN LL_GPIO_PIN_8
#define UARTx_RX_PORT GPIOB
#define UARTx_RX_PIN LL_GPIO_PIN_2
#define UARTx_TX_ALTERNATE_AFn LL_GPIO_AF8_USART1
#define UARTx_RX_ALTERNATE_AFn LL_GPIO_AF0_USART1
#define UARTx_IRQn USART1_IRQn

#define LENGTH 10

static void __bsp_uart_init(void)
{

  /*使能USART1时钟*/
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_USART1);

  /* USART Init */
  LL_USART_SetBaudRate(UARTx, SystemCoreClock, LL_USART_OVERSAMPLING_16, BAUDRATE);
  LL_USART_SetDataWidth(UARTx, LL_USART_DATAWIDTH_8B);
  LL_USART_SetStopBitsLength(UARTx, LL_USART_STOPBITS_1);
  LL_USART_SetParity(UARTx, LL_USART_PARITY_NONE);
  LL_USART_SetHWFlowCtrl(UARTx, LL_USART_HWCONTROL_NONE);
  LL_USART_SetTransferDirection(UARTx, LL_USART_DIRECTION_TX_RX);
  LL_USART_Enable(UARTx);
  LL_USART_ClearFlag_TC(UARTx);
}

static void __bsp_uart_gpio_init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct;

  /*使能GPIOA时钟*/
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
  /**if UARTx is UART0
  GPIO Configuration:
  PB2     ------> USART1_RXD
  PB8     ------> USART1_TXD
  */
  GPIO_InitStruct.Pin = UARTx_RX_PIN;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = UARTx_RX_ALTERNATE_AFn;
  LL_GPIO_Init(UARTx_RX_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = UARTx_TX_PIN;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Alternate = UARTx_TX_ALTERNATE_AFn;
  LL_GPIO_Init(UARTx_TX_PORT, &GPIO_InitStruct);
}

void bsp_uart_init(void)
{
  __bsp_uart_gpio_init();
  __bsp_uart_init();
}

void bsp_uart_send(uint8_t data)
{
  LL_USART_TransmitData8(UARTx, data);
  while (!LL_USART_IsActiveFlag_TC(UARTx))
    ;
  LL_USART_ClearFlag_TC(UARTx);
}

uint8_t bsp_uart_get_byte(uint8_t *data)
{
  if (LL_USART_IsActiveFlag_RXNE(UARTx))
  {
    *data = LL_USART_ReceiveData8(UARTx);
    return SUCCESS;
  }
  return ERROR;
}

int fputc(int ch, FILE *f)
{
  //  /* Send a byte to USART */
  LL_USART_TransmitData8(UARTx, ch);
  while (!LL_USART_IsActiveFlag_TC(UARTx))
    ;
  LL_USART_ClearFlag_TC(UARTx);

  return (ch);
}
