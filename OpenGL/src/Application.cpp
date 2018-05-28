#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <vector>
#include <map>
#include <cfenv>
#include <algorithm>

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600

enum AgeTypes {p = 1, j = 2, im = 5, v = 10, g1 = 14, g2 = 19, g3 = 21, ss = 23, s = 25};

class Plant {
private:
	GLfloat x, y, z, radius;
	AgeTypes ageType;
	GLfloat age;
public:
	Plant();
	Plant(GLfloat x, GLfloat y, GLfloat z, GLfloat r, AgeTypes ageType, GLfloat age);
	~Plant();

	GLfloat GetX();
	GLfloat GetY();
	GLfloat GetZ();
	GLfloat GetRadius();
	AgeTypes GetAgeType();
	GLfloat GetAge();

	void SetAgeType(AgeTypes ageType);
	void SetAge(GLfloat age);
	void SetRadius(double radius);

	void Print();
};

Plant::Plant() {

}

Plant::Plant(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, AgeTypes ageType, GLfloat age) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->radius = radius;
	this->ageType = ageType;
	this->age = age;
}

Plant::~Plant() {

}

GLfloat Plant::GetX() {
	return this->x;
}

GLfloat Plant::GetY() {
	return this->y;
}

GLfloat Plant::GetZ() {
	return this->z;
}

GLfloat Plant::GetRadius() {
	return this->radius;
}

AgeTypes Plant::GetAgeType() {
	return this->ageType;
}

GLfloat Plant::GetAge() {
	return this->age;
}

void Plant::SetAgeType(AgeTypes ageType) {
	this->ageType = ageType;
}

void Plant::SetAge(GLfloat age) {
	this->age = age;
}

void Plant::SetRadius(double radius) {
	this->radius = radius;
}

void Plant::Print() {
	std::cout << "Age = " << this->age << std::endl;
	std::cout << "Age Type = " << this->ageType << std::endl;
	std::cout << "X = " << this->x << std::endl;
	std::cout << "Y = " << this->y << std::endl;
	std::cout << "Z = " << this->z << std::endl;
	std::cout << "Radius = " << this->radius << std::endl;
}

void drawCircle(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides);
void calcAvgRadius(std::vector<Plant> *plants, std::map<AgeTypes, GLfloat> *averageRadiuses, AgeTypes ageType);

int main(void)
{
	double seconds = 0.0;
	GLfloat passedYears = 0.0;
	std::vector<Plant> plants;
	std::map<AgeTypes, GLfloat> averageRadiuses;

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
	calcAvgRadius(&plants, &averageRadiuses, p);
	calcAvgRadius(&plants, &averageRadiuses, j);
	calcAvgRadius(&plants, &averageRadiuses, im);
	calcAvgRadius(&plants, &averageRadiuses, v);
	calcAvgRadius(&plants, &averageRadiuses, g1);
	calcAvgRadius(&plants, &averageRadiuses, g2);
	calcAvgRadius(&plants, &averageRadiuses, g3);
	calcAvgRadius(&plants, &averageRadiuses, ss);
	calcAvgRadius(&plants, &averageRadiuses, s);

	for (std::map<AgeTypes, GLfloat>::iterator it = averageRadiuses.begin(); it != averageRadiuses.end(); ++it) {
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

	glViewport(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT); // specifies the part of the window to which OpenGL will draw (in pixels), convert from normalised to pixels
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
		//drawCircle(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0, 12, 10);
		//drawCircle(30, 40, 0, 12, 10);
		seconds = glfwGetTime();
		std::cout << seconds << std::endl;

		//std::fesetround(FE_DOWNWARD);
		//passedYears = std::nearbyint(seconds / 10);
		passedYears = seconds / 3;
		//std::cout << passedYears << std::endl;

		std::vector<int> plantsToDeleteIdx;

		for (std::vector<Plant>::iterator i = plants.begin(); i != plants.end(); ++i) {
			//i->SetAge(i->GetAge() + passedYears);

			int index = i - plants.begin();

			if ( i->GetAge() + passedYears > static_cast<int>(i->GetAgeType()) ) {
				std::cout << "Switching ages!" << std::endl;
				switch (i->GetAgeType()) {
					case p:
						i->SetAgeType(j);
						i->SetRadius(averageRadiuses[i->GetAgeType()]);
						break;
					case j:
						i->SetAgeType(im);
						i->SetRadius(averageRadiuses[i->GetAgeType()]);
						break;
					case im:
						i->SetAgeType(v);
						i->SetRadius(averageRadiuses[i->GetAgeType()]);
						break;
					case v:
						i->SetAgeType(g1);
						i->SetRadius(averageRadiuses[i->GetAgeType()]);
						break;
					case g1:
						i->SetAgeType(g2);
						i->SetRadius(averageRadiuses[i->GetAgeType()]);
						break;
					case g2:
						i->SetAgeType(g3);
						i->SetRadius(averageRadiuses[i->GetAgeType()]);
						break;
					case g3:
						i->SetAgeType(ss);
						i->SetRadius(averageRadiuses[i->GetAgeType()]);
						break;
					case ss:
						i->SetAgeType(s);
						i->SetRadius(averageRadiuses[i->GetAgeType()]);
						break;
					case s:
						plantsToDeleteIdx.push_back(index);
						i->SetRadius(averageRadiuses[i->GetAgeType()]);
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
			GLfloat windowX = (i->GetX() * SCREEN_WIDTH) / 5;
			GLfloat windowY = (i->GetY() * SCREEN_HEIGHT) / 5;
			GLfloat windowRadius = (i->GetRadius() * SCREEN_HEIGHT) / 5;
			//std::cout << windowRadius << std::endl;
			drawCircle(windowX, windowY, i->GetZ(), windowRadius, 10);
		}

		// Swap front and back buffers
		glfwSwapBuffers(window);

		// Poll for and process events
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

void drawCircle(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides)
{
	const int numberOfVertices = numberOfSides + 2;

	const GLfloat twicePi = static_cast<GLfloat>(2.0f * M_PI);

	GLfloat circleVerticesX[12];
	GLfloat circleVerticesY[12];
	GLfloat circleVerticesZ[12];

	circleVerticesX[0] = x;
	circleVerticesY[0] = y;
	circleVerticesZ[0] = z;

	for (int i = 1; i < numberOfVertices; i++)
	{
		circleVerticesX[i] = x + (radius * cos(i *  twicePi / numberOfSides));
		circleVerticesY[i] = y + (radius * sin(i * twicePi / numberOfSides));
		circleVerticesZ[i] = z;
	}

	GLfloat allCircleVertices[(12) * 3];

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

void calcAvgRadius(std::vector<Plant> *plants, std::map<AgeTypes, GLfloat> *averageRadiuses, AgeTypes ageType) {
	GLint ageCount = 0;

	std::map<AgeTypes, GLfloat>::iterator it = averageRadiuses->find(ageType);
	if (it != averageRadiuses->end()) {
		it->second = 0;
	}
	else {
		averageRadiuses->insert(std::pair<AgeTypes, GLfloat>(ageType, 0));
	}

	for (std::vector<Plant>::iterator i = plants->begin(); i != plants->end(); ++i) {
		if (i->GetAgeType() == ageType) {
			averageRadiuses->find(ageType)->second += i->GetRadius();
			++ageCount;
		}
	}

	if(ageCount > 0)
		averageRadiuses->at(ageType) /= ageCount;
}