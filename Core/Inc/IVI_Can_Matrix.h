#ifndef _IVI_CAN_MATRIX_H_
#define _IVI_CAN_MATRIX_H_
#ifdef __cplusplus
 extern "C" {
#endif

#include <stdbool.h>
#include <inttypes.h>
//#include "RTE_IO_IF.h"
#include "../Src/uds/IVI_Diagnostic.h"
#include "../Src/uds/uds.h"
#define CAN_PORTS                  2
#define FFT_SIZE 512

#define MASTER
 //#define SLAVE
//#define L_IVI_XGF
//#define L-IVI_XGA


/* Definition of the TX and RX message buffers depending on the bus role */
#if defined(MASTER)
    #define TX_MAILBOX  (10UL)
    #define TX_MSG_ID   (0x3D7UL)
    #define RX_MAILBOX  (0UL)
    #define RX_MSG_ID   (0xfffUL)
#elif defined(SLAVE)
    #define TX_MAILBOX  (0UL)
    #define TX_MSG_ID   (2UL)
    #define RX_MAILBOX  (1UL)
    #define RX_MSG_ID   (1UL)
#endif


#define V_IVI_Read_Timeout     			5   //ms
//#define IVI_SLEEP_Timeout				1 //min
#define IVI_Twake_min     		    	10   //20 ms
#define IVI_Twake_max     		    	25   // 50 ms
#define IVI_Tsleep_min     		    	10   //20 ms
#define IVI_Tsleep_max     		    	25   // 50 ms
#define CAN_V_MAX_IDs      				7U
#define RX_QUEUE_SIZE			        32U
#define BUFFER_SIZE 					64U


#define	IVI_BCM_CANHS_R_04_ID  	    	0x350  	// vesta,xray
#define	IVI_BCM_CANHS_R_05_ID  	    	0x55D  	// vesta,xray


#define IVI_Radio_to_DTOOL_ID 			0x712   // receive
#define	IVI_DTOOL_to_Radio_ID 			0x732    //tx
#define	IVI_MMI_BCM_CANHS_R_01_ID 		0x5DE

#define	IVI_TCU_NMEA_01_ID 				0x42A
#define	IVI_TCU_NMEA_02_ID				0x4A6
#define	IVI_TCU_NMEA_03_ID 				0x4A8
 typedef int16_t q15_t;
 //fft_Adc_pResult_q15 fft_pResult

#define  fft_Real_Amp_min    3000
#define  fft_pResult_f1_min  1000
#define  fft_pResult_f1_max  1100

 typedef struct {

 	q15_t fft_Real_Nm1;
 	q15_t fft_Real_Nm2;
 	q15_t fft_Real_Nm3;
 	uint32_t  fft_Real_Idxm1;
 	uint32_t  fft_Real_Idxm2;
 	uint32_t  fft_Real_Idxm3;
	uint32_t  fft_Real_Pos1;
	uint32_t  fft_Real_Pos2;
	q15_t fft_DAmp[FFT_SIZE] ;
	uint32_t  f1;
	float  f2;
	uint32_t  estimate;
	uint32_t mute_adc;
	uint32_t mute_type_ok;
	uint32_t mute_adc_off;
	uint32_t mute_adc_on;
 }  fft_Adc_pResult_q15;

typedef enum
   {
 	OFF                       = 0x0U,
 	ON                        = 0x1U,
   }CAN_ACC_ON;

 typedef enum
    {
  	M_CAN                        = 0x0U,
  	V_CAN                        = 0x1U,

    }CAN_tNum;


   typedef enum
     {
	STATUS_NORMAL = 0x0U,
	STATUS_LISTENING = 0x1U,
	STATUS_STANDBY = 0x2U,
	STATUS_SLEEP = 0x3U,
} status_tMode;



typedef enum {
	Sleeping = 0x0U, /*!< Generic operation success status */
	TechnicalWakeup = 0x1U, /*!< Generic operation failure status */

	CutOffPending = 0x2U,
	BatTempoLevel = 0x3U,
	AccessoryLevel = 0x4U,
	IgnitionLevel = 0x5U,
	StartingInProgress = 0x6U,
	EngineRunning = 0x7U,
	AutoStart = 0x8U,
	EngineSystemStop = 0x9U,
	UnvailableValue = 0xfU,
} status_tVehicleStateExtented;

typedef enum {

	Tail_Gate_is_closed = 0x1U,
	Tail_Gate_is_open = 0x2U,
	NotUsed = 0x3U,
} status_tTailGateStatus;

typedef enum {

	Driver_door_closed = 0x1U,
	Driver_door_open = 0x2U
} status_tDriverDoorState;




typedef enum {
	fuel_level_OK = 0x0U, /*!< Generic operation success status */
	fuel_level_low = 0x1U, /*!< Generic operation failure status */

} status_tFuelLevel;



typedef struct  brake_canhs_r_01{
	//cancfg cfg;
     uint32_t rx;
	uint16_t period;
	uint16_t VehicleSpeed;
} brake_canhs_r_01;
/*------------------------------------------------------------------------------
 *  CLUSTER_CANHS_RNr_01 struct
 */
typedef struct  cluster_canhs_rnr_01{

	uint32_t rx;
	uint8_t HandBrakeSwitch ;
	uint8_t FuelLow  ;


} cluster_canhs_rnr_01;

/*------------------------------------------------------------------------------
 *  CLUSTER_CANHS_R_04 struct
 */
typedef struct  cluster_canhs_r_04{

	  uint32_t rx;

	uint8_t ExternalTemp;
	//uint8_t NightRheostatedLightMaxPercent;
	//unsigned DayNightStatusForBacklights :1;

} cluster_canhs_r_04;

/*------------------------------------------------------------------------------
 *   SWA_CANHS_RNr_01 struct
 */
typedef struct swa_canhs_rnr_01 {
	uint32_t rx;
	uint16_t SteeringWheelAngle;
	uint16_t SteeringWheelAngle_Offset;

} swa_canhs_rnr_01;//SWA_CANHS_RNr_01;

/*------------------------------------------------------------------------------
 *   CLUSTER_CANHS_R_01 struct
 */
typedef struct cluster_canhs_r_01 {
	uint32_t rx;
	uint8_t FuelLevelDisplayed;

} cluster_canhs_r_01 ;





 typedef struct uartcfg_t {
	uint32_t speed;
	uint8_t word_length;
	uint8_t XON;
	uint8_t XOFF;
	uint8_t parity ;
	uint8_t stop_length ;
	uint8_t flow_control ;
	uint8_t port_use ;
	uint8_t echo_mode ;
	uint8_t device ;
	uint8_t rxBuf[ BUFFER_SIZE];
	uint8_t rxLen;
	uint8_t txBuf[ BUFFER_SIZE];
	uint8_t txLen;
} uartcfg_t;


typedef struct {
	 status_tMode Mode;
	uint8_t Radio_to_DTOOL;
	uint32_t CAN_timer_activity;
	uint32_t CAN_timer_idle;
	uint32_t rx_err;
	uint32_t tx_err;
	uint32_t rx_ID;
	uint8_t rx_mes[8];
    uint32_t rx;
    uint32_t tx;
	uint8_t rx_state;

} can_events;

typedef struct sys_events {


	can_events eCAN[CAN_PORTS];
    uint32_t pcktSource;
    uint8_t  saving_tmr;
    long int Radio_to_DTOOL;
    uint8_t diagnostic_session;
    uint8_t restart_device;
    uint8_t device_cfg;
    uint8_t con_echo;
    uint8_t usb_bridge;
    volatile unsigned uart_echo;
	uint8_t mode;
	bool BTP_enable;

	bool CAN_ACC_ON_SIGNAL ;
} sys_events;

// �������� �������� CAN
typedef struct {
	// �������� ����
	uint32_t bitrate;

	uint32_t StdId;

	uint32_t Rtr;
	uint32_t StdEx;
	uint32_t IVI_Sleep_Timeout;

} can_settings;


typedef struct {
   // CANSTM_HWAttrs      *hw_info;                   // ��������� �� ���������� �������� ����������
   // SemaphoreHandle_t   mutex;
  //  SemaphoreHandle_t   trComplete;
   // SemaphoreHandle_t   rxComplete;   				// ������� ������ ������ �� ����
   // QueueHandle_t       rx_queue;                   // ��������� �� ������� ������

} CAN_Object;

typedef struct  {


	can_settings sCAN[CAN_PORTS];
	/*status_tType */CAR_TYPE Type;
} sys_config;
///



  uint8_t CNF_REF[10];



extern  CAN_Object *handle;
extern can_settings sCAN;
extern can_events eCAN;
//extern  radio_to_dtool Radio_to_DTOOL;
//extern dtool_to_radio_t DTOOL_to_Radio;
//extern bcm_canhs_r_04  BCM_CANHS_R_04;
//extern bcm_canhs_r_05  BCM_CANHS_R_05;


extern fft_Adc_pResult_q15 fft_pResult;
extern uartcfg_t Uart;
extern sys_events Evnt;
extern sys_config Cfg;
//extern IVI_CAN_HWAttrs CAN_HWAttrs[];
void Settings_Set(void);
 void CanTask_TX( void *pvParameters );
 void CanTask_RX( void *pvParameters );
 void M_CAN_task_main(void *param);
 void  Init_VCAN_RxBuff(uint8_t);
 //status_t Init_MCAN_RxBuff(uint32_t ID , can_buff_config_t *buf);
 //status_t Init_MCAN_TxBuff( can_buff_config_t *buf);



uint32_t IVI_Can_Frame_Parse(
		   	   	   	   	   	    const can_message_t *message, uint32_t id,
							   	uint8_t pck_num);
status_t IVI_Send_Frame  (
							  can_message_t *message);
uint32_t  Uart_printf_binary(uint8_t *tp_data,  const can_message_t *message   );

uint32_t Uart_read(uint32_t instance,
					uint8_t * rxBuff,
					uint32_t rxSize, uint32_t timeout);

//void set_uint16(void* lpDestBuf, uint16_t usVal, _Bool endian);
//uint16_t get_uint16(void *lpBuf, _Bool endian);

//uint32_t get_uint32(void *lpBuf, _Bool endian);
//uint32_t get_uint64(void *lpBuf, _Bool endian);
void set_uint64(void *lpDestBuf, uint64_t ulVal, _Bool endian);

//void can_transceiver_to_sleep(uint8_t can);
void can_transceiver_to_normal(uint8_t can);
void can_transceiver_to_standby(uint8_t can);
//BOOL can_transceiver_get_RX(uint8_t can);
//void can_transceiver_wakeup_RX(uint8_t can);
//BOOL can_transceiver_get_ERR(uint8_t can);
//bool can_get_sleep_condition();
void DefaultCFGx01(/*sys_config *cfg*/);
bool   audio_fft (void);
uint8_t  get_VehicleStateExtented() ;
uint8_t  get_TailGateStatus();
uint8_t  get_EngineHoodState();
uint8_t  get_DriverDoorState();
uint8_t  get_PassengerDoorState();
uint8_t get_RearDoorsState();
uint8_t  get_RearGearEngaged();
uint16_t get_VehicleSpeed();
uint8_t  get_HandBrakeSwitch();
uint8_t  get_FuelLow();
uint8_t get_ExternalTemp();
uint16_t get_SteeringWheelAngle();
uint16_t get_SteeringWheelAngle_Offset() ;
uint8_t get_FuelLevelDisplayed();
uint8_t get_CfgTypeCar();




#ifdef __cplusplus
}
#endif

/**************************CAN1************************************/

#define CAN_SET_WAKE() 	 rte_io_set_pin(RTE_PORT_A, RTE_PIN_16);
#define CAN_CLR_WAKE() 	 rte_io_clear_pin(RTE_PORT_A, RTE_PIN_16);
#define CAN_SET_STB()	 rte_io_set_pin(RTE_PORT_A, RTE_PIN_15);
#define CAN_CLR_STB()	 rte_io_clear_pin(RTE_PORT_A, RTE_PIN_15);
#define CAN_SET_EN()	 rte_io_set_pin(RTE_PORT_B, RTE_PIN_8);
#define CAN_CLR_EN()	 rte_io_clear_pin(RTE_PORT_B, RTE_PIN_8);
#define CAN_GET_ERR()    rte_io_read_pin(RTE_PORT_B, RTE_PIN_10);
#define CAN_GET_INH()    rte_io_read_pin(RTE_PORT_B, RTE_PIN_9);


/**************************CAN2************************************/

#define CAN2_SET_WAKE()  rte_io_set_pin(RTE_PORT_A, RTE_PIN_0);
#define CAN2_CLR_WAKE()  rte_io_clear_pin(RTE_PORT_A, RTE_PIN_0);
#define CAN2_SET_STB()	 rte_io_set_pin(RTE_PORT_B, RTE_PIN_11);
#define CAN2_CLR_STB()	 rte_io_clear_pin(RTE_PORT_B, RTE_PIN_11);
#define CAN2_SET_EN()	 rte_io_set_pin(RTE_PORT_D, RTE_PIN_3);
#define CAN2_CLR_EN()	 rte_io_clear_pin(RTE_PORT_D, RTE_PIN_3);
#define CAN2_GET_ERR()   rte_io_read_pin(RTE_PORT_D, RTE_PIN_2);
#define CAN2_GET_INH()   rte_io_read_pin(RTE_PORT_D, RTE_PIN_4);
#define CAN2_GET_RX()    rte_io_read_pin(RTE_PORT_B, RTE_PIN_12);
#endif

