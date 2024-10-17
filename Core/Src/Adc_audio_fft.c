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
//#define ARM_MATH_CM4


#include "arm_math.h"
#include "arm_const_structs.h"

//#define TEST_LENGTH_SAMPLES 2048
#define fft_Freq_step 41.764
#define fft_Freq_offset 3

#include <arm_math.h> //подключаем библиотеку
//указываем размер FFT

//Создаём два буфера, в одном будем хранить выборки АЦП, во втором результаты БФП
q15_t fft_Sbuff[FFT_SIZE *2] = {0};   //
q15_t fft_Dbuff[FFT_SIZE*2] = {0};
//q15_t fft_DAmp[FFT_SIZE] = {0};

uint8_t status;
arm_rfft_instance_q15 S;

 fft_Adc_pResult_q15 fft_pResult;

   arm_rfft_instance_f32 rffti;
	arm_cfft_radix4_instance_f32 cffti;



	bool  audio_fft (void)
{
		 arm_status  status;
	memset (fft_pResult.fft_DAmp ,0,  sizeof (fft_pResult.fft_DAmp));
	fft_pResult.fft_Real_Idxm1 =0;
	fft_pResult.fft_Real_Nm1 =0;

	fft_pResult.fft_Real_Idxm2 =0;
	fft_pResult.fft_Real_Nm2 =0;

	fft_pResult.fft_Real_Idxm3 =0;
	fft_pResult.fft_Real_Nm3 =0;
	fft_pResult.estimate = 0;
   status = arm_rfft_init_q15(&S, FFT_SIZE, 0, 1);//функция инициализации необходима для БФП

 if(status == ARM_MATH_SUCCESS)
  {
    arm_rfft_q15(&S, fft_Sbuff, fft_Dbuff);   //выполнение БФП

   for(int idx = 0; idx < FFT_SIZE ; idx++)
   {
	   fft_Dbuff[idx] =  fft_Dbuff[idx] << 8;
   }
    arm_cmplx_mag_q15(fft_Dbuff, fft_pResult.fft_DAmp, FFT_SIZE);//вычисляем амплитуды гармоник
  //  arm_cmplx_mag_squared_f32( fft_Dbuff, fft_Sbuff, FFT_SIZE);
  }


   arm_max_q15(  &fft_pResult.fft_DAmp[1],  (uint32_t) (FFT_SIZE /2) ,   &fft_pResult.fft_Real_Nm1,
		         &fft_pResult.fft_Real_Idxm1);

   fft_pResult.fft_Real_Pos1 = fft_pResult.fft_Real_Idxm1 + fft_Freq_offset;


      arm_max_q15(  &fft_pResult.fft_DAmp[ fft_pResult.fft_Real_Pos1],   (FFT_SIZE - fft_pResult.fft_Real_Pos1),
    		     &fft_pResult.fft_Real_Nm2,
   		         &fft_pResult.fft_Real_Idxm2);

      if(fft_pResult.fft_Real_Idxm1 > fft_Freq_offset)
      arm_max_q15(  &fft_pResult.fft_DAmp[1],  ( fft_pResult.fft_Real_Idxm1 - fft_Freq_offset),
         		     &fft_pResult.fft_Real_Nm3,
        		         &fft_pResult.fft_Real_Idxm3);

      fft_pResult.f1 =  (uint32_t)fft_Freq_step * fft_pResult.fft_Real_Idxm1 ;      // 46 mks for 23.944   41.764


    		if (fft_pResult.fft_Real_Nm1 > (7 * fft_pResult.fft_Real_Nm2 /6)
    				&& fft_pResult.fft_Real_Nm1 > (7 * fft_pResult.fft_Real_Nm3 /6)
    	            &&(fft_pResult.fft_Real_Nm1  > fft_Real_Amp_min)
    	         	&& (fft_pResult.f1 > fft_pResult_f1_min)
    		         &&(fft_pResult.f1 < fft_pResult_f1_max))

    	 fft_pResult.estimate = 1;


   return true;
}



uint32_t   InOutADC_Data_Fast(ADC_HandleTypeDef *hadc, uint32_t num)
      {
	 //  ADC_ChannelConfTypeDef sConfig;
	//  84/2 = 42 Mhz  3 +12 = 15 sampling 35,714 mks  512- 18,25 ms  54,7
	 xSemaphoreTake(cons.gate_audio, portMAX_DELAY);



	 HAL_ADC_Start(hadc);
	      for(int i= 0; i < FFT_SIZE; i++)
	      {

    	  HAL_ADC_PollForConversion(hadc, 100);

          ADC_chan.aInputs[num] = HAL_ADC_GetValue(hadc);
    	  fft_Sbuff[i]=  (q15_t)ADC_chan.aInputs[num];// HAL_ADC_GetValue(hadc);//ADC_chan.aInputs[num];

	      }
	      HAL_ADC_Stop(hadc);
	      audio_fft ();
     		  xSemaphoreGive(cons.gate_audio);
      		return ADC_chan.aInputs[num];

      }
