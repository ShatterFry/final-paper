#pragma once
#include <AgeType.h>

class Plant {
private:
	double x, y, z, radius;
	AgeType ageType;
	float _ageInit;
	float _ageAccumulated;
	float _ageResulting;
public:
	Plant();
	Plant(
		GLfloat x,
		GLfloat y, 
		GLfloat z,
		GLfloat r,
		AgeType ageType,
		float aI
	);
	~Plant();

	double GetX();
	double GetY();
	double GetZ();
	double GetRadius();
	AgeType GetAgeType();
	float GetAgeInit();
	float GetAgeAccumulated();
	float GetAgeResulting();

	void SetAgeType(AgeType ageType);
	void SetAgeInit(float aCT);
	void SetAgeAccumulated(float aA);
	void SetAgeResulting();
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
	float aI
)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->radius = radius;
	this->ageType = ageType;
	this->_ageInit = aI;
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

float Plant::GetAgeAccumulated() {
	return this->_ageAccumulated;
}

float Plant::GetAgeInit()
{
	return this->_ageInit;
}

float Plant::GetAgeResulting()
{
	return this->_ageResulting;
}

void Plant::SetAgeType(AgeType ageType) {
	this->ageType = ageType;
}

void Plant::SetAgeAccumulated(float aA) {
	this->_ageAccumulated = aA;
}

void Plant::SetAgeInit(float aI)
{
	this->_ageInit = aI;
}

void Plant::SetAgeResulting()
{
	this->_ageResulting = this->_ageInit + this->_ageAccumulated;
}

void Plant::SetRadius(double radius) {
	this->radius = radius;
}

void Plant::Print() {
	std::cout << "Age Init = " << this->_ageInit << std::endl;
	std::cout << "Age Accumulated = " << this->_ageAccumulated << std::endl;
	std::cout << "Age Type = " << this->ageType << std::endl;
	std::cout << "X = " << this->x << std::endl;
	std::cout << "Y = " << this->y << std::endl;
	std::cout << "Z = " << this->z << std::endl;
	std::cout << "Radius = " << this->radius << std::endl;
}