#define TEST

using System;
using System.Collections.Generic;
using System.Management;
using System.Text.RegularExpressions;

using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Windows.Forms;
using System.IO.Ports;
using System.Threading;


using System.Net;
using System.Net.Sockets;

using System.Net.Http;
using System.Web;
//using System.Web.Http;

//using System.Web.Http;
using System.IO;
using static System.Net.Mime.MediaTypeNames;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;
//using LibUsbDotNet.Main;
//using LibUsbDotNet;
using LibUsbDotNet.Info;


using static System.Windows.Forms.VisualStyles.VisualStyleElement.ToolBar;
using System.Diagnostics.Eventing.Reader;
using System.Reflection;


namespace WindowsFormsApp1
{
    public partial class Form1 : Form
    {
        public int Stop;
        public int Start;
       SerialPort port;
        public const string vidPattern = @"VID_([0-9A-F]{4})";
        public const string pidPattern = @"PID_([0-9A-F]{4})";

        //   public event System.IO.Ports.SerialDataReceivedEventHandler DataReceived;
        // public int kol = 0;
        //  int D_SRN = 1;

        Settings set = new Settings();
        hw_1 HW = new hw_1();
        rx Rx = new rx();
        Scanner scan = new Scanner();
        char[] SRN = new char[16];
        char[] ICCID_GSM = new char[8];
        byte[] QScanArray = new byte[32];

         int Error_set = 0x0;
       
        String Port;
    //    String QR_SN_String;
      
        System.IO.StreamWriter file_log;
 
        bool file_save() { 

               String output = DateTime.Now.ToString("yyyy-MM-dd");
               String time = DateTime.Now.ToString("HH: mm: ss");

            TcpIP_Port_Read();



            byte[] Qarr = new byte[200];

            if (encrypt(ref Qarr, 200) == 0)
                return false;
            return true;          
}

UInt64 strToU64(UInt32[] data_in, int len , int offs)
        {
            int idx;
            char[] data = new char[8];
            for (idx = 0; idx < len; idx++)
            {
                data[idx] = (Char)data_in[offs + idx];
            }

            UInt64 temp64 = 0;
            temp64 = data[7];
            temp64 |= (ulong)data[6] << 8;
            temp64 |= (ulong)data[5] << 16;
            temp64 |= (ulong)data[4] << 24;
            temp64 |= (ulong)data[3] << 32;
            temp64 |= (ulong)data[2] << 40;
            temp64 |= (ulong)data[1] << 48;
            temp64 |= (ulong)data[0] << 56;
            return temp64;
        }
          UInt32 Get_UInt32(Int32 uOffs  )
          {
              UInt32 res = 0;
              res = Rx.UbyteArray[uOffs] | (Rx.UbyteArray[uOffs + 1] << 8) |
                    (Rx.UbyteArray[uOffs + 2] << 16) | (Rx.UbyteArray[uOffs + 3] << 24);
            return res;
          }
       
   
        Int32 Get_Chars(Int32 uOffs , Int32 Len, Int32 Cell)
        {
            int idx = 0;
            UInt64 U64_str;
            Byte[] ICCID_str = new byte[Len];
     
            U64_str = strToU64( Rx.UbyteArray, 8, uOffs);
            uOffs += 8;
            do 
            {
                ICCID_str[idx] = (byte)(U64_str % 10);
                U64_str /= 10;
                idx ++;
            } while (idx < Len);
            for (idx = 0; idx < Len; idx++)
            {              
               dataGridView2.Rows[Cell].Cells[4].Value += ICCID_str[(Len - 1) - idx].ToString();
            }
            return uOffs;
        }



        public bool COM_Port_Read()
        {
            List<ComPort> ports = GetSerialPorts();
            List<ComPort> coms = ports.FindAll(c => c.vid.Equals("0483") && c.pid.Equals("374B"));
            try
            {
                //    port_scan = new SerialPort(coms[0].name, 115200);
                this.port = new SerialPort(coms[0].name, 115200);
                port.Open();

                //  port.ReadTimeout = 30000;

                try
                {
                    if (!port.IsOpen)
                        port.Open();
                    textBox8.Text += "\r\nPort_Open\r\n";
                }
                catch
                {
                    textBox8.Text += "\r\nPort_Closed\r\n";
                }
                Thread.Sleep(20);
                return true;
            }
            catch
            {
                Thread.Sleep(50);
                textBox8.Text += "\r\nPort_Not_Found\r\n";
                return false;
            }
            //       return false;
        }


        public int Config_Read()
        {
            string[] allport = SerialPort.GetPortNames();
            Scanner scan = new Scanner();
            comboBox1.Items.Clear();
            comboBox1.Items.AddRange(allport);


        
            string path = "config.txt";
           
            timer1.Enabled = false;
            timer1.Stop();
            try
            {
                if (COM_Port_Read() == true)
                    comboBox1.SelectedItem = this.port.PortName;//this.Port;
                else
                {
                    textBox8.Text += "Tester not found\r\n";
                    return 0;
                }
                System.IO.StreamReader file = new System.IO.StreamReader(path);
                //  {
                set.Version = Convert.ToInt32(file.ReadLine());
                file.Close();

                if (set.Version > 3)
                    set.Version = 0;
      
            }
            catch
            {
               // MessageBox.Show("Error Config Open");
                set.Version = 0;
                return 0;
            }
         //   Init_dataGrid();      
            return 1;
        }
        public Form1()
        {

            var text = Console.ReadLine();

            InitializeComponent();
            Init_dataGrid();
            scan.present = Scanner_Init();


            if(Config_Read() == 0){
                timer1.Enabled = false;
                timer1.Stop(); ;
                button5.Enabled = false; }

            comboBox2.SelectedIndex = set.Version;

        }


        void Power_Monitor(string S)
        {
            if (S.Contains("Finish"))
            {
                Clear_Form();
                this.Start = state.Start_Test;
            }
            if (S.Contains("Next"))
            {
                startScanToolStripMenuItem_Click(new object(), new EventArgs());

            }
        }
       
        private void timer1_Tick(object sender, EventArgs e)
        {
            this.Stop = 0;
            int bytes = 0;
       
            int uOffs = 0;
            switch (this.Start)
            {
                case state.Start_Test:
                    try
                    {

                        if (port.IsOpen)
                        {
                            string s0 = port.ReadExisting();
                            Power_Monitor(s0);
                        }
                        

                     
                    }
                    catch
                    {
                      MessageBox.Show( "\n(Port   closed");
                    }
                  
                    break;


                case state.HW_Test:


                    string s1 = port.ReadExisting();
                    textBox8.Text += s1;
                    Rx.Start_timer += 1;

                    if (Rx.Start_timer > state.F0_Timeout)
                    {
                        Rx.Start_timer = 0;

                        textBox1.Text += "F0 ";
                        HW.HW_error_state |= (1 << Error_Codes.F0);
                        dataGridView2.Rows[6].Cells[4].Value = HW.ICCID_scanner;
                        this.Start = state.Errors_Test;
                        port.WriteLine("End");
                        MessageBox.Show("\n Not responding, restart");
                    }
                    if (s1.Contains("HW"))
                    {
                        Rx.Start_timer = 0;

                    }

                    if (s1.Contains("SW"))
                    {
                        this.Start = state.SW_Test;

                    }
                   
                    Power_Monitor(s1);

                    break;


                case state.SW_Test:

                    port.ReadTimeout= state.Uart_Timeout;
         
                    try
                    {                                                
                        do
                        {

                            Rx.UbyteArray[bytes] = (UInt32)port.ReadByte();
                            bytes += 1;

                        } while (bytes < state.HW_Lenght);

                        uOffs = 4;
                        Parse_RxArray(uOffs);
                        Draw_dataGrid();
                        this.Start = state.Cops_Test;
                    }
                    catch (TimeoutException ex)
                    {
                      
                        textBox8.Text += "\r\nNot Responding...Restart";
                        this.Start = state.Start_Test;
                        startScanToolStripMenuItem_Click(new object(), new EventArgs());
                     
                    }
               
                    break;



                case state.Cops_Test:

                    string s3 = port.ReadExisting();
                    uOffs = 4;
                
                    Power_Monitor(s3);
                    if (s3.Contains("AVT1"))
                    {

                        HW.Net_Num = (UInt32)Convert.ToInt32(s3[uOffs]);
                        dataGridView2.Rows[0].Cells[4].Value = (Convert.ToInt32(HW.Net_Num)).ToString();
                        textBox8.Text +="\r\nGPS Test...";

                    }
                    if (s3.Contains("AVT2"))
                    {
                       
                        HW.GNSS_fix = (UInt32)Convert.ToInt32(s3[uOffs + 2]);
                        dataGridView2.Rows[1].Cells[4].Value = (Convert.ToInt32(HW.GNSS_fix)).ToString() + "D";

                        HW.Sat_num = (UInt32)Convert.ToInt32(s3[uOffs + 3]);
                        dataGridView2.Rows[2].Cells[4].Value = (Convert.ToInt32(HW.Sat_num)).ToString();
                        textBox8.Text += "\r\nSound Test";
                        textBox8.Text += "\r\nAudio Record...\r\n";
                
                    }

                    if (s3.Contains("Play"))
                    {

                        textBox8.Text += s3;
                        Thread.Sleep(6000);
                        port.ReadTimeout = 20000;
                        bytes = 0;
                        try
                        {
                            do
                            {
                                Rx.UbyteArray[bytes] = (UInt32)port.ReadByte();
                                bytes += 1;
                            } while (bytes < state.SW_Lenght);

                            uOffs = 4;
                            HW.estimate = (Byte)Rx.UbyteArray[uOffs];
                            uOffs += 1;
                            HW.f1 = Rx.UbyteArray[uOffs] | (Rx.UbyteArray[uOffs + 1] << 8);
                            uOffs += 4;
                            HW.mute_adc = Rx.UbyteArray[uOffs] | (Rx.UbyteArray[uOffs + 1] << 8);
                            dataGridView2.Rows[11].Cells[2].Value = HW.mute_adc;
                            uOffs += 4;
                            HW.Vbat_Supply = Rx.UbyteArray[uOffs] | (Rx.UbyteArray[uOffs + 1] << 8);
                            //     if ((HW.Vbat_Supply & (0x1 << 15))==1)

                    
                            dataGridView2.Rows[5].Cells[4].Value = (Convert.ToInt32(HW.Vbat_Supply & (0x7fff))).ToString();

                            if (HW.estimate == 1)
                            {     
                                textBox8.Text += "Audio OK, ";
                                textBox3.Text = "\r\nAudio OK";
                                //  button2.Text = "AUDIO OK";
                            }
                            else
                            {
                                textBox8.Text += "Audio Error, ";
                                textBox3.Text = "\r\nAudio ERROR";
                                //button2.Text = "AUDIO ERROR";
                            }
                            textBox8.Text += "f=";
                            textBox8.Text += Convert.ToInt32(HW.f1).ToString();
                            textBox8.Text += "Hz\r\n";

                            Error_set = Validate_Settings();

                            this.Start = state.Errors_Test;
                        }
                        catch (TimeoutException ex)
                        {
                            HW.estimate = 0;
                            textBox8.Text += "Audio Error, ";
                            textBox3.Text = "\r\nAudio ERROR";

                            Error_set = Validate_Settings();

                            this.Start = state.Errors_Test;
                            MessageBox.Show(ex.Message + "\n(Port " + port.PortName + " not responding");
                           // textBox8.Text += "\r\nNot Responding...Restart";
                            Thread.Sleep(5000);
                            this.Start = state.Start_Test;
                           // startScanToolStripMenuItem_Click(new object(), new EventArgs());
                          
                        }

                        Thread.Sleep(1000);
                    }
           
                    break;
                case state.Errors_Test:

                    if (/*(Error_set == 0) &&*/ (HW.HW_error_state == 0))
                    {

                        textBox2.BackColor = Color.Green;
                        textBox2.Text = "\r\nOK"; 
                        textBox1.Text = "NO Errors";

                    }
                    else
                    {
                        textBox2.Text = "\r\nErrors";
                    }

                    if (file_save() == false)
                    {                     
                        textBox8.Text += "Server is not connected\r\n";
                    }

                    file_save_to_disk();
                    radioButton2.BackColor = Color.Green;
                    radioButton2.Text = "YES";
                    textBox8.Text += "Finish Test\r\n";
                    textBox8.Text += "Connect a new device\r\n";


                  //  port.Close();
                    this.Start = state.Start_Test;
                    break;
            }
        }

            async private void startScanToolStripMenuItem_Click(object sender, EventArgs e)
        {

            Clear_Form();
          
            byte[] StrToByteArray(string str)
            {
                System.Text.UTF8Encoding encoding = new System.Text.UTF8Encoding();
                return encoding.GetBytes(str);
            }

            if (port.IsOpen)
            {
                textBox8.Text += "Port Open\r\n";
                set.Version = comboBox2.SelectedIndex;


                // Read_Scanner_Monitor();

                if (Read_Scanner_Monitor() == true)
                //  Read_Scanner_Monitor();
                {
                    try
                    {

                        Thread.Sleep(50);
                        Rx.Start_timer = 0;

                        string s = "StartV ";
                        s += set.Version.ToString();
                        port.WriteLine(s);
                        Thread.Sleep(50);
                        port.WriteLine(s);
                        timer1.Enabled = true;
                        timer1.Start();
                        this.Start = state.HW_Test;
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show(ex.Message + "\nPort " + port.PortName /*+ " Unavailable"*/);
                        //  return false;
                    }
                }   // read_monitor
                else
                {
                    textBox8.Text += "Sticker not Found\r\n";
                    textBox8.Text += " \r\n";
                }

            }    ///port_is_open
                else
                    textBox8.Text += "Port Closed\r\n";
          //  }//// scaner_init
        //    else Form1.Close();
        
        }
        public void mySerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            SerialPort sp = (SerialPort)sender;
            scan.QR_SN_String = sp.ReadExisting();
            if (scan.QR_SN_String.Length > 50)
                //    scan.QR_SN_String = sp.ReadExisting();
                scan.flag = true;

            //     textBox8.Text+= sp.ReadExisting() + "\n";
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
          

        }

        private void configSaveToolStripMenuItem_Click(object sender, EventArgs e)
        {

            if (saveFileDialog1.ShowDialog() == DialogResult.Cancel)
                    return;
            // получаем выбранный файл
            string filename = saveFileDialog1.FileName;
            System.IO.StreamWriter file = new System.IO.StreamWriter(saveFileDialog1.FileName);
        
            file.WriteLine(this.Port);
      
            MessageBox.Show("Settings Saved");
            file.Close();
            
        }

        private void configOpenToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (openFileDialog1.ShowDialog() == DialogResult.Cancel)
                return;
            // получаем выбранный файл
            string filename = openFileDialog1.FileName; 
            try
            {

                System.IO.StreamReader file = new System.IO.StreamReader(openFileDialog1.FileName);
             
                port.Close();
                this.Port = file.ReadLine();

                port = new SerialPort(Port, 115200);
                comboBox1.Items.Add(this.Port);
                comboBox1.SelectedItem = this.Port;
              //  D_SRN = Convert.ToInt32(file.ReadLine());
                set.Version = Convert.ToInt32(file.ReadLine());
                comboBox1.SelectedItem = this.Port;              
                port.Open();
                file.Close();

            }
            catch
            {
                MessageBox.Show("Error Config Open");
              //  port = new SerialPort("COM3", 115200);
            }
       
        }


        public void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
      //      MessageBox.Show("Error Settings File");
            string s = "End";
            try
            {
                if (port!=null)
                    port.WriteLine(s);
            }
            catch {
                ;
            }
            string path = "config.txt";
            try
            {
                System.IO.StreamWriter file = new System.IO.StreamWriter(path);
                file.WriteLine(set.Version.ToString());
                file.Close();
            }
            catch
            {
               MessageBox.Show("Error Config File");
    
       
            }
            Thread.Sleep(1000);
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

        private void Form1_Shown(object sender, EventArgs e)
        {
            textBox1.Clear();
           
            textBox8.Clear();
            textBox1.Clear();
            radioButton2.BackColor = Color.Red;
            radioButton2.Text = "NO";
            radioButton2.ForeColor = Color.White;

            textBox2.Text = "\r\nRESULT";
            textBox2.BackColor = Color.Red;

    
       //     startScanToolStripMenuItem_Click(new object(), new EventArgs());
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {

        }
    }
}
