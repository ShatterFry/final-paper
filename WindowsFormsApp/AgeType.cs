using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WindowsFormsApp
{
    public enum AgeType
    { 
        se,
        p,
        j,
        im,
        v,
        g1,
        g2,
        g3,
        ss,
        s,
        MAX
    }
    public class AgeTypeData
    {
        public double _minAge;
        public double _maxAge;
    }

    class AgeTypeConverter
    {
        public static AgeType FromString(string inString)
        {
            switch (inString)
            {
                case "se":
                    return AgeType.se;
                case "p":
                    return AgeType.p;
                case "j":
                    return AgeType.j;
                case "im":
                    return AgeType.im;
                case "v":
                    return AgeType.v;
                case "g1":
                    return AgeType.g1;
                case "g2":
                    return AgeType.g2;
                case "g3":
                    return AgeType.g3;
                case "ss":
                    return AgeType.ss;
                case "s":
                    return AgeType.s;
            }

            return AgeType.MAX;
        }
    }
}
