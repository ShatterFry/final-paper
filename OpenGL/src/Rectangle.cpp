#include <Rectangle.h>

void Rectangle::SetTopLeftPoint(const Point2f point)
{
	_topLeftPoint = point;
}

Point2f Rectangle::GetTopLeftPoint() const
{
	return _topLeftPoint;
}

void Rectangle::SetTopRightPoint(const Point2f point)
{
	_topRightPoint = point;
}

Point2f Rectangle::GetTopRightPoint() const
{
	return _topRightPoint;
}

void Rectangle::SetBottomRightPoint(const Point2f point)
{
	_bottomRightPoint = point;
}

Point2f Rectangle::GetBottomRightPoint() const
{
	return _bottomRightPoint;
}

void Rectangle::SetBottomLeftPoint(const Point2f point)
{
	_bottomLeftPoint = point;
}

Point2f Rectangle::GetBottomLeftPoint() const
{
	return _bottomLeftPoint;
}

Rectangle::Rectangle()
{
	
}

Rectangle::Rectangle(const Point2f bottomLeft, const Point2f topLeft, const Point2f topRight, const Point2f bottomRight)
{
	_bottomLeftPoint = bottomLeft;
	_topLeftPoint = topLeft;
	_topRightPoint = topRight;
	_bottomRightPoint = bottomRight;

	_pointsVectorSPtr->push_back(bottomLeft);
	_pointsVectorSPtr->push_back(topLeft);
	_pointsVectorSPtr->push_back(topRight);
	_pointsVectorSPtr->push_back(bottomRight);

	_leftLine = Line(_bottomLeftPoint, _topLeftPoint);
	_topLine = Line(_topLeftPoint, _topRightPoint);
	_rightLine = Line(_topRightPoint, _bottomRightPoint);
	_bottomLine = Line(_bottomRightPoint, bottomLeft);

	if (!_lines)
	{
		_lines = std::make_shared<LinesVector>();
	}
	_lines->push_back(_leftLine);
	_lines->push_back(_topLine);
	_lines->push_back(_rightLine);
	_lines->push_back(_bottomLine);
}

std::shared_ptr<Point2fVector> Rectangle::GetPointsVector() const
{
	return _pointsVectorSPtr;
}

std::shared_ptr<LinesVector> Rectangle::GetLines() const
{
	return _lines;
}