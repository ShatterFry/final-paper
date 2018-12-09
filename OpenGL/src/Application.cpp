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

/*void CalcAvgRadius(const std::vector<Plant>& plants, std::shared_ptr<AgeRadiusVector>& averageRadiuses)
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
}*/

int main(void)
{
	//AgeType se, p, j, im, v, g1, g2, g3, ss, s;
	AppManager appManager;
	float passedTime;
	float initialTime;
	float passedYears;
	std::vector<Plant> plants;
	std::shared_ptr<AgeRadiusVector> averageRadiuses = std::make_shared<AgeRadiusVector>(static_cast<int>(EAgeType::MAX) - 1, 0.0f);
	
	bool bProduceChildren = false;
	bool bStartBtnClicked = false;

	int halocnemumId = 0;

	//TODO - add setting map scale in menu
	float mapScale = 1.0f / 30.0f;
	bool bDataWasScaled = false;
	auto scaleToReal = [](std::vector<Plant>& inPlants, const float scale)
	{
		for (int i = 0; i < inPlants.size(); ++i)
		{
			inPlants[i].SetDiameter(inPlants[i].GetDiameter() * scale);
		}
	};

	// TODO - scale diameters
	plants.push_back(Plant(halocnemumId, { 0.24243f, 4.50999f, 0.0f }, (12.0f), EAgeType::g1));
	plants.push_back(Plant(halocnemumId, { 0.27837f, 0.41184f, 0.0f }, (13.0f), EAgeType::g3));
	plants.push_back(Plant(halocnemumId, { 0.36258f, 3.3966f, 0.0f }, (12.0f), EAgeType::g1));
	plants.push_back(Plant(halocnemumId, { 0.62103f, 2.6586f, 0.0f }, (12.0f), EAgeType::g1));
	plants.push_back(Plant(halocnemumId, { 0.96021f, 3.41421f, 0.0f }, (13.0f), EAgeType::g2));
	plants.push_back(Plant(halocnemumId, { 1.15797f, 0.47283f, 0.0f }, (15.0f), EAgeType::g3));
	plants.push_back(Plant(halocnemumId, { 1.32927f, 3.0903f, 0.0f }, (2.0f), EAgeType::j));
	plants.push_back(Plant(halocnemumId, { 1.37949f, 3.23448f, 0.0f }, (2.0f), EAgeType::j));
	plants.push_back(Plant(halocnemumId, { 1.43334f, 3.4041f, 0.0f }, (3.0f), EAgeType::im));
	plants.push_back(Plant(halocnemumId, { 1.47093f, 3.08031f, 0.0f }, (2.0f), EAgeType::j));
	plants.push_back(Plant(halocnemumId, { 1.63161f, 2.96799f, 0.0f }, (13.0f), EAgeType::g2));
	plants.push_back(Plant(halocnemumId, { 1.70658f, 3.09102f, 0.0f }, (4.0f), EAgeType::v));
	plants.push_back(Plant(halocnemumId, { 2.01927f, 0.69852f, 0.0f }, (15.0f), EAgeType::g3));
	plants.push_back(Plant(halocnemumId, { 2.19657f, 3.59253f, 0.0f }, (12.0f), EAgeType::g1));
	plants.push_back(Plant(halocnemumId, { 2.58516f, 2.65056f, 0.0f }, (12.0f), EAgeType::g2));
	plants.push_back(Plant(halocnemumId, { 2.97834f, 0.49773f, 0.0f }, (11.0f), EAgeType::ss));
	plants.push_back(Plant(halocnemumId, { 3.47964f, 0.69081f, 0.0f }, (12.0f), EAgeType::g3));
	plants.push_back(Plant(halocnemumId, { 3.49155f, 2.62122f, 0.0f }, (10.0f), EAgeType::s));
	plants.push_back(Plant(halocnemumId, { 3.97167f, 3.67674f, 0.0f }, (12.0f), EAgeType::g3));
	plants.push_back(Plant(halocnemumId, { 4.36764f, 0.33888f, 0.0f }, (15.0f), EAgeType::ss));

	for (std::vector<Plant>::iterator i = plants.begin(); i != plants.end(); ++i)
	{
		//std::cout << "Index = " << i - plants.begin() << std::endl;
		//i->Print();
	}

	//CalcAvgRadius(plants, averageRadiuses);

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

				//renderer.Draw(va, ib, shader, drawType);
			}

			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
				glm::mat4 mvp = proj * view * model;
				shader->Bind();
				shader->SetUniformMat4f("u_MVP", mvp);
				shader->SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
				EDrawTypes drawType = EDrawTypes::EDT_TRIANGLES;

				//renderer.Draw(va, ib, shader, drawType);
			}

			Rectangle boundingRectangle = Rectangle(Point2f(0.0f, 0.0f), Point2f(0.0f, 250.0f), Point2f(250.0f, 250.0f), Point2f(250.0f, 0.0f));
			std::shared_ptr<Grid> grid = std::make_shared<Grid>(boundingRectangle, 5, 5);
			grid->Draw(proj, view, gridTranslation, shader, renderer);

			if (r > 1.0f) {
				increment = -0.05f;
			}
			else if (r < 0.0f) {
				increment = 0.05f;
			}

			r += increment;

			float actualEcoPct = 0.498f;
			{
				//ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, 960.0f);
				//ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, 960.0f);
				//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				
				ImGui::Text("1) Halocnemum strobilaceum (Pall.) M.Bieb.");
				ImGui::Text("2) Suaeda maritima (L.) Dumort.");
				ImGui::Text("3) Eremopyrum orientale (L.) Jaub. & Spach");
				ImGui::Separator();
				ImGui::Text("Implemented niche = %.1f", actualEcoPct);

				ImGui::InputFloat("Scale", &mapScale);

				ImGui::Checkbox("Produce Children", &bProduceChildren);

				if (ImGui::Button("Start"))
				{
					bStartBtnClicked = true;
					initialTime = glfwGetTime();
				}
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			if (bStartBtnClicked)
			{
				if (!bDataWasScaled)
				{
					bDataWasScaled = true;
					scaleToReal(plants, mapScale);
				}

				passedTime = glfwGetTime() - initialTime;
				std::cout << passedTime << std::endl;

				passedYears = passedTime / 3.0f;

				std::vector<int> plantsToDeleteIdx;
				std::vector<Plant> plantsToAdd;

				for (std::vector<Plant>::iterator it = plants.begin(); it != plants.end(); ++it)
				{
					if ((it->GetAgeType() == EAgeType::g1 || it->GetAgeType() == EAgeType::g2 || it->GetAgeType() == EAgeType::g3) && !it->GetProducedChild() && bProduceChildren)
					{
						auto generateRandOffset = []()
						{
							return (-8.0f + rand() % 10) / 16.0f;
						};

						float positionDiffX = generateRandOffset();
						float positionDiffY = generateRandOffset();

						float childX = it->GetCenter()[0] + positionDiffX;
						float childY = it->GetCenter()[1] + positionDiffY;
						float childZ = it->GetCenter()[2];

						float childRadius = averageRadiuses->at(static_cast<int>(EAgeType::se) - 1);
						Plant childPlant(halocnemumId, { childX, childY, childZ }, childRadius, EAgeType::se);
						plantsToAdd.push_back(childPlant);
						it->SetProducedChild(true);
					}

					int index = static_cast<int>(it - plants.begin());

					std::vector<AgeTypeDataEntry> plantAgeData = appManager.GetAgeTypeData(halocnemumId);

					int minAgeTypeDuration = plantAgeData.at(static_cast<int>(it->GetAgeType()) - 1).minAge;
					int maxAgeTypeDuration = plantAgeData.at(static_cast<int>(it->GetAgeType()) - 1).maxAge;
					
					int resultingAgeDuration = minAgeTypeDuration + (maxAgeTypeDuration - minAgeTypeDuration) * actualEcoPct;

					float ageIncrease = passedYears - it->GetAccumulatedAge();

					if (ageIncrease > resultingAgeDuration)
					{
						std::cout << "Switching ages!" << std::endl;

						it->SetProducedChild(false);

						EAgeType prevAgeType = it->GetAgeType();
						EAgeType newAgeType = static_cast<EAgeType>(static_cast<int>(prevAgeType) + 1);
						
						if (newAgeType != EAgeType::MAX)
						{
							it->SetAgeType(newAgeType);
							it->SetAccumulatedAge(it->GetAccumulatedAge() + ageIncrease);
							int ageTypesNum = static_cast<int>(EAgeType::MAX) - 1;
							float curtainGrowthCoeff = 1.0f / (ageTypesNum);

							std::vector<float> curtainGrowthCoeffs(ageTypesNum, curtainGrowthCoeff);

							curtainGrowthCoeffs[static_cast<int>(EAgeType::se) - 1] *= 5;
							curtainGrowthCoeffs[static_cast<int>(EAgeType::p) - 1] *= 5;
							curtainGrowthCoeffs[static_cast<int>(EAgeType::j) - 1] *= 32;
							curtainGrowthCoeffs[static_cast<int>(EAgeType::im) - 1] *= 5;
							curtainGrowthCoeffs[static_cast<int>(EAgeType::v) - 1] *= 5;
							curtainGrowthCoeffs[static_cast<int>(EAgeType::g1) - 1] *= 5;
							curtainGrowthCoeffs[static_cast<int>(EAgeType::g2) - 1] *= 5;
							curtainGrowthCoeffs[static_cast<int>(EAgeType::g3) - 1] *= 5;
							curtainGrowthCoeffs[static_cast<int>(EAgeType::ss) - 1] *= 2;
							curtainGrowthCoeffs[static_cast<int>(EAgeType::s) - 1] *= 2;

							float currentDiameter = it->GetDiameter();

							bool bPlantsRepulsion = false;

							auto calcDistanceBetweenPlants = [](const Plant& plant1, const Plant& plant2)
							{
								const std::vector<float> plant1Center = plant1.GetCenter();
								const std::vector<float> plant2Center = plant2.GetCenter();

								float result =  sqrt
									(
										pow((plant2Center[0] - plant1Center[0]), 2) +
										pow((plant2Center[1] - plant1Center[1]), 2)
									)
									- 0.5f * 0.5f * plant2.GetDiameter()
									- 0.5f * 0.5f * plant1.GetDiameter();

								return result;
							};

							float curtainGrowthIncrease;
							const float plantsRepulseDistance = 0.2f;
							float maxAvailableCurtainIncrease = 0.0f;

							for (int i = 0; i < plants.size(); ++i)
							{

								if (i != index && calcDistanceBetweenPlants(*it, plants[i]) < plantsRepulseDistance)
								{
									bPlantsRepulsion = true;
									break;
								}
							}

							if (bPlantsRepulsion)
							{
								curtainGrowthIncrease = 0.0f;
							}
							else
							{
								curtainGrowthIncrease = currentDiameter * curtainGrowthCoeffs[static_cast<int>(prevAgeType) - 1];
							}

							it->SetDiameter(currentDiameter + curtainGrowthIncrease);
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

				for (int i = 0; i < plantsToAdd.size(); ++i)
				{
					plants.emplace_back(plantsToAdd[i]);
				}

				for (std::vector<Plant>::iterator i = plants.begin(); i != plants.end(); ++i)
				{
					Rectangle gridSquare = grid->GetBoundingRectangle();

					float gridXLeft = gridSquare.GetBottomLeftPoint()._x;
					float gridXRight = gridSquare.GetBottomRightPoint()._x;

					float gridYBottom = gridSquare.GetBottomLeftPoint()._y;
					float gridYTop = gridSquare.GetTopLeftPoint()._y;

					float windowX = gridXLeft + i->GetCenter()[0] * gridSquare.GetWidht() / 5;
					float windowY = gridYBottom + i->GetCenter()[1] * gridSquare.GetHeight() / 5;
					float windowZ = i->GetCenter()[2];
					float windowRadius = i->GetDiameter() * 0.5f * gridSquare.GetWidht() / 5;

					Circle circle(windowRadius, Point2f(windowX, windowY), 10);
					circle.Draw(proj, view, circleTranslation, shader, renderer);
				}
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