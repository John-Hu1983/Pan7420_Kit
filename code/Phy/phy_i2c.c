#include "../Inc/includes.h"

uint8_t *pBuffPtr = NULL;
__IO uint16_t XferCount = 0;
__IO uint32_t Devaddress = 0;
__IO uint32_t State = I2C_STATE_READY;

/*******************************************************************************
**�������� ��I2C���ú���
**������� ��
**������� ��
*******************************************************************************/
void APP_ConfigI2cMaster(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);

  /* �� SCL ��������Ϊ����ѡ���ܡ����١���©������ */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_6;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_6;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* �� SDA ��������Ϊ����ѡ���ܡ����١���©������ */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_7;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_6;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* ��λI2C */
  LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_I2C1);
  LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_I2C1);

  /* I2C��ʼ�� */
  LL_I2C_InitTypeDef I2C_InitStruct;
  I2C_InitStruct.ClockSpeed = I2C_SPEEDCLOCK;
  I2C_InitStruct.DutyCycle = LL_I2C_DUTYCYCLE_16_9;
  I2C_InitStruct.OwnAddress1 = I2C_ADDRESS;
  I2C_InitStruct.TypeAcknowledge = LL_I2C_NACK;
  LL_I2C_Init(I2C1, &I2C_InitStruct);
}

/*******************************************************************************
**�������� ��I2C���ͺ���
**������� ��DevAddress���ӻ���ַ��
**           pData��Ҫ��������ָ�룻
**           Size��Ҫ�������ݴ�С
**������� ��
*******************************************************************************/
void APP_MasterTransmit(uint16_t DevAddress, uint8_t *pData, uint16_t Size)
{
  /* ��pos */
  LL_I2C_DisableBitPOS(I2C1);

  /* ���ӻ���ַ��Ҫ�������ݡ����ݴ�С��״̬����ȫ�ֱ��� */
  pBuffPtr = pData;
  XferCount = Size;
  Devaddress = DevAddress;
  State = I2C_STATE_BUSY_TX;

  /* ������ʼλ */
  LL_I2C_GenerateStartCondition(I2C1);
  while (LL_I2C_IsActiveFlag_SB(I2C1) != 1)
    ;

  /* ���ʹӻ���ַ */
  LL_I2C_TransmitData8(I2C1, (Devaddress & (uint8_t)(~0x01)));
  while (LL_I2C_IsActiveFlag_ADDR(I2C1) != 1)
    ;
  LL_I2C_ClearFlag_ADDR(I2C1);

  /* �������� */
  while (XferCount > 0)
  {
    while (LL_I2C_IsActiveFlag_TXE(I2C1) != 1)
      ;
    LL_I2C_TransmitData8(I2C1, *pBuffPtr);
    pBuffPtr++;
    XferCount--;

    if ((LL_I2C_IsActiveFlag_BTF(I2C1) == 1) && (XferCount != 0U))
    {
      LL_I2C_TransmitData8(I2C1, *pBuffPtr);
      pBuffPtr++;
      XferCount--;
    }

    while (LL_I2C_IsActiveFlag_BTF(I2C1) != 1)
      ;
  }

  /* ����ֹͣλ */
  LL_I2C_GenerateStopCondition(I2C1);

  State = I2C_STATE_READY;
}

/*******************************************************************************
**�������� ��I2C���պ���
**������� ��DevAddress���ӻ���ַ��
**           Size��Ҫ�������ݴ�С
**������� ��pData��Ҫ��������ָ�룻
*******************************************************************************/
void APP_MasterReceive(uint16_t DevAddress, uint8_t *pData, uint16_t Size)
{
  /* ��pos */
  LL_I2C_DisableBitPOS(I2C1);

  /* ���ӻ���ַ��Ҫ�������ݡ����ݴ�С����ȫ�ֱ��� */
  pBuffPtr = pData;
  XferCount = Size;
  Devaddress = DevAddress;
  State = I2C_STATE_BUSY_RX;

  /* ʹ��Ӧ�� */
  LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_ACK);

  /* ������ʼλ */
  LL_I2C_GenerateStartCondition(I2C1);
  while (LL_I2C_IsActiveFlag_SB(I2C1) != 1)
    ;

  /* ���ʹӻ���ַ */
  LL_I2C_TransmitData8(I2C1, (Devaddress | 0x1));
  while (LL_I2C_IsActiveFlag_ADDR(I2C1) != 1)
    ;

  /* �������� */
  if (XferCount == 0U)
  {
    LL_I2C_ClearFlag_ADDR(I2C1);
    LL_I2C_GenerateStopCondition(I2C1);
  }
  else if (XferCount == 1U)
  {
    LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_NACK);

    __disable_irq();
    LL_I2C_ClearFlag_ADDR(I2C1);
    LL_I2C_GenerateStopCondition(I2C1);
    __enable_irq();
  }
  else if (XferCount == 2U)
  {
    LL_I2C_EnableBitPOS(I2C1);

    __disable_irq();
    LL_I2C_ClearFlag_ADDR(I2C1);
    LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_NACK);
    __enable_irq();
  }
  else
  {
    LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_ACK);
    LL_I2C_ClearFlag_ADDR(I2C1);
  }

  while (XferCount > 0U)
  {
    if (XferCount <= 3U)
    {
      if (XferCount == 1U)
      {
        while (LL_I2C_IsActiveFlag_RXNE(I2C1) != 1)
          ;
        *pBuffPtr = LL_I2C_ReceiveData8(I2C1);
        pBuffPtr++;
        XferCount--;
      }
      else if (XferCount == 2U)
      {
        while (LL_I2C_IsActiveFlag_BTF(I2C1) != 1)
          ;

        __disable_irq();
        LL_I2C_GenerateStopCondition(I2C1);
        *pBuffPtr = LL_I2C_ReceiveData8(I2C1);
        pBuffPtr++;
        XferCount--;
        __enable_irq();

        *pBuffPtr = LL_I2C_ReceiveData8(I2C1);
        pBuffPtr++;
        XferCount--;
      }
      else
      {
        while (LL_I2C_IsActiveFlag_BTF(I2C1) != 1)
          ;

        LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_NACK);

        __disable_irq();
        *pBuffPtr = LL_I2C_ReceiveData8(I2C1);
        pBuffPtr++;
        XferCount--;
        while (LL_I2C_IsActiveFlag_BTF(I2C1) != 1)
          ;
        LL_I2C_GenerateStopCondition(I2C1);
        *pBuffPtr = LL_I2C_ReceiveData8(I2C1);
        pBuffPtr++;
        XferCount--;
        __enable_irq();

        *pBuffPtr = LL_I2C_ReceiveData8(I2C1);
        pBuffPtr++;
        XferCount--;
      }
    }
    else
    {
      while (LL_I2C_IsActiveFlag_RXNE(I2C1) != 1)
        ;

      *pBuffPtr = LL_I2C_ReceiveData8(I2C1);
      pBuffPtr++;
      XferCount--;

      if (LL_I2C_IsActiveFlag_BTF(I2C1) == 1)
      {
        *pBuffPtr = LL_I2C_ReceiveData8(I2C1);
        pBuffPtr++;
        XferCount--;
      }
    }
  }

  State = I2C_STATE_READY;
}