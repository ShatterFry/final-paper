#include <Line.h>

Line::Line()
{
	_begin = Point2f(0.0f, 0.0f);
	_end = Point2f(0.0f, 0.0f);
}

Line::Line(const Point2f begin, const Point2f end)
{
	_begin = begin;
	_end = end;
}

Point2f Line::GetBegin() const
{
	return _begin;
}

Point2f Line::GetEnd() const
{
	return _end;
}