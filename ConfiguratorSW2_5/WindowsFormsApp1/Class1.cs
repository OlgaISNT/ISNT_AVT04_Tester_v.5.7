using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WindowsFormsApp1
{

    public class Settings
    {
        
        public Int32 Version;
        public const int ISNT_M_plus = 0;
        public const int ISNT_M_min = 1;
        public const int AVT_04 = 2;
        public const int ISNT_24V = 3;

    }

    public class rx
    {
        public UInt32[] UbyteArray = new UInt32[200];
        public UInt32 Start_timer;
        public UInt32 Audio_Test;
        public String SN_str;
        public String ICCID_str;
    }
      public class state{
     
         public const int Start_Test = 0;
          public const int HW_Test = 1;
          public const  int SW_Test = 2;
          public const int Cops_Test = 3;
          public const int Errors_Test = 4;
          public const int Mute_Limit = 1400;
          public const int Mute_Limit_24V = 3500;
          public const int Mute_Limit_ISNT = 300;
          public const int Mute_Limit_ISNT_PLUS_MIN = 1100;
          public const int Mute_Limit_ISNT_PLUS_MAX = 4000;

          public const int Mute_Limit_ISNT_MIN_MIN = 200;
          public const int Mute_Limit_ISNT_MIN_MAX = 450;

          public const int Mute_Limit_ISNT_MIN = 400;
         public const int Uart_Timeout = 20000;
         public const int F0_Timeout = 1100;
         public const int HW_Lenght = 89 + 1;
         public const int SW_Lenght = 12 + 4;
          // public string
    }


    public class hw_1
    {
        //   public UInt32 kl30_adc;
        //    public UInt32[] ICCID = new UInt32[200];
        public Byte estimate;
        public String ICCID_scan = "";
        public String ICCID_scanner = "";
        public UInt32 kl30;

        public UInt32 kl30_16v;
        public UInt32 kl30_8v;


        public UInt32 mic_adc;
        public UInt32 kl30_adc;
        public UInt32 batt_adc;
        public UInt32 HW_error_state;
        public UInt32 IO_sos_state;
        public UInt32 IO_ign_state;
        public UInt32 Led_Pwr;
        public UInt32 Led_Pwr_Green;
        public UInt32 Led_Pwr_Red;
        public UInt32 kl30_OFF_adc;




        public UInt32 Vbat_Supply;
        public UInt32 GNSS_fix;
        public UInt32 Sat_num;

        public UInt32 Error_code;

        public UInt32 ACodec_whoami;
        public UInt32 Acell_whoami;
        public UInt32 Audio_Test;
        public UInt32 Net_Num;
        public UInt32 f1;
       
        public UInt32 mute_adc;

   
    }




}

   
