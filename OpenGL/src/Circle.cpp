#ifndef _USE_MATH_DEFINES
    #define _USE_MATH_DEFINES
#endif

#ifdef __STRICT_ANSI__
    #undef __STRICT_ANSI__
#endif

#include <cmath>

#include "Circle.h"

#include <iostream>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#include "Renderer.h"

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

std::shared_ptr<Point2fVector> Circle::GetPositionsPoints()
{
	return _positionsPoints;
}

int Circle::GetNumberOfSides()
{
	return _numberOfSides;
}

std::shared_ptr<FloatVector> Circle::Decompose()
{
	auto decomposition = std::make_shared<FloatVector>();

	for (int i = 0; i < _positionsPoints->size(); ++i)
	{
		decomposition->push_back(_positionsPoints->at(i)._x);
		decomposition->push_back(_positionsPoints->at(i)._y);
	}

	return decomposition;
}

void Circle::FillIndices()
{
	int indicesOffset = 0;

	int numOfPosPoints = _positionsPoints->size();

	for (int i = 0; i < numOfPosPoints - 1; ++i)
	{
		/*indices->push_back(0);
		indices->push_back(i + 1);
		indices->push_back(i == (numberOfSides - 1) ? 1 : (i + 2));*/

		// 0-10
		// 0 1 2
		// 11-21
		// 11 12 13
		_indices->push_back(indicesOffset);
		_indices->push_back(indicesOffset + i + 1);
		_indices->push_back(i == (numOfPosPoints - 2) ? (indicesOffset + 1) : (indicesOffset + i + 2));
	}
	indicesOffset += numOfPosPoints;
}

void Circle::Draw(const glm::mat4& proj, const glm::mat4& view, const glm::vec3& translation, std::shared_ptr<Shader>& shader, const Renderer& renderer)
{
	const std::shared_ptr<FloatVector>& circlesPositions = Decompose();

	FillIndices();

	VertexArray vaCircles;
	VertexBuffer<float> vbCircles(circlesPositions);

	VertexBufferLayout circlesLayout;
	circlesLayout.Push<float>(2);
	vaCircles.AddBuffer(vbCircles, circlesLayout);

	IndexBuffer ibCircles(_indices);

	vaCircles.Unbind();
	vbCircles.Unbind();
	ibCircles.Unbind();

	glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
	glm::mat4 mvp = proj * view * model;
	shader->Bind();
	shader->SetUniformMat4f("u_MVP", mvp);
	shader->SetUniform4f("u_Color", 0.1f, 0.1f, 1.0f, 1.0f);
	EDrawTypes drawType = EDrawTypes::EDT_TRIANGLES;

	renderer.Draw(vaCircles, ibCircles, shader, drawType);
}
