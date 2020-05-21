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

                int rectWidth = 200;
                int rectHeight = 200;
                int topLeftX = 0;
                int topLeftY = 0;
                int maxX = topLeftX + rectWidth;
                int maxY = topLeftY + rectHeight;
                Rectangle boundingRect = new Rectangle(0, 0, 200, 200);
                System.Drawing.Pen rectPen = new System.Drawing.Pen(System.Drawing.Color.Chocolate);

                const int gridSectionsNum = 5;
                int gridSectionWidth = rectWidth / gridSectionsNum;
                int gridSectionHeight = rectHeight / gridSectionsNum;
                //Rectangle[] gridRects = new Rectangle[gridSectionsNum * gridSectionsNum];
                List<Rectangle> gridRects = new List<Rectangle>();
                for (int i = 0; i < gridSectionsNum; ++i)
                {
                    for (int j = 0; j < gridSectionsNum; ++j)
                    {
                        gridRects.Add(new Rectangle(topLeftX + j * gridSectionWidth, topLeftY + i * gridSectionHeight, gridSectionWidth, gridSectionHeight));
                    }
                }

                g.DrawEllipse(myPen, boundingRect);
                g.FillEllipse(myBrush, boundingRect);

                //g.DrawRectangle(rectPen, boundingRect);
                g.DrawRectangles(rectPen, gridRects.ToArray());

                myPen.Dispose();
                rectPen.Dispose();
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
