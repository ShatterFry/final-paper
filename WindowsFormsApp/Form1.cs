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

            AgeType se = new AgeType();
            se._minAge = 1;
            se._minAge = 1;

            AgeType g1 = new AgeType();
            g1._minAge = 1;
            g1._maxAge = 4;

            List<Plant> plants = new List<Plant>();

            System.IO.FileInfo plantsXML_File = new System.IO.FileInfo("..\\..\\data\\plants.xml");
            if (plantsXML_File.Exists)
            {
                //MessageBox.Show("XML exists!");
                System.Xml.XmlDocument xmlDocument = new System.Xml.XmlDocument();
                xmlDocument.Load(plantsXML_File.FullName);
                System.Xml.XmlElement xmlRoot = xmlDocument.DocumentElement;
                foreach (System.Xml.XmlNode xmlNode in xmlRoot)
                {
                    //MessageBox.Show(string.Format("Node Name = {0}", xmlNode.Name));
                    System.Xml.XmlAttributeCollection nodeAttributes = xmlNode.Attributes;
                    //MessageBox.Show(string.Format("Attributes Count = {0}", nodeAttributes.Count));

                    for (int i = 0; i < nodeAttributes.Count; ++i)
                    {
                        //MessageBox.Show(string.Format("Attribute[{0}] = {1} {2}", i, nodeAttributes[i].Name, nodeAttributes[i].Value));
                    }

                    //MessageBox.Show(string.Format("Getting x attribute: {0}", nodeAttributes.GetNamedItem("x").Value));

                    //MessageBox.Show(string.Format("Child Nodes Cound = {0}", xmlNode.ChildNodes.Count));
                    foreach (System.Xml.XmlNode childNode in xmlNode.ChildNodes)
                    {
                        //MessageBox.Show(string.Format("Child Node Name = {0}", childNode.Name));
                    }

                    double plantX = double.Parse(nodeAttributes.GetNamedItem("x").Value);
                    double plantY = double.Parse(nodeAttributes.GetNamedItem("y").Value);
                    double plantRadius = double.Parse(nodeAttributes.GetNamedItem("radius").Value);
                    string plantAgeTypeStr = nodeAttributes.GetNamedItem("ageType").Value;
                    AgeType plantAgeType = null;
                    switch (plantAgeTypeStr)
                    {
                        case "se":
                            plantAgeType = se;
                            break;
                        case "g1":
                            plantAgeType = g1;
                            break;
                        default:
                            MessageBox.Show(string.Format("XML Error: Invalid age type: {0}", plantAgeTypeStr));
                            break;
                    }
                    plants.Add(new Plant(plantAgeType, plantX, plantY, plantRadius));
                }
            }
            else
            {
                MessageBox.Show("XML is NOT exist!");
            }

            SetPlants(plants);
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
        {
            mPlants = inPlants;
        }

        private bool bShouldDrawEllipse = false;
        private List<Plant> mPlants = new List<Plant>();

        private void button2_Click(object sender, EventArgs e)
        {
            //MessageBox.Show(System.IO.Directory.GetCurrentDirectory());

            System.IO.FileInfo plantsXML_File = new System.IO.FileInfo("..\\..\\data\\plants.xml");
            if (plantsXML_File.Exists)
            {
                MessageBox.Show("XML exists!");
                System.Xml.XmlDocument xmlDocument = new System.Xml.XmlDocument();
                xmlDocument.Load(plantsXML_File.FullName);
                System.Xml.XmlElement xmlRoot = xmlDocument.DocumentElement;
                foreach (System.Xml.XmlNode xmlNode in xmlRoot)
                {
                    MessageBox.Show(string.Format("Node Name = {0}", xmlNode.Name));
                    System.Xml.XmlAttributeCollection nodeAttributes = xmlNode.Attributes;
                    MessageBox.Show(string.Format("Attributes Count = {0}", nodeAttributes.Count));

                    for (int i = 0; i < nodeAttributes.Count; ++i)
                    {
                        MessageBox.Show(string.Format("Attribute[{0}] = {1} {2}", i, nodeAttributes[i].Name, nodeAttributes[i].Value));
                    }

                    MessageBox.Show(string.Format("Getting x attribute: {0}", nodeAttributes.GetNamedItem("x").Value));

                    MessageBox.Show(string.Format("Child Nodes Cound = {0}", xmlNode.ChildNodes.Count));
                    foreach (System.Xml.XmlNode childNode in xmlNode.ChildNodes)
                    {
                        MessageBox.Show(string.Format("Child Node Name = {0}", childNode.Name));
                    }
                }
            }
            else
            {
                MessageBox.Show("XML is NOT exist!");
            }
        }

        //public List<Plant> mPlants = new List<Plant>();
    }
}
