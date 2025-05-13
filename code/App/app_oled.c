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

    /* 主机发送数据 */
    APP_MasterTransmit(I2C_ADDRESS, (uint8_t *)aTxBuffer, sizeof(aTxBuffer));

    /* 等待主机发送数据完成 */
    while (State != I2C_STATE_READY)
      ;

    /* 主机接收数据 */
    APP_MasterReceive(I2C_ADDRESS, (uint8_t *)aRxBuffer, sizeof(aRxBuffer));

    /* 等待主机接收数据完成 */
    while (State != I2C_STATE_READY)
      ;

    // TEST_MainPage(); // 主界面显示测试
    // OLED_Clear(0);
    // Test_Color(); // 刷屏测试
    // OLED_Clear(0);
    // Test_Rectangular(); // 矩形绘制测试
    // OLED_Clear(0);
    // Test_Circle(); // 圆形绘制测试
    // OLED_Clear(0);
    // Test_Triangle(); // 三角形绘制测试
    // OLED_Clear(0);
    // TEST_English(); // 英文显示测试
    // OLED_Clear(0);
    // TEST_Number_Character(); // 数字和符号显示测试
    // OLED_Clear(0);
    // TEST_Chinese(); // 中文显示测试
    // OLED_Clear(0);
    // TEST_BMP(); // BMP单色图片显示测试
    // OLED_Clear(0);
    // TEST_Menu1(); // 菜单1显示测试
    // OLED_Clear(0);
    // TEST_Menu2(); // 菜单2显示测试
    // OLED_Clear(0);
    break;
  }
}