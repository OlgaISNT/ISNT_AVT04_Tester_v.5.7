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
      
        bool Read_Scanner_Monitor()
        {
            int read_num = 0;
            if (scan.present == false)
                return false;
           do 
            {
              
                if (Scanner_Monitor() == true)   return true;
                else
                {
                    Thread.Sleep(200);
                    read_num++;
                }
            }
            while( read_num < 5);
            return false;

        }

        bool Scanner_Monitor()
        {
           
            int QR_Offs = 0;
            String textbox;
            int version;
            //        rx Rx = new rx();
            String SN_ICCID_str = null;
            // scan.QR_SN_String = null;
            //Workbook wb = new Workbook("Excel.xlsx");

         
            byte[] cmd_start = { 0x04, 0xe4, 0x04, 0x00, 0xff, 0x14 };
            byte[] cmd_stop = { 0x04, 0xe4, 0x04, 0x00, 0xff, 0x13 };
     //       bool res;
         //   byte[] cmd_start_answ = new byte[20];
       
            try
            {
              
                scan.port_scan.Write(cmd_start, 0, 6);
                scan.port_scan.ReadTimeout = 2000;
                Thread.Sleep(2000);
            }
            catch
            {
                //     WindowsFormsApp1.Form1.textBox8.Text += "Port Closed\r\n";
                //       MessageBox.Show( "\nScanner Port Unavailable");
                Thread.Sleep(200);
                return false;
            }

            Rx.SN_str = null;
            Rx.ICCID_str = null;

           textbox = "Sticker Scan...\r\n";
            textBox8.AppendText(textbox);
            switch (set.Version)
            {
                case 0:
                    QR_Offs = scan.Scan_AVT04_offs + scan.Scan_ISNT_offs;
                    scan.ICCID_len = scan.Scan_AVT04 + scan.Scan_ISNT_offs;
                    break;
                case 1:
                    QR_Offs = scan.Scan_AVT04_offs + scan.Scan_ISNT_offs + scan.Scan_ISNT_offs_Mmin;
                    scan.ICCID_len = scan.Scan_AVT04 + scan.Scan_ISNT_offs + scan.Scan_ISNT_offs_Mmin;
                    break;
                case 2:
                    QR_Offs = scan.Scan_AVT04_offs;
                    scan.ICCID_len = scan.Scan_AVT04;
                    break;
                case 3:
                    QR_Offs = scan.Scan_AVT04_offs + scan.Scan_ISNT_offs + scan.Scan_ISNT_offs_Mmin;
                    scan.ICCID_len = scan.Scan_AVT04 + scan.Scan_ISNT_offs + scan.Scan_ISNT_offs_Mmin;
                    break;
                default:
                    break;
            }

            if (scan.flag == true)
            {


          
                ///////////////////
                if (scan.QR_SN_String.Length == scan.Scan_AVT04)
                {
                    set.Version = scan.AVT04;
                    QR_Offs = scan.Scan_AVT04_offs;


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


               // if (set.Version == scan.ISNT_Mmin || set.Version == scan.ISNT_24V || set.Version == scan.AVT04)
                 Rx.SN_str = scan.QR_SN_String.Substring(QR_Offs , 14);
            
                     //   if (set.Version == scan.ISNT_Mmin || set.Version == scan.ISNT_24V || set.Version == scan.AVT04)
                 Rx.ICCID_str = scan.QR_SN_String.Substring(QR_Offs + 15, 19);

               

                    textbox = "SN= " + Rx.SN_str  +"\r\n";
                    textBox8.AppendText(textbox);
                    textbox ="ICCID= " + Rx.ICCID_str;
                    textBox8.AppendText(textbox);
                    
                        scan.flag = false;
                   //     scan.port_scan.Close();
                    Thread.Sleep(200);
                return true;
  

            }


                   else
                {
               //     scan.port_scan.Close();
                    return false;
                }




                return false;
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
                //      Form1.textBox8.Text += "Ready\r\n";
                res = true;
                // port_scan.Write(cmd_start, 0, 6);
                //port_scan.ReadTimeout = 5000;
                //Thread.Sleep(20);
            }
            catch
            {
                //     WindowsFormsApp1.Form1.textBox8.Text += "Port Closed\r\n";
                MessageBox.Show("\nNo scanner");
                res = false;
                Thread.Sleep(  1000);
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
            public int Scan_ISNT_offs = 4;
            public int Scan_ISNT_offs_Mmin = 3;
            public SerialPort port_scan;
            public int ISNT_Mplus = 0;
            public int ISNT_Mmin =1;
            public int AVT04 = 2;
            public int ISNT_24V = 3;
            struct ComPort // custom struct with our desired values
            {
                public string name;
                public string vid;
                public string pid;
                public string description;
                //    byte[] QScanArray;//= new byte[32];

            }


       /*     public bool Scanner_Read()
            {

                // List<ComPort> ports = GetSerialPorts();
                byte[] cmd_start = { 0x04, 0xe4, 0x04, 0x00, 0xff, 0x14 };
                byte[] cmd_stop = { 0x04, 0xe4, 0x04, 0x00, 0xff, 0x13 };
                bool res;
                byte[] cmd_start_answ = new byte[20];


                port_scan.Write(cmd_start, 0, 6);
                //     port_scan.ReadTimeout = 5000;
                Thread.Sleep(20);

                Read_Port_Byte(port_scan, cmd_start_answ, 6);

                if (cmd_start_answ[0] == 0x4)
                {
                    res = Read_Port_Byte(port_scan, QScanArray, ICCID_len);


                }
                else return false;


                //   port_scan.Close();
                return res;
            }*/

       /*     public bool Scanner_Read()
             {
                 SerialPort port_scan;
                 List<ComPort> ports = GetSerialPorts();
                 byte[] cmd_start = { 0x04, 0xe4, 0x04, 0x00, 0xff, 0x14 };
                 byte[] cmd_stop = { 0x04, 0xe4, 0x04, 0x00, 0xff, 0x13 };
                 bool res;
                 byte[] cmd_start_answ = new byte[20];
                 //   ComPort com = ports.FindLast(c => c.vid.Equals("0x0483") && c.pid.Equals("0x374B"));
                 //or if we want to extract all devices with specified values:

                 //  List<ComPort> coms = ports.FindAll(c => c.vid.Equals("0C2E") && c.pid.Equals("090A"));

                 List<ComPort> coms = ports.FindAll(c => c.vid.Equals("05E0") && c.pid.Equals("1701"));
                 try
                 {
                     //    port_scan = new SerialPort(coms[0].name, 115200);
                     port_scan = new SerialPort(coms[0].name, 9600);
                     port_scan.Open();

                     port_scan.Write(cmd_start, 0, 6);
                     port_scan.ReadTimeout = 2000;
                     Thread.Sleep(20);
                 }
                 catch
                 {
                     //     WindowsFormsApp1.Form1.textBox8.Text += "Port Closed\r\n";
                     //       MessageBox.Show( "\nScanner Port Unavailable");
                     Thread.Sleep(2000);
                     return false;
                 }
                 Read_Port_Byte(port_scan, cmd_start_answ, 6);
                 if (cmd_start_answ[0] == 0x4)
                 {
                     res = Read_Port_Byte(port_scan, QScanArray, ICCID_len);


                 }
                 else return false;


                 port_scan.Close();
                 return res;
             }





            public bool Read_Port_Byte(SerialPort port, byte[] buf, Int32 len)
            {
                Int32 ind = 0;
                try
                {
                    do
                    {
                        buf[ind] = (byte)port.ReadByte();
                        ind += 1;} while (ind < len);
                }

                catch
                {

                    return false;
                }
                return true;

            }
       */
  
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
