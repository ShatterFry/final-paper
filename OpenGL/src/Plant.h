#pragma once

#include <AgeType.h>
#include <AgeTypeDataEntry.h>

class Plant {

private:
	int id;
	double X, Y, Z, Radius;

	EAgeType ageType;

	float AgeInit;
	float AgeAccumulated;
	float AgeResulting;

	std::vector<AgeTypeDataEntry> ageTypesData;

public:

	Plant(const int id, const GLfloat x, const GLfloat y, const GLfloat z, const GLfloat radius, const EAgeType ageType, const float initialAge)
	{
		this->id = id;
		this->X = x;
		this->Y = y;
		this->Z = z;
		this->Radius = radius;
		this->ageType = ageType;
		this->AgeInit = initialAge;
	}

	double GetX() const { return X; }
	double GetY() const { return Y; }
	double GetZ() const { return Z; }
	double GetRadius() const { return Radius; }
	EAgeType GetAgeType() const { return this->ageType; }
	float GetAgeInit() const { return AgeInit; };
	float GetAgeAccumulated() const { return AgeAccumulated; };
	float GetAgeResulting() const { return AgeResulting; };

	void SetAgeType(EAgeType ageType) { this->ageType = ageType; }
	void SetAgeInit(float initialAge) { this->AgeInit = initialAge; }
	void SetAgeAccumulated(float accumulatedAge) { this->AgeAccumulated = accumulatedAge; }
	void SetAgeResulting(float resultingAge) { this->AgeResulting = resultingAge; }
	void SetRadius(double radius) { this->Radius = radius; }

	void Print() const
	{
		std::cout << "Age Init = " << this->AgeInit << std::endl;
		std::cout << "Age Accumulated = " << this->AgeAccumulated << std::endl;
		//std::cout << "Age Type = " << this->ageType << std::endl;
		std::cout << "X = " << this->X << std::endl;
		std::cout << "Y = " << this->Y << std::endl;
		std::cout << "Z = " << this->Z << std::endl;
		std::cout << "Radius = " << this->Radius << std::endl;
	}
};