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

#include <VertexBuffer.h>
#include <VertexBufferLayout.h>
#include <IndexBuffer.h>
#include <VertexArray.h>
#include <Shader.h>
#include <Texture.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <AgeType.h>
#include <Plant.h>

void DrawCircle(float x, float y, float z, float radius, int numberOfSides)
{
	const int numberOfVertices = numberOfSides + 2;

	const float twicePi = 2.0f * M_PI;

	float circleVerticesX[12];
	float circleVerticesY[12];
	float circleVerticesZ[12];

	circleVerticesX[0] = x;
	circleVerticesY[0] = y;
	circleVerticesZ[0] = z;

	for (int i = 1; i < numberOfVertices; i++)
	{
		circleVerticesX[i] = x + (radius * cos(i *  twicePi / numberOfSides));
		circleVerticesY[i] = y + (radius * sin(i * twicePi / numberOfSides));
		circleVerticesZ[i] = z;
	}

	float allCircleVertices[(12) * 3];

	for (int i = 0; i < numberOfVertices; i++)
	{
		allCircleVertices[i * 3] = circleVerticesX[i];
		allCircleVertices[(i * 3) + 1] = circleVerticesY[i];
		allCircleVertices[(i * 3) + 2] = circleVerticesZ[i];
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, allCircleVertices);
	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void CalcAvgRadius(
	std::vector<Plant> *plants,
	std::map<AgeTypeId, double> *averageRadiuses,
	AgeTypeId ageTypeId
)
{
	int ageCount = 0;

	std::map<AgeTypeId, double>::iterator it = averageRadiuses->find(ageTypeId);
	if (it != averageRadiuses->end()) {
		it->second = 0;
	}
	else {
		averageRadiuses->insert(std::pair<AgeTypeId, double>(ageTypeId, 0));
	}

	for (std::vector<Plant>::iterator i = plants->begin(); i != plants->end(); ++i) {
		if (i->GetAgeType().GetId() == ageTypeId) {
			averageRadiuses->find(ageTypeId)->second += i->GetRadius();
			++ageCount;
		}
	}

	if (ageCount > 0)
		averageRadiuses->at(ageTypeId) /= ageCount;
}

int main(void)
{
	AgeType se, p, j, im, v, g1, g2, g3, ss, s;
	double seconds = 0.0;
	double passedYears = 0.0;
	std::vector<Plant> plants;
	std::map<AgeTypeId, double> averageRadiuses;

	se.SetId(AgeTypeId::seId);
	se.SetName("se");
	se.SetMinAge(1);
	se.SetMaxAge(1);
	p.SetId(AgeTypeId::pId);
	p.SetName("p");
	p.SetMinAge(1);
	p.SetMaxAge(1);
	j.SetId(AgeTypeId::jId);
	j.SetName("j");
	j.SetMinAge(1);
	j.SetMaxAge(1);
	im.SetId(AgeTypeId::imId);
	im.SetName("im");
	im.SetMinAge(1);
	im.SetMaxAge(3);
	v.SetId(AgeTypeId::vId);
	v.SetName("v");
	v.SetMinAge(1);
	v.SetMaxAge(5);
	g1.SetId(AgeTypeId::g1Id);
	g1.SetName("g1");
	g1.SetMinAge(1);
	g1.SetMaxAge(4);
	g2.SetId(AgeTypeId::g2Id);
	g2.SetName("g2");
	g2.SetMinAge(5);
	g2.SetMaxAge(5);
	g3.SetId(AgeTypeId::g3Id);
	g3.SetName("g3");
	g3.SetMinAge(1);
	g3.SetMaxAge(2);
	ss.SetId(AgeTypeId::ssId);
	ss.SetName("ss");
	ss.SetMinAge(1);
	ss.SetMaxAge(2);
	s.SetId(AgeTypeId::sId);
	s.SetName("s");
	s.SetMinAge(1);
	s.SetMaxAge(2);

	plants.push_back(Plant(0.24243, 4.50999, 0, static_cast<GLfloat>(12./30/2), g1, 10.0));
	plants.push_back(Plant(0.27837, 0.41184, 0, static_cast<GLfloat>(13./30/2), g3, 19.0));
	plants.push_back(Plant(0.36258, 3.3966, 0, static_cast<GLfloat>(12./30/2), g1, 10.0));
	plants.push_back(Plant(0.62103, 2.6586, 0, static_cast<GLfloat>(12./30/2), g1, 10.0));
	plants.push_back(Plant(0.96021, 3.41421, 0, static_cast<GLfloat>(13./30/2), g2, 14.0));
	plants.push_back(Plant(1.15797, 0.47283, 0, static_cast<GLfloat>(15./30/2), g3, 19.0));
	plants.push_back(Plant(1.32927, 3.0903, 0, static_cast<GLfloat>(2. / 30/2), j, 1.0));
	plants.push_back(Plant(1.37949, 3.23448, 0, static_cast<GLfloat>(2./30/2), j, 1.0));
	plants.push_back(Plant(1.43334, 3.4041, 0, static_cast<GLfloat>(3./30/2), im, 2.0));
	plants.push_back(Plant(1.47093, 3.08031, 0, static_cast<GLfloat>(2./30/2), j, 1.0));
	plants.push_back(Plant(1.63161, 2.96799, 0, static_cast<GLfloat>(13./30/2), g2, 14.0));
	plants.push_back(Plant(1.70658, 3.09102, 0, static_cast<GLfloat>(4./30/2), v, 5.0));
	plants.push_back(Plant(2.01927, 0.69852, 0, static_cast<GLfloat>(15./30/2), g3, 19.0));
	plants.push_back(Plant(2.19657, 3.59253, 0, static_cast<GLfloat>(12./30/2), g1, 10.0));
	plants.push_back(Plant(2.58516, 2.65056, 0, static_cast<GLfloat>(12./30/2), g2, 14.0));
	plants.push_back(Plant(2.97834, 0.49773, 0, static_cast<GLfloat>(11./30/2), g3, 19.0));
	plants.push_back(Plant(3.36951, 3.57684, 0, static_cast<GLfloat>(15./30/2), ss, 21.0));
	plants.push_back(Plant(3.47964, 0.69081, 0, static_cast<GLfloat>(12./30/2), g3, 19.0));
	plants.push_back(Plant(3.49155, 2.62122, 0, static_cast<GLfloat>(10./30/2), s, 23.0));
	plants.push_back(Plant(3.97167, 3.67674, 0, static_cast<GLfloat>(12./30/2), g3, 19.0));
	plants.push_back(Plant(4.36764, 0.33888, 0, static_cast<GLfloat>(15./30/2), ss, 21.0));

	for (std::vector<Plant>::iterator i = plants.begin(); i != plants.end(); ++i) {
		std::cout << "Index = " << i - plants.begin() << std::endl;
		i->Print();
	}

	CalcAvgRadius(&plants, &averageRadiuses, AgeTypeId::pId);
	CalcAvgRadius(&plants, &averageRadiuses, AgeTypeId::jId);
	CalcAvgRadius(&plants, &averageRadiuses, AgeTypeId::imId);
	CalcAvgRadius(&plants, &averageRadiuses, AgeTypeId::vId);
	CalcAvgRadius(&plants, &averageRadiuses, AgeTypeId::g1Id);
	CalcAvgRadius(&plants, &averageRadiuses, AgeTypeId::g2Id);
	CalcAvgRadius(&plants, &averageRadiuses, AgeTypeId::g3Id);
	CalcAvgRadius(&plants, &averageRadiuses, AgeTypeId::ssId);
	CalcAvgRadius(&plants, &averageRadiuses, AgeTypeId::sId);

	for (
		std::map<AgeTypeId, double>::iterator it = averageRadiuses.begin();
		it != averageRadiuses.end();
		++it
		)
	{
		std::cout << it->first << " " << it->second << std::endl;
	}

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
		float positions[] = {
			-50.0f, -50.0f, 0.0f, 0.0f, //0
			50.0f, -50.0f, 1.0f, 0.0f, // 1
			50.0f, 50.0f, 1.0f, 1.0f, // 2
			-50.0f, 50.0f, 0.0f, 1.0f // 3
		};

		float linePositions[] = {
			0.0f, 0.0f,
			0.0f, 250.0f,
			250.0f, 250.0f,
			250.0f, 0.0f
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		unsigned int lineIndices[] = {
			0, 1,
			1, 2,
			2, 3,
			3, 0
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		VertexArray va;
		VertexBuffer vb(positions, 4 * 4 * sizeof(float));

		VertexArray vaLines;
		VertexBuffer vbLines(linePositions, 2 * 4 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);

		VertexBufferLayout layoutLines;
		layoutLines.Push<float>(2);
		vaLines.AddBuffer(vbLines, layoutLines);

		IndexBuffer ib(indices, 6);
		IndexBuffer ibLines(lineIndices, 2 * 4);

		glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
		shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

		Texture texture("res/textures/ChernoLogo.png");
		texture.Bind();
		shader.SetUniform1i("u_Texture", 0);

		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		shader.Unbind();

		vaLines.Unbind();
		vbLines.Unbind();
		ibLines.Unbind();

		Renderer renderer;

		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init();
		ImGui::StyleColorsDark();

		glm::vec3 translationA(200, 200, 0);
		glm::vec3 translationB(400, 200, 0);

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
				shader.Bind();
				shader.SetUniformMat4f("u_MVP", mvp);
				shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

				renderer.Draw(va, ib, shader);
			}

			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
				glm::mat4 mvp = proj * view * model;
				shader.Bind();
				shader.SetUniformMat4f("u_MVP", mvp);
				shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

				renderer.Draw(va, ib, shader);
			}

			glm::vec3 translationLines(0, 0, 0);
			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), translationLines);
				glm::mat4 mvp = proj * view * model;
				shader.Bind();
				shader.SetUniformMat4f("u_MVP", mvp);
				shader.SetUniform4f("u_Color", 1.0f, 0.3f, 0.3f, 1.0f);

				renderer.DrawGrid(vaLines, ibLines, shader);
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

			/*seconds = glfwGetTime();
			std::cout << seconds << std::endl;

			passedYears = seconds / 3;

			std::vector<int> plantsToDeleteIdx;

			for (
				std::vector<Plant>::iterator i = plants.begin();
				i != plants.end();
				++i
			)
			{

				int index = static_cast<int>(i - plants.begin());
			
				if (
					(passedYears - i->GetAgeAccumulated()) >
					i->GetAgeType().GetMaxAge()
				)
				{
					std::cout << "Switching ages!" << std::endl;

					switch (i->GetAgeType().GetId())
					{
					case AgeTypeId::seId:
						i->SetAgeType(p);
						i->SetAgeAccumulated(i->GetAgeAccumulated() + (passedYears - i->GetAgeAccumulated()));
						i->SetRadius(averageRadiuses[i->GetAgeType().GetId()]);
						break;
					case AgeTypeId::pId:
							i->SetAgeType(j);
							i->SetAgeAccumulated(i->GetAgeAccumulated() + (passedYears - i->GetAgeAccumulated()));
							i->SetRadius(averageRadiuses[i->GetAgeType().GetId()]);
							break;
					case AgeTypeId::jId:
							i->SetAgeType(im);
							i->SetAgeAccumulated(i->GetAgeAccumulated() + (passedYears - i->GetAgeAccumulated()));
							i->SetRadius(averageRadiuses[i->GetAgeType().GetId()]);
							break;
					case AgeTypeId::imId:
							i->SetAgeType(v);
							i->SetAgeAccumulated(i->GetAgeAccumulated() + (passedYears - i->GetAgeAccumulated()));
							i->SetRadius(averageRadiuses[i->GetAgeType().GetId()]);
							break;
					case AgeTypeId::vId:
							i->SetAgeType(g1);
							i->SetAgeAccumulated(i->GetAgeAccumulated() + (passedYears - i->GetAgeAccumulated()));
							i->SetRadius(averageRadiuses[i->GetAgeType().GetId()]);
							break;
					case AgeTypeId::g1Id:
							i->SetAgeType(g2);
							i->SetAgeAccumulated(i->GetAgeAccumulated() + (passedYears - i->GetAgeAccumulated()));
							i->SetRadius(averageRadiuses[i->GetAgeType().GetId()]);
							break;
					case AgeTypeId::g2Id:
							i->SetAgeType(g3);
							i->SetAgeAccumulated(i->GetAgeAccumulated() + (passedYears - i->GetAgeAccumulated()));
							i->SetRadius(averageRadiuses[i->GetAgeType().GetId()]);
							break;
					case AgeTypeId::g3Id:
							i->SetAgeType(ss);
							i->SetAgeAccumulated(i->GetAgeAccumulated() + (passedYears - i->GetAgeAccumulated()));
							i->SetRadius(averageRadiuses[i->GetAgeType().GetId()]);
							break;
					case AgeTypeId::ssId:
							i->SetAgeType(s);
							i->SetAgeAccumulated(i->GetAgeAccumulated() + (passedYears - i->GetAgeAccumulated()));
							i->SetRadius(averageRadiuses[i->GetAgeType().GetId()]);
							break;
					case AgeTypeId::sId:
							plantsToDeleteIdx.push_back(index);
							i->SetRadius(averageRadiuses[i->GetAgeType().GetId()]);
							break;
					}
				}
			}

			std::sort(plantsToDeleteIdx.begin(), plantsToDeleteIdx.end(), [](int a, int b) {return a > b; });

			for (std::vector<int>::iterator i = plantsToDeleteIdx.begin(); i != plantsToDeleteIdx.end(); ++i) {
				std::cout << "Vector size = " << plants.size() << std::endl;
				std::cout << "Will delete element with index " << *i << std::endl;
				plants.erase(plants.begin() + *i);
			}

			for (std::vector<Plant>::iterator i = plants.begin(); i != plants.end(); ++i) {
				double windowX = (i->GetX() * SCREEN_WIDTH) / 5;
				double windowY = (i->GetY() * SCREEN_HEIGHT) / 5;
				double windowZ = i->GetZ();
				double windowRadius = (i->GetRadius() * SCREEN_HEIGHT) / 5;
				DrawCircle(windowX, windowY, windowZ, windowRadius, 10);
			}*/

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