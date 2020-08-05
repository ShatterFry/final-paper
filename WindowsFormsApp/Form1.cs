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

                const int gridRectWidth = 200;
                const int gridRectHeight = 200;
                const int topLeftX = 0;
                const int topLeftY = 0;
                int maxX = topLeftX + gridRectWidth;
                int maxY = topLeftY + gridRectHeight;
                
                System.Drawing.Pen rectPen = new System.Drawing.Pen(System.Drawing.Color.Chocolate);

                const int gridSectionsNum = 5;
                int gridSectionWidth = gridRectWidth / gridSectionsNum;
                int gridSectionHeight = gridRectHeight / gridSectionsNum;
                //Rectangle[] gridRects = new Rectangle[gridSectionsNum * gridSectionsNum];
                List<Rectangle> gridRects = new List<Rectangle>();
                for (int i = 0; i < gridSectionsNum; ++i)
                {
                    for (int j = 0; j < gridSectionsNum; ++j)
                    {
                        gridRects.Add(new Rectangle(topLeftX + j * gridSectionWidth, topLeftY + i * gridSectionHeight, gridSectionWidth, gridSectionHeight));
                    }
                }

                for (int i = 0; i < mPlants.Count; ++i)
                {
                    double boundRectX = mPlants[i].GetX() - mPlants[i].GetRadius();
                    double boundRectY = mPlants[i].GetY() + mPlants[i].GetRadius();
                    float plantDiameter = (float)(mPlants[i].GetRadius() * 2.0);
                    float rectHeight = plantDiameter * (gridRectHeight / gridSectionsNum);
                    float rectiWidth = plantDiameter * (gridRectWidth / gridSectionsNum);
                    RectangleF boundingRect = new RectangleF((float)boundRectX, (float)boundRectY, rectHeight, rectiWidth);
                    g.DrawEllipse(myPen, boundingRect);
                    g.FillEllipse(myBrush, boundingRect);
                }

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

        public void SetPlants(List<Plant> inPlants)
        //public void SetPlants(Plant inPlant)
        {
            List<Plant> tmpPlants = new List<Plant>();
            //mPlants = tmpPlants;
            mPlants = inPlants;
        }

        private bool bShouldDrawEllipse = false;
        private List<Plant> mPlants = new List<Plant>();

        private void button2_Click(object sender, EventArgs e)
        {
            //MessageBox.Show(System.IO.Directory.GetCurrentDirectory());
            System.IO.FileInfo plantsXML = new System.IO.FileInfo("..\\..\\data\\plants.xml");
            if (plantsXML.Exists)
            {
                MessageBox.Show("XML exists!");
            }
            else
            {
                MessageBox.Show("XML is NOT exist!");
            }
        }
        //public List<Plant> mPlants = new List<Plant>();
    }
}
