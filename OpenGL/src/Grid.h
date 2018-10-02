#pragma once
#include <Rectangle.h>

class Grid
{
public:
	Grid();
	Grid(Rectangle rectangle, int rows, int columns);

	void SetBoundingRectangle(const Rectangle rectangle);
	Rectangle GetBoundingRectangle() const;

	std::shared_ptr<LinesVector> GetLines() const;
private:
	Rectangle _boundingRectangle;
	std::shared_ptr<LinesVector> _lines = std::make_shared<std::vector<Line>>();
};