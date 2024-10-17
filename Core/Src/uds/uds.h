#ifndef __UDS_H__
#define __UDS_H__

#include "uds_types.h"
#include <stdint.h>
#include <stdbool.h>
#include "FreeRTOS.h"
#include "semphr.h"
//#include "IVI_Can_Matrix.h"


#define OBD2_FUNCTIONAL_BROADCAST_ID 0x7df
#define OBD2_FUNCTIONAL_RESPONSE_START 0x7e8

#define OBD2_FUNCTIONAL_RESPONSE_COUNT 8
#define OBD2_CMD_WRITE 0x2E
#define OBD2_CMD_READ 0x22
#define OBD2_CMD_ROUTINE 0x31
#define OBD2_CMD_SEC 0x27

#define OBD2_CHERRY_ID 0x768
#define OBD2_CMD_RESET 0x11


#define OBD2_ANSWER_READ_OK   0x62
#define OBD2_ANSWER_WRITE_OK   0x6E
#define OBD2_ANSWER_OK         0

#define OBD2_ANSWER_NO 0x7F

#define ASN_ADDRES_UDS 0x02

#define GSM04_group 0x4
#define P99_group  0x99

#define IVI_TIMEOUT_ANSWER_NO 10000
#define IVI_TIMEOUT_ANSWER_OK 10000
#define IVI_TIMEOUT_FLOW_CONTROL 3000
#define TIMEOUT_1s 1000
#define TIMEOUT_30s 30000
#define TIMEOUT_40s 40000
#define TIMEOUT_50s 50000
//#define STmin 10
#define N_As  25
#define  STATUS_SUCCESS 0

#define PCI_FIRST_FRAME_OFFSET 5
#define PCI_SINGLE_OFFSET 4
#define CMD62_OFFSET 3
#define IVI_QUERY_ATTEMPTS 5
#define AINP_AMNT  5

#define IVI_QUERY_ATTEMPTS 5

typedef struct can
{
	unsigned char error;
	unsigned long id;
	unsigned char data[8];
	unsigned char length;
	unsigned char format;
	unsigned char type;
	unsigned char cs;
}can_message_t;

extern can_message_t can_message;


typedef enum
{
	HCan_OK,
	HCan_ERR,
	HCan_BUSY,

}
status_t;
extern status_t status;

#ifdef __cplusplus
extern "C" {
#endif

/* Public: Initialize an DiagnosticShims with the given callback functions.
 *
 * If any callbacks are not to be used, set them to NULL. For documentation of
 * the function type signatures, see higher up in this header file. This struct
 * is a handy encapsulation used to pass the shims around to the various
 * diagnostic_* functions.
 *
 * Returns a struct with the fields initailized to the callbacks.
 */
/*DiagnosticShims diagnostic_init_shims(LogShim log,
        SendCanMessageShim send_can_message,
        SetTimerShim set_timer);*/

/* Public: Generate a new diagnostic request, send the first CAN message frame
 * and set up the handle required to process the response via
 * diagnostic_receive_can_frame(...).
 *
 * shims -  Low-level shims required to send CAN messages, etc.
 * request -
 * callback - an optional function to be called when the response is receved
 *      (use NULL if no callback is required).
 *
 * Returns a handle to be used with diagnostic_receive_can_frame to complete
 * sending the request and receive the response. The 'completed' field in the
 * returned DiagnosticRequestHandle will be true when the message is completely
 * sent. The first frame of the message will already be sent.
 */
DiagnosticRequestHandle diagnostic_request(DiagnosticShims* shims,
        DiagnosticRequest* request, DiagnosticResponseReceived callback);

/* Public: Generate the handle for a new diagnostic request, but do not send any
 * data to CAN yet - you must call start_diagnostic_request(...) on the handle
 * returned from this function actually kick off the request.
 *
 * shims -  Low-level shims required to send CAN messages, etc.
 * request -
 * callback - an optional function to be called when the response is receved
 *      (use NULL if no callback is required).
 *
 * Returns a handle to be used with start_diagnostic_request and then
 * diagnostic_receive_can_frame to complete sending the request and receive the
 * response. The 'completed' field in the returned DiagnosticRequestHandle will
 * be true when the message is completely sent.
 */
DiagnosticRequestHandle generate_diagnostic_request(DiagnosticShims* shims,
        DiagnosticRequest* request, DiagnosticResponseReceived callback);

/* Public: Send the first frame of the request to CAN for the handle, generated
 * by generate_diagnostic_request.
 *
 * You can also call this method to re-do the request for a handle that has
 * already completed.
 */
void start_diagnostic_request(DiagnosticShims* shims,
                DiagnosticRequestHandle* handle);

/* Public: Request a PID from the given arbitration ID, determining the mode
 * automatically based on the PID type.
 *
 * shims -  Low-level shims required to send CAN messages, etc.
 * pid_request_type - either DIAGNOSTIC_STANDARD_PID (will use mode 0x1 and 1
 *      byte PIDs) or DIAGNOSTIC_ENHANCED_PID (will use mode 0x22 and 2 byte
 *      PIDs)
 * arbitration_id - The arbitration ID to send the request to.
 * pid - The PID to request from the other node.
 * callback - an optional function to be called when the response is receved
 *      (use NULL if no callback is required).
 *
 * Returns a handle to be used with diagnostic_receive_can_frame to complete
 * sending the request and receive the response. The 'completed' field in the
 * returned DiagnosticRequestHandle will be true when the message is completely
 * sent.
 */
DiagnosticRequestHandle diagnostic_request_pid(DiagnosticShims* shims,
        DiagnosticPidRequestType pid_request_type, uint32_t arbitration_id,
        uint16_t pid, DiagnosticResponseReceived callback);

/* Public: Continue to send and receive a single diagnostic request, based on a
 * freshly received CAN message.
 *
 * shims -  Low-level shims required to send CAN messages, etc.
 * handle - A DiagnosticRequestHandle previously returned by one of the
 *      diagnostic_request*(..) functions.
 * arbitration_id - The arbitration_id of the received CAN message.
 * data - The data of the received CAN message.
 * size - The size of the data in the received CAN message.
 *
 * Returns true if the request was completed and response received, or the
 * request was otherwise cancelled. Check the 'success' field of the handle to
 * see if it was successful.
 */
DiagnosticResponse diagnostic_receive_can_frame(DiagnosticShims* shims,
        DiagnosticRequestHandle* handle,
        const uint32_t arbitration_id, const uint8_t data[],
        const uint8_t size);

/* Public: Parse the entier payload of the reponse as a single integer.
 *
 * response - the received DiagnosticResponse.
 */
int diagnostic_payload_to_integer(const DiagnosticResponse* response);

/* Public: Render a DiagnosticResponse as a string into the given buffer.
 *
 * response - the response to convert to a string, for debug logging.
 * destination - the target string buffer.
 * destination_length - the size of the destination buffer, i.e. the max size
 *      for the rendered string.
 */
void diagnostic_response_to_string(const DiagnosticResponse* response,
        char* destination, size_t destination_length);

/* Public: Render a DiagnosticRequest as a string into the given buffer.
 *
 * request - the request to convert to a string, for debug logging.
 * destination - the target string buffer.
 * destination_length - the size of the destination buffer, i.e. the max size
 *      for the rendered string.
 */
void diagnostic_request_to_string(const DiagnosticRequest* request,
        char* destination, size_t destination_length);

/* Public: For many OBD-II PIDs with a numerical result, translate a diagnostic
 * response payload into a meaningful number using the standard formulas.
 *
 * Functions pulled from http://en.wikipedia.org/wiki/OBD-II_PIDs#Mode_01
 *
 * Returns the translated value or 0 if the PID is not in the OBD-II standard or
 * does not use a numerical value (e.g. VIN).
 */
float diagnostic_decode_obd2_pid(const DiagnosticResponse* response);

/* Public: Returns true if the "fingerprint" of the two diagnostic messages
 * matches - the arbitration_id, mode and pid (or lack of pid).
 */
bool diagnostic_request_equals(const DiagnosticRequest* ours,
        const DiagnosticRequest* theirs);

/* Public: Returns true if the request has been completely sent - if false, make
 * sure you called start_diagnostic_request once to start it, and then pass
 * incoming CAN messages to it with diagnostic_receive_can_frame(...) so it can
 * continue the ISO-TP transfer.
 */
bool diagnostic_request_sent(DiagnosticRequestHandle* handle);

typedef enum {
	Check_number_networks = 0x10,
	Select_sim_slot = 0xb,
	Check_GNSS_fix = 0x11,
    Check_Satellites = 0x12,
    Get_VBAT = 0x13
}cmds;

typedef enum {
	Init_Test = 0xff,
	Start_Test = 0x0,
	Get_Who_Am_I_Accelerometer = 0x1, // 2
	Get_Who_Am_I_AudioCodec = 0x2,  //3
	Get_SIM_ICCID = 0x3,   //4
	Get_SIM_IMSI = 0x4,   	//5

//	Get_Microphone_Record = 0x5,


	Get_Microphone_ADC = 0x6,
	Get_KL30_ADC = 0x7,          // 8
	Get_BATTERY_ADC = 0x8,

	Get_Set_digital_IO_state = 0x9,
	//Set_digital_IO_state = 0x9,

	Get_GSM_IMEI = 0xa,


	Get_serial_number = 0xb,
	Get_Microphone_Record = 0xc,

//	Start_searching_networks = 0xf,
	Get_searching_networks = 0xd,

	Get_activate_block = 0xe,

		Get_GNSS_fix = 0xf,
		Get_Sat = 0x10,

	Play_Microphone_Record = 0x11,

	//Set_serial_number = 0x12,

	Kl30_off = 0x12,
	//Disable_Test_flag = 0x13,
	Reset = 0x14,

} modes;

typedef enum Gpio_test_byte {
	            kl30cur_test = 0,
	    	    kl3016V_test = 1,
	    	    kl308V_test = 2,
	    	    kl15_test  = 3,
	    	    battery_test = 4,
	    	    mic_test = 5,
	    	   sos_btn_test = 6,
	    	   led_cur_test = 7,
	    	   led_red_test = 8,
	    	   led_green_test = 9,
	    	 //  en_audio_test  = 10,
	    	   diag_spn_in = 10,
	    	   test_flag = 11,
	    	   kl30_off = 12,



	}Gpio_test_byte;


	typedef enum AVT_cmd_enum {
		            AVT_HW_Cmd = 0,
		            AVT_GSM_Cmd = 1,
		            AVT_GPS_Cmd  = 2,
		            AVT_Audio_Cmd   = 3,

		}AVT_cmd;


typedef enum Events_state_byte {

	    	    On_charge_out = 0,
	    	    Sos_ctrl_out = 1,
	    	    Green_mode_ctrl_out = 2,
	    	   Service_ctrl_out = 3,
	    	   Service_btn_in = 4,
	    	   en_audio_out = 5,
	    	   Diag_sos_btn_in  = 6,
	    	   Sos_btn_in  = 7



	}Events_state_byte;

	typedef enum {
		AVT04 = 0x2,
		ISNT_24 = 0x3,
		ISNT_M_plus = 0x0,
		ISNT_M_min = 0x01
		// 2
	}versions;

typedef enum {

	ASN_MSG_NULL    	= 0x0,    	   ///< ���������� ���
	ASN_SUBINDEX     	= 0x02,
	ASN_SET_RS232_THREAD = 0x02,
	ASN_SET_RS232         = 0x01,
	ASN_DATA_RS232       = 0x03,

	ASN_SET_RS485_THREAD= 0x12,
	ASN_SET_RS485       = 0x11,

	ASN_SET_CAN2_THREAD= 0x22,
	ASN_SET_CAN2       = 0x21,




	IVI_MSG_ECU_INPUT = 0xA0,
	IVI_MSG_ECU_Configuration = 0xA3,
    IVI_MSG_UNKNOWN    = 0xEFFFFF,



	ECU_Configuration_cmd = 0x04,
	Speaker_in_hands_free_mode_cmd = 0x13,
	Time_delay_for_turning_off_the_image_cmd = 0x1A,
	Time_delay_for_turning_on_the_image_cmd  = 0x19,
	Comfort_Sound_Level  					=  0x1C,
	SD_TIMEOUT_TIMER 						= 0x1D,				//SD TIMEOUT TIMER
	SLEEP_TIMEOUT_TIMER 					= 0x1E,			  // SLEEP TIMEOUT TIMER.CNF_SLEEP
	Voltage_threshold_to_reduce_CNF_OFF		= 0x1F,
	Enhancing_the_color_difference_signal_U  =  0x17          //Enhancing the color difference signal U of the rear view camera


} IVI_Desay_Objects_t;

typedef enum{
	ASN_MSG_RAW        = 0x220001,    ///< ������ ������� RAW (������ 0x2200 ���-������ 01)
	ASN_MSG_BBDATA     = 0x221002,    ///< ������ ������� BlackBox (������ 0x2210 ���-������ 02)
} ASN_SDO_Objects_t;

//int CAN_UDS_diagnostic_parse(can_message_t  *handle);
//uint32_t CAN_UDS_diagnostic_cmd_parse(can_message_t  *handle,
	//							uint16_t cmd, uint8_t pck_num) ;


#ifdef __cplusplus
}
#endif



 /*------------------------------------------------------------------------------
  *
  *  DTOOL_to_Radio struct
  */
 typedef struct dtool_to_radio_t {
 //	cancfg cfg;
 	uint16_t period;
 	uint64_t Radio_to_DTOOL;
 } dtool_to_radio_t;

 /*------------------------------------------------------------------------------
  *  BCM_CANHS_R_04 struct
  */
 typedef struct IOState_t {
 	uint16_t mS_timer;                         //

 	uint16_t dOutput;                        //
 	uint16_t cntr_mask;
 	uint8_t reguest_mode;
 //	ds_t dOutputs[DOUT_AMNT];//
 //	ds_t dInputs[DINP_AMNT];                 //
 	uint32_t aInputs[AINP_AMNT];                 //
 //	as_t counter[CINP_AMNT];                 //


 	//ds1820_t ds[DS_AMNT];
 //	as_t aJ1939[J1939_A_AMNT];               // J1939
 	//ds_t dJ1939[J1939_D_AMNT];               //  J1939
 } IOState;

 typedef struct  bcm_IO_r{
 //	cancfg cfg;

  	uint16_t period;
  	uint32_t rx;
     uint8_t Byte_status_IO;

     struct  byte1{
    	     unsigned sos_btn_in : 1;
    	     unsigned diag_sos_btn_in : 1;
    	     unsigned en_audio_out : 1;
    	     unsigned service_btn_in : 1;
    	     unsigned service_ctrl_out : 1;
    	     unsigned green_mode_ctrl_out : 1;
    	     unsigned sos_ctrl_out : 1;
    	     unsigned on_charge_out: 1;
     	} io_sos_btn ;


        struct  byte2{
     	         unsigned on_off_mode : 1;
     	         unsigned diag_spn_in : 1;
     	         unsigned wd_out : 1;
     	         unsigned ignition_in : 1;
     	         unsigned red_mode_out : 1;
          	} io_ign_btn;



 } bcm_IO_r;


 typedef struct set_r {

    uint32_t kl30;
 	uint32_t kl30_adc;
 	uint32_t kl30_16v;
 	uint32_t kl30_8v;
 	uint32_t mic_adc;
 	uint32_t batt_adc;

 //	uint32_t batt_uds;
 	//uint32_t mute_adc;

 	uint32_t led_pwr;

 	uint32_t led_pwr_red;
 	uint32_t led_pwr_green;
 	uint16_t error_state;
 	uint8_t  IO_sos_state;
 	uint8_t  IO_ign_state;
 	//uint8_t activate;
 	uint16_t VBat;

 	uint8_t GNSS_fix;
 	uint8_t Sat_num;

 	uint8_t ACodec_whoami;
 	uint8_t Acell_whoami;
 	uint8_t ICCID[8];
 	uint8_t IMSI[8];
 	uint8_t GSM_IMEI[8];
 	uint8_t SRN[16];



 } Set_r;



typedef struct bcm_mic_r{
    	//cancfg cfg;
    	uint32_t rx;
    	uint16_t period;
    	uint32_t adc;                         //ECU software version number
    	uint8_t GSM_network;
    	uint8_t Configured;
    	uint8_t SRN[16];

    } bcm_mic_r;



    typedef struct bcm_r{
       	//cancfg cfg;
       	uint32_t rx;
       	uint16_t period;
       	uint32_t adc;                         //ECU software version number
       	uint8_t GSM_network;
       	uint8_t Configured;
       	uint8_t SRN[16];

       } bcm_r;



       typedef struct can_stat{
       	//cancfg cfg;

       	uint8_t version;
       	bool power_is_on;
       	bool power_up;
       	bool start_test_cmd22;

       	bool start_test_cmd31;
       	bool io_test;
       	bool sn_write;
       	bool sn_read;
       	uint8_t state;
       	uint16_t period;
       	uint8_t res;
       	volatile uint32_t t1;
       	volatile uint32_t sh;
       	bool Config;
       	uint32_t err;
       	uint32_t num;
       	uint8_t GSM_network;
       	uint8_t SRN[16];
       	uint8_t txbuf[100];
       	bool test_audio;
       	bool test_audio_write;
       	bool audio;
       	bool disable_test_mode;
       	uint32_t time;//ECU software version number

       //uint8_t CNF_SW_NO[10];

       } can_state;



 extern dtool_to_radio_t DTOOL_to_Radio;
 extern Set_r Config;

// extern bcm_acell_who_r Who_Am_I_Accelerometer; // 2
// extern bcm_codec_who_r Who_Am_I_AudioCodec    ;   //3//
 //extern bcm_iccid_sim_r SIM_ICCID;    //4    multi
 //extern bcm_imsi_sim_r SIM_IMSI;          //5   multi
 //extern bcm_mic_rec_r Microphone_Record;  // 6

 extern bcm_r MicNet;            // 7
 extern IOState ADC_chan;           // 8
  extern bcm_mic_r Microphone_ADC;          //9


 extern bcm_IO_r  digital_IO_state;
// extern bcm_IO_s  digital_out_statese;


 //extern bcm_canhs_r GSM_IMEI;
// extern bcm_canhs_s serial_number;-
 extern can_state Can_state ;

 typedef struct console_io_buf {
     SemaphoreHandle_t   gate;
     SemaphoreHandle_t   gate_audio;
     SemaphoreHandle_t start_test_cmd22;
     uint32_t            ticks;
     uint16_t            len;
     uint8_t             timer;
     uint8_t             echo;

 } console_io_buf;

 extern char *mstr;
 extern console_io_buf cons;

 char *SearchStr(char *s1, int32_t l1, char *s2);
 size_t ustrlen(const char *s);
 bool Check_AVT04_Test(void);
 bool HW_test();
 bool  Power_Monitor(void);
 bool  Button_read(void);
 bool AVT_cmd_Send(uint32_t cmd);
 uint32_t GSM_GPS_Get_Networks(uint32_t timeout , uint8_t cmd);
DiagnosticRequest  Create_UDS_Diagnostic_Qreate_Request_ID(DiagnosticResponse* resp, uint32_t request_id, uint8_t subindex, uint8_t cmd);

uint32_t  CAN_UDS_Diagnostic_Positive_Response_Multi(  IsoTpMessage *message, DiagnosticResponse* resp, uint32_t subindex, uint32_t cmd);
//uint32_t IVI_Send_Diagnostic_Single_Response(
  //   								uint32_t id, uint32_t dlc, uint32_t event);
status_t CAN_UDS_Diagnostic_Send (
				    	DiagnosticRequest* req,
						DiagnosticResponse* resp );
bool CAN_UDS_Diagnostic_Response_Wait(DiagnosticResponse *resp, uint32_t cmd , uint32_t timeout) ;
//uint32_t  CAN_UDS_Diagnostic_Positive_Response_Multi(  IsoTpMessage *message, DiagnosticResponse* response,uint32_t subindex,  uint32_t cmd);
void CAN_UDS_diagnostic_Cmd_message(  IsoTpMessage *message, DiagnosticRequest* req,  uint32_t cmd);
status_t  IVI_Send_Diagnostic_Single_Response( /*IsoTpMessage *mess,*/
     								 uint32_t dlc, uint32_t event , uint32_t send );
uint32_t CAN_UDS_Diagnostic_Positive_Response_Single_Data_Read_99_Group(  uint8_t *ptr, DiagnosticResponse* resp, uint8_t group, uint8_t cmd  );
//uint32_t  CAN_UDS_Diagnostic_Parse_ECU_Configuration( /* IsoTpMessage *message*/uint8_t *ptr, DiagnosticResponse* resp);
uint32_t  CAN_UDS_Diagnostic_Positive_Response_ECU_Configuration(  uint8_t *ptr, DiagnosticResponse* resp);
uint32_t  CAN_UDS_Diagnostic_Positive_Response_Speaker_in_hands_free_mode (  uint8_t *ptr, DiagnosticResponse* resp);
uint32_t CAN_UDS_Diagnostic_Positive_Response_Single_Data_Read_F1_Group(  uint8_t *ptr, DiagnosticResponse* resp, uint8_t cmd  );
//void CAN_UDS_diagnostic_cmd_parse(can_message_t  *handle,  IsoTpMessage message, DiagnosticRequest* request, uint32_t pck_num);
bool   CAN_UDS_Request_Parse(  IsoTpMessage *mess, can_message_t  *handle ,
					     DiagnosticRequest* request,
						DiagnosticResponse* response ,
						uint32_t arbitration_id);
status_t IVI_Receive_Frame(can_message_t *message, uint32_t buf_idx, uint8_t num_can);
uint32_t CAN_UDS_Diagnostic_Req_Send(IsoTpMessage *message,uint32_t arbitration_id, uint8_t subindex, uint32_t cmd , uint8_t frame );
bool CAN_UDS_Diagnostic_IO_test( uint8_t single ,  uint8_t event , bool positive);
bool CAN_UDS_Set_IO_test( uint8_t single ,  uint8_t event , bool positive);
bool Validate_Config(uint32_t val, uint32_t min, uint32_t max);
bool GPIO_IO_check(  uint8_t event , bool positive);
bool Get_SN(can_state *cfg);
bool Autorization_test(bool cfg);
bool Test_Audio_Rec(uint8_t timer);
bool ConsoleWrite (uint8_t *pcString, int len) ;
void  Play_Audio(uint32_t sec , uint32_t freq);
bool Test_Audio_Play(uint8_t timer);
void uaprintf(uint8_t test , uint8_t ch ,  uint8_t *lpPack);
void set_uint32(unsigned char* lpDestBuf, const uint32_t ulVal, bool endian);
uint32_t get_uint32( const unsigned char* lpBuf, _Bool endian);
void Hex_Str(unsigned char hex, unsigned char *str);
#endif // __UDS_H__
