using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApp
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            AgeType se = new AgeType();
            se._minAge = 1;
            se._minAge = 1;

            AgeType g1 = new AgeType();
            g1._minAge = 1;
            g1._maxAge = 4;

            List<Plant> plants = new List<Plant>();
            plants.Add( new Plant( g1, 0.24243, 4.50999, (12.0 / 30.0 / 2.0) ) );

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1());
        }
    }
}
