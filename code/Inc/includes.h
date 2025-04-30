#ifndef _OVERALL_H_
#define _OVERALL_H_

#include "string.h"
#include "stdbool.h" 
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "stddef.h"
#include "math.h"

#include "../Inc/typedef.h"
#include "../Inc/usr_config.h"

#include "../Drivers/CMSIS/Device/PY32F0xx/Include/system_py32f030xx.h"
#include "../Drivers/CMSIS/Device/PY32F0xx/Include/py32f030x8.h"

#include "../Drivers/PY32F0xx_HAL_Driver/Inc/py32f0xx_ll_rcc.h"
#include "../Drivers/PY32F0xx_HAL_Driver/Inc/py32f0xx_ll_bus.h"
#include "../Drivers/PY32F0xx_HAL_Driver/Inc/py32f0xx_ll_system.h"
#include "../Drivers/PY32F0xx_HAL_Driver/Inc/py32f0xx_ll_exti.h"
#include "../Drivers/PY32F0xx_HAL_Driver/Inc/py32f0xx_ll_cortex.h"
#include "../Drivers/PY32F0xx_HAL_Driver/Inc/py32f0xx_ll_utils.h"
#include "../Drivers/PY32F0xx_HAL_Driver/Inc/py32f0xx_ll_pwr.h"
#include "../Drivers/PY32F0xx_HAL_Driver/Inc/py32f0xx_ll_dma.h"
#include "../Drivers/PY32F0xx_HAL_Driver/Inc/py32f0xx_ll_gpio.h"
#include "../Drivers/PY32F0xx_HAL_Driver/Inc/py32f0xx_ll_tim.h"
#include "../Drivers/PY32F0xx_HAL_Driver/Inc/py32f0xx_ll_rcc.h"
#include "../Drivers/PY32F0xx_HAL_Driver/Inc/py32f0xx_ll_usart.h"
#include "../Drivers/PY32F0xx_HAL_Driver/Inc/py32f0xx_ll_i2c.h"

#include "../Phy/phy_i2c.h"
#include "../Phy/phy_timer.h"
#include "../Phy/phy_uart.h"
#include "../Phy/phy_gpio.h"
#include "../phy/phy_watchdog.h"

#include "../Dev/Board.h"
#include "../Dev/dev_leds.h"

#include "../RFid/config_rf.h"
#include "../RFid/bsp_rfspi_sw.h"
#include "../RFid/bsp_pn298.h"

#include "../Kernel/os_beason.h"
#include "../Kernel/kernel.h"

#include "../App/app_leds.h"

#endif
