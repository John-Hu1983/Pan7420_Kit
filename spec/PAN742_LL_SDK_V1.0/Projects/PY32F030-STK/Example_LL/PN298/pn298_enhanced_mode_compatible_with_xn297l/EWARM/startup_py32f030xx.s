;****************************************************************************** 
;* @file              : startup_py32f030xx.s
;* @brief             : P32F030xx devices vector table for EWARM toolchain.
;*                      This module performs:
;*                      - Set the initial SP
;*                      - Set the initial PC == __iar_program_start
;*                      - Set the vector table entries with the exceptions ISR address
;*                      - Branches to __main in the C library (which eventually
;*                        calls main()).
;*                      After Reset the CortexM0+ processor is in Thread mode,
;*                      priority is Privileged, and the Stack is set to Main.
;****************************************************************************** 
;* @attention
;*
;*      Copyright (c) 2021, Puya Semiconductor Inc.
;*
;*      All rights reserved.
;*
;*      Redistribution and use in source and binary forms, with or without modification,
;*  are permitted provided that the following conditions are met:
;*
;*   1. Redistributions of source code must retain the above copyright notice,
;*      this list of conditions and the following disclaimer.
;*   2. Redistributions in binary form must reproduce the above copyright notice,
;*      this list of conditions and the following disclaimer in the documentation
;*      and/or other materials provided with the distribution.
;*   3. Neither the name of the copyright holder nor the names of its contributors
;*      may be used to endorse or promote products derived from this software
;*      without specific prior written permission.
;*
;*      THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
;* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
;* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
;* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
;* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
;* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
;* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
;* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
;* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
;* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
;*
;****************************************************************************** 
;
; The modules in this file are included in the libraries, and may be replaced
; by any user-defined modules that define the PUBLIC symbol _program_start or
; a user defined start symbol.
; To override the cstartup defined in the library, simply add your modified
; version to the workbench project.
;
; The vector table is normally located at address 0.
; When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
; The name "__vector_table" has special meaning for C-SPY:
; it is where the SP start value is found, and the NVIC vector
; table register (VTOR) is initialized to this address if != 0.
;
; Cortex-M version
;

        MODULE  ?cstartup
        
        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        EXTERN  SystemInit        
        PUBLIC  __vector_table

        DATA
__vector_table
        DCD     sfe(CSTACK)               ; Top of Stack
        DCD     Reset_Handler             ; Reset Handler
        DCD     NMI_Handler               ; NMI Handler
        DCD     HardFault_Handler         ; Hard Fault Handler
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     SVC_Handler               ; SVCall Handler
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     PendSV_Handler            ; PendSV Handler
        DCD     SysTick_Handler           ; SysTick Handler

        ; External Interrupts
        DCD     WWDG_IRQHandler                ; 0Window Watchdog
        DCD     PVD_IRQHandler                 ; 1PVD through EXTI Line detect
        DCD     RTC_IRQHandler                 ; 2RTC through EXTI Line
        DCD     FLASH_IRQHandler               ; 3FLASH
        DCD     RCC_IRQHandler                 ; 4RCC
        DCD     EXTI0_1_IRQHandler             ; 5EXTI Line 0 and 1
        DCD     EXTI2_3_IRQHandler             ; 6EXTI Line 2 and 3
        DCD     EXTI4_15_IRQHandler            ; 7EXTI Line 4 to 15
        DCD     0                              ; 8Reserved 
        DCD     DMA1_Channel1_IRQHandler       ; 9DMA1 Channel 1
        DCD     DMA1_Channel2_3_IRQHandler     ; 10DMA1 Channel 2 and Channel 3
        DCD     0                              ; 11Reserved 
        DCD     ADC_COMP_IRQHandler            ; 12ADC&COMP
        DCD     TIM1_BRK_UP_TRG_COM_IRQHandler ; 13TIM1 Break, Update, Trigger and Commutation
        DCD     TIM1_CC_IRQHandler             ; 14TIM1 Capture Compare
        DCD     0                              ; 15Reserved 
        DCD     TIM3_IRQHandler                ; 16TIM3
        DCD     LPTIM1_IRQHandler              ; 17LPTIM1
        DCD     0                              ; 18Reserved 
        DCD     TIM14_IRQHandler               ; 19TIM14
        DCD     0                              ; 20Reserved 
        DCD     TIM16_IRQHandler               ; 21TIM16
        DCD     TIM17_IRQHandler               ; 22TIM17
        DCD     I2C1_IRQHandler                ; 23I2C1
        DCD     0                              ; 24Reserved 
        DCD     SPI1_IRQHandler                ; 25SPI1
        DCD     SPI2_IRQHandler                ; 26SPI2
        DCD     USART1_IRQHandler              ; 27USART1
        DCD     USART2_IRQHandler              ; 28USART2
        DCD     0                              ; 29Reserved
        DCD     LED_IRQHandler                 ; 30LED
        DCD     0                              ; 31Reserved

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB

        PUBWEAK Reset_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reset_Handler
        LDR     R0, =SystemInit
        BLX     R0
        LDR     R0, =__iar_program_start
        BX      R0
        
        PUBWEAK NMI_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
NMI_Handler
        B NMI_Handler

        PUBWEAK HardFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
HardFault_Handler
        B HardFault_Handler

        PUBWEAK SVC_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SVC_Handler
        B SVC_Handler
        
        PUBWEAK PendSV_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PendSV_Handler
        B PendSV_Handler
        
        PUBWEAK SysTick_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SysTick_Handler
        B SysTick_Handler
        
        PUBWEAK WWDG_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
WWDG_IRQHandler
        B WWDG_IRQHandler

        PUBWEAK PVD_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
PVD_IRQHandler
        B PVD_IRQHandler
        
        PUBWEAK RTC_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
RTC_IRQHandler
        B RTC_IRQHandler

        PUBWEAK FLASH_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
FLASH_IRQHandler
        B FLASH_IRQHandler

        PUBWEAK RCC_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
RCC_IRQHandler
        B RCC_IRQHandler
        
        PUBWEAK EXTI0_1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
EXTI0_1_IRQHandler
        B EXTI0_1_IRQHandler

        PUBWEAK EXTI2_3_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
EXTI2_3_IRQHandler
        B EXTI2_3_IRQHandler

        PUBWEAK EXTI4_15_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
EXTI4_15_IRQHandler
        B EXTI4_15_IRQHandler
        
        PUBWEAK DMA1_Channel1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
DMA1_Channel1_IRQHandler
        B DMA1_Channel1_IRQHandler
        
        PUBWEAK DMA1_Channel2_3_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
DMA1_Channel2_3_IRQHandler
        B DMA1_Channel2_3_IRQHandler
            
        PUBWEAK ADC_COMP_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
ADC_COMP_IRQHandler
        B ADC_COMP_IRQHandler
        
        PUBWEAK TIM1_BRK_UP_TRG_COM_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIM1_BRK_UP_TRG_COM_IRQHandler
        B TIM1_BRK_UP_TRG_COM_IRQHandler
        
        PUBWEAK TIM1_CC_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIM1_CC_IRQHandler
        B TIM1_CC_IRQHandler
        
        PUBWEAK TIM3_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIM3_IRQHandler
        B TIM3_IRQHandler
        
        PUBWEAK LPTIM1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
LPTIM1_IRQHandler
        B LPTIM1_IRQHandler
        
        PUBWEAK TIM14_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIM14_IRQHandler
        B TIM14_IRQHandler
        
        PUBWEAK TIM16_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIM16_IRQHandler
        B TIM16_IRQHandler
        
        PUBWEAK TIM17_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIM17_IRQHandler
        B TIM17_IRQHandler
        
        PUBWEAK I2C1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
I2C1_IRQHandler
        B I2C1_IRQHandler
        
        PUBWEAK SPI1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
SPI1_IRQHandler
        B SPI1_IRQHandler
        
        PUBWEAK SPI2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
SPI2_IRQHandler
        B SPI2_IRQHandler
        
        PUBWEAK USART1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
USART1_IRQHandler
        B USART1_IRQHandler
        
        PUBWEAK USART2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
USART2_IRQHandler
        B USART2_IRQHandler
        
        PUBWEAK LED_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
LED_IRQHandler
        B LED_IRQHandler
        
         END
/*****************************END OF FILE************************************/