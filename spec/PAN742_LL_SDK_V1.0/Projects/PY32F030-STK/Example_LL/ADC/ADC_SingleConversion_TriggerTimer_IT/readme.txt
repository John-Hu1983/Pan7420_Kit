================================================================================
                                样例使用说明
================================================================================
功能描述：
此样例演示了ADC采集通过TIM触发的方式打印通道4的电压值，PA4为模拟输入，每隔1s会从
串口PA2/PA3打印当前的电压值。

================================================================================
测试环境：
测试用板：PY32F030_STK
MDK版本： 5.28
IAR版本： 9.20
================================================================================
使用步骤：
1. 编译下载程序到MCU，并运行；
2. 连接串口PA2/PA3.
3. 每隔1s会打印一次PA4的电压值

================================================================================
注意事项：
STK板        USB转TTL模块
PA02(TX) --> RX
PA03(RX) --> TX
GND      --> GND


================================================================================