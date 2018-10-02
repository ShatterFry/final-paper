#pragma once

#include <Point.h>
#include <vector>

class Circle
{
private:
	float _radius;
	Point2f _center;
	int _numberOfSides;
	std::shared_ptr<std::vector<Point2f>> _positionsPoints;

	void CalculatePoints();
public:
	Circle();
	Circle(const float radius, Point2f center, const int numberOfSides);

	void SetRadius(const float radius);
	float GetRadius();

	void SetCenter(const Point2f center);
	Point2f GetCenter();

	void SetNumberOfSides(const int numberOfSides);
	int GetNumberOfSides();

	void SetPositionsPoints(std::shared_ptr<std::vector<Point2f>> positionsPoints);
	std::shared_ptr<std::vector<Point2f>> GetPositionsPoints();

	void Print();
};