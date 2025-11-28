#undef Honewell

using System;
using System.Collections.Generic;
using System.Linq;
using System.Management;
using System.Text.RegularExpressions;
using System.IO.Ports;
using System.Windows.Forms;
using System.Text;
using System.Threading.Tasks;
using System.Threading;

using static System.Windows.Forms.VisualStyles.VisualStyleElement;
using System.Drawing;
using System.Diagnostics.Eventing.Reader;

namespace WindowsFormsApp1
{
    public partial class Form1
    {
      
        int Read_Scanner_Monitor()
        {
            int read_num = 0;
            if (scan.present == false)
                return -1;
           do 
            {

                int Qr_detected = Scanner_Monitor();
                if (Qr_detected >= 0)   return Qr_detected;  // wrong or success
                else
                {
                    Thread.Sleep(200);
                    read_num++;
                }
            }
            while( read_num < 5);
            return -1;

        }

       int Scanner_Monitor()
        {
           
            int QR_Offs = 0;
            int ICCID_len = 19;
            int SN_len = 14;
            int QR_offs_SN1 = 0;
            String textbox;
            int version;
            //        rx Rx = new rx();
            String SN_ICCID_str = null;
            // scan.QR_SN_String = null;
            //Workbook wb = new Workbook("Excel.xlsx");

         
            byte[] cmd_start = { 0x04, 0xe4, 0x04, 0x00, 0xff, 0x14 };
            byte[] cmd_stop = { 0x04, 0xe4, 0x04, 0x00, 0xff, 0x13 };

            //    7E 01 30 30 30 30 40 4F 52 54 53 45 54 35 30 30 30 3B 03


            byte[] cmd_scaner_newland_timeout = { 0x7E, 0x01, 0x30, 0x30, 0x30, 0x30 , 0x40, 0x4F, 0x52 , 0x54 , 0x53 , 0x45 , 0x54 , 0x39 , 0x30 , 0x30 , 0x30 , 0x3B , 030 };
            byte[] cmd_scaner_newland_start = { 0x7E, 0x01, 0x30, 0x30 , 0x30 , 0x30 , 0x23 , 0x53 , 0x43 ,  0x4E , 0x54 , 0x52 ,0x47 , 0x31 , 0x3B , 0x03 };
            byte[] cmd_scaner_newland_stop = { 0x7E, 0x01, 0x30, 0x30, 0x30 , 0x30 ,0x23 ,  0x53 ,  0x43,  0x4E, 0x54 ,0x52 , 0x47 ,0x30 ,0x3B ,0x03 };
            //       bool res;   
            //   byte[] cmd_start_answ = new byte[20];

            try
            {
                if (scan.type == scan.Zebra_type)
                {
                    scan.port_scan.Write(cmd_start, 0, 6);
                    scan.port_scan.ReadTimeout = 2000;
                    Thread.Sleep(2000);
                }

                if (scan.type == scan.Newland_type)
                {
                    scan.port_scan.Write(cmd_scaner_newland_timeout, 0, /*19*/cmd_scaner_newland_timeout.Length);
                    Thread.Sleep(1000);
                    scan.port_scan.Write(cmd_scaner_newland_start, 0, /*16*/cmd_scaner_newland_start.Length);
                    Thread.Sleep(1000);
                    scan.port_scan.ReadTimeout = 5000;
                 

                }

                }
            catch
            {
                //     WindowsFormsApp1.Form1.textBox8.Text += "Port Closed\r\n";
                //       MessageBox.Show( "\nScanner Port Unavailable");
                Thread.Sleep(200);
                return -1;
            }

            Rx.SN_str = null;
            Rx.ICCID_str = null;

           textbox = "Sticker Scan...\r\n";
            textBox8.AppendText(textbox);
            switch (set.Version)
            {
                case 0:
                    QR_Offs = scan.Scan_AVT04_offs + scan.Scan_ISNT_offs;
                 //   scan.ICCID_len = scan.Scan_AVT04 + scan.Scan_ISNT_offs;
                    break;
                case 1:
                    QR_Offs = scan.Scan_AVT04_offs + scan.Scan_ISNT_offs + scan.Scan_ISNT_offs_Mmin;
                 //   scan.ICCID_len = scan.Scan_AVT04 + scan.Scan_ISNT_offs + scan.Scan_ISNT_offs_Mmin;
                    break;
                case 2:
                    QR_Offs = scan.Scan_AVT04_offs_new; // + 7;
              //      scan.ICCID_len = scan.Scan_AVT04_new + 25;
                    break;
                case 3:
                    QR_Offs = scan.Scan_AVT04_offs + scan.Scan_ISNT_offs + scan.Scan_ISNT_offs_Mmin;
                 //   scan.ICCID_len = scan.Scan_AVT04 + scan.Scan_ISNT_offs + scan.Scan_ISNT_offs_Mmin;
                    break;
                default:
                    break;
            }

            if (scan.flag == true)
            {


             //   scan.port_scan.Write(cmd_scaner_newland_stop, 0, 19);
                //Thread.Sleep(1000);
                ///////////////////
                if (scan.QR_SN_String.Length == scan.Scan_AVT04_new)
                {
                    set.Version = scan.AVT04;
                    QR_Offs = scan.Scan_AVT04_offs_new;


                }
                else if (scan.QR_SN_String.Length == (scan.Scan_AVT04 + scan.Scan_ISNT_offs))
                {
                    set.Version = scan.ISNT_Mplus;
                    QR_Offs = scan.Scan_AVT04_offs + scan.Scan_ISNT_offs;
                 //   scan.ICCID_len = scan.Scan_AVT04 + scan.Scan_ISNT_offs;
                    //  comboBox2.SelectedIndex = set.Version;
                }
                else if (scan.QR_SN_String.Length == (scan.Scan_AVT04 + scan.Scan_ISNT_offs +  scan.Scan_ISNT_offs_Mmin))
                {
                    version = (int)Convert.ToInt32(scan.QR_SN_String[20]);
                    if(version == 0x31)
                     set.Version = scan.ISNT_Mmin;
                    if ((version == 0x32))
                        set.Version = scan.ISNT_24V;
                    QR_Offs = scan.Scan_AVT04_offs + scan.Scan_ISNT_offs + scan.Scan_ISNT_offs_Mmin;

                    //  comboBox2.SelectedIndex = set.Version;
                }
                ////////////////////

                comboBox2.SelectedIndex = set.Version;
                int res_SN1 = 0;

                // if (set.Version == scan.ISNT_Mmin || set.Version == scan.ISNT_24V || set.Version == scan.AVT04)
                if (scan.QR_SN_String.Length > /*59*/63)//63)
                {
                    Rx.SN_str = scan.QR_SN_String.Substring(QR_Offs, SN_len);
                    if (Rx.SN_str != null)
                    {
                        //   if (set.Version == scan.ISNT_Mmin || set.Version == scan.ISNT_24V || set.Version == scan.AVT04)
                        Rx.ICCID_str = scan.QR_SN_String.Substring(QR_Offs + 15, ICCID_len);
                        if (set.Version == scan.AVT04)
                        {
                            if (scan.QR_SN_String.Length > 120)
                            {
                                Rx.ICCID_str = scan.QR_SN_String.Substring(QR_Offs + 71, ICCID_len);
                                Rx.SN_1_str = scan.QR_SN_String.Substring(QR_Offs + 47, 6);
                                textbox = "SN_low= " + Rx.SN_1_str + "\r\n";
                                textBox8.AppendText(textbox);
                                Thread.Sleep(100);
                                res_SN1 = Rx.SN_str.IndexOf(Rx.SN_1_str);
                                if (res_SN1 <= 0)
                                    return SN1_Error();
                               
                            }
                            else
                            {
                                return SN1_Error();
                            }
                        }
                       

                        textbox = "SN= " + Rx.SN_str + "\r\n";
                        textBox8.AppendText(textbox);
                        textbox = "ICCID= " + Rx.ICCID_str;
                        textBox8.AppendText(textbox);

                        scan.flag = false;
                        //     scan.port_scan.Close();
                        Thread.Sleep(200);
                        return 1;

                    }
                        else
                           return   SN1_Error();   //  if (Rx.SN_str != null)

                    //Rx.SN_str
                }
              


                else
                {
                   
                    return SN1_Error();


                }

            }


                   else
                {
               //     scan.port_scan.Close();
                    return -1;
                }




                return -1;
        }

        public int SN1_Error()
        {
            int QR_offs_SN1 = 0;
            String textbox;
            textbox = "SN= " + Rx.SN_str + "\r\n";
            if(textbox !=null)
            textBox8.AppendText(textbox);
            Thread.Sleep(200);
            textbox = "ICCID= " + Rx.ICCID_str +"\r\n";
            if (textbox != null)
                textBox8.AppendText(textbox);
            Thread.Sleep(200);

            textBox8.Text += "\r\n" + "Wrong QR code\r\n ";
            HW.HW_error_state |= (1 << Error_Codes.SN1);
         //   this.Start = state.Errors_Test;
            port.WriteLine("End");
            return 0;
        }

        public bool Scanner_Init()
        {

            List<ComPort> ports = GetSerialPorts();
            bool res;
            //   ComPort com = ports.FindLast(c => c.vid.Equals("0x0483") && c.pid.Equals("0x374B"));
            //or if we want to extract all devices with specified values:

            //  List<ComPort> coms = ports.FindAll(c => c.vid.Equals("0C2E") && c.pid.Equals("090A"));

            List<ComPort> coms = ports.FindAll(c => c.vid.Equals("05E0") && c.pid.Equals("1701"));
            try
            {
                //    port_scan = new SerialPort(coms[0].name, 115200);
                scan.port_scan = new SerialPort(coms[0].name, 9600);
                scan.port_scan.DataReceived += new SerialDataReceivedEventHandler(mySerialPort_DataReceived);
                scan.port_scan.Open();
                scan.type = scan.Zebra_type;
                //      Form1.textBox8.Text += "Ready\r\n";
                res = true;
                // port_scan.Write(cmd_start, 0, 6);
                //port_scan.ReadTimeout = 5000;
                //Thread.Sleep(20);
            }
            catch
            {
              //     WindowsFormsApp1.Form1.textBox8.Text += "Port Closed\r\n";
              //   MessageBox.Show("\nNo scanner");


                List<ComPort> coms_newland = ports.FindAll(c => c.vid.Equals("1EAB") && c.pid.Equals("1D06"));
                try
                {
                    scan.port_scan = new SerialPort(coms_newland[0].name, 115200);
                    scan.port_scan.DataReceived += new SerialDataReceivedEventHandler(mySerialPort_DataReceived);
                    scan.port_scan.Open();
                    scan.type = scan.Newland_type; ;
                    //      Form1.textBox8.Text += "Ready\r\n";
                    res = true;
                }

                catch

                {
                    res = false;
                    Thread.Sleep(1000);
                    //return false;
                }




                //    res = false;
                //  Thread.Sleep(  1000);
                //return false;
            }




            return res;
        }
        public class Scanner
        {
            public byte[] QScanArray = new byte[80];
            public String QR_SN_String;
            public int ICCID_len;
            public bool flag;
            public bool present;
            private const string vidPattern = @"VID_([0-9A-F]{4})";
            private const string pidPattern = @"PID_([0-9A-F]{4})";
            public int Scan_AVT04 = 60;//59;
            public int Scan_AVT04_offs = 25;

            public int Scan_AVT04_new = 32 +71 + 20;//59;
            public int Scan_AVT04_offs_new = 32;


            public int Scan_ISNT_offs = 4;
            public int Scan_ISNT_offs_Mmin = 3;
            public SerialPort port_scan;
            public int ISNT_Mplus = 0;
            public int ISNT_Mmin =1;
            public int AVT04 = 2;
            public int ISNT_24V = 3;
            public int type;
            public int Zebra_type =0;
            public int Newland_type =1;
            struct ComPort // custom struct with our desired values
            {
                public string name;
                public string vid;
                public string pid;
                public string description;
               
          
                //    byte[] QScanArray;//= new byte[32];

            }


      
  
private static List<ComPort> GetSerialPorts()
            {
                using (var searcher = new ManagementObjectSearcher
                    ("SELECT * FROM WIN32_SerialPort"))
                {
                    var ports = searcher.Get().Cast<ManagementBaseObject>().ToList();
                    return ports.Select(p =>
                    {
                        ComPort c = new ComPort();
                        c.name = p.GetPropertyValue("DeviceID").ToString();
                        c.vid = p.GetPropertyValue("PNPDeviceID").ToString();
                        c.description = p.GetPropertyValue("Caption").ToString();

                        Match mVID = Regex.Match(c.vid, vidPattern, RegexOptions.IgnoreCase);
                        Match mPID = Regex.Match(c.vid, pidPattern, RegexOptions.IgnoreCase);

                        if (mVID.Success)
                            c.vid = mVID.Groups[1].Value;
                        if (mPID.Success)
                            c.pid = mPID.Groups[1].Value;

                        return c;

                    }).ToList();
                }
            }
        }
    }
}
