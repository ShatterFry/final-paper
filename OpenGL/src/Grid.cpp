#include <Grid.h>

Grid::Grid()
{
	
}

Grid::Grid(Rectangle rectangle, int rows, int columns)
{
	_boundingRectangle = rectangle;

	Point2f topLeft = _boundingRectangle.GetTopLeftPoint();
	Point2f topRight = _boundingRectangle.GetTopRightPoint();
	Point2f bottomLeft = _boundingRectangle.GetBottomLeftPoint();
	Point2f bottomRight = _boundingRectangle.GetBottomRightPoint();

	std::shared_ptr<LinesVector> rectangleLines = rectangle.GetLines();

	for (auto it = rectangleLines->begin(); it != rectangleLines->end(); ++it)
	{
		_lines->push_back(*it);
	}

	float rectangleHeight = topLeft._y - bottomLeft._y;
	float rectangleWidth = topRight._x - topLeft._x;

	float singleRowHeight = rectangleHeight / rows;
	float singleColumnWidth = rectangleWidth / columns;

	for (int i = 0; i < rows-1; ++i)
	{
		Point2f leftPoint(bottomLeft._x, bottomLeft._y + singleRowHeight *(i+1));
		Point2f rightPoint(bottomRight._x, bottomRight._y + singleRowHeight *(i + 1));
		_lines->push_back(Line(leftPoint, rightPoint));
	}

	for (int i = 0; i < columns-1; ++i)
	{
		Point2f topPoint(topLeft._x + (i+1)*singleColumnWidth, topLeft._y);
		Point2f bottomPoint(bottomLeft._x + (i + 1)*singleColumnWidth, bottomLeft._y);
		_lines->push_back(Line(bottomPoint, topPoint));
	}
}

void Grid::SetBoundingRectangle(Rectangle boundingRectangle)
{
	_boundingRectangle = boundingRectangle;
}

Rectangle Grid::GetBoundingRectangle() const
{
	return _boundingRectangle;
}

std::shared_ptr<LinesVector> Grid::GetLines() const
{
	return _lines;
}