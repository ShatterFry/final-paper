#pragma once
#include <AgeType.h>

class Plant {
private:
	double x, y, z, radius;
	AgeType ageType;
	GLfloat age;
public:
	Plant();
	Plant(
		GLfloat x,
		GLfloat y, 
		GLfloat z,
		GLfloat r,
		AgeType ageType,
		GLfloat age
	);
	~Plant();

	double GetX();
	double GetY();
	double GetZ();
	double GetRadius();
	AgeType GetAgeType();
	double GetAge();

	void SetAgeType(AgeType ageType);
	void SetAge(float age);
	void SetRadius(double radius);

	void Print();
};

Plant::Plant() {

}

Plant::Plant(
	GLfloat x, 
	GLfloat y, 
	GLfloat z, 
	GLfloat radius, 
	AgeType ageType, 
	GLfloat age
)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->radius = radius;
	this->ageType = ageType;
	this->age = age;
}

Plant::~Plant() {

}

double Plant::GetX() {
	return this->x;
}

double Plant::GetY() {
	return this->y;
}

double Plant::GetZ() {
	return this->z;
}

double Plant::GetRadius() {
	return this->radius;
}

AgeType Plant::GetAgeType() {
	return this->ageType;
}

double Plant::GetAge() {
	return this->age;
}

void Plant::SetAgeType(AgeType ageType) {
	this->ageType = ageType;
}

void Plant::SetAge(float age) {
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