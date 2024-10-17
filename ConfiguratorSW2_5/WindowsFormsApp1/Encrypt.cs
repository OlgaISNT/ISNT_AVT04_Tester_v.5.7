using System;
using System.IO;
using System.Reflection;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;
using System.Threading.Tasks;
using System.Threading;
using System.IO.Ports;
namespace WindowsFormsApp1
{


    struct ComPort // custom struct with our desired values
    {
        public string name;
        public string vid;
        public string pid;
        public string description;
        //    byte[] QScanArray;//= new byte[32];

    }
    public class decryption
	{

		public int decrypt_lenght;
		public Byte[] decrypt_packet;//= new byte[200];
		public Byte[] stream_packet;//= new byte[200];
		public String inform_packet;
	}

	public partial class Form1
	{
		/*
Name  : CRC-16 CCITT
Poly  : 0x1021    x^16 + x^12 + x^5 + 1
Init  : 0x1D0F
Revert: false
XorOut: 0x0000
Check : 0xE5CC ("123456789")
MaxLen: 4095 байт (32767 бит) - обнаружение
одинарных, двойных, тройных и всех нечетных ошибок
*/
		

		bool file_save_to_server(String decr_p)
		{
		//	decryption decr = new decryption();
			String output = DateTime.Now.ToString("ServerBox-yyyy-MM-dd");
			String time = DateTime.Now.ToString("HH: mm: ss");
			try
			{
				System.IO.StreamWriter file = new System.IO.StreamWriter(output, true);
				{
					file.Write(decr_p);
					file.Write("\r\n");
					file.Close();
					return true;

				}
			}
			catch
			{
				return false;
			}	
		


		}

	bool 	file_save_to_disk()
		{
			String output = DateTime.Now.ToString("yyyy-MM-dd");
			String time = DateTime.Now.ToString("HH: mm: ss");




			System.IO.StreamWriter file = new System.IO.StreamWriter(output, true);
			{

				file.Write(";");
				file.WriteLine(time);
				file.Write(";");
				for (int rows = 6; rows < 10; rows++)
				{
					file.WriteLine(dataGridView2.Rows[rows].Cells[4].Value);
					file.Write(";");
				}
			
				file.Write(textBox1.Text);
				file.WriteLine("\r\n");

				file.Close();
			}

			return true;
				}


				Byte[] data_encrypt = new byte[200];
				Byte[] encrypt_packet = new byte[200];



				UInt16[] crc_table = new UInt16[]{
			0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
			0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
			0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
			0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
			0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
			0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
			0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
			0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
			0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
			0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
			0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
			0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
			0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
			0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
			0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
			0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
			0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
			0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
			0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
			0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
			0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
			0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
			0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
			0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
			0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
			0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
			0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
			0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
			0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
			0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
			0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
			0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
		};


		

			UInt32 crc16(ref byte[] buffer, int size)
		{
			byte tmp;
			UInt32 crc = 0x1d0f;

			for (int i = 0; i < size; i++)
			{
				tmp = (byte)((crc >> 8) ^ buffer[i]);
				crc = (UInt32)((crc << 8) ^ crc_table[tmp]);
			}

			return crc;
		}


		

        UInt32 TcpIP_Port_Read()
		{
			
		/*	string path = "ServerParam.ini";
			
			String server_param = "1234";
			try
			{
				System.IO.StreamReader file = new System.IO.StreamReader(path);
				//     {
				file.ReadLine();
				file.ReadLine();
				//server_param = new string(server_param.TakeWhile(x => x != '=').ToArray());
			
				String file_string = file.ReadLine(); ;
				int startIndex = file_string.IndexOf("=");
			
				server_param = file_string.Substring(startIndex + 1);
				
				TcpIP_port = Convert.ToInt32(server_param);
				
				file.Close();


			}
			catch
			{
				MessageBox.Show("ServerParam.ini not found. Default port:1234");
			
			}
		*/
			return 1;
		}





		public byte encrypt(ref byte[] buf, int size)
		{


			decryption decr = new decryption();

			String year = DateTime.Now.ToString("yyyy-MM-dd-");
			String time = DateTime.Now.ToString("HH:mm");



			UInt32 errors = HW.HW_error_state;
			String mymessage = "AppenedTestInfo=" + dataGridView2.Rows[6].Cells[4].Value + " " + year + time + " " + errors.ToString();

		
			
			decr.inform_packet = dataGridView2.Rows[6].Cells[4].Value  + " " + year + time + " " + errors.ToString();

	//		TcpIP_Port_Read();
            try
            {
           //    TcpClient client = new TcpClient("127.0.0.1", /*TcpIP_port*/1234);
           TcpClient client = new TcpClient("195.24.66.236", 3345);  //2345
                   //2345
               
                if (client.Connected == true)
				{
					NetworkStream stream = client.GetStream();

				

					stream.ReadTimeout = 5000;
                    stream.WriteTimeout = 5000;
	
					String autorization = "ActiveModule=AVT04Tests";
					int len = en_packet(autorization, autorization.Length);
					stream.Write(encrypt_packet, 0, len + 4 + 5);
					Thread.Sleep(10);
				
					decr_packet(stream, 100);
					////////////////
					history_send(stream);


					TCPIP_send(stream, mymessage);



			     	client.Close();

				}
			
			}
            catch (SocketException ex)
            {
           //    MessageBox.Show(/*ex.Message +*/ "\nServer is not responding" );
                Thread.Sleep(200);
				file_save_to_server(decr.inform_packet);


				return 0;
            }

            return 1;
		}
	
		bool TCPIP_send(NetworkStream stream , String mes)
		{
			int len;
			int res;
			String decr_s = "";
	
			len = en_packet(mes, mes.Length);
			Thread.Sleep(50);
			stream.Write(encrypt_packet, 0, len + 4 + 5);
			decr_s = decr_packet(stream, 100);

			textBox8.Text += "Save to server...";

			res = decr_s.IndexOf("OK");
			   if (res >= 0) textBox8.Text += "OK\r\n";

			res = decr_s.IndexOf("Not Found");
			  if (res >= 0) textBox8.Text += "Not found\r\n";
			if (res >= 0)
				return true;
			else return false;
		}

		bool history_send(NetworkStream stream)
		{
			string Partialname = "ServerBox";
		//	string str;


			string root = Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
	//		FileInfo[] filesInDir = root.GetFiles("*" + Partialname + "*.*");
			string[] files = Directory.GetFiles(root);
				foreach ( string fil in files)
			{
				//string fullName = foundFile.FullName;
				if (fil.Contains(Partialname))
				{
				
					System.IO.StreamReader file = new System.IO.StreamReader(fil);
				while (file.Peek() >= 0)
					{
						String file_string = file.ReadLine();
						String mymessage = "AppenedTestInfo=" + file_string;
					    	if(mymessage.Length > 19 + 16)
					         	TCPIP_send(stream, mymessage);
					}
					file.Close();
					if (File.Exists(fil))
					{
						File.Delete(fil);
					}


				}


			}
		


				return true;
		}
	byte[]	translate_function(byte[] arr, int len, int off, int arg )

			{
			int i = 0;
			byte[] v = new byte[200];
			byte temp;
			while (i < len)
			{
				//	byte temp = arr[i];
				switch (arg)
				{
					case 0:
				     v[i] = rorb(arr[i + off], 3);
					 break;

					case 1:
					v[i] = rolb(arr[i + off], 3);
					break;

					case 2:
					v[i] = reverse(arr[i + off], 3);
					break;

					case 3:
						temp = rorb(arr[i + off], 3);
						v[i] = reverse(temp, 3);
					break;
					case 4:
						temp = rolb(arr[i + off], 3);
						v[i] = reverse(temp, 3);
						break;

					default:
						break;

				}
		//		textBox8.Text += (char) v[i];//.ToString();
				++i;
			}
			return v;

		}
public String decr_packet(NetworkStream stream, int size)
		{

			var decr = new decryption();
			int offs = 0;
			String decr_s = "";
			decr.decrypt_packet = new byte[200];
			decr.stream_packet = new byte[200];
			stream.ReadTimeout = 5000;

			try
			{

				stream.Read(decr.stream_packet, 0, 1);
			}
			catch (/*TimeoutException ex*/Exception e)
			{
				textBox8.Text += "Packet not received from server";
				//    MessageBox.Show(/*ex.Message +*/ "\nServer is not responding" );
				Thread.Sleep(200);
				return "";
			}
		    	decr.decrypt_lenght = decr.stream_packet[0];
				offs += 1;
				if (decr.decrypt_lenght < decr.stream_packet.Length)
				{
					stream.Read(decr.stream_packet, offs, decr.decrypt_lenght);
					offs += 3;
					int algo = decr.stream_packet[offs];
					decr.decrypt_packet = translate_function(decr.stream_packet, decr.decrypt_lenght, offs + 1, algo);
					for (int i = 0; i < decr.decrypt_lenght; i++)
					{
						decr_s += (char)decr.decrypt_packet[i];
					//	textBox8.Text += (char)decr.decrypt_packet[i];
					}
				}


           // }
            
			return decr_s;//decr.decrypt_lenght;

		}

	
		public int en_packet(/*ref byte[]*/String buf, int size)
		{
			UInt32 CRC16;
			Byte[] data_prev = System.Text.Encoding.ASCII.GetBytes(buf);
			//String enc_s = "";
			int size_full = data_prev.Length;
			int fullsize = 4 + 1 + size_full + 4;
			byte algo = 0;
			
			encrypt_packet[0] = (byte)fullsize;
			encrypt_packet[1] = 0;
			encrypt_packet[2] = 0;
			encrypt_packet[3] = 0;
			encrypt_packet[4] = algo;
			for (int i = 0; i < data_prev.Length; i++)
			{
				data_encrypt[i] = rolb(data_prev[i], 3);
			}

			for (int i = 0; i < data_prev.Length; i++)
			{
				encrypt_packet[5 + i] = data_encrypt[i];

               // enc_s += (char)encrypt_packet[i];

               // textBox8.Text += (char)encrypt_packet[i];

            }

			CRC16 = crc16(ref data_prev, size_full);


			encrypt_packet[5 + data_prev.Length] = (byte)((CRC16) & 0xff);// (byte)((CRC16 & 0xff000000) >> 24);
			encrypt_packet[5 + data_prev.Length + 1] = (byte)((CRC16 & 0x0000ff00) >> 8);//(byte)((CRC16 & 0x00ff0000) >> 16);
			encrypt_packet[5 + data_prev.Length + 2] = (byte)((CRC16 & 0x00ff0000) >> 16);
			encrypt_packet[5 + data_prev.Length + 3] = (byte)((CRC16 & 0xff000000) >> 24);
			//  return size_in_byte(fullsize) + QByteArray(1, algo) + encryp + size_in_byte(crc16((uchar*)data.data(), data.size()));

		
			// stream.Write(CRC16, 0, 4);
			return data_prev.Length;
		}

		

		byte rolb(byte x, byte y)
		{
			return (byte)((x << y) | (x >> (8 - y)));
		}

		byte rorb(byte x, byte y)
		{
			return (byte)((x >> y) | (x << (8 - y)));
		}

		byte reverse(byte x, byte y)
		{
			//	(void)y;
			return (byte)(~x);
		}




	}



	
}
