================================================================================
                                样例使用说明
================================================================================
功能描述:
此样例是利用DMA对串口外设接口（SPI）与外部设备以全双工串行方式进行通信的演示,此
接口设置为从模式。主机通过MOSI引脚发送数据,从MISO引脚接收从机的数据，数据以主机
提供的SCK沿同步被移位，完成全双工通信。
================================================================================
测试环境：
测试用板：PY32F030_STK
MDK版本： 5.28
IAR版本： 9.20
================================================================================
使用步骤:
1.选择两块PY32F030_STK板，一块作为主机，一块作为从机
2.编译下载从机程序（本样例程序）
3.编译下载主机程序SPI_TwoBoards_FullDuplexMaster_DMA_Init
4.主机与从机引脚连接(箭头指向为信号传输方向) 
主机MASTER：         从机SLAVE：
SCK(PB3)   ----->    SCK(PB3)
MISO(PB4)  <-----    MISO(PB4)
MOSI(PB5)  ----->    MOSI(PB5)
GND          <----->   GND
5.主从机上电
3.按下从机复位按键先运行从机程序，再按下主机用户按键运行主机程序 
4.观察到主从机绿灯都保持“常亮”则通信成功，否则“闪烁”通信失败
================================================================================
注意事项:
1.必须先按从机复位按键使从机程序先运行，再按主机用户按键开始运行主机程序，否则会
导致主从机通信失败。
2.主机模式可通过程序设置IO控制方式为DMA方式、中断方式和查询方式
3.NSS引脚只需要在使用NSS硬件方式时连接（本样例使用的是NSS软件方式，无需连接NSS引
脚）
================================================================================