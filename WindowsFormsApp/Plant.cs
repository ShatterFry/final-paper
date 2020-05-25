using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WindowsFormsApp
{
    public class Plant
    {
        //public AgeType _AgeType;
        private AgeType _AgeType;
        private double _X;
        private double _Y;
        private double _Radius;
        private double _InitialAge;

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

        public double GetX()
        {
            return _X;
        }

        public double GetY()
        {
            return _Y;
        }

        public double GetRadius()
        {
            return _Radius;
        }
    }
}
