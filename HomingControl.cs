using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Servotest
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            serialPort1.PortName = "COM4";
            serialPort1.BaudRate = 9600;
            serialPort1.Open();
        }

        private void button1_Click(object sender, EventArgs e)
        {
        
            if (serialPort1.IsOpen == true)
            {
                serialPort1.Write("Y");
            }
            else
            {
                return;
            }
        }
    }
}
