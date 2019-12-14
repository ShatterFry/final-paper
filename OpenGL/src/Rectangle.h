#pragma once

#include "Line.h"

using RectangleVector = std::vector<class URectangle>;

class URectangle
{
public:
	URectangle();
	URectangle(const Point2f bottomLeft, const Point2f topLeft, const Point2f topRight, const Point2f bottomRight);

	void SetTopLeftPoint(const Point2f point);
	Point2f GetTopLeftPoint() const;

	void SetTopRightPoint(const Point2f point);
	Point2f GetTopRightPoint() const;

	void SetBottomRightPoint(const Point2f point);
	Point2f GetBottomRightPoint() const;

	void SetBottomLeftPoint(const Point2f point);
	Point2f GetBottomLeftPoint() const;

	float GetHeight() const { return _height; }
	float GetWidht() const { return _width; }

	std::shared_ptr<Point2fVector> GetPoints() const { return _points; }
	std::shared_ptr<LinesVector> GetLines() const;
private:
	Point2f _bottomLeftPoint;
	Point2f _topLeftPoint;
	Point2f _topRightPoint;
	Point2f _bottomRightPoint;

	float _height = 0;
	float _width = 0;

	std::shared_ptr<Point2fVector> _points = std::make_shared<Point2fVector>();

	Line _leftLine;
	Line _topLine;
	Line _rightLine;
	Line _bottomLine;

	std::shared_ptr<LinesVector> _lines;
};
