//#include "extras.h"
#include "uds.h"
#include <string.h>
//#include "IVI_Can_Matrix.h"
#include "IVI_Diagnostic.h"


  bcm_r MicNet;            // 7
  IOState KL30_ADC;           // 8
// bcm_mic_r Microphone_ADC;          //9
Set_r Config;


uint32_t CAN_UDS_Diagnostic_Positive_Response_Single_Data_Read_99_Group(  uint8_t *ptr, DiagnosticResponse* resp, uint8_t group, uint8_t cmd  )
{
	 uint32_t offset = 0;
	 uint8_t sat_glonass;


   if( group == 0x4 ){

		 switch(cmd)
		 {
		 case	0x11: //(Get_GSM_IMEI + 1) :					// 0x22  0x04  0x11

			 memcpy(&Config.GSM_IMEI[0], (ptr + (CMD62_OFFSET)), 8 );
			     offset = 8;

							 break;

		case	0x08://Get_activate_block:					// 0x22  0x04  0x8

		//	Config.activate = *(ptr + (CMD62_OFFSET ));
			 				 	 offset = 1;

			 					 break;
			 default:
					break;

		 }
   }
   if((group == 0xF1))
		   {
	   memcpy(&Can_state.SRN[0], (ptr + (CMD62_OFFSET)), 16 );
	  						 	 offset = 16;
	  						 	 return offset;
		   }
   if((group == 0x99) ||(group == 0xFD) ){
	 switch(cmd)
	 {


	 case Get_Who_Am_I_Accelerometer:		  //	 0x01  //Enhancing the color difference signal U of the rear view camera


			Config.Acell_whoami = *(ptr + (CMD62_OFFSET ));
			offset = 1;

		 break;

	 case Get_Who_Am_I_AudioCodec:        // 0x22  0x99 0x02


		 	Config.ACodec_whoami = *(ptr + (CMD62_OFFSET ));
		 	 offset = 1;


		 break;


	 case  Get_SIM_ICCID :         // 0x22  0xFD 0x03
		 memcpy(&Config.ICCID[0], (ptr + (CMD62_OFFSET )), 8 );
	 	 offset = 8;
		 break;


	 case	Get_SIM_IMSI:					// 0x22  0x99 0x04

		 memcpy(&Config.IMSI[0], (ptr + (CMD62_OFFSET)), 8 );
			 	 offset = 8;

				 break;
		 break;

	 case   (Get_Microphone_ADC ) :         // 0x22 0x99 0x06


		 Config.mic_adc =   get_uint32(ptr + CMD62_OFFSET,   true);

		  offset = 4;

		  break;

  	 case (Get_KL30_ADC  ):  // 0x22 0x99 0x07

		 Config.kl30 =  get_uint32(ptr + CMD62_OFFSET,   true);
				  offset = 4;

		 		break;
	   case (Get_BATTERY_ADC) :                       // 0x22 0x99 0x08

   //    Config.batt_adc = *ptr;
		 Config.batt_adc  = get_uint32(ptr + CMD62_OFFSET,   true);
		  offset = 4;

		break;



	    case (Get_VBAT) :
	    	Config.VBat =    *(ptr + (CMD62_OFFSET )) << 8
	    	               | *(ptr + (CMD62_OFFSET + 1 ));
	    break;

	 case (Get_Set_digital_IO_state ) :   // 0x22 0x99 0x09
		 Config.IO_sos_state =  *(ptr + (CMD62_OFFSET ));
		 Config.IO_ign_state =  *(ptr + (CMD62_OFFSET + 1));
		 offset = 2;
     break;




  	  case 0x10:					// 0x22  0x99 0x11

		MicNet.GSM_network = *(ptr + (CMD62_OFFSET));
		  offset = 1;

				break;

	  case Check_GNSS_fix:					// 0x22  0x99 0x11

			Config.GNSS_fix = *(ptr + (CMD62_OFFSET));
			offset = 1;

			break;

		case Check_Satellites:					// 0x22  0x99 0x12

			Config.Sat_num = *(ptr + (CMD62_OFFSET));
			sat_glonass = *(ptr + (CMD62_OFFSET + 1));
			Config.Sat_num += sat_glonass;
			offset = 1;

			break;


	 default:
		break;

  	 }
   }
	  resp->mode = PCI_SINGLE;
	     resp->multi_frame = false;
	     return offset;


	}

