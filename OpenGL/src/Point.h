#pragma once

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