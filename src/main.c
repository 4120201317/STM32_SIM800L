/**
  ******************************************************************************
  * @file    SysTick/TimeBase/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body.
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
#include "main.h"
#include "system_init.h"
#include "sim800l.h"

char rx_buf[SIM_BUFFER];
char publish_mes[MAX_PUBLISH_MES][LEN_PUBLISH_MES]={"******Loi loc nuoc het han, vui long thay loi loc moi******",
                                                    "**Dau do khong tiep xuc voi nuoc, vui long kiem tra dau do*",
                                                    "*********************DANG KY THANH CONG********************"};
static __IO uint32_t TimingDelay;
char topic[LEN_TOPIC]="water";           //topic
char input_topic[LEN_TOPIC];
uint8_t ct_wrk;
struct PHONEBOOK contact[MAX_CLIENT];


uint16_t i;
bool stat;
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
    booting();
    Delay(2000);
    while( (sim_set_text_mode(1,rx_buf) & SIM_RES_OK) == 0 )
    Delay(1000);
    while( (sim_set_cnmi_mode(0,0,0,0,0,rx_buf) & SIM_RES_OK) == 0 )
    Delay(1000);
    for(i=0;i<MAX_CLIENT;i++)
    {
        // contact[i].number[] = "+ZZxxxxxxxxx";
        contact[i].subscribed = FALSE;
        contact[i].published = FALSE;
    }
  while (1)
  {
    Delay(1000);
    GPIO_WriteBit(GPIOA, GPIO_Pin_6, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_6)));
    // if(RxCounter > 40){
    //   for(i=40;i>0;i--)
    //     putchar(rx_buf[i]);
    //   RxCounter=0;
    //   putchar('\r');
    //   putchar('\n');
    // }
    // for(i=1;i<MAX_SMS;i++)
    //     stat=sim_read_sms(i,rx_buf);
    // stat=sim_dele_sms(1,rx_buf);
    // stat=sim_send_sms("+84975738366",publish_mes[PUBLISH_WATER_UNSAFE],rx_buf);
    // stat=sim_send_sms("+84975738366",publish_mes[PUBLISH_TDS_PROBE_NOWATER],rx_buf);
    // stat=sim_send_sms("+84975738366",publish_mes[PUBLISH_SUBSCRIBED_OK],rx_buf);
    
    // stat=sim_set_cnmi_mode(0,0,0,0,0,rx_buf);
    update_phonebook();
    /* Reset PA6 */
    Delay(1000);
    GPIO_WriteBit(GPIOA, GPIO_Pin_6, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_6)));
  }
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  * @this function loop through all sms in memory. 
  * @>  If message contain valid activating code:
  * @     - If this is the first time reading (message status is "REC UNREAD"), consider this is
  * @       the activating message sent from user
  * @     - Updated to phonebook
  * @       + If this contact is already in the phonebook -> ignore
  * @       + If this is a new contact, put into the end of phonebook
  * @       + respond ok if this message status is "REC UNREAD"
  * @>  Else if message does not contain activate code -> remove this message
  */
void update_phonebook(void){
    uint8_t i,j;
    char temp_contact[LEN_PHONE_NUM]="+xxxxxxxxxxx";//no phonenumber, not sent message yet, not in phonebook
    //go through all sms(read/unread) in sim
    for(i=1;i<=MAX_SMS;i++)
    {
        stat=sim_read_sms(i,rx_buf);
        //check if message contain activate code
        if( ((stat & SIM_RES_OK)  != 0 ) && ((sim_get_sms_data(input_topic,rx_buf) & SIM_RES_OK) != 0 ) ){
            if( !strcmp(topic,input_topic,LEN_TOPIC) )
            {
                sim_get_sms_contact(temp_contact,rx_buf);

                for(j=0; j<MAX_CLIENT; j++)
                {
                    //if this contact already exist, just ignore it
                    stat = strcmp(temp_contact,contact[i].number,LEN_PHONE_NUM);
                    if( stat == TRUE)
                        break;

                    //this is new contact, push it to the end of phonebook
                    if( contact[j].subscribed == FALSE ){
                        contact[j].subscribed = TRUE;
                        contact[j].published = FALSE;
                        strcpy(contact[j].number,temp_contact,LEN_PHONE_NUM);
                        break;
                    }

                }

                //if this is "REC UNREAD"
                if( sim_get_sms_state(rx_buf) == SMS_UNREAD )
                    if( (sim_send_sms(contact[j].number,publish_mes[PUBLISH_SUBSCRIBED_OK],rx_buf) & SIM_RES_OK) !=0 )
                        contact[j].subscribed = TRUE;
            }
            //this is a trash message, remove it
            else
            {
                sim_dele_sms(i,rx_buf);
            }
        }
    }
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
