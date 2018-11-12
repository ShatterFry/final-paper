#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <vector>
#include <map>
#include <cfenv>
#include <algorithm>

#include <Renderer.h>
#include <VertexBufferLayout.h>
#include <Texture.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <AgeTypeDataEntry.h>
#include <Plant.h>

#include <Point.h>
#include <Circle.h>
#include <Rectangle.h>
#include <Aliases.h>
#include <Grid.h>
#include <AppManager.h>

#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 540

void PrintArray(std::vector<float>* array)
{
	for (int i = 0; i < array->size(); ++i)
	{
		std::cout << "[" << i << "]" << " = " << array->at(i) << std::endl;
	}
}

void CalcAvgRadius(const std::vector<Plant>& plants, std::shared_ptr<AgeRadiusVector>& averageRadiuses)
{
	std::vector<int> typesCount(static_cast<int>(EAgeType::MAX) - 1, 0);

	for (std::vector<Plant>::const_iterator i = plants.begin(); i != plants.end(); ++i)
	{
		EAgeType ageType = i->GetAgeType();
		float radius = i->GetRadius();

		averageRadiuses->at(static_cast<int>(ageType) - 1) += radius;
		++typesCount[static_cast<int>(ageType) - 1];
	}

	for (int i = 0; i < averageRadiuses->size(); ++i)
	{
		if (typesCount[i] != 0)
		{
			averageRadiuses->at(i) /= typesCount[i];
		}
	}
}

int main(void)
{
	//AgeType se, p, j, im, v, g1, g2, g3, ss, s;
	AppManager appManager;
	float seconds = 0.0f;
	float passedYears = 0.0f;
	std::vector<Plant> plants;
	std::shared_ptr<AgeRadiusVector> averageRadiuses = std::make_shared<AgeRadiusVector>(static_cast<int>(EAgeType::MAX) - 1, 0.0f);

	int halocnemumId = 0;
	plants.push_back(Plant(halocnemumId, 0.24243f, 4.50999f, 0.0f, static_cast<GLfloat>(12./30/2), EAgeType::g1, 10.0f));
	plants.push_back(Plant(halocnemumId, 0.27837f, 0.41184f, 0.0f, static_cast<GLfloat>(13./30/2), EAgeType::g3, 19.0f));
	plants.push_back(Plant(halocnemumId, 0.36258f, 3.3966f, 0.0f, static_cast<GLfloat>(12./30/2), EAgeType::g1, 10.0f));
	plants.push_back(Plant(halocnemumId, 0.62103f, 2.6586f, 0.0f, static_cast<GLfloat>(12./30/2), EAgeType::g1, 10.0f));
	plants.push_back(Plant(halocnemumId, 0.96021f, 3.41421f, 0.0f, static_cast<GLfloat>(13./30/2), EAgeType::g2, 14.0f));
	plants.push_back(Plant(halocnemumId, 1.15797f, 0.47283f, 0.0f, static_cast<GLfloat>(15./30/2), EAgeType::g3, 19.0f));
	plants.push_back(Plant(halocnemumId, 1.32927f, 3.0903f, 0.0f, static_cast<GLfloat>(2.0/30/2), EAgeType::j, 1.0f));
	plants.push_back(Plant(halocnemumId, 1.37949f, 3.23448f, 0.0f, static_cast<GLfloat>(2./30/2), EAgeType::j, 1.0f));
	plants.push_back(Plant(halocnemumId, 1.43334f, 3.4041f, 0.0f, static_cast<GLfloat>(3./30/2), EAgeType::im, 2.0f));
	plants.push_back(Plant(halocnemumId, 1.47093f, 3.08031f, 0.0f, static_cast<GLfloat>(2./30/2), EAgeType::j, 1.0f));
	plants.push_back(Plant(halocnemumId, 1.63161f, 2.96799f, 0.0f, static_cast<GLfloat>(13./30/2), EAgeType::g2, 14.0f));
	plants.push_back(Plant(halocnemumId, 1.70658f, 3.09102f, 0.0f, static_cast<GLfloat>(4./30/2), EAgeType::v, 5.0f));
	plants.push_back(Plant(halocnemumId, 2.01927f, 0.69852f, 0.0f, static_cast<GLfloat>(15./30/2), EAgeType::g3, 19.0f));
	plants.push_back(Plant(halocnemumId, 2.19657f, 3.59253f, 0.0f, static_cast<GLfloat>(12./30/2), EAgeType::g1, 10.0f));
	plants.push_back(Plant(halocnemumId, 2.58516f, 2.65056f, 0.0f, static_cast<GLfloat>(12./30/2), EAgeType::g2, 14.0f));
	plants.push_back(Plant(halocnemumId, 2.97834f, 0.49773f, 0.0f, static_cast<GLfloat>(11./30/2), EAgeType::g3, 19.0f));
	plants.push_back(Plant(halocnemumId, 3.36951f, 3.57684f, 0.0f, static_cast<GLfloat>(15./30/2), EAgeType::ss, 21.0f));
	plants.push_back(Plant(halocnemumId, 3.47964f, 0.69081f, 0.0f, static_cast<GLfloat>(12./30/2), EAgeType::g3, 19.0f));
	plants.push_back(Plant(halocnemumId, 3.49155f, 2.62122f, 0.0f, static_cast<GLfloat>(10./30/2), EAgeType::s, 23.0f));
	plants.push_back(Plant(halocnemumId, 3.97167f, 3.67674f, 0.0f, static_cast<GLfloat>(12./30/2), EAgeType::g3, 19.0f));
	plants.push_back(Plant(halocnemumId, 4.36764f, 0.33888f, 0.0f, static_cast<GLfloat>(15./30/2), EAgeType::ss, 21.0f));

	for (std::vector<Plant>::iterator i = plants.begin(); i != plants.end(); ++i)
	{
		//std::cout << "Index = " << i - plants.begin() << std::endl;
		//i->Print();
	}

	CalcAvgRadius(plants, averageRadiuses);

	GLFWwindow *window;

	// Initialize the library
	if (!glfwInit())
	{
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(960, 540, "Modelling", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) {
		std::cout << "Error!" << std::endl;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	{
		std::shared_ptr<std::vector<float>> positions = std::make_shared<std::vector<float>>();
		*positions = {
			-50.0f, -50.0f, 0.0f, 0.0f, //0
			50.0f, -50.0f, 1.0f, 0.0f, // 1
			50.0f, 50.0f, 1.0f, 1.0f, // 2
			-50.0f, 50.0f, 0.0f, 1.0f // 3
		};

		std::shared_ptr<UIntVector> indices = std::make_shared<UIntVector>();
		*indices = {
			0, 1, 2,
			2, 3, 0
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		VertexArray va;
		VertexBuffer<float> vb(positions);

		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);

		IndexBuffer ib(indices);

		glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

		std::shared_ptr<Shader> shader = std::make_shared<Shader>("res/shaders/Basic.shader");
		//Shader shader("res/shaders/Basic.shader");
		shader->Bind();
		shader->SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

		Texture texture("res/textures/ChernoLogo.png");
		texture.Bind();
		shader->SetUniform1i("u_Texture", 0);

		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		shader->Unbind();

		//std::shared_ptr<Renderer> renderer = std::make_shared<Renderer>();
		Renderer renderer;

		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init();
		ImGui::StyleColorsDark();

		glm::vec3 translationA(200, 200, 0);
		glm::vec3 translationB(400, 200, 0);
		glm::vec3 gridTranslation(0, 0, 0);
		glm::vec3 circleTranslation(0, 0, 0);

		float r = 0.0f;
		float increment = 0.05f;

		while (!glfwWindowShouldClose(window))
		{
			renderer.Clear();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
				glm::mat4 mvp = proj * view * model;
				shader->Bind();
				shader->SetUniformMat4f("u_MVP", mvp);
				shader->SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
				EDrawTypes drawType = EDrawTypes::EDT_TRIANGLES;

				renderer.Draw(va, ib, shader, drawType);
			}

			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
				glm::mat4 mvp = proj * view * model;
				shader->Bind();
				shader->SetUniformMat4f("u_MVP", mvp);
				shader->SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
				EDrawTypes drawType = EDrawTypes::EDT_TRIANGLES;

				renderer.Draw(va, ib, shader, drawType);
			}

			Rectangle boundingRectangle = Rectangle(Point2f(0.0f, 0.0f), Point2f(0.0f, 250.0f), Point2f(250.0f, 250.0f), Point2f(250.0f, 0.0f));
			std::shared_ptr<Grid> grid = std::make_shared<Grid>(boundingRectangle, 5, 5);
			grid->Draw(proj, view, gridTranslation, shader, renderer);

			{
				std::shared_ptr<Circle> circle = std::make_shared<Circle>(100.0f, Point2f(100.0f, 100.0f), 10);
				//circle->Draw(proj, view, circleTranslation, shader, renderer);
			}

			if (r > 1.0f) {
				increment = -0.05f;
			}
			else if (r < 0.0f) {
				increment = 0.05f;
			}

			r += increment;

			{
				ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, 960.0f);
				ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, 960.0f);
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			seconds = glfwGetTime();
			std::cout << seconds << std::endl;

			passedYears = seconds / 3;

			std::vector<int> plantsToDeleteIdx;

			for (std::vector<Plant>::iterator i = plants.begin(); i != plants.end(); ++i)
			{

				int index = static_cast<int>(i - plants.begin());
				
				std::shared_ptr<AgeTypeData> plantAgeData = appManager.GetAgeTypeData(halocnemumId);
				if ((passedYears - i->GetAgeAccumulated()) > plantAgeData->at(static_cast<int>(i->GetAgeType()) - 1).maxAge)
				{
					std::cout << "Switching ages!" << std::endl;

					EAgeType newAgeType = static_cast<EAgeType>(static_cast<int>(i->GetAgeType()) + 1);
					if (newAgeType != EAgeType::MAX)
					{
						i->SetAgeType(newAgeType);
						i->SetAgeAccumulated(i->GetAgeAccumulated() + (passedYears - i->GetAgeAccumulated()));
						i->SetRadius(averageRadiuses->at(static_cast<int>(newAgeType) - 1));
					}
					else
					{
						plantsToDeleteIdx.push_back(index);
					}
				}
			}

			std::sort(plantsToDeleteIdx.begin(), plantsToDeleteIdx.end(), [](int a, int b) {return a > b; });

			for (std::vector<int>::iterator i = plantsToDeleteIdx.begin(); i != plantsToDeleteIdx.end(); ++i)
			{
				std::cout << "Vector size = " << plants.size() << std::endl;
				std::cout << "Will delete element with index " << *i << std::endl;
				plants.erase(plants.begin() + *i);
			}

			for (std::vector<Plant>::iterator i = plants.begin(); i != plants.end(); ++i)
			{
				Rectangle gridSquare = grid->GetBoundingRectangle();

				float gridXLeft = gridSquare.GetBottomLeftPoint()._x;
				float gridXRight = gridSquare.GetBottomRightPoint()._x;

				float gridYBottom = gridSquare.GetBottomLeftPoint()._y;
				float gridYTop = gridSquare.GetTopLeftPoint()._y;

				float windowX = gridXLeft + i->GetX() * gridSquare.GetWidht() / 5;
				float windowY = gridYBottom + i->GetY() * gridSquare.GetHeight() / 5;
				float windowZ = i->GetZ();
				float windowRadius = i->GetRadius() * gridSquare.GetWidht() / 5;

				Circle circle(windowRadius, Point2f(windowX, windowY), 10);
				circle.Draw(proj, view, circleTranslation, shader, renderer);
			}

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();

	return 0;
}