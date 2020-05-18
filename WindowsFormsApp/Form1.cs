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
            if (bShouldDrawEllipse)
            {
                Graphics g = pe.Graphics;
                System.Drawing.Pen myPen = new System.Drawing.Pen(System.Drawing.Color.Red);
                System.Drawing.SolidBrush myBrush = new System.Drawing.SolidBrush(System.Drawing.Color.Aquamarine);

                Rectangle boundingRect = new Rectangle(0, 0, 200, 200);
                g.DrawEllipse(myPen, boundingRect);
                g.FillEllipse(myBrush, boundingRect);

                myPen.Dispose();
                g.Dispose();
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            bShouldDrawEllipse = true;
            Invalidate();
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private bool bShouldDrawEllipse = false;
    }
}
