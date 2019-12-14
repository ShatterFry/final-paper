#pragma once

#include "Aliases.h"
#include "Line.h"
#include "Rectangle.h"

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

#include <memory>

class Grid
{
public:
	Grid();
	Grid(class URectangle rectangle, int rows, int columns);

	void SetBoundingRectangle(const class URectangle rectangle);
	class URectangle GetBoundingRectangle() const;

	std::shared_ptr<LinesVector> GetLines() const;
	void Draw(const glm::mat4& proj, const glm::mat4& view, const glm::vec3& translation, std::shared_ptr<class Shader>& shader, const class Renderer& renderer);
private:
    URectangle _boundingRectangle;
	std::shared_ptr<LinesVector> _lines = std::make_shared<LinesVector>();
	std::shared_ptr<UIntVector> _lineIndices = std::make_shared<UIntVector>();

	std::shared_ptr<FloatVector> Decompose() const;
	void FillLineIndices();
};
