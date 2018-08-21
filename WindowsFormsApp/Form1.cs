using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApp
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            this.Paint += new PaintEventHandler(Form1_Paint);
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void Form1_Paint(object sender, System.Windows.Forms.PaintEventArgs pe)
        {
            Graphics g = pe.Graphics;
            System.Drawing.Pen myPen = new System.Drawing.Pen(System.Drawing.Color.Red);
            System.Drawing.SolidBrush myBrush = new System.Drawing.SolidBrush(System.Drawing.Color.Aquamarine);

            g.DrawEllipse(myPen, new Rectangle(0, 0, 200, 300));
            g.FillEllipse(myBrush, new Rectangle(0, 0, 200, 300));

            myPen.Dispose();
            g.Dispose();
        }

        private void button1_Click(object sender, EventArgs e)
        {

        }
    }
}
