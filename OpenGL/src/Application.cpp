#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <vector>
#include <map>
#include <cfenv>
#include <algorithm>

#include <AgeType.h>
#include <Plant.h>

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600

void DrawCircle(double x, double y, double z, double radius, int numberOfSides);
void CalcAvgRadius(
	std::vector<Plant> *plants,
	std::map<AgeTypeId, double> *averageRadiuses, AgeTypeId ageTypeId
);

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

	// Create a windowed mode window and its OpenGL context
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello World", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	// Make the window's context current
	glfwMakeContextCurrent(window);

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT); // specifies the part of the window to which OpenGL will draw (in pixels), convert from normalised to pixels
	glMatrixMode(GL_PROJECTION); // projection matrix defines the properties of the camera that views the objects in the world coordinate frame. Here you typically set the zoom factor, aspect ratio and the near and far clipping planes
	glLoadIdentity(); // replace the current matrix with the identity matrix and starts us a fresh because matrix transforms such as glOrpho and glRotate cumulate, basically puts us at (0, 0, 0)
	glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 1); // essentially set coordinate system
	glMatrixMode(GL_MODELVIEW); // (default matrix mode) modelview matrix defines how your objects are transformed (meaning translation, rotation and scaling) in your world
	glLoadIdentity(); // same as above comment
					  
	// Loop until the user closes the window
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		// render OpenGL here
		seconds = glfwGetTime();
		std::cout << seconds << std::endl;

		passedYears = seconds / 3;

		std::vector<int> plantsToDeleteIdx;

		/*for (
			std::vector<Plant>::iterator i = plants.begin();
			i != plants.end();
			++i
		)
		{

			int index = static_cast<int>(i - plants.begin());
			
			if ( i->GetAge() + passedYears > i->GetAgeType().GetMaxAge() )
			{
				std::cout << "Switching ages!" << std::endl;

				switch (i->GetAgeType().GetId())
				{
				case AgeTypeId::seId:
					i->SetAgeType(p);
					i->SetRadius(averageRadiuses[i->GetAgeType().GetId()]);
					break;
				case AgeTypeId::pId:
						i->SetAgeType(j);
						i->SetRadius(averageRadiuses[i->GetAgeType().GetId()]);
						break;
				case AgeTypeId::jId:
						i->SetAgeType(im);
						i->SetRadius(averageRadiuses[i->GetAgeType().GetId()]);
						break;
				case AgeTypeId::imId:
						i->SetAgeType(v);
						i->SetRadius(averageRadiuses[i->GetAgeType().GetId()]);
						break;
				case AgeTypeId::vId:
						i->SetAgeType(g1);
						i->SetRadius(averageRadiuses[i->GetAgeType().GetId()]);
						break;
				case AgeTypeId::g1Id:
						i->SetAgeType(g2);
						i->SetRadius(averageRadiuses[i->GetAgeType().GetId()]);
						break;
				case AgeTypeId::g2Id:
						i->SetAgeType(g3);
						i->SetRadius(averageRadiuses[i->GetAgeType().GetId()]);
						break;
				case AgeTypeId::g3Id:
						i->SetAgeType(ss);
						i->SetRadius(averageRadiuses[i->GetAgeType().GetId()]);
						break;
				case AgeTypeId::ssId:
						i->SetAgeType(s);
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
		}*/

		for (std::vector<Plant>::iterator i = plants.begin(); i != plants.end(); ++i) {
			double windowX = (i->GetX() * SCREEN_WIDTH) / 5;
			double windowY = (i->GetY() * SCREEN_HEIGHT) / 5;
			double windowZ = i->GetZ();
			double windowRadius = (i->GetRadius() * SCREEN_HEIGHT) / 5;
			DrawCircle(windowX, windowY, windowZ, windowRadius, 10);
		}

		// Swap front and back buffers
		glfwSwapBuffers(window);

		// Poll for and process events
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

void DrawCircle(double x, double y, double z, double radius, int numberOfSides)
{
	const int numberOfVertices = numberOfSides + 2;

	const double twicePi = 2.0f * M_PI;

	double circleVerticesX[12];
	double circleVerticesY[12];
	double circleVerticesZ[12];

	circleVerticesX[0] = x;
	circleVerticesY[0] = y;
	circleVerticesZ[0] = z;

	for (int i = 1; i < numberOfVertices; i++)
	{
		circleVerticesX[i] = x + (radius * cos(i *  twicePi / numberOfSides));
		circleVerticesY[i] = y + (radius * sin(i * twicePi / numberOfSides));
		circleVerticesZ[i] = z;
	}

	double allCircleVertices[(12) * 3];

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

	if(ageCount > 0)
		averageRadiuses->at(ageTypeId) /= ageCount;
}