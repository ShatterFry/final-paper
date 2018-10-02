#include <Circle.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

void Circle::CalculatePoints()
{
	_positionsPoints->push_back(_center);

	float rotationAngleDegrees = 360.0f / _numberOfSides;
	float rotationAngleRadians = rotationAngleDegrees * M_PI / 180.0f;

	for (int i = 1; i < _numberOfSides + 1; ++i)
	{
		float xRotated = _center._x + _radius * cos(rotationAngleRadians * (i - 1));
		float yRotated = _center._y + _radius * sin(rotationAngleRadians * (i - 1));
		_positionsPoints->push_back(Point2f(xRotated, yRotated));
	}
}

Circle::Circle()
{

}

Circle::Circle(float radius, Point2f center, int numberOfSides)
{
	Circle();

	_radius = radius;
	_center = center;
	_numberOfSides = numberOfSides;

	_positionsPoints = std::make_shared<std::vector<Point2f>>();

	CalculatePoints();
}

void Circle::Print()
{
	for (int i = 0; i < _positionsPoints->size(); ++i)
	{
		std::cout << "(" << _positionsPoints->at(i)._x << "; " << _positionsPoints->at(i)._y << ")\n";
	}
}

void Circle::SetPositionsPoints(std::shared_ptr<std::vector<Point2f>> positionsPoints)
{
	_positionsPoints = positionsPoints;
}

std::shared_ptr<std::vector<Point2f>> Circle::GetPositionsPoints()
{
	return _positionsPoints;
}

int Circle::GetNumberOfSides()
{
	return _numberOfSides;
}
