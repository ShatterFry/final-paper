﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WindowsFormsApp
{
    public class Plant
    {
        private AgeType _AgeType;
        private double _X;
        private double _Y;
        private double _Radius;
        private double _InitialAge;

        static System.Drawing.Color DEFAULT_FILL_COLOR = System.Drawing.Color.Aquamarine;
        private System.Drawing.Color FillColor = DEFAULT_FILL_COLOR;

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

        public void SetX(double value)
        {
            _X = value;
        }

        public double GetY()
        {
            return _Y;
        }

        public void SetY(double value)
        {
            _Y = value;
        }

        public double GetRadius()
        {
            return _Radius;
        }

        public double GetDiameter()
        {
            return (double)(GetRadius() * 2.0);
        }

        public void SetRadius(double value)
        {
            _Radius = value;
        }

        public System.Drawing.Color GetFillColor()
        {
            return FillColor;
        }

        public void SetFillColor(System.Drawing.Color inFillColor)
        {
            FillColor = inFillColor;
        }

        public void ResetFillColor()
        {
            this.SetFillColor(DEFAULT_FILL_COLOR);
        }

        public AgeType GetAgeType()
        {
            return _AgeType;
        }

        public void Draw(Grid inGrid, System.Drawing.Graphics inGraphics, bool bPrintAgeType)
        {
            double boundRectX = inGrid.GetTopLeftX() + inGrid.GetSectionWidth() * (GetX() - GetRadius());
            int maxY = inGrid.GetTopLeftY() + inGrid.GetHeight();
            double boundRectY = maxY - inGrid.GetSectionHeight() * (GetY() + GetRadius());

            float plantDiameter = (float)GetDiameter();

            float rectHeight = plantDiameter * inGrid.GetSectionHeight();
            float rectiWidth = plantDiameter * inGrid.GetSectionWidth();

            System.Drawing.RectangleF boundingRect = new System.Drawing.RectangleF((float)boundRectX, (float)boundRectY, rectHeight, rectiWidth);
            System.Drawing.Color EllipseBorderColor = System.Drawing.Color.Red;
            System.Drawing.Pen ellipseBorderPen = new System.Drawing.Pen(EllipseBorderColor);

            inGraphics.DrawEllipse(ellipseBorderPen, boundingRect);
            System.Drawing.SolidBrush ellipseFillBrush = new System.Drawing.SolidBrush(GetFillColor());
            inGraphics.FillEllipse(ellipseFillBrush, boundingRect);

            if (bPrintAgeType)
            {
                System.Drawing.Font textFont = new System.Drawing.Font("Arial", 16);
                System.Drawing.SolidBrush textBrush = new System.Drawing.SolidBrush(System.Drawing.Color.Black);
                float textX = (float)(inGrid.GetTopLeftX() + inGrid.GetSectionWidth() * (GetX() - GetRadius()));
                float textY = (float)(maxY - inGrid.GetSectionHeight() * (GetY() + GetRadius()));
                inGraphics.DrawString(GetAgeType().ToString(), textFont, textBrush, textX, textY);
            }

            ellipseBorderPen.Dispose();
        }

        public System.Drawing.PointF GetCenter()
        {
            return new System.Drawing.PointF((float)GetX(), (float)GetY());
        }

        public double CalcArea()
        {
            return Math.PI * Math.Pow(GetRadius(), 2);
        }

        public bool IsPointBelongsToPlant(System.Drawing.PointF InPoint)
        {
            System.Drawing.PointF PlantCenter = GetCenter();

            if (Utils.CalcDistanceBetweenPoints(PlantCenter, InPoint) < GetRadius())
            {
                return true;
            }

            return false;
        }
    }
}
