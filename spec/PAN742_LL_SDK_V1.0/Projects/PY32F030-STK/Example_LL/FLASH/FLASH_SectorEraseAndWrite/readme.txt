================================================================================
                                样例使用说明
================================================================================
功能描述：
此样例演示了flash sector擦除和page写功能，通过keil debug仿真界面，可观察flash存储
器中是否擦除成功和page写成功。

================================================================================
测试环境：
测试用板：PY32F030_STK
MDK版本： 5.28
IAR版本： 9.20
================================================================================
使用步骤：
1. 编译下载程序并通过debug运行；
2. 全速运行，等待按键按下，防止每次上电都擦写FLASH；
3. 单步运行，并观察flash地址0x0800F000~0x0800F100存储区的变化；
4. 当执行扇区擦除时，上述地址的存储值应该全部为0xFFFFFFFF;
5. 当执行完写操作后，上述地址的存储值应该和写入的数值（数组DATA[64]）一致；
6. 程序执行成功，LED灯翻转。
================================================================================
注意事项：


================================================================================