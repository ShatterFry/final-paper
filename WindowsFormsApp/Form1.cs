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
    public class Grid
    { 
        private int mWidth;
        private int mHeight;
        private int mSectionsNum;
        private int mTopLeftX;
        private int mTopLeftY;

        public Grid(int inWidth, int inHeight, int inSectionsNum, int inTopLeftX, int inTopLeftY)
        {
            mWidth = inWidth;
            mHeight = inHeight;
            mSectionsNum = inSectionsNum;
            mTopLeftX = inTopLeftX;
            mTopLeftY = inTopLeftY;
        }

        public int GetHeight()
        {
            return mHeight;
        }

        public int GetTopLeftX()
        {
            return mTopLeftX;
        }

        public int GetTopLeftY()
        {
            return mTopLeftY;
        }

        public int GetSectionWidth()
        {
            return mWidth / mSectionsNum;
        }

        public int GetSectionHeight()
        {
            return mHeight / mSectionsNum;
        }

        public void Draw(Graphics inGraphics)
        {
            int maxY = mTopLeftY + mHeight;

            int gridSectionWidth = GetSectionWidth();
            int gridSectionHeight = mHeight / mSectionsNum;

            List<Rectangle> gridRects = new List<Rectangle>();
            for (int i = 0; i < mSectionsNum; ++i)
            {
                for (int j = 0; j < mSectionsNum; ++j)
                {
                    gridRects.Add(new Rectangle(mTopLeftX + j * gridSectionWidth, mTopLeftY + i * gridSectionHeight, gridSectionWidth, gridSectionHeight));
                }
            }

            System.Drawing.Pen rectPen = new System.Drawing.Pen(System.Drawing.Color.Chocolate);
            inGraphics.DrawRectangles(rectPen, gridRects.ToArray());
            rectPen.Dispose();
        }
    }

    public partial class Form1 : Form
    {
        Dictionary<AgeType, AgeTypeData> AgeData = new Dictionary<AgeType, AgeTypeData>();

        private void FillAgeData()
        {
            AgeTypeData se = new AgeTypeData();
            se._minAge = 1;
            se._minAge = 1;
            AgeData[AgeType.se] = se;

            AgeTypeData p = new AgeTypeData();
            p._minAge = 1;
            p._maxAge = 1;
            AgeData[AgeType.p] = p;

            AgeTypeData j = new AgeTypeData();
            j._minAge = 1;
            j._maxAge = 1;
            AgeData[AgeType.j] = j;

            AgeTypeData im = new AgeTypeData();
            im._minAge = 1;
            im._maxAge = 3;
            AgeData[AgeType.im] = im;

            AgeTypeData v = new AgeTypeData();
            v._minAge = 1;
            v._maxAge = 5;
            AgeData[AgeType.v] = v;

            AgeTypeData g1 = new AgeTypeData();
            g1._minAge = 1;
            g1._maxAge = 4;
            AgeData[AgeType.g1] = g1;

            AgeTypeData g2 = new AgeTypeData();
            g2._minAge = 5;
            g2._maxAge = 5;
            AgeData[AgeType.g2] = g2;

            AgeTypeData g3 = new AgeTypeData();
            g3._minAge = 1;
            g3._maxAge = 2;
            AgeData[AgeType.g3] = g3;

            AgeTypeData ss = new AgeTypeData();
            ss._minAge = 1;
            ss._maxAge = 2;
            AgeData[AgeType.ss] = ss;

            AgeTypeData s = new AgeTypeData();
            s._minAge = 1;
            s._maxAge = 2;
            AgeData[AgeType.s] = s;
        }

        List<Plant> CreatePlants()
        {
            List<Plant> plants = new List<Plant>();

            System.IO.FileInfo plantsXML_File = new System.IO.FileInfo("..\\..\\data\\plants.xml");

            if (!plantsXML_File.Exists)
            {
                MessageBox.Show(string.Format("XML does NOT exist!\nPath: {0}", plantsXML_File.FullName));
                return plants;
            }
            
            System.Xml.XmlDocument xmlDocument = new System.Xml.XmlDocument();
            xmlDocument.Load(plantsXML_File.FullName);
            System.Xml.XmlElement xmlRoot = xmlDocument.DocumentElement;

            foreach (System.Xml.XmlNode xmlNode in xmlRoot)
            {
                System.Xml.XmlAttributeCollection nodeAttributes = xmlNode.Attributes;

                double plantX = double.Parse(nodeAttributes.GetNamedItem("x").Value);
                double plantY = double.Parse(nodeAttributes.GetNamedItem("y").Value);
                double plantRadius = double.Parse(nodeAttributes.GetNamedItem("radius").Value);
                string plantAgeTypeStr = nodeAttributes.GetNamedItem("ageType").Value;

                System.Xml.XmlNode fillAttr = nodeAttributes.GetNamedItem("fill");
                string fillColorStr = null;
                System.Drawing.Color fillColor = System.Drawing.Color.Empty;
                if (fillAttr != null)
                {
                    fillColorStr = fillAttr.Value;
                    fillColor = System.Drawing.ColorTranslator.FromHtml(fillColorStr);
                }

                AgeType plantAgeType = AgeTypeConverter.FromString(plantAgeTypeStr);

                if (plantAgeType == AgeType.MAX)
                {
                    MessageBox.Show(string.Format("XML Error: Invalid age type: {0}", plantAgeTypeStr));
                }

                Plant singlePlant = new Plant(plantAgeType, plantX, plantY, plantRadius);
                if (!fillColor.IsEmpty)
                {
                    singlePlant.SetFillColor(fillColor);
                }
                plants.Add(singlePlant);
            }
            

            return plants;
        }
        
        public Form1()
        {
            InitializeComponent();
            this.Paint += new PaintEventHandler(Form1_Paint);

            FillAgeData();

            List<Plant> plants = CreatePlants();
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

                Grid grid = new Grid(400, 400, 5, 0, 0);

                for (int i = 0; i < mPlants.Count; ++i)
                {
                    Plant currentPlant = mPlants[i];
                    currentPlant.Draw(grid, g);
                }

                grid.Draw(g);

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
    }
}
