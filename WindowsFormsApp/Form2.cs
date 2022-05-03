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
    public partial class Form2 : Form
    {
        public Form2()
        {
            InitializeComponent();
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void dataGridView1_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            //dataGridView1.Columns.Add("Column Name", "Header text");

            dataGridView1.ColumnCount = 5;

            dataGridView1.Columns[0].Name = "Name";
            dataGridView1.Columns[1].Name = "Age type";
            dataGridView1.Columns[2].Name = "X";
            dataGridView1.Columns[3].Name = "Y";
            dataGridView1.Columns[4].Name = "Radius";

            string HALOCNEMUM_STROBILACEUM = "Halocnemum Strobilaceum";

            // TODO: read data from XML
            string[] row0 = {HALOCNEMUM_STROBILACEUM, "g1", "0,24243", "4,50999", "0,2"};
            dataGridView1.Rows.Add(row0);

            string[] row1 = {HALOCNEMUM_STROBILACEUM, "g3", "0,27837", "0,41184", "0,217"};
            dataGridView1.Rows.Add(row1);

            string[] row3 = {HALOCNEMUM_STROBILACEUM, "g1", "0,36258", "3,3966", "0,2"};
            dataGridView1.Rows.Add(row3);
        }
    }
}
