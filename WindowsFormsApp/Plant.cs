using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WindowsFormsApp
{
    class Plant
    {
        public AgeType _AgeType;
        public double _X;
        public double _Y;
        public double _Radius;
        public double _InitialAge;

        public Plant()
        {

        }

        public Plant(AgeType ageType, double x, double y, double radius)
        {
            _AgeType = ageType;
            _X = x;
            _Y = y;
            _Radius = radius;
        }

        ~Plant()
        {

        }
    }
}
