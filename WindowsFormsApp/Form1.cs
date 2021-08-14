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

        double CalcDistanceBetweenPoints(PointF FirstPoint, PointF SecondPoint)
        {
            return Math.Sqrt(Math.Pow(SecondPoint.X - FirstPoint.X, 2) + Math.Pow(SecondPoint.Y - FirstPoint.Y, 2));
        }

        bool ArePointsEqual(PointF FirstPoint, PointF SecondPoint)
        {
            float XDiff = Math.Abs(SecondPoint.X - FirstPoint.X);
            float YDiff = Math.Abs(SecondPoint.Y - FirstPoint.Y);

            double eps = 0.001;
            return XDiff < eps && YDiff < eps;
        }

        bool ArePlantsIntersecting(Plant FirstPlant, Plant SecondPlant)
        {
            PointF FirstCenter = FirstPlant.GetCenter();
            PointF SecondCenter = SecondPlant.GetCenter();

            double distanceBetweenCenters = CalcDistanceBetweenPoints(FirstCenter, SecondCenter);
            return distanceBetweenCenters < (FirstPlant.GetRadius() + SecondPlant.GetRadius());
        }

        int CalcIntersections()
        {
            int IntersectionsCount = 0;
            List<Tuple<PointF, PointF>> IntersectingPairs = new List<Tuple<PointF, PointF>>();

            for (int i = 0; i < mPlants.Count; ++i)
            {
                for (int j = 0; j < mPlants.Count; ++j)
                {
                    if (i == j)
                    {
                        continue;
                    }

                    PointF FirstCenter = mPlants[i].GetCenter();
                    PointF SecondCenter = mPlants[j].GetCenter();
                    Tuple<PointF, PointF> ReversePair = new Tuple<PointF, PointF>(SecondCenter, FirstCenter);

                    bool bFoundEqualPair = false;
                    foreach (Tuple<PointF, PointF> IntersectingPair in IntersectingPairs)
                    {
                        if (ArePointsEqual(IntersectingPair.Item1, ReversePair.Item1) && ArePointsEqual(IntersectingPair.Item2, ReversePair.Item2))
                        {
                            bFoundEqualPair = true;
                            break;
                        }
                    }

                    if (bFoundEqualPair)
                    {
                        continue;
                    }
                    
                    //double distanceBetweenCenters = CalcDistanceBetweenPoints(FirstCenter, SecondCenter);
                    //if ( distanceBetweenCenters < (mPlants[i].GetRadius() + mPlants[j].GetRadius()) )
                    if(ArePlantsIntersecting(mPlants[i], mPlants[j]))
                    {
                        Console.WriteLine( "Found intersection between plants at {0} and {1}", FirstCenter, SecondCenter );
                        IntersectingPairs.Add(new Tuple<PointF, PointF>(FirstCenter, SecondCenter));
                        ++IntersectionsCount;
                    }
                }
            }

            return IntersectionsCount;
        }

        List<Plant> SortPlantsForDraw()
        {
            List<Plant> SortedPlants = new List<Plant>(mPlants);

            SortedPlants.Sort(delegate(Plant FirstPlant, Plant SecondPlant)
            {
                if (FirstPlant.CalcArea() > SecondPlant.CalcArea())
                {
                    return -1;
                }
                return 1;
            });

            return SortedPlants;
        }

        void OnPictureBoxPaint(object sender, System.Windows.Forms.PaintEventArgs pe)
        {
            Console.WriteLine("Form1.OnPictureBoxPaint()");
        }

        void Form1_OnMouseHover(object sender, System.EventArgs e)
        {
            Console.WriteLine("Form1.OnMouseHover()");
        }

        Point LastMouseLocation;

        void Form1_OnMouseMove(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            Console.WriteLine("Form1.Form1_OnMouseMove()");
            Console.WriteLine("Location: {0}", e.Location);
            LastMouseLocation = e.Location;
            Console.WriteLine("Current time: {0}", DateTime.Now);
        }

        void SortPlantsByCoords()
        {
            mPlants.Sort(delegate(Plant FirstPlant, Plant SecondPlant)
            {
                double eps = 0.001;
                if (Math.Abs(FirstPlant.GetCenter().X - SecondPlant.GetCenter().X) < eps)
                {
                    return FirstPlant.GetCenter().Y < SecondPlant.GetCenter().Y ? -1 : 1;
                }
                return FirstPlant.GetCenter().X < SecondPlant.GetCenter().X ? -1 : 1;
            });
        }

        /*void OnPlantSelected(object sender, System.EventArgs e)
        {
            Console.WriteLine("OnPlantSelected()");
        }*/
        
        public Form1()
        {
            InitializeComponent();
            this.Paint += new PaintEventHandler(Form1_Paint);
            this.MouseHover += new EventHandler(Form1_OnMouseHover);
            this.MouseMove += new MouseEventHandler(Form1_OnMouseMove);

            FillAgeData();

            List<Plant> plants = CreatePlants();
            SetPlants(plants);
            SortPlantsByCoords();
            int IntersectionsNum = CalcIntersections();
            Console.WriteLine("IntersectionsNum: {0}", IntersectionsNum);

            /*comboBox1.Items.Add("Test item 1");
            comboBox1.Items.Add("Test item 2");
            comboBox1.Items.Add("Test item 3");*/

            foreach (Plant PlantItem in mPlants)
            {
                mPlantSelect.Items.Add(PlantItem.GetCenter());
            }

            //mPlantSelect.SelectionChangeCommitted += new EventHandler(OnPlantSelected);

            //pictureBox1.Paint += new PaintEventHandler(OnPictureBoxPaint);
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void Form1_Paint(object sender, System.Windows.Forms.PaintEventArgs pe)
        {
            if (bShouldDrawEllipse)
            {
                //Console.WriteLine("Form1.Form1_Paint() Drawing plants");

                Graphics g = pe.Graphics;

                int GridWidth = 400;
                int GridHeight = 400;
                int GridSectionsNum = 5;
                int GridTopLeftX = 0;
                int GridTopLeftY = 0;

                Grid grid = new Grid(GridWidth, GridHeight, GridSectionsNum, GridTopLeftX, GridTopLeftY);

                List<Plant> SortedPlants = SortPlantsForDraw();

                //Console.WriteLine("PictureBox index: {0}", this.Controls.GetChildIndex(pictureBox1));

                for (int i = 0; i < SortedPlants.Count; ++i)
                {
                    Plant currentPlant = SortedPlants[i];
                    currentPlant.Draw(grid, g, mShowAgeTypeCheckBox.Checked);
                }

                grid.Draw(g);

                g.Dispose();
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            //Console.WriteLine("Button 1 click");
            bShouldDrawEllipse = true;
            //Console.WriteLine("Invalidate() BEGIN");
            Invalidate();
            //Console.WriteLine("Invalidate() END");
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            Console.WriteLine("ComboBox::SelectedIndexChanged()");
            //Console.WriteLine("Selected Index: {0}", mPlantSelect.SelectedIndex);

            System.Drawing.PointF SelectedPlantCoords = (System.Drawing.PointF)mPlantSelect.SelectedItem;
            //Console.WriteLine("Selected Item: {0}", mPlantSelect.SelectedItem);
            Console.WriteLine("Selected Plant Coords: {0}", SelectedPlantCoords);

            foreach (Plant PlantItem in mPlants)
            {
                System.Drawing.PointF CurrentPlantCenter = PlantItem.GetCenter();

                if (ArePointsEqual(CurrentPlantCenter, SelectedPlantCoords))
                {
                    Console.WriteLine("Found matching plant!");
                    PlantItem.SetFillColor(System.Drawing.Color.Black);
                }
                else
                {
                    PlantItem.ResetFillColor();
                }
            }
            Invalidate();

            //Console.WriteLine("Selected Value: {0}", mPlantSelect.SelectedValue);
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

        private void mShowAgeTypeCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            Console.WriteLine("mShowAgeTypeCheckBox Checked {0}", mShowAgeTypeCheckBox.Checked);
        }

        private void button3_Click(object sender, EventArgs e)
        {
            /*System.Windows.Forms.PictureBox TestImage = new System.Windows.Forms.PictureBox();
            TestImage.Location = new System.Drawing.Point(336, 244);
            TestImage.Size = new Size(100, 50);
            TestImage.BackColor = Color.FromArgb(255, 192, 255);*/
        }
    }
}
