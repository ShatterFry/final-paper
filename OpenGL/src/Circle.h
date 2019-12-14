#pragma once

#include "Point.h"
#include "Aliases.h"

#include <vector>

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

using CircleVector = std::vector<class Circle>;

class Circle
{
private:
	float _radius;
	Point2f _center;
	int _numberOfSides;
	std::shared_ptr<Point2fVector> _positionsPoints;

	std::shared_ptr<UIntVector> _indices = std::make_shared<UIntVector>();

	void CalculatePoints();
	std::shared_ptr<FloatVector> Decompose();
	void FillIndices();

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
	std::shared_ptr<Point2fVector> GetPositionsPoints();

	void Print();
	void Draw(const glm::mat4& proj, const glm::mat4& view, const glm::vec3& translation, std::shared_ptr<class Shader>& shader, const class Renderer& renderer);
};
