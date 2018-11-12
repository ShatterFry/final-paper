#include <Grid.h>

#include <VertexArray.h>
#include <IndexBuffer.h>
#include <Shader.h>

#include <Renderer.h>

Grid::Grid()
{
	
}

Grid::Grid(Rectangle rectangle, int rows, int columns)
{
	_boundingRectangle = rectangle;

	Point2f topLeft = _boundingRectangle.GetTopLeftPoint();
	Point2f topRight = _boundingRectangle.GetTopRightPoint();
	Point2f bottomLeft = _boundingRectangle.GetBottomLeftPoint();
	Point2f bottomRight = _boundingRectangle.GetBottomRightPoint();

	std::shared_ptr<LinesVector> rectangleLines = rectangle.GetLines();

	for (auto it = rectangleLines->begin(); it != rectangleLines->end(); ++it)
	{
		_lines->push_back(*it);
	}

	float singleRowHeight = _boundingRectangle.GetHeight() / rows;
	float singleColumnWidth = _boundingRectangle.GetWidht() / columns;

	for (int i = 0; i < rows-1; ++i)
	{
		Point2f leftPoint(bottomLeft._x, bottomLeft._y + singleRowHeight *(i+1));
		Point2f rightPoint(bottomRight._x, bottomRight._y + singleRowHeight *(i + 1));
		_lines->push_back(Line(leftPoint, rightPoint));
	}

	for (int i = 0; i < columns-1; ++i)
	{
		Point2f topPoint(topLeft._x + (i+1)*singleColumnWidth, topLeft._y);
		Point2f bottomPoint(bottomLeft._x + (i + 1)*singleColumnWidth, bottomLeft._y);
		_lines->push_back(Line(bottomPoint, topPoint));
	}
}

void Grid::SetBoundingRectangle(Rectangle boundingRectangle)
{
	_boundingRectangle = boundingRectangle;
}

Rectangle Grid::GetBoundingRectangle() const
{
	return _boundingRectangle;
}

std::shared_ptr<LinesVector> Grid::GetLines() const
{
	return _lines;
}

std::shared_ptr<FloatVector> Grid::Decompose() const
{
	auto decomposition = std::make_shared<std::vector<float>>();

	//const std::shared_ptr<LinesVector>& lines = grid->GetLines();
	for (auto it = _lines->begin(); it != _lines->end(); ++it)
	{
		decomposition->push_back(it->GetBegin()._x);
		decomposition->push_back(it->GetBegin()._y);

		if (it - _lines->begin() > 3)
		{
			decomposition->push_back(it->GetEnd()._x);
			decomposition->push_back(it->GetEnd()._y);
		}
	}

	return decomposition;
}

void Grid::FillLineIndices()
{
	int rectangleMaxNum = 4;
	for (int i = 0; i < rectangleMaxNum; ++i)
	{
		_lineIndices->push_back(i);
		if (i < rectangleMaxNum - 1)
		{
			_lineIndices->push_back(i + 1);
		}
		else
		{
			_lineIndices->push_back(0);
		}
	}

	//std::shared_ptr<LinesVector> gridLines = grid->GetLines();
	int allLinesCount = _lines->size();
	int additionalLinesCount = allLinesCount - 4;
	if (allLinesCount > 4)
	{
		for (int i = 4; i < 4 + additionalLinesCount * 2; i += 2)
		{
			_lineIndices->push_back(i);
			_lineIndices->push_back(i + 1);
		}
	}
}

void Grid::Draw(const glm::mat4& proj, const glm::mat4& view, const glm::vec3& translation, std::shared_ptr<Shader>& shader, const Renderer& renderer)
{
	const std::shared_ptr<FloatVector>& gridPositions = Decompose();

	FillLineIndices();

	VertexArray vaLines;
	VertexBuffer<float> vbLines(gridPositions);

	VertexBufferLayout layoutLines;
	layoutLines.Push<float>(2);
	vaLines.AddBuffer(vbLines, layoutLines);

	IndexBuffer ibLines(_lineIndices);

	vaLines.Unbind();
	vbLines.Unbind();
	ibLines.Unbind();

	glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
	glm::mat4 mvp = proj * view * model;
	shader->Bind();
	shader->SetUniformMat4f("u_MVP", mvp);
	shader->SetUniform4f("u_Color", 1.0f, 0.3f, 0.3f, 1.0f);
	EDrawTypes drawType = EDrawTypes::EDT_LINES;

	renderer.Draw(vaLines, ibLines, shader, drawType);
}