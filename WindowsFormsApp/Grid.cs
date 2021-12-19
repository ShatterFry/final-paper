using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

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

        public void Draw(System.Drawing.Graphics inGraphics)
        {
            int maxY = mTopLeftY + mHeight;

            int gridSectionWidth = GetSectionWidth();
            int gridSectionHeight = mHeight / mSectionsNum;

            List<System.Drawing.Rectangle> gridRects = new List<System.Drawing.Rectangle>();
            for (int i = 0; i < mSectionsNum; ++i)
            {
                for (int j = 0; j < mSectionsNum; ++j)
                {
                    gridRects.Add(new System.Drawing.Rectangle(mTopLeftX + j * gridSectionWidth, mTopLeftY + i * gridSectionHeight, gridSectionWidth, gridSectionHeight));
                }
            }

            System.Drawing.Pen rectPen = new System.Drawing.Pen(System.Drawing.Color.Chocolate);
            inGraphics.DrawRectangles(rectPen, gridRects.ToArray());
            rectPen.Dispose();
        }

        public void AbsoluteToLocalPoint(System.Drawing.Point InPoint, out System.Drawing.PointF LocalPoint)
        {
            float LocalX = (float)InPoint.X / GetSectionWidth();
            float LocalY = (float)mSectionsNum - ((float)InPoint.Y / GetSectionHeight());

            LocalPoint = new System.Drawing.PointF(LocalX, LocalY);
        }
    }
}
