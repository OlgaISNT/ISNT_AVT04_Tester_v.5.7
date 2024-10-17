#ifndef _IVI_DIAGNOSTIC_H_
#define _IVI_DIAGNOSTIC_H_
#ifdef __cplusplus
 extern "C" {
#endif

#include <stdbool.h>
#include <inttypes.h>



//#define DIAGNOSTIC_ON    ( 0x1 << 28 )
//#define DIAGNOSTIC_OFF    ~( 0x1 << 28 )

#define FLOW_CONTROL_ON    ( 0x1 << 31 )
#define FLOW_CONTROL_OFF    ~( 0x1 << 31 )


#define ANSWER_OK_ON    ( 0x1 << 30 )
#define ANSWER_OK_OFF   ~( 0x1 << 30 )

#define ANSWER_NO_ON       ( 0x1 << 29 )
#define ANSWER_NO_OFF       ~( 0x1 << 29 )


 typedef enum
  {
  NO_DATA =0,
  LGF = 1,
  LGF_SPORT = 2,
  LGF_CROSS = 3,
  KGF = 4,
  KGF_CROSS = 5,
  LGF_ph2 = 6,
  LGF_ph2_SPORT = 7,
  LGF_ph2_CROSS = 8,
  KGF_ph2 = 9,
  KGF_ph2_CROSS = 10,
  BGA = 11,
  BGA_CROSS = 12,
  CGD = 13,
  LGD = 14,
  LGD_SPORT = 15,
  BGD = 16,
  KGD = 17,
  KGD_CROSS = 18,
  KGE = 19,
  FGE = 20
  }CAR_TYPE;                     //ECU Configuration.Car body type



 typedef enum
  {
 Missing = 0,
 Is_present = 1
  }presence;
 // extern cnf_ecu  CNF_ECU;
 // extern cnf_car  CNF_CAR;

#ifdef __cplusplus
}
#endif

#endif
