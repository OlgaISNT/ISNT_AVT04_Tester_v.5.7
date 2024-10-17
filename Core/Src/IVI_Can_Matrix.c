/* ###################################################################
 **     Filename    : INI_Can_Matrix.c
 **     Project     : can_pal_s32k146
 **     Processor   : S32K146_144
 **     Version     : Driver 01.00
 **     Compiler    : GNU C Compiler
 **     Date/Time   : 2019-10-01, 11:48, # CodeGen: 2
 **     Abstract    :
 **         Main module.
 **         This module contains user's application code.
 **     Settings    :
 **     Contents    :
 **         No public methods
 **
 ** ###################################################################*/
/*!
 ** @file INI_Can_Martix.c
 ** @version 01.00
 ** @brief
 **         Main module.
 **         This module contains user's application code.
 */
#include <stdint.h>

#include <stdbool.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
//#include "clockMan1.h"
//#include "dmaController1.h"
//#include "pin_mux.h"
#include "IVI_Can_Matrix.h"
#include "uds/IVI_Diagnostic.h"
#include "main.h"
extern TIM_HandleTypeDef htim1;
const char Hex[]={"0123456789ABCDEF"};
//#include "task.h"
//#include "main.h"
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern ADC_HandleTypeDef hadc3;
  //CAN_Object *handle;
 IOState ADC_chan;
//const char Hex[] = { "0123456789ABCDEF" };
 char  cmd_vcp_start[]= "StartV";
 char  cmd_end_test[]= "E";

uint8_t  cmd_wait_start_ISNT[]= "\r\nLoading ISNT TCU...\r\n";



uint8_t  cmd_wait_start_ISNT_Mmin[]= "\r\nLoading ISNT TCU M-...\r\n";
uint8_t  cmd_wait_start_ISNT_Mplus[]= "\r\nLoading ISNT TCU M+...\r\n";
uint8_t  cmd_wait_start_AVT04[]= "\r\nLoading AVT04 TCU...\r\n";
uint8_t  cmd_wait_start_ISNT24[]= "\r\nLoading ISNT(24V) TCU...\r\n";
console_io_buf cons;
extern uint32_t   timer1_ch;

sys_config Cfg;
sys_events Evnt;
// can_settings sCAN;
// can_events eCAN;
//uartcfg_t Uart;
//cnf_ecu  CNF_ECU;
//CAN_HandleTypeDef hcan1;
 DiagnosticRequest request;
  DiagnosticResponse response;
  extern UART_HandleTypeDef huart3;
  extern  DAC_HandleTypeDef hdac;

  uint32_t Wave_LUT[128] = {
      2048, 2149, 2250, 2350, 2450, 2549, 2646, 2742, 2837, 2929, 3020, 3108, 3193, 3275, 3355,
      3431, 3504, 3574, 3639, 3701, 3759, 3812, 3861, 3906, 3946, 3982, 4013, 4039, 4060, 4076,
      4087, 4094, 4095, 4091, 4082, 4069, 4050, 4026, 3998, 3965, 3927, 3884, 3837, 3786, 3730,
      3671, 3607, 3539, 3468, 3394, 3316, 3235, 3151, 3064, 2975, 2883, 2790, 2695, 2598, 2500,
      2400, 2300, 2199, 2098, 1997, 1896, 1795, 1695, 1595, 1497, 1400, 1305, 1212, 1120, 1031,
      944, 860, 779, 701, 627, 556, 488, 424, 365, 309, 258, 211, 168, 130, 97,
      69, 45, 26, 13, 4, 0, 1, 8, 19, 35, 56, 82, 113, 149, 189,
      234, 283, 336, 394, 456, 521, 591, 664, 740, 820, 902, 987, 1075, 1166, 1258,
      1353, 1449, 1546, 1645, 1745, 1845, 1946, 2047
  };


 size_t ustrlen(const char *s)
 {
     size_t len;

     //
     // Check the arguments.
     //
     assert_param(s);

     //
     // Initialize the length.
     //
     len = 0;

     //
     // Step throug the string looking for a zero character (marking its end).
     //
     while(s[len])
     {
         //
         // Zero not found so move on to the next character.
         //
         len++;
     }

     return(len);
 }

 char *SearchStr(char *s1, int32_t l1, char *s2) {
     char chr1, chr2;
     uint32_t l2;

     if(l1 <= 0) return 0;
     l2 = ustrlen(s2);
     while (l1 > 0) {
         chr1 = (((*s1 >= 'A') && (*s1 <= 'Z')) ? (*s1 + ('a' - 'A')) : *s1);
         chr2 = (((*s2 >= 'A') && (*s2 <= 'Z')) ? (*s2 + ('a' - 'A')) : *s2);

         if (chr1 == chr2) {
             uint16_t cl2 = l2;
             char *cd2 = s2;
             while ((l1 > 0) && (cl2 > 0)) {
                 chr1 = (((*s1 >= 'A') && (*s1 <= 'Z')) ? (*s1 + ('a' - 'A')) : *s1);
                 chr2 = (((*cd2 >= 'A') && (*cd2 <= 'Z')) ? (*cd2 + ('a' - 'A')) : *cd2);
                 if (chr1 != chr2) break;
                 ++s1;
                 --l1;
                 ++cd2;
                 --cl2;
             }
             if (!cl2) return (s1 - l2);
         } else {
             ++s1;
             --l1;
         }
     }
     return (0);
 }

 void Hex_Str(unsigned char hex, unsigned char *str) {
     *str++ = Hex[hex >> 4];
     *str++ = Hex[hex & 0xF];
 }

 void Print_Loading()
 {
	 vTaskDelay(100);
	if(Can_state.version == ISNT_M_plus )
			ConsoleWrite (&cmd_wait_start_ISNT_Mplus[0],   sizeof(cmd_wait_start_ISNT_Mplus) );
	if(Can_state.version == ISNT_M_min )
		ConsoleWrite (&cmd_wait_start_ISNT_Mmin[0],   sizeof(cmd_wait_start_ISNT_Mmin) );
	if(Can_state.version == AVT04)
	   ConsoleWrite (&cmd_wait_start_AVT04[0],   sizeof(cmd_wait_start_AVT04) );
	else
		  ConsoleWrite (&cmd_wait_start_ISNT24[0],   sizeof(cmd_wait_start_ISNT24) );
 }

 void RS232_Task(void const * argument) {
 	//request.succes = true;

   uint8_t rxbuf[100];

   char *str , str_s;
   uint8_t offs = 0x7;
   HAL_UART_Receive_IT(&huart3, &rxbuf[0], 5);

   cons.gate_audio = xSemaphoreCreateRecursiveMutex();

 	for (;;) {


 		if(!Can_state.test_audio)
 		 xSemaphoreTake(cons.gate_audio, portMAX_DELAY);
 	//	{
                memset(rxbuf, 0 , sizeof(rxbuf));


 			     HAL_UART_Receive(&huart3, &rxbuf[0], 10, 300);


 		         str = strstr((char *)rxbuf, cmd_vcp_start);
 				if (str) {

 					 Can_state.version = *(str + offs) - 0x30;
 					if(Can_state.version > ISNT_24)  Can_state.version = ISNT_M_plus;
 					Can_state.start_test_cmd22 = false;
 					Can_state.state = Start_Test;
 				    Can_state.err  = 0;



 		  		   	memset(&Config, 0,sizeof(Config));
 					memset(Can_state.txbuf , 0 , sizeof(Can_state.txbuf));
 					vTaskDelay(TIMEOUT_1s);
 					Init_Pins();
 				//	vTaskDelay(TIMEOUT_1s);
 					vTaskDelay(50);
 					  Print_Loading();
 					  memset(rxbuf, 0 , sizeof(rxbuf));
        			vTaskDelay(50);
        			Can_state.start_test_cmd22 = true;
        		 	Can_state.state = Start_Test;
        		 	Can_state.err  = 0;

				}
 				str_s = SearchStr((char*) rxbuf, 1, cmd_end_test);
 				 		if (str_s) {

 				  	Can_state.start_test_cmd22 = false;
 				//	 vTaskDelay(500);
 			     	 Can_state.state = Start_Test ;
 				     Can_state.err  = 0;
 				     memset(&Can_state.txbuf[0] , 0 , sizeof(Can_state.txbuf));

 				 						}

 	      xSemaphoreGive(cons.gate_audio);
 				vTaskDelay(50);

 		}


 	}


 void  Play_Audio(uint32_t sec , uint32_t freq)
    {
	  int wave = 0;
	  uint32_t amp;
	  int audio_routine_scan = xTaskGetTickCount();
	  xSemaphoreTake(cons.gate_audio, portMAX_DELAY);

	  Can_state.t1 =0;
	  timer1_ch =0;

	 				do {

	 					for(wave = 0 ; wave < 128 ; wave++){
	 					  if(Can_state.version == AVT04)
	 						  amp = (uint32_t)(Wave_LUT[wave] / AVT04_AUDIO_COEFF)  ;
	 						else
	 							amp = (uint32_t)(Wave_LUT[wave] / ISNT_AUDIO_COEFF)  ; /////////////// 16
	 						HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, amp );

	 					   for(int nop = 0 ; nop < freq; nop++);

	 				          	}
	 				}while ((xTaskGetTickCount() - audio_routine_scan) < sec *1000);
	  xSemaphoreGive(cons.gate_audio);
    }


   uint32_t   InOutADC_Data(ADC_HandleTypeDef *hadc, uint32_t num)
      {

	if ((num == 2) && (hadc->Instance == ADC2) ) {

		MX_ADC2_Init(ADC_CHANNEL_13);
		vTaskDelay(100);
		HAL_ADC_Start(hadc);
		HAL_ADC_PollForConversion(hadc, 100);
		//  ADC_chan.aInputs[1] = HAL_ADC_GetValue(hadc);
		ADC_chan.aInputs[num] = HAL_ADC_GetValue(hadc);
		fft_pResult.mute_adc = ADC_chan.aInputs[2];
		HAL_ADC_Stop(hadc);

		MX_ADC2_Init(ADC_CHANNEL_10);
	} else

	{
		HAL_ADC_Start(hadc);
		HAL_ADC_PollForConversion(hadc, 100);
		ADC_chan.aInputs[num] = HAL_ADC_GetValue(hadc);

		HAL_ADC_Stop(hadc);
	}
	return ADC_chan.aInputs[num];

      }


