#include "Rectangle.h"

void URectangle::SetTopLeftPoint(const Point2f point)
{
	_topLeftPoint = point;
}

Point2f URectangle::GetTopLeftPoint() const
{
	return _topLeftPoint;
}

void URectangle::SetTopRightPoint(const Point2f point)
{
	_topRightPoint = point;
}

Point2f URectangle::GetTopRightPoint() const
{
	return _topRightPoint;
}

void URectangle::SetBottomRightPoint(const Point2f point)
{
	_bottomRightPoint = point;
}

Point2f URectangle::GetBottomRightPoint() const
{
	return _bottomRightPoint;
}

void URectangle::SetBottomLeftPoint(const Point2f point)
{
	_bottomLeftPoint = point;
}

Point2f URectangle::GetBottomLeftPoint() const
{
	return _bottomLeftPoint;
}

URectangle::URectangle()
{

}

URectangle::URectangle(const Point2f bottomLeft, const Point2f topLeft, const Point2f topRight, const Point2f bottomRight)
{
	_bottomLeftPoint = bottomLeft;
	_topLeftPoint = topLeft;
	_topRightPoint = topRight;
	_bottomRightPoint = bottomRight;

	_height = topLeft._y - bottomLeft._y;
	_width = topRight._x - topLeft._x;

	_points->push_back(bottomLeft);
	_points->push_back(topLeft);
	_points->push_back(topRight);
	_points->push_back(bottomRight);

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

std::shared_ptr<LinesVector> URectangle::GetLines() const
{
	return _lines;
}
