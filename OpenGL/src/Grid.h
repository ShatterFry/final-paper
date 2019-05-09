#pragma once

#include "Rectangle.h"
#include "Aliases.h"

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

class Grid
{
public:
	Grid();
	Grid(Rectangle rectangle, int rows, int columns);

	void SetBoundingRectangle(const Rectangle rectangle);
	Rectangle GetBoundingRectangle() const;

	std::shared_ptr<LinesVector> GetLines() const;
	void Draw(const glm::mat4& proj, const glm::mat4& view, const glm::vec3& translation, std::shared_ptr<class Shader>& shader, const class Renderer& renderer);
private:
	Rectangle _boundingRectangle;
	std::shared_ptr<LinesVector> _lines = std::make_shared<LinesVector>();
	std::shared_ptr<UIntVector> _lineIndices = std::make_shared<UIntVector>();

	std::shared_ptr<FloatVector> Decompose() const;
	void FillLineIndices();
};
