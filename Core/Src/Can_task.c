/* ###################################################################
 **     Filename    : CANAPP.c
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
 /**** System include files ****/

#include <stdint.h>

#include <stdbool.h>
#include <inttypes.h>
#include <string.h>
//#include <tasks.h>
#include "IVI_Can_Matrix.h"
#include "FreeRTOS.h"
#include "task.h"
#include "uds/uds.h"

#include "uds/uds_types.h"
//#include "uds/isotp/receive.h"
#include "uds/isotp/isotp_types.h"
#include "uds/IVI_Diagnostic.h"
//#include <check.h>
//#include <stdint.h>
//#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "main.h"
 can_state Can_state ;
 extern ADC_HandleTypeDef hadc1;
 extern ADC_HandleTypeDef hadc2;
 extern ADC_HandleTypeDef hadc3;
 extern  DAC_HandleTypeDef hdac;

//extern CAN_HandleTypeDef hcan1;
//#include "Cpu.h"
///#include "CAN_TASK_IF.H"
extern UART_HandleTypeDef huart3;
uint8_t Padding_byte = 0xAA;

 IsoTpShims SHIMS;
 IsoTpReceiveHandle RECEIVE_HANDLE;
 DiagnosticRequest request;
 DiagnosticResponse response;
  IsoTpMessage message ;
#define V_2BW
#define STOP1 (3u) /* Stop option 1       */
/*---Global-----------------------------------------------------------*/
 can_message_t recvMsg;
 can_message_t mrecvMsg;
 extern CAN_HandleTypeDef hcan1;
 CAN_TxHeaderTypeDef TxHeader;
 CAN_RxHeaderTypeDef RxHeader;
uint8_t COUNT=1;


uint8_t TxData[8] = {0,};
uint8_t RxData[8] = {0,};
uint32_t TxMailbox = 0;

bcm_IO_r  digital_IO_state;
IsoTpMessage message ;
uint8_t  cmd_log_gsm_start[]=
"GSM GPS Searching...\r\n";
uint8_t  cmd_adcKl30off[]=
"ADC";
uint8_t  cmd_play_audio[]=
"Play Audio...\r\n";
uint8_t cmd_sw_start[]=
"SW Test ...\r\n";
uint8_t cmd_sw_gps[]=
"GPS Test ...\r\n";
uint8_t  cmd_work_start[]=
"Work mode start ...\r\n";
uint8_t  cmd_work_stand[]=
"Work mode stand ...\r\n";
uint8_t  cmd_next_block[]=
"Next block ...\r\n";
uint8_t  cmd_finished_block[]=
"Finish testing ...\r\n";
uint8_t  AVT_cmd_res[]=
"AVT";
//extern CAN_HandleTypeDef hcan1;
extern DiagnosticResponse response;






void Can_Task(void const * argument) {
	request.succes = true;
    bool res;
     //  uint8_t buf[10];
      uint32_t timer_network_scan =  0;
      uint32_t audio_routine_scan =  0;

	  HAL_CAN_Start(&hcan1);
	  Can_state.state = 0x0;
   // #ifdef V_2BW
     Can_state.version = ISNT_M_plus;
    // #endif

       memset(&Can_state.SRN[0] , 0 , sizeof(Can_state.SRN));

       Can_state.io_test =1;

       memset(&Can_state.txbuf[0] , 0 , sizeof(Can_state.txbuf));
       memset(&Config , 0 , sizeof(Config));

      Init_Pins();

      cons.gate = xSemaphoreCreateRecursiveMutex();
  //    cons.start_test_cmd22 = xSemaphoreCreateBinary();

	for (;;) {



		Power_Monitor();


    if(Can_state.start_test_cmd22  && Can_state.power_is_on)
   //xSemaphoreTake(cons.start_test_cmd22, portMAX_DELAY);
		{


	switch (Can_state.state)
		{

	/*  case Init_Test:
		   memset(&Can_state.txbuf[0] , 0 , sizeof(Can_state.txbuf));
		   memset(&Config , 0 , sizeof(Config));
		   
		break;*/
		
	case Start_Test:

		   memset(&fft_pResult, 0, sizeof(fft_pResult));
	      	fft_pResult.mute_adc_off = InOutADC_Data(&hadc2, 2);
				if (Can_state.version == ISNT_M_min ||Can_state.version == ISNT_24) {
					if (fft_pResult.mute_adc_off > MUTE_Mmin_OFF_MIN
							&& fft_pResult.mute_adc_off < MUTE_Mmin_OFF_MAX) {
						fft_pResult.mute_type_ok = 1;
					}
				}
				if (Can_state.version == ISNT_M_plus || Can_state.version == AVT04) {
					if (/*fft_pResult.mute_adc_off > MUTE_Mplus_OFF_MIN
							&& */fft_pResult.mute_adc_off < MUTE_Mplus_OFF_MAX) {
						fft_pResult.mute_type_ok = 1;
					}
				}


     Can_state.Config = Get_SN(&Can_state);


	if(Autorization_test(Can_state.Config))
	{

		CAN_UDS_Diagnostic_IO_test(1, 2, 1);

		if(HW_test()){
	            ConsoleWrite (&cmd_sw_start[0],   sizeof(cmd_sw_start) );
	         	Can_state.state +=1;

		}
	}


			     break;

		case Get_Who_Am_I_Accelerometer:




		  if(CAN_UDS_Diagnostic_Req_Send(&message, OBD2_CMD_READ , P99_group , Get_Who_Am_I_Accelerometer,  3))
    				Can_state.state +=1;
	     break;

	     case Get_Who_Am_I_AudioCodec:
	    	 if(CAN_UDS_Diagnostic_Req_Send(&message, OBD2_CMD_READ , P99_group, Get_Who_Am_I_AudioCodec,  3))
	    	 				Can_state.state +=1;
		break;

	     case Get_SIM_ICCID:
	    	 if(CAN_UDS_Diagnostic_Req_Send(&message, OBD2_CMD_READ , 0xFD , Get_SIM_ICCID,  3))
	    		   	Can_state.state +=1;
	    	 break;

	     case Get_SIM_IMSI:
	   	   if(CAN_UDS_Diagnostic_Req_Send(&message, OBD2_CMD_READ , P99_group , Get_SIM_IMSI,  3))   //0x4
	   	    		 Can_state.state += 2;
	   	    	 break;




	     case  	Get_Microphone_ADC:
	   //   if(CAN_UDS_Diagnostic_Req_Send(&message, OBD2_CMD_READ , P99_group , Get_Microphone_ADC,  3))    //0x6
	     	    	 	Can_state.state +=1;
	     	    		   	    	 break;

	     case  	Get_KL30_ADC:

	   	   	Can_state.state +=1;
	   	 	 break;
	     case  	Get_BATTERY_ADC :
	     	   //	    if(CAN_UDS_Diagnostic_Req_Send(&message, OBD2_CMD_READ , P99_group , Get_BATTERY_ADC,  3))  // 0x8
	     	 Can_state.state +=1;

	     	   	   break;


	     case  	Get_Set_digital_IO_state :


	    	 Config.error_state = Can_state.err;

	    	 Can_state.state +=1;
	    	    	     break;

	     case  	Get_GSM_IMEI :

	   	    	 if(CAN_UDS_Diagnostic_Req_Send(&message, OBD2_CMD_READ , GSM04_group , 0x11,  3)){  // multi
	   	    	   	Can_state.state +=1;
	   	    	 }
	   	    	    	    break;

	        case Get_serial_number:

            AVT_cmd_Send(AVT_HW_Cmd);

	    	  Can_state.state += 1;

	    				break;

	      case Get_Microphone_Record:


	          	  Can_state.state +=1;
	   	    	   break;



			case Get_searching_networks:

				memset(&Can_state.txbuf[0], 0, sizeof(Can_state.txbuf));
				GSM_GPS_Get_Networks(TIMEOUT_40s, Check_number_networks);

				AVT_cmd_Send(AVT_GSM_Cmd);

				Can_state.state += 1;

				break;

			case Get_activate_block:

				timer_network_scan = xTaskGetTickCount();
				do {

					vTaskDelay(500);

					if (!CAN_UDS_Diagnostic_Req_Send(&message, OBD2_CMD_READ,
							P99_group, Get_VBAT, 3)) {
						Config.VBat = 0;
						Can_state.state += 1;
					} else {
						Config.VBat |= (0x1 << 15);
						break;
					}
				} while ((xTaskGetTickCount() - timer_network_scan) < 2000);
				Can_state.state += 1;

				break;

			case Get_GNSS_fix:

				GSM_GPS_Get_Networks(TIMEOUT_30s, Check_GNSS_fix);
				Can_state.state += 1;
				break;

			case Get_Sat:

				if (Config.GNSS_fix == 0) {
					GSM_GPS_Get_Networks(TIMEOUT_30s, Check_GNSS_fix);
				}

				if (CAN_UDS_Diagnostic_Req_Send(&message, OBD2_CMD_READ,
						P99_group, Check_Satellites, 3));



				AVT_cmd_Send(AVT_GPS_Cmd);

				vTaskDelay(200);

				Can_state.state += 1;
				//}

				break;

			case Play_Microphone_Record:

				//   {

				res = 0;
				Can_state.num = 0;

				Can_state.time = 6;
				//	fft_pResult.mute_adc =   	InOutADC_Data( &hadc2, 2);  //!!!!!mute
				audio_routine_scan = xTaskGetTickCount();
				do {

					res = Test_Audio_Rec(Can_state.time);

					if (res == true)
						break;
					vTaskDelay(TIMEOUT_1s);
					if (Check_AVT04_Test() == false)
						break;
				} while ((xTaskGetTickCount() - audio_routine_scan)
						< TIMEOUT_30s);

				// Can_state.num += 100;

				ConsoleWrite(&cmd_play_audio[0], sizeof(cmd_play_audio));

				vTaskDelay(TIMEOUT_1s);
				audio_routine_scan = xTaskGetTickCount();

				do {
					res = Test_Audio_Play(Can_state.time);

					if (res == true) {
						//vTaskDelay(TIMEOUT_1s * Can_state.time);
						break;
					}

					vTaskDelay(TIMEOUT_1s);
				} while ((xTaskGetTickCount() - audio_routine_scan)
						< TIMEOUT_30s);
				MX_ADC3_Init(ADC_CHANNEL_15);
				// adc current
				if (fft_pResult.mute_type_ok) {
					fft_pResult.mute_adc_on = InOutADC_Data(&hadc2, 2);
				}
				HAL_GPIO_WritePin(MIC_ON_Port, MIC_ON, GPIO_PIN_SET);


				AVT_cmd_Send(AVT_Audio_Cmd);

				//    }
				Can_state.state += 1;

				break;

			case Kl30_off:

				if (CAN_UDS_Diagnostic_Req_Send(&message, OBD2_CMD_RESET, 0x62,
						0x0, 2)) {
					vTaskDelay(1000);
				}
				Can_state.disable_test_mode = 1;

				Can_state.start_test_cmd22 = 0;

				ConsoleWrite(&cmd_work_start[0], sizeof(cmd_work_start));
				Can_state.state = 0;

				Init_Pins();

				break;
			}

		}
		//  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET);
		vTaskDelay(100);
	}

}

bool ConsoleWrite(uint8_t *pcString, int len) {

	xSemaphoreTake(cons.gate, portMAX_DELAY);
	if (HAL_UART_Transmit(&huart3, pcString, len, 300) == HAL_OK) {
		xSemaphoreGive(cons.gate);
		return true;

	} else {
		xSemaphoreGive(cons.gate);
		return false;

	}
}




status_t IVI_Send_Frame(	can_message_t *message)
{
	status_t status;

	      while(HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0);
	 			           if(HAL_CAN_AddTxMessage(&hcan1, &TxHeader, &message->data[0], &TxMailbox) != HAL_OK)
		 			           {
		 			             status = HCan_ERR;

		 			           }
      status =  HCan_OK;

	return status;
}
	// USER CODE END 5
uint32_t CAN_UDS_Diagnostic_Req_Send(IsoTpMessage *message,
										uint32_t arbitration_id, uint8_t subindex, uint32_t cmd , uint8_t size )
{

     uint32_t res = 0;
		//uint8_t offset;
		  can_message_t handle = {
					            .cs =  RxHeader.ExtId,//0U,
					            .id =  RxHeader.StdId,//778,
					            .length = RxHeader.DLC,
		                               };

			    	 message->size = size;
                     if(size > 7)
			 	          message->multi_frame = true;
                     else

                    	  message->multi_frame = false;
		//	 offset = CAN_UDS_Diagnostic_Positive_Response_Single_Data_Read_99_Group( &mes[0], &response,  cmd  );

			 			if (message->multi_frame == false) {

			 				memset(message->payload, 0, CAN_MESSAGE_BYTE_SIZE);
			 				message->payload[0] = ((PCI_SINGLE) | (size ));
			 				message->payload[1] = arbitration_id ;
			 				request.arbitration_id = message->payload[1];
			 				message->payload[2] = subindex; /////
			 				request.subindex = message->payload[2];
			 				message->payload[3] = cmd; //positive response;


			 			    request.succes = true;
			 				request.completed = true;
			 				memset(&message->payload[size + 1], Padding_byte, (7 - size) );
			 				//memset(&message->payload[4],   (8 - message->size), 0xAA );
                              if((arbitration_id == OBD2_CMD_SEC) && ( subindex == GSM04_group) ){
                            	            message->payload[4] = 0x11;}

                              //// Test Mode ON
                              if((arbitration_id == OBD2_CMD_WRITE)
                            		  && ( subindex == P99_group)
                            		  && ( cmd == 0x0B))
									  {

                              message->payload[4] = Can_state.disable_test_mode;

									  }
                             if((arbitration_id == OBD2_CMD_WRITE)
                                        && ( subindex == P99_group)
                                        && ( cmd == Get_Set_digital_IO_state)){

                                         message->payload[4] = digital_IO_state.Byte_status_IO;
                                                                   }

                              if((arbitration_id == OBD2_CMD_ROUTINE)
                            		  &&  (Can_state.state == Get_Microphone_Record)){
                                       message->payload[4] = 0x04;
                                       message->payload[5] = 0x04;
                                       message->payload[6] = Can_state.time;
                              }
                              if((arbitration_id == OBD2_CMD_ROUTINE)
                                  &&  (Can_state.state == Play_Microphone_Record)){
                                               message->payload[4] = 0x04;
                                               message->payload[5] = 0x05;

                                                            }
                              if((arbitration_id == OBD2_CMD_ROUTINE)
                            		  &&  (Can_state.state == Start_Test)){
                                      message->payload[4] = 0x03;
                                      message->payload[5] = 0x04;
                              }

			 		      	}

		                    while(HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0);
			                if(HAL_CAN_AddTxMessage(&hcan1, &TxHeader, &message->payload[0], &TxMailbox) != HAL_OK)
			                {

			         	      HAL_Delay(50);
			                }

			           response.completed  = false;
			         //  while (response ->completed == false)
			           for(int resp = 0 ; resp < 50; resp++)
			           {
			        	//  if(Check_AVT04_Test()== false)
			        	//	  return 0;
			        	   Power_Monitor();
			             if(HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader, handle.data) == HAL_OK  )
			           	              {
			        	         if(CAN_UDS_Request_Parse (  message,   &handle ,  &request,
			        	   				  		&response ,
			        	 						arbitration_id) )

			                     	   {
			        	         	  if( response.completed == true)
			        	          	    {
			        	         		 if(request.arbitration_id == OBD2_CMD_ROUTINE && handle.data[5] == 0xFF)
			        	         		 	    {
			        	         		 	       response.completed = false;
			        	         		 	       response.success = false;
			        	         		 	       return  response.success ;
			        	         		 	                                               }
			        	         		 CAN_UDS_Diagnostic_Positive_Response_Single_Data_Read_99_Group( message->payload, &response,subindex , cmd  );

			        	         		 res = 1;
			        	                return res;
			        	        	    }


			        	         	  /////////if responce

			        	             } //if parse

			           	        }   //if rx
			                        else  {
			                        	vTaskDelay(100);
			         			          resp++;
			         			     //      if(Check_AVT04_Test()== false)
			         			       //  		 return 0;
			                        }
			           }  // for
  	return res;

   }

 bool Get_SN(can_state *cfg)
 {
       uint32_t timer_network_scan;

	   char *str;
	   cfg-> Config = false;

	   timer_network_scan = xTaskGetTickCount() ;
	  	do {
	   		 vTaskDelay(TIMEOUT_1s );
          if (CAN_UDS_Diagnostic_Req_Send(&message, OBD2_CMD_READ, 0xF1, 0x8C, 3)) {

        	 str = strstr((char *) cfg-> SRN, "20");
			if (str) {

				return true;
			}

				else  return false;//cfg-> Config = false;*/
                 }

          } while ((xTaskGetTickCount() - timer_network_scan) < 60000);

		return false; // cfg-> Config;
}

bool Autorization_test(bool cfg)
{



	Can_state.disable_test_mode = 0;


			  if(CAN_UDS_Diagnostic_Req_Send(&message,   OBD2_CMD_SEC  , 0x03 , 0x0,  2))
			  {
				    if(CAN_UDS_Diagnostic_Req_Send(&message, OBD2_CMD_SEC , 0x04 , 0x9,  4)){
			   	  // 	CAN_UDS_Diagnostic_Req_Send(&message, OBD2_CMD_WRITE , P99_group , 0x0B,  4);

				            CAN_UDS_Diagnostic_Req_Send(&message, OBD2_CMD_ROUTINE , 0x01 , 0x0,  5);
				            	return true;// 31  !!
				   		//	 	Can_state.state +=1;

				         }
				    return false;

			  }

	return false;
 }

bool GPIO_IO_check(  uint8_t event , bool positive)
{
	//uint8_t state;
	bool res;
   uint8_t event_1 = event ;

      memcpy(&Can_state.txbuf[0], "HW Test ", 8);

     Hex_Str(event_1, &Can_state.txbuf[8]);
     for(int num = 0; num < 5; num ++)
    	 ADC_chan.aInputs[num] = 0;

     	ConsoleWrite(&Can_state.txbuf[0] , 10);
	   CAN_UDS_Diagnostic_IO_test(1, 2, 1);      ///on-off mode !!!
   // Power_Monitor();

	     switch (event)

	     {
	     case  kl30cur_test:  //


	    	 CAN_UDS_Diagnostic_IO_test(1, 0, 1);

	    	/* if(Can_state.version == AVT04){
	    		 Config.kl30_adc =  InOutADC_Data( &hadc1, 0);
	    	    res = Validate_Config(ADC_chan.aInputs[0], KL3016VADC_MIN, KL3016VADC_MAX);
	    	 }
	    	 else
	    	 {*/
	    		 Config.kl30_adc = InOutADC_Data( &hadc3, 4);
	    		 //Config.kl30_adc = InOutADC_Data( &hadc3, 4);
	    		 res = Validate_Config(Config.kl30_adc, KL3016VADC_CUR_MIN, KL3016VADC_CUR_MAX);
	    //	 }
	    	 MX_ADC3_Init(ADC_CHANNEL_9);

	   break;

	     case  kl3016V_test:


	    	 if(CAN_UDS_Diagnostic_Req_Send(&message, OBD2_CMD_READ , P99_group ,Get_KL30_ADC,  3))
	    	 {

	    		 Config.kl30_16v = 	Config.kl30;
	    		 if(Can_state.version == AVT04){
	    		      res = Validate_Config(Config.kl30, KL3016V_MIN, KL3016V_MAX);}
	    		 else if(Can_state.version == ISNT_24){
	    			 res = Validate_Config(Config.kl30, KL3016V_ISNT_24_MIN, KL3016V_ISNT_24_MAX);}

	    		 else
	    			 res = Validate_Config(Config.kl30, KL3016V_ISNT_MIN, KL3016V_ISNT_MAX);

	    	 }
	    	 else
	    		 res= false;
	    	break;

	     case kl308V_test:


	    	 HAL_GPIO_WritePin(LOW_VOL_ON_Port, LOW_VOL_ON, GPIO_PIN_RESET);

	     		if (CAN_UDS_Diagnostic_Req_Send(&message, OBD2_CMD_READ, P99_group,
	     		        Get_KL30_ADC, 3))
	     		{

	     			vTaskDelay(TIMEOUT_1s);
	     		//	Config.kl30 = InOutADC_Data( &hadc1, 0);

	     	     	HAL_GPIO_WritePin(LOW_VOL_ON_Port, LOW_VOL_ON, GPIO_PIN_SET);
	     		    Config.kl30_8v = 	Config.kl30;

	     		//	HAL_GPIO_WritePin(LOW_VOL_ON_Port, LOW_VOL_ON, GPIO_PIN_SET);
	     			 if(Can_state.version == AVT04){
	     			 res = Validate_Config(Config.kl30_8v, KL308V_MIN, KL308V_MAX);
	     			 	 }

	     			 else  if(Can_state.version == ISNT_24){
	     			 res = Validate_Config(Config.kl30_8v, KL308V_ISNT_24B_MIN, KL308V_ISNT_24B_MAX);
	     					 }
	     			 else
	     			 {res = Validate_Config(Config.kl30_8v, KL308V_ISNT_MIN, KL308V_ISNT_MAX);}

	     		}
	     		else
	     			res = false;
	     		HAL_GPIO_WritePin(LOW_VOL_ON_Port, LOW_VOL_ON, GPIO_PIN_SET);
	     		break;


	case kl15_test:

		CAN_UDS_Diagnostic_IO_test(1, 2, 1);

	if(CAN_UDS_Diagnostic_Req_Send(&message, OBD2_CMD_READ , P99_group , Get_Set_digital_IO_state, 3))
			    		  {
			  digital_IO_state.io_ign_btn.ignition_in = (Config.IO_ign_state & (1 << 4)) >> 4;


			    		  }
	    else res = false;
		if( digital_IO_state.io_ign_btn.ignition_in )
		{
			res = true;

		      	}
	   		else
		         res= false;
		//}

		break;

	case battery_test:

		   if(CAN_UDS_Diagnostic_Req_Send(&message, OBD2_CMD_READ , P99_group ,  Get_BATTERY_ADC,  3))
			    	 	    	 {
			   res = Validate_Config(Config.batt_adc, BATT_MIN, BATT_MAX);


			    	 	    	 }
		   else
			   res = false;

		break;

	case mic_test:


			if (CAN_UDS_Diagnostic_Req_Send(&message, OBD2_CMD_READ, P99_group,
			        Get_Microphone_ADC, 3))
			         {
            if(Can_state.version == AVT04){

        		 HAL_GPIO_WritePin( BUT_CUR_ON_Port,  BUT_CUR_ON, GPIO_PIN_RESET);  //
        		// vTaskDelay(1500);
        		 if (CAN_UDS_Diagnostic_Req_Send(&message, OBD2_CMD_READ, P99_group,
        		 			        Get_Microphone_ADC, 3))
        		 {
        			 if(Config.mic_adc > MIC_UDS_AVT04_OFF){
        				 HAL_GPIO_WritePin( BUT_CUR_ON_Port,  BUT_CUR_ON, GPIO_PIN_SET);  // off
        				 vTaskDelay(1700);
        			 if (CAN_UDS_Diagnostic_Req_Send(&message, OBD2_CMD_READ, P99_group,
        			         		 			        Get_Microphone_ADC, 3))
        			 {
        					res = Validate_Config(Config.mic_adc, MIC_UDS_AVT04_MIN, MIC_UDS_AVT04_MAX);

        			 }
        			 else res= false;
        			 }
        			 else res = false;

        		 }

             }
             else   // ISNT
             {res = Validate_Config(Config.mic_adc, MIC_UDS_ISNT_MIN, MIC_UDS_ISNT_MAX);}
	      //   res = false;
			}

			break;





	case sos_btn_test:


		// HAL_GPIO_WritePin( BUT_CUR_ON_Port,  BUT_CUR_ON, GPIO_PIN_SET);  // off


		if (CAN_UDS_Diagnostic_Req_Send(&message, OBD2_CMD_READ, P99_group,
		        Get_Set_digital_IO_state, 3)) {
			digital_IO_state.io_sos_btn.sos_btn_in = ((Config.IO_sos_state
						        & (1 << Sos_btn_in))) >> Sos_btn_in;

			if(!digital_IO_state.io_sos_btn.sos_btn_in){
			HAL_GPIO_WritePin(SOS_ON_Port, SOS_ON, GPIO_PIN_SET);   // ON
						vTaskDelay(500);


     	if (CAN_UDS_Diagnostic_Req_Send(&message, OBD2_CMD_READ, P99_group,
								        Get_Set_digital_IO_state, 3)) {

			digital_IO_state.io_sos_btn.sos_btn_in = ((Config.IO_sos_state
			        & (1 << Sos_btn_in))) >> Sos_btn_in;


			  if(digital_IO_state.io_sos_btn.sos_btn_in)
				  res = true;
					    else
			 res = false;


						}
                 	else res = false;
	    	}
			else res = false;
		}
		HAL_GPIO_WritePin(SOS_ON_Port, SOS_ON, GPIO_PIN_RESET);   // ON
		break;

	case led_cur_test:


	    CAN_UDS_Set_IO_test(1, 0, 0);
	    CAN_UDS_Set_IO_test(1, 5, 0);

		 if(Can_state.version == AVT04){
			 Config.led_pwr= InOutADC_Data( &hadc2, 1);
             res = Validate_Config(Config.led_pwr, LEDADC_LOW_MIN, LEDADC_LOW_MAX);}
		 else
		 {
			 Config.led_pwr= InOutADC_Data( &hadc1, 0);
		 res = Validate_Config(Config.led_pwr, LEDADC_LOW_ISNT_MIN, LEDADC_LOW_ISNT_MAX);}


		 break;


	case led_red_test:

		    CAN_UDS_Diagnostic_IO_test(1, 4, 1);

			CAN_UDS_Diagnostic_IO_test(1, 0, 1);


				 digital_IO_state.io_ign_btn.red_mode_out = ((Config.IO_ign_state  & (1 << Service_ctrl_out))) >> Service_ctrl_out;
				 if(digital_IO_state.io_ign_btn.red_mode_out)
				 {
					 if(Can_state.version == AVT04){
						 Config.led_pwr_red= InOutADC_Data( &hadc2, 1);
						res = Validate_Config(/*ADC_chan.aInputs[1]*/Config.led_pwr_red , LEDADC_HIGH_MIN,LEDADC_HIGH_MAX);
					 }
					 else{
						 Config.led_pwr_red= InOutADC_Data( &hadc1, 0);
						 res = Validate_Config(/*ADC_chan.aInputs[1]*/Config.led_pwr_red , LEDADC_HIGH_ISNT_MIN,LEDADC_HIGH_ISNT_MAX);
					 }
				 }
				 else
					 res = false;


				  CAN_UDS_Diagnostic_IO_test(1, 0, 0);
				  CAN_UDS_Diagnostic_IO_test(1, 4, 0);
					// CAN_UDS_Set_IO_test(1, 0, 0);
		           //  }

				//  CAN_UDS_Set_IO_test(0, 4, 0);



		break;


	case led_green_test:

		if(Can_state.version == AVT04){
		   CAN_UDS_Diagnostic_IO_test(1, 5, 1);

				   InOutADC_Data( &hadc2, 1);
				  Config.led_pwr_green = ADC_chan.aInputs[1];

				 digital_IO_state.io_sos_btn.green_mode_ctrl_out =
						 ((Config.IO_sos_state  & (1 << Green_mode_ctrl_out ))) >> Green_mode_ctrl_out ;
				if( digital_IO_state.io_sos_btn.green_mode_ctrl_out)
				{
					res = Validate_Config(ADC_chan.aInputs[1] , LEDADC_HIGH_MIN,LEDADC_HIGH_MAX);
				}
				else res = false;

				   CAN_UDS_Set_IO_test(1, 5, 0);

		}
		else
		{
			res = true;
			ADC_chan.aInputs[1]=0xff;
		}
			break;


	case  diag_spn_in:

			if (CAN_UDS_Diagnostic_Req_Send(&message, OBD2_CMD_READ, P99_group,
			        Get_Set_digital_IO_state, 3)) {
				digital_IO_state.io_ign_btn.diag_spn_in = ((Config.IO_ign_state
							        & (1 << Diag_sos_btn_in))) >> Diag_sos_btn_in;

				HAL_GPIO_WritePin(SPK_ON_Port, SPK_ON, GPIO_PIN_RESET);   // ON
							vTaskDelay(500);

			if(!digital_IO_state.io_ign_btn.diag_spn_in){
	    	if (CAN_UDS_Diagnostic_Req_Send(&message, OBD2_CMD_READ, P99_group,
									        Get_Set_digital_IO_state, 3)) {

			     digital_IO_state.io_ign_btn.diag_spn_in = ((Config.IO_ign_state
										        & (1 << Diag_sos_btn_in))) >> Diag_sos_btn_in;

				//   uaprintf(4 , 2 , &Can_state.txbuf[0]);
				  if(digital_IO_state.io_ign_btn.diag_spn_in)
					   res = true;
						    	  else
				    	 res = false;

							}
	                     	else res = false;
		    	   }
						else res = false;
			}
		//	HAL_GPIO_WritePin(SPK_ON_Port, SPK_ON, GPIO_PIN_SET);   // ON
			break;

	case  kl30_off:



		break;

	default:
		break;
	}


	if(res)
		memcpy(&Can_state.txbuf[0], " OK\r\n", 5);
	else
		memcpy(&Can_state.txbuf[0], " NO\r\n", 5);
	//HAL_UART_Transmit(&huart3, &Can_state.txbuf[0], 5, 100);
	ConsoleWrite(&Can_state.txbuf[0] , 5);


	return res;
}

   void uaprintf(uint8_t test , uint8_t ch , uint8_t *lpPack)
   {

	   	uint32_t offset = 0;
		set_uint32((lpPack + offset), Config.kl30_adc, 1);
		offset += 4;


		set_uint32((lpPack + offset), Config.batt_adc, 1);
		offset += 4;

		set_uint32((lpPack + offset), Config.mic_adc, 1);
		offset += 4;

		set_uint32((lpPack + offset), ADC_chan.aInputs[ch], 1);
				offset += 4;
		*(lpPack + offset) = ADC_chan.aInputs[ch];
	   	offset += 1;
	   	*(lpPack + offset)  = Config.IO_ign_state;//digital_IO_state.io_ign_btn.ignition_in;
	   	offset += 1;

		*(lpPack + offset)  = Config.IO_sos_state;//digital_IO_state.io_ign_btn.ignition_in;
		   	offset += 1;

	  // 	*(lpPack + offset) = Egts_Vehicle.Stat;
	   	offset += 1;

    }



bool CAN_UDS_Diagnostic_IO_test(uint8_t single, uint8_t event, bool positive) {

	if(single && positive)

		digital_IO_state.Byte_status_IO |= (0x1U << event);

	if(single && !positive)

			digital_IO_state.Byte_status_IO &= ~(1 << event);;


	if (CAN_UDS_Diagnostic_Req_Send(&message, OBD2_CMD_WRITE, P99_group,
	        Get_Set_digital_IO_state, 4))   // 2e
	        {
		  if(Can_state.version == AVT04)
			  vTaskDelay(1000);
		 else
			 vTaskDelay(300);
		if (CAN_UDS_Diagnostic_Req_Send(&message, OBD2_CMD_READ, P99_group,
		        Get_Set_digital_IO_state, 3));

	}

	return 0;
}


bool CAN_UDS_Set_IO_test(uint8_t single, uint8_t event, bool positive) {

	if(single && positive)

		digital_IO_state.Byte_status_IO |= (0x1U << event);

	if(single && !positive)

			digital_IO_state.Byte_status_IO &= ~(1 << event);


	else

		digital_IO_state.Byte_status_IO =  event;


	if (CAN_UDS_Diagnostic_Req_Send(&message, OBD2_CMD_WRITE, P99_group,
	        Get_Set_digital_IO_state, 4))   // 2e
	        {
		return true;

	}

	return false;
}


bool CAN_UDS_Diagnostic_Response_Wait(DiagnosticResponse *resp, uint32_t cmd , uint32_t timeout) {


  //  status_t status;
    resp->success = false;
    can_message_t handle = {
  					   .cs = RxHeader.ExtId,//0U,
  					   .id = RxHeader.StdId,//0x778,
  					   .length = RxHeader.DLC,// 8,

  					  };

    uint32_t timer_start = xTaskGetTickCount();

	do {
		vTaskDelay(3);

	   if(HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader, handle.data) == HAL_OK)
		    {
 	  if((resp->arbitration_id == cmd))
			  	{
			  		resp->attempts = 0;
			  		resp->success = true;
			  		return resp->success;
			  				}
			  if((resp->flow == true) & (cmd == PCI_FLOW_CONTROL_FRAME)  )
			  {
				  resp->success = true;
				  return resp->success;
			  }

	        }



	} while ((xTaskGetTickCount() - timer_start) < timeout);

	return (resp->success);

}


void DefaultCFGx01(/*sys_config *cfg*/)
{

	                 TxHeader.StdId = 0x768;
				 	  TxHeader.ExtId = 0;
				 	  TxHeader.RTR = CAN_RTR_DATA; //CAN_RTR_REMOTE
				 	  TxHeader.IDE = CAN_ID_STD;   // CAN_ID_EXT
				 	  TxHeader.DLC = 8;
				 	  TxHeader.TransmitGlobalTime = 0;

				 	  RxHeader.StdId = 0x778;
				 	  RxHeader.ExtId = 0;
				 	  RxHeader.RTR = CAN_RTR_DATA; //CAN_RTR_REMOTE
				 	  RxHeader.IDE = CAN_ID_STD;   // CAN_ID_EXT
				 	  RxHeader.DLC = 8;
				 	  request.BS=1;
				      request.STmin = 0;




}


		void set_uint32(unsigned char* lpDestBuf, const uint32_t ulVal, bool endian)
			{
				if (endian) // big endian (The highest bit first)
				{
					*(lpDestBuf++) = (unsigned char)((ulVal >> 24) & 0xFF);
					*(lpDestBuf++) = (unsigned char)((ulVal >> 16)& 0xFF);
					*(lpDestBuf++) = (unsigned char)((ulVal >> 8) & 0xFF);
					*(lpDestBuf++) = (unsigned char)(ulVal & 0xFF);
				}
				else            // litle endian (the lowest bit first)
				{
					*(lpDestBuf++) = (unsigned char)(ulVal & 0xFF);
					*(lpDestBuf++)  = (unsigned char)((ulVal >> 8) & 0xFF);
					*(lpDestBuf++)  = (unsigned char)((ulVal >> 16) & 0xFF);
					*(lpDestBuf++) = (unsigned char)((ulVal >> 24) & 0xFF);
				}
			}

		 uint32_t get_uint32( const unsigned char* lpBuf, _Bool endian)
			{
			uint32_t	ulVal = 0L;
				if (endian) // big endian
				{
					ulVal |= *lpBuf;
					ulVal <<= 8;
					ulVal |= *(lpBuf + 1);
					ulVal <<= 8;
					ulVal |= *(lpBuf + 2);
					ulVal <<= 8;
					ulVal |= *(lpBuf + 3);
				}
				else            // litle endian
				{
					ulVal |= *(lpBuf + 3);
					ulVal <<= 8;
					ulVal |= *(lpBuf + 2);
					ulVal <<= 8;
					ulVal |= *(lpBuf + 1);
					ulVal <<= 8;
					ulVal |= *lpBuf;
				}
				return ulVal ;
			}


		 bool Validate_Config(uint32_t val, uint32_t min, uint32_t max){
			 bool res = false;
			res = (val < max && val > min) ? true : false ;

				 return res;

		 }

	 bool Test_Audio_Rec(uint8_t timer) {

	HAL_GPIO_WritePin(SPK_ON_Port, SPK_ON, GPIO_PIN_RESET);


   	Can_state.state = Get_Microphone_Record;

	if (CAN_UDS_Diagnostic_Req_Send(&message, OBD2_CMD_ROUTINE, 0x01, 0x0, 6))
	{
     	if (HAL_DAC_Start(&hdac, DAC_CHANNEL_1) != HAL_OK) {

			Error_Handler();}




		Play_Audio(timer,/* 100 + Can_state.num */100 );
		// 31  !!

		HAL_DAC_Stop(&hdac, DAC_CHANNEL_1);

       return true;

	}

	else
        return false;
}


	 bool Test_Audio_Play(uint8_t timer) {


		Can_state.state = Play_Microphone_Record;
		   if (CAN_UDS_Diagnostic_Req_Send(&message, OBD2_CMD_ROUTINE, 0x01, 0x0, 5)) ///{	// 31  !!
			   {

			   vTaskDelay(2300);
			   InOutADC_Data_Fast(&hadc3, 1);

			   return true;
			   }

			//   else return false;

		//}


	  return false;
	}

	 bool Check_AVT04_Test(){

	 if( Can_state.start_test_cmd22 == false)
	 	 	{
	 	 		Can_state.state = 0;
	 	 		memset(&Can_state.txbuf[0] , 0 , sizeof(Can_state.txbuf));
	 	 		//vTaskDelay(5000);
	 	 		return false;

	 	 	}
	 else return true;
	 }

	 bool HW_test(void)
	 {

	 	for(int test = 0 ; test <= 10; test ++)
	 	{
	 		Power_Monitor();
	 		//if(Can_state.start_test_cmd22 == false)   return false;
	 		if(Check_AVT04_Test() == true){

	 		 if(!GPIO_IO_check( test, 0))
	 			 Can_state.err |= ( 1 << test );

	 		}
	 		else return false;
	 	}

	       return true;
	 	}



	bool  Power_Monitor(void)
	 {
		Check_AVT04_Test();



		if( (Button_read()== true) && (Can_state.power_is_on == false) )

		{
			Can_state.power_is_on = true;
			Can_state.power_up = true;
			//Can_state.state = 0;
			ConsoleWrite (&cmd_next_block[0],    sizeof(cmd_next_block) );
	//		Print_Loading();
		//	Can_state.start_test_cmd22 = 1;

		}



		if((Button_read()== false)   && (Can_state.power_is_on == true))
				{
			 			Can_state.power_is_on = false;
			 			Can_state.power_up = false;
			 			Can_state.state = 0;
			 			Can_state.start_test_cmd22 = 0;
			 			ConsoleWrite (&cmd_finished_block[0],    sizeof(cmd_finished_block) );
			 			Init_Pins();
			 		}
	 		 return Can_state.power_is_on;
	 	//	else return false;
	 }

	bool  Button_read(void)
	{

			if(HAL_GPIO_ReadPin(BUTTON_ISNT_ON_Port, BUTTON_ISNT_ON) == GPIO_PIN_SET)
					 return true;
			else
				return true;//false;


	}
 uint32_t GSM_GPS_Get_Networks(uint32_t timeout , uint8_t cmd)
 {
	uint32_t  timer_network_scan = 0;
	timer_network_scan = xTaskGetTickCount();

				do {

					vTaskDelay(TIMEOUT_1s);
					if(Check_AVT04_Test() == false)
							   return 0;
					if (CAN_UDS_Diagnostic_Req_Send(&message, OBD2_CMD_READ, P99_group,  /*Check_number_networks*/cmd,   3)) {   // 31  !!
						if ((cmd == Check_number_networks) && (MicNet.GSM_network != 0x0))

								return MicNet.GSM_network;


							if((cmd == Check_GNSS_fix) && Config.GNSS_fix !=0)
							return Config.GNSS_fix;
					}

				} while ((xTaskGetTickCount() - timer_network_scan) < timeout);
				return 0;
 }
	bool AVT_cmd_Send(uint32_t cmd)
	{
		uint32_t uOffs;
		bool res;
		memcpy(&Can_state.txbuf[0], AVT_cmd_res, sizeof(AVT_cmd_res));
		uOffs = 3;
		uint32_t cmd_str= 0x30 +cmd;
		memcpy(&Can_state.txbuf[uOffs ], &(cmd_str), 1);
		uOffs += 1;
		switch (cmd)
	{
		case AVT_HW_Cmd:
		memcpy(&Config.SRN[0], &Can_state.SRN[0] , 16 );

		memcpy(&Can_state.txbuf[uOffs ], &Config, sizeof(Config));
		res = ConsoleWrite (&Can_state.txbuf[0],  sizeof(Config) + 4);
		break;
		case AVT_GSM_Cmd:

		memcpy(&Can_state.txbuf[uOffs], &MicNet.GSM_network, 1);
		uOffs +=1;
							//HAL_UART_Transmit(&huart3, &Can_state.txbuf[0], 4, 100);
		res =  ConsoleWrite (&Can_state.txbuf[0], uOffs);
				   break;

		case AVT_GPS_Cmd:


			memcpy(&Can_state.txbuf[uOffs + 2], &Config.GNSS_fix, 1);
			memcpy(&Can_state.txbuf[uOffs + 3], &Config.Sat_num, 1);
			uOffs += 4;
			res =  ConsoleWrite (&Can_state.txbuf[0], uOffs);

			   break;



		case  AVT_Audio_Cmd:
			  // memcpy(&Can_state.txbuf[0], "Sound", 5);
		    	 memcpy(&Can_state.txbuf[uOffs], &fft_pResult.estimate, 1);
		     	 memcpy(&Can_state.txbuf[uOffs + 1], &fft_pResult.f1, 4);
		    	   uOffs += 5;

		    	 memcpy(&Can_state.txbuf[uOffs ], &fft_pResult.mute_adc, 4);

			   uOffs += 4;

			   memcpy(&Can_state.txbuf[uOffs ], &Config.VBat, 2);
			   uOffs += 2;
			    ConsoleWrite (&Can_state.txbuf[0], uOffs);
			    break;

	}
		return res;
	}




	void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
	{
	   if(GPIO_Pin == GPIO_PIN_13) // если прерывание поступило от ножки PA9
	   {
			ConsoleWrite (&cmd_finished_block[0],    sizeof(cmd_finished_block) );


	   }
	}

