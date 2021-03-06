/**
  ******************************************************************************
  * @file    SysTick/TimeBase/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "main.h"
#include "sim800l.h"
#include "system_init.h"

/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup SysTick_TimeBase
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern uint16_t RxCounter;
extern char rx_buf[SIM_BUFFER];
#if defined DEBUG
extern uint16_t DbgCounter;
extern char rx_dbg[DBG_BUF];
extern bool new_cmd;
#endif
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
    TimingDelay_Decrement();
}

#if defined SMS
/**
  * @brief  This function handles Usart1 Handler.
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)
{

    char temp;
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        temp = (char)(USART_ReceiveData(USART1) & 0x1FF);
        // if( RxCounter < SIM_BUFFER)
            rx_buf[RxCounter++]=temp;
#if defined TEST_SIM
        printf(temp);
#endif
    }
    if(USART_GetITStatus(USART1, USART_FLAG_TXE) != RESET)
    {
        USART_ClearFlag(USART1,USART_FLAG_TXE);
    }
}
#endif

#if defined DEBUG
/**
  * @brief  This function handles Usart1 Handler.
  * @param  None
  * @retval None
  */
void USART2_IRQHandler(void)
{
    char temp;
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        temp = (char)(USART_ReceiveData(USART2) & 0x1FF);
#if defined DEBUG
        //maximum command length = 255, frontend needs to validate
        if( !new_cmd )
        {
            rx_dbg[DbgCounter++]=temp;
            if(temp == '\n')
            {
                new_cmd = TRUE;
                // USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
            }
        }
#endif
#if defined TEST_SIM
        putchar(temp);
#endif        
    }
    // if(USART_GetITStatus(USART2, USART_FLAG_TC) != RESET)
    // {
    //     USART_ClearFlag(USART2,USART_FLAG_TC);
    // }
}
#endif
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
