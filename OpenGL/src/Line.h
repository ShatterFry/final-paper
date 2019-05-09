#pragma once
#include "Point.h"

using LinesVector = std::vector<class Line>;

class Line
{
public:
	Line();
	Line(const Point2f begin, const Point2f end);

	Point2f GetBegin() const;
	Point2f GetEnd() const;
private:
	Point2f _begin;
	Point2f _end;
};
