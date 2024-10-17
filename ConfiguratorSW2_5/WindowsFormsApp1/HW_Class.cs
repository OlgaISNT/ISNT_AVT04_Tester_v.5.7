using System;
using System.Windows.Forms;
using System.IO.Ports;
using System.Threading;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Data;
using System.Drawing;


namespace WindowsFormsApp1
{


       public partial class Form1
    {
    //    Scanner scan = new Scanner();

        unsafe public int get_uint32(int* lpBuf, UInt32 endian)
        {
            int ulVal = 0;

            ulVal |= lpBuf[0];
            ulVal <<= 8;
            ulVal |= lpBuf[1];
            ulVal <<= 8;
            ulVal |= lpBuf[2];
            ulVal <<= 8;
            ulVal |= lpBuf[3];

            return ulVal;
        }
   

        public class Error_Codes
        {
            public const int H0 = 0;
            public const int H1 = 1;
            public const int H2 = 2;
            public const int H3 = 3;
            public const int H4 = 4;
            public const int H5 = 5;
            public const int H6 = 6;
            public const int H7 = 7;
            public const int H8 = 8;
            public const int H9 = 9;
            public const int H10 = 10;
            public const int P0 = 11;
            public const int P0_min = 230;
            public const int P0_max = 4000;
            public const int B0 = 12;
            public const int B0_min = 1000;
            public const int G0 = 13;
            public const int G0_min = 1;
            public const int G0_max = 4;
            public const int G1 = 14;
            public const int G1_min = 1;
            public const int G1_max = 40;
            public const int M0 = 15;
            public const int M0_min = 150;
            public const int AC0 = 16;
            public const int AC0_val = 64;
            public const int A0 = 17;
            public const int A0_val = 51;
            public const int C0 = 18;
            public const int C0_min = 1;
            public const int F0 = 19;
            public const int S0 = 20;
            public const int SN0 = 21;
            public const int M1= 22;
            public const int B1 = 23;
            public const string P0_s = "P0";
            public const string B0_s = "B0";
            public const string G0_s = "G0";
            public const string G1_s = "G1";
            public const string M0_s = "M0";
            public const string AC0_s = "AC0";
            public const string A0_s = "A0";
            public const string C0_s = "С0";
            public const string F0_s = "F0";
            public const string S0_s = "S0";
            public const string SN0_s = "SN0";
            public const string M1_s = "M1";
            public const string B1_s = "B1";
            public const string E_spase = ", ";
        }






        int Validate_Settings()
        {
          //  UInt32 Err_num = 0;

            int error_code = 0;
            int result = 0;
            int uOffs;
            // HW_error_state = 43;
            for (uOffs = 0; uOffs < 12; uOffs++)
            {
                result = (int)HW.HW_error_state & (1 << uOffs);
                if (result != 0)
                {
                    textBox1.Text += "H";
                    textBox1.Text += uOffs;
                    textBox1.Text += ", ";


                }
            }
           
            if ((HW.kl30_adc < Error_Codes.P0_min) || (HW.kl30_adc > Error_Codes.P0_max))   // 11 bit
            {
             

                textBox1.Text += Error_Codes.P0_s + Error_Codes.E_spase;
                HW.HW_error_state |= (1 << Error_Codes.P0);
             

            }
            if ((HW.batt_adc < Error_Codes.B0_min))             // 12 bit
            {
            
                textBox1.Text += Error_Codes.B0_s + Error_Codes.E_spase;
                HW.HW_error_state |= ( 1 << Error_Codes.B0);
          
            }
            if (HW.GNSS_fix < Error_Codes.G0_min || HW.GNSS_fix > Error_Codes.G0_max)                // 13
            {
           
                HW.HW_error_state |= (1 << Error_Codes.G0);
                textBox1.Text += Error_Codes.G0_s + Error_Codes.E_spase; ;
       
            }
            if (HW.Sat_num < Error_Codes.G1_min || HW.Sat_num > Error_Codes.G1_max)     // 14
            {        
                HW.HW_error_state |= (1 << Error_Codes.G1);
                textBox1.Text += Error_Codes.G1_s + Error_Codes.E_spase; ;
                
            } 
            if (HW.mic_adc < Error_Codes.M0_min)    // 15
            {
            
                textBox1.Text += Error_Codes.M0_s + Error_Codes.E_spase; ;
                HW.HW_error_state |= (1 << Error_Codes.M0);
            }

            if (HW.ACodec_whoami != Error_Codes.AC0_val)   // 16
            {
               
                textBox1.Text += Error_Codes.AC0_s + Error_Codes.E_spase; ;
                HW.HW_error_state |= (1 << Error_Codes.AC0);

            }
            if (HW.Acell_whoami != Error_Codes.A0_val)   // 17
            {
             
                textBox1.Text += Error_Codes.A0_s + Error_Codes.E_spase; ;
                HW.HW_error_state |= (1 << Error_Codes.A0);

            }
            if (HW.Net_Num < Error_Codes.C0_min)     // 18
            {
             
                textBox1.Text += Error_Codes.C0_s + Error_Codes.E_spase; ;
                HW.HW_error_state |= (1 << Error_Codes.C0);               

            }
            if (HW.estimate !=1 )     // 20
            {
            
                textBox1.Text += Error_Codes.S0_s + Error_Codes.E_spase; ;
                HW.HW_error_state |= (1 << Error_Codes.S0);
        
            }
          /*  if ((HW.mute_adc < state.Mute_Limit) &&  (set.Version == 0))     // 21
            {
               
                textBox1.Text += Error_Codes.M1_s + Error_Codes.E_spase; ;
                HW.HW_error_state |= (1 << 22);

            }*/


            if (set.Version == 0)
            {


                if ((HW.mute_adc < state.Mute_Limit_ISNT_PLUS_MIN) || (HW.mute_adc > state.Mute_Limit_ISNT_PLUS_MAX))     // 21
                {

                    textBox1.Text += Error_Codes.M1_s + Error_Codes.E_spase; ;
                    HW.HW_error_state |= (1 << 22);

                }
            }
            if (set.Version == 1 )
            {


                if ((HW.mute_adc < state.Mute_Limit_ISNT_MIN_MIN) || (HW.mute_adc > state.Mute_Limit_ISNT_MIN_MAX))
                {

                    textBox1.Text += Error_Codes.M1_s + Error_Codes.E_spase; ;
                    HW.HW_error_state |= (1 << 22);

                }
            }

            if (set.Version == 2)
            {


                if ((HW.mute_adc < state.Mute_Limit) /*&& (set.Version == 1)*/)     // 21
                {

                    textBox1.Text += Error_Codes.M1_s + Error_Codes.E_spase; ;
                    HW.HW_error_state |= (1 << 22);

                }
            }
            if (set.Version == 3)
            {


                if ((HW.mute_adc > state.Mute_Limit_24V) /*&& (set.Version == 1)*/)     // 21
                {

                    textBox1.Text += Error_Codes.M1_s + Error_Codes.E_spase; ;
                    HW.HW_error_state |= (1 << 22);

                }
            }
            if ((HW.Vbat_Supply & (0x1 << 15)) == 1 && HW.Vbat_Supply < 1500)
            {
                textBox1.Text += Error_Codes.B1_s + Error_Codes.E_spase; ;
                HW.HW_error_state |= (1 << 23);
            }

            int res_IMEI = 0;
            int res_SN = 0;
            String mystring;
       //     Scanner scan = new Scanner();
            if (scan.QR_SN_String  != null)
            {
                res_IMEI = scan.QR_SN_String.IndexOf(dataGridView2.Rows[6].Cells[4].Value.ToString());

                // res_SN = SN_String.IndexOf(dataGridView2.Rows[9].Cells[4].Value.ToString());
                mystring = dataGridView2.Rows[9].Cells[4].Value.ToString().Substring(16 - 4);
                res_SN = scan.QR_SN_String.IndexOf(mystring);
                if (res_IMEI != -1 && res_SN != -1) //((res_SRN != -1) && (res_IMEI != -1))
                {
                    dataGridView2.Rows[10].Cells[4].Value = "OK";
                }
                else
                {
                    dataGridView2.Rows[10].Cells[4].Value = "Wrong";
                    textBox1.Text += "SNO ";
                    HW.HW_error_state |= (1 << 21);
                }
            }
            else
            {
                dataGridView2.Rows[10].Cells[4].Value = "Not Found";
                textBox1.Text += "SNO ";
                HW.HW_error_state |= (1 << 21);
            }
            // Compare
            return error_code;
        }

        Int32 Parse_RxArray(Int32 uOffs)
        {
            int idx = 0;
            HW.kl30 = Get_UInt32(uOffs);
            uOffs += 4;
            if (HW.kl30 > 10000)
            {
                MessageBox.Show(" Wrong Data, restart");
                Thread.Sleep(2000);
                this.Start = 0;
            }


            HW.kl30_adc = Get_UInt32(uOffs);
            uOffs += 4;

            HW.kl30_16v = Get_UInt32(uOffs);
            uOffs += 4;

            HW.kl30_8v = Get_UInt32(uOffs);
            uOffs += 4;
            HW.mic_adc = Get_UInt32(uOffs);
            uOffs += 4;
            HW.batt_adc = Get_UInt32(uOffs);
            uOffs += 4;

            HW.Led_Pwr = Get_UInt32(uOffs);
            uOffs += 4;
            HW.Led_Pwr_Red = Get_UInt32(uOffs);
            uOffs += 4;
            HW.Led_Pwr_Green = Get_UInt32(uOffs);
            uOffs += 4;
            
            HW.HW_error_state = Rx.UbyteArray[uOffs] | (Rx.UbyteArray[uOffs + 1] << 8);
            uOffs += 2;
            HW.IO_sos_state = Rx.UbyteArray[uOffs];
            uOffs += 1;
            HW.IO_ign_state = Rx.UbyteArray[uOffs];
            uOffs += 1;
            HW.Vbat_Supply = Rx.UbyteArray[uOffs] | (Rx.UbyteArray[uOffs + 1] << 8);
            uOffs += 2;
            HW.GNSS_fix = Rx.UbyteArray[uOffs];
            uOffs += 1;
            HW.Sat_num = Rx.UbyteArray[uOffs];
            uOffs += 1;
            HW.ACodec_whoami = Rx.UbyteArray[uOffs];
            uOffs += 1;
            HW.Acell_whoami = Rx.UbyteArray[uOffs];
            uOffs += 1;


            Get_Chars(uOffs, 19, 6);
            uOffs += 8;
            Get_Chars(uOffs, 19, 7);
            uOffs += 8;
            Get_Chars(uOffs, 19, 8);
            uOffs += 8;
            ///IMSI

            //IMSI_GSM

            for (idx = 0; idx < 16; idx++)
            {
                SRN[idx] = (Char)Rx.UbyteArray[idx + uOffs];
               
                dataGridView2.Rows[9].Cells[4].Value += SRN[idx].ToString();

            }
            return 1;
        }
        void Init_dataGrid()
        {
            for (int row = 1; row < 10; row++)
                dataGridView2.Rows.Add(row);

            for (int row = 1; row < 10; row++)
                dataGridView1.Rows.Add(row);
            textBox4.Text += "ISNT MUTE+\r\n";
            textBox4.Text += "Jetour Dashing \r\n";
            textBox4.Text += "Jetour T2 \r\n";
            textBox4.Text += "Jetour X70 \r\n";
            textBox4.Text += "Jetour T1 \r\n";

            textBox5.Text += "ISNT MUTE-\r\n";
            textBox5.Text += "SWM \r\n";
            textBox5.Text += "KAIYI\r\n";
            ///////////////////////////////////////////////

            dataGridView2.Rows[0].Cells[0].Value = "Kl30";
            dataGridView2.Rows[1].Cells[0].Value = "Kl30(16V)";
            dataGridView2.Rows[2].Cells[0].Value = "Kl30(8V)";

            dataGridView2.Rows[3].Cells[0].Value = "Kl15";
            dataGridView2.Rows[4].Cells[0].Value = "Batt";
            dataGridView2.Rows[5].Cells[0].Value = "Mic";
            dataGridView2.Rows[6].Cells[0].Value = "SOS Btn";

            dataGridView2.Rows[7].Cells[0].Value = "Led Pwr";
            dataGridView2.Rows[8].Cells[0].Value = "Led Red";
            dataGridView2.Rows[9].Cells[0].Value = "Led Green";
            dataGridView2.Rows[10].Cells[0].Value = "Spk";
            dataGridView2.Rows[11].Cells[0].Value = "Mute";
      //      dataGridView2.Rows[12].Cells[0].Value = "VBatt Supply";


            dataGridView2.Rows[0].Cells[3].Value = "COPS";

            dataGridView2.Rows[1].Cells[3].Value = "Gps_fix";
            dataGridView2.Rows[2].Cells[3].Value = "Sattelites";

            dataGridView2.Rows[3].Cells[3].Value = "AccellWhoAmI";
            dataGridView2.Rows[4].Cells[3].Value = "CodecWhoAmI";
            dataGridView2.Rows[5].Cells[3].Value = "VBatt Supply";
            dataGridView2.Rows[6].Cells[3].Value = "ICCID SIM";
            dataGridView2.Rows[7].Cells[3].Value = "IMSI SIM";
            dataGridView2.Rows[8].Cells[3].Value = "IMEI GSM";
            dataGridView2.Rows[9].Cells[3].Value = "SN";
            dataGridView2.Rows[10].Cells[3].Value = "SN&IMEI Sticker ";




            

            dataGridView1.Rows[0].Cells[0].Value = "Kl30 Volt";
            dataGridView1.Rows[1].Cells[0].Value = "Kl30(16V) Volt";
            dataGridView1.Rows[2].Cells[0].Value = "Kl30(8V) Volt";

            dataGridView1.Rows[3].Cells[0].Value = "Kl15 PIN";
            dataGridView1.Rows[4].Cells[0].Value = "Battery Volt";
            dataGridView1.Rows[5].Cells[0].Value = "Microphone Volt";
            dataGridView1.Rows[6].Cells[0].Value = "SOS Button PIN";

            dataGridView1.Rows[7].Cells[0].Value = "Led Volt";
            dataGridView1.Rows[8].Cells[0].Value = "Led Red  Volt";
            dataGridView1.Rows[9].Cells[0].Value = "Led Green Volt";
            dataGridView1.Rows[10].Cells[0].Value = "Speaker PIN";
            dataGridView1.Rows[11].Cells[0].Value = "Mute PIN";

            dataGridView1.Rows[0].Cells[1].Value = "H0,P0";
            dataGridView1.Rows[1].Cells[1].Value = "H1";
            dataGridView1.Rows[2].Cells[1].Value = "H2";

            dataGridView1.Rows[3].Cells[1].Value = "H3";
            dataGridView1.Rows[4].Cells[1].Value = "H4,B0";
            dataGridView1.Rows[5].Cells[1].Value = "H5,M0";
            dataGridView1.Rows[6].Cells[1].Value = "H6";

            dataGridView1.Rows[7].Cells[1].Value = "H7";
            dataGridView1.Rows[8].Cells[1].Value = "H8";
            dataGridView1.Rows[9].Cells[1].Value = "H9";
            dataGridView1.Rows[10].Cells[1].Value = "H10";
            dataGridView1.Rows[11].Cells[1].Value = "M1";


            dataGridView1.Rows[0].Cells[2].Value = "GSM COPS";
            dataGridView1.Rows[1].Cells[2].Value = "GPS FIX";
            dataGridView1.Rows[2].Cells[2].Value = "GPS Sattelites";
            dataGridView1.Rows[3].Cells[2].Value = "AccellWhoAmI";
            dataGridView1.Rows[4].Cells[2].Value = "CodecWhoAmI";
            dataGridView1.Rows[5].Cells[2].Value = "VBatt Supply";
            dataGridView1.Rows[6].Cells[2].Value = "SN&IMEI Sticker ";
            dataGridView1.Rows[8].Cells[2].Value = "No connect to TCU";
             dataGridView1.Rows[7].Cells[2].Value = "Audio Error";
            dataGridView1.Rows[9].Cells[2].Value = "Not programmed ";

            dataGridView1.Rows[0].Cells[3].Value = "C0";
            dataGridView1.Rows[1].Cells[3].Value = "G0";
            dataGridView1.Rows[2].Cells[3].Value = "G1";
            dataGridView1.Rows[3].Cells[3].Value = "A0";
            dataGridView1.Rows[4].Cells[3].Value = "AC0";
            dataGridView1.Rows[5].Cells[3].Value = "B1";
            dataGridView1.Rows[6].Cells[3].Value = "SN0";
            dataGridView1.Rows[8].Cells[3].Value = "F0";
            dataGridView1.Rows[7].Cells[3].Value = "S0";
            dataGridView1.Rows[9].Cells[3].Value = "U0";



        }
        void Clear_Form()
        {
            int rows = 0;
            textBox1.Clear();
            //   textBox9.Clear();
            textBox8.Clear();
            textBox1.Clear();

            radioButton2.BackColor = Color.Red;
            radioButton2.Text = "NO";
            radioButton2.ForeColor = Color.White;

            textBox2.Text = "\r\nRESULT";
            textBox2.BackColor = Color.Red;
            textBox3.Text = "\r\nAUDIO";

            do
            {
                dataGridView2.Rows[rows].Cells[1].Value = "";
                dataGridView2.Rows[rows].Cells[2].Value = "";
                rows++;
            } while (rows < 12);
            rows = 0;
            do
            {
                dataGridView2.Rows[rows].Cells[4].Value = "";
                rows += 1;
            } while (rows < 11);
        }
       void  Draw_dataGrid()
        {

            dataGridView2.Rows[4].Cells[4].Value = (Convert.ToInt32(HW.ACodec_whoami)).ToString();
            dataGridView2.Rows[3].Cells[4].Value = (Convert.ToInt32(HW.Acell_whoami)).ToString();

            dataGridView2.Rows[0].Cells[2].Value = (Convert.ToInt32(HW.kl30_adc)).ToString();
            dataGridView2.Rows[1].Cells[2].Value = (Convert.ToInt32(HW.kl30_16v)).ToString();
            dataGridView2.Rows[2].Cells[2].Value = (Convert.ToInt32(HW.kl30_8v)).ToString();
            // 3
            dataGridView2.Rows[5].Cells[2].Value = (Convert.ToInt32(HW.mic_adc)).ToString();

            //         dataGridView2.Rows[3].Cells[2].Value = (Convert.ToInt32(kl30_adc)).ToString();
            dataGridView2.Rows[4].Cells[2].Value = (Convert.ToInt32(HW.batt_adc)).ToString();

            dataGridView2.Rows[7].Cells[2].Value = (Convert.ToInt32(HW.Led_Pwr)).ToString();
            dataGridView2.Rows[9].Cells[2].Value = (Convert.ToInt32(HW.Led_Pwr_Green)).ToString();
            dataGridView2.Rows[8].Cells[2].Value = (Convert.ToInt32(HW.Led_Pwr_Red)).ToString();



            int result = 0;
            int UOffs;

            for (UOffs = 0; UOffs < 12; UOffs++)
            {
                result = (int)HW.HW_error_state & (1 << UOffs);
                if (result != 0)
                    dataGridView2.Rows[UOffs].Cells[1].Value = "Error";
                else
                    dataGridView2.Rows[UOffs].Cells[1].Value = "OK";

            }


    
            result = (int)HW.HW_error_state & (1 << 3);
            if (result != 0)
                dataGridView2.Rows[3].Cells[2].Value = "false";
            else
                dataGridView2.Rows[3].Cells[2].Value = "true";

            result = (int)HW.HW_error_state & (1 << 6);
            if (result != 0)
                dataGridView2.Rows[6].Cells[2].Value = "false";
            else
                dataGridView2.Rows[6].Cells[2].Value = "true";


            result = (int)HW.HW_error_state & (1 << 10);
            if (result != 0)
                dataGridView2.Rows[10].Cells[2].Value = "false";
            else
                dataGridView2.Rows[10].Cells[2].Value = "true";
            textBox8.Text += "GSM Test...";

        }


    }
}
