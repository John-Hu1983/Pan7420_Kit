#include "../Inc/includes.h"

static event_state schedule = {
    .words = 0,
};
/* Private variables ---------------------------------------------------------*/
uint8_t aTxBuffer[15] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
uint8_t aRxBuffer[15] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
extern __IO uint32_t State;
/*
 * @description	:
 * @param		:
 * @return		:
 */
void task_OLED_screen(osvar_t ms)
{
  switch (schedule.bits.step)
  {
  case 0:
    OLED_Init();
    schedule.bits.step++;
    break;

  case 1:

    /* ������������ */
    APP_MasterTransmit(I2C_ADDRESS, (uint8_t *)aTxBuffer, sizeof(aTxBuffer));

    /* �ȴ���������������� */
    while (State != I2C_STATE_READY)
      ;

    /* ������������ */
    APP_MasterReceive(I2C_ADDRESS, (uint8_t *)aRxBuffer, sizeof(aRxBuffer));

    /* �ȴ���������������� */
    while (State != I2C_STATE_READY)
      ;

    // TEST_MainPage(); // ��������ʾ����
    // OLED_Clear(0);
    // Test_Color(); // ˢ������
    // OLED_Clear(0);
    // Test_Rectangular(); // ���λ��Ʋ���
    // OLED_Clear(0);
    // Test_Circle(); // Բ�λ��Ʋ���
    // OLED_Clear(0);
    // Test_Triangle(); // �����λ��Ʋ���
    // OLED_Clear(0);
    // TEST_English(); // Ӣ����ʾ����
    // OLED_Clear(0);
    // TEST_Number_Character(); // ���ֺͷ�����ʾ����
    // OLED_Clear(0);
    // TEST_Chinese(); // ������ʾ����
    // OLED_Clear(0);
    // TEST_BMP(); // BMP��ɫͼƬ��ʾ����
    // OLED_Clear(0);
    // TEST_Menu1(); // �˵�1��ʾ����
    // OLED_Clear(0);
    // TEST_Menu2(); // �˵�2��ʾ����
    // OLED_Clear(0);
    break;
  }
}