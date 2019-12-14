#pragma once

#include <memory>
#include <vector>

using Point2fVector = std::vector<struct Point2f>;

struct Point2f
{
	float _x;
	float _y;

	Point2f()
	{
		_x = 0.0f;
		_y = 0.0f;
	}

	Point2f(const float x, const float y)
	{
		_x = x;
		_y = y;
	}
};

using Point2fVector = std::vector<Point2f>;