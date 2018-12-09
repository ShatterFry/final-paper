#pragma once

#include <AgeType.h>
#include <AgeTypeDataEntry.h>
#include <vector>
#include <iostream>

class Plant {

private:
	int mId;
	std::vector<float> mCenter;
	float mDiameter;
	float mAccumulatedAge = 0.0f;
	EAgeType mAgeType;
	std::vector<AgeTypeDataEntry> mAgeTypesData;
	bool bProducedChild = false;

public:
	Plant(const int id, const std::vector<float>& center, const float diameter, const EAgeType ageType);
	//Plant(Plant&& other);

	std::vector<float> GetCenter() const;
	float GetDiameter() const { return mDiameter; }
	EAgeType GetAgeType() const { return this->mAgeType; }
	bool GetProducedChild() { return bProducedChild; }
	float GetAccumulatedAge() { return mAccumulatedAge; }

	void SetAgeType(EAgeType ageType) { mAgeType = ageType; }
	void SetDiameter(float diameter) { mDiameter = diameter; }
	void SetProducedChild(bool inProducedChild) { bProducedChild = inProducedChild; }
	void SetAccumulatedAge(float age) { mAccumulatedAge = age; }

	void Print() const;
};