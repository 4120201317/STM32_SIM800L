//------------------------------------------------
// INCLUDE
//------------------------------------------------
#include "system_init.h"
#include "stm32f10x.h"

//------------------------------------------------
// booting the system
// Cau hinh cac module can su dung
//------------------------------------------------
void booting(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f10x_xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f10x.c file
     */     

  /* Setup SysTick Timer for 1 msec interrupts.
     ------------------------------------------
    1. The SysTick_Config() function is a CMSIS function which configure:
       - The SysTick Reload register with value passed as function parameter.
       - Configure the SysTick IRQ priority to the lowest value (0x0F).
       - Reset the SysTick Counter register.
       - Configure the SysTick Counter clock source to be Core Clock Source (HCLK).
       - Enable the SysTick Interrupt.
       - Start the SysTick Counter.
    
    2. You can change the SysTick Clock source to be HCLK_Div8 by calling the
       SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8) just after the
       SysTick_Config() function call. The SysTick_CLKSourceConfig() is defined
       inside the misc.c file.

    3. You can change the SysTick IRQ priority by calling the
       NVIC_SetPriority(SysTick_IRQn,...) just after the SysTick_Config() function 
       call. The NVIC_SetPriority() is defined inside the core_cm3.h file.

    4. To adjust the SysTick time base, use the following formula:
                            
         Reload Value = SysTick Counter Clock (Hz) x  Desired Time base (s)
    
       - Reload Value is the parameter to be passed for SysTick_Config() function
       - Reload Value should not exceed 0xFFFFFF
   */
  	uint16_t i=0xFFFF;
	while(i>0)	i--;
	CLK_Config();
	NVIC_Config();
	GPIO_Config();
	UART1_Config();
	if (SysTick_Config(SystemCoreClock / 1000))
	{ 
		/* Capture error */ 
		while (1);
	}
}



//------------------------------------------------
// RCC_Configuration
// Cau hinh clock cua cac module can su dung
//------------------------------------------------
void CLK_Config(void)
{
	// RCC_PCLK2Config(RCC_HCLK_Div1);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
}


//------------------------------------------------
// NVIC_Config
// Config ngat cho Timer 2 de xuat du lieu ra
// Config EXTI de tinh toc do quay cua banh xe
//------------------------------------------------
void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;	
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x00);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
 
}



//------------------------------------------------
// GPIO_Configuration
//------------------------------------------------
void GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

// 	/* Configure USARTy Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

// 	/* Configure USARTy Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

//------------------------------------------------
  /* USARTx configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
//------------------------------------------------
void UART1_Config(void)
{
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);
}

//------------------------------------------------
// putchar
//------------------------------------------------
void putchar(int ch)
{
	USART_SendData(USART1, (uint8_t) ch);
	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
}

//------------------------------------------------
// strcmp
// if Return value < 0 then it indicates X is less than Y.
// if Return value > 0 then it indicates Y is less than X.
// if Return value = 0 then it indicates X is equal to Y.
//------------------------------------------------
bool strcmp(char*s1,char*s2,u8 len){
    bool flag = TRUE;
    while(len>0){
        len-=1;
        if( *(s1+len) != *(s2+len) ){
            flag = FALSE;
            break;
        }
    }
    return flag;
}


//------------------------------------------------
// strcpy
//------------------------------------------------
void strcpy(char*s1,char*s2,u8 len){
  while(len>0){
    *(s1+len-1) = *(s2+len-1);
    len-=1;
  }
  return;
}


//------------------------------------------------
// memset
//------------------------------------------------
void memset(void* str, char ch, u16 n){
	int i;
	//type cast the str from void* to char*
	char *s = (char*) str;
	//fill "n" elements/blocks with ch
	for(i=0; i<n; i++)
		s[i]=ch;
    return;
}
