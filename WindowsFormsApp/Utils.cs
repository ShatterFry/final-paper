using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WindowsFormsApp
{
    internal class Utils
    {
        public static double CalcDistanceBetweenPoints(System.Drawing.PointF FirstPoint, System.Drawing.PointF SecondPoint)
        {
            return Math.Sqrt(Math.Pow(SecondPoint.X - FirstPoint.X, 2) + Math.Pow(SecondPoint.Y - FirstPoint.Y, 2));
        }
    }
}
