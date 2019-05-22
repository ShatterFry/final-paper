#pragma once

#include "AgeType.h"
#include "AgeTypeDataEntry.h"
#include <vector>
#include <iostream>
#include "AgeTypeData.h"

struct FPlantSpecieData
{
    int Id = -1;
    std::string Name;
	AgeTypeData mAgeTypesData;
};

struct FPlantData
{
	int PlantId = -1;
	std::vector<float> Center;
	float Radius = 0.0f;
	EAgeType AgeType = EAgeType::NONE;
};

class Plant
{
public:
	Plant();
	Plant(const int id, const std::vector<float>& center, const float radius, const EAgeType ageType);
	//Plant(Plant&& other);

	std::vector<float> GetCenter() const;

	float GetRadius() const { return mData.Radius; }
	float GetDiameter() const { return mData.Radius * 2.0f; }

	EAgeType GetAgeType() const { return mData.AgeType; }
	float GetAccumulatedAge() const { return mAccumulatedAge; }

	void SetAgeType(EAgeType ageType) { mData.AgeType = ageType; }
	void SetRadius(float radius) { mData.Radius = radius; }

	bool GetProducedChild() const { return bProducedChild; }
	void SetProducedChild(bool inProducedChild) { bProducedChild = inProducedChild; }

	void SetAccumulatedAge(float age) { mAccumulatedAge = age; }

	void Print() const;

	bool GetDieOnNextAge() const { return bDieOnNextAge; }
	void SetDieOnNextAge(bool inDieOnNextAge) { bDieOnNextAge = inDieOnNextAge; }

	void SetData(const FPlantData& data);

private:
	int mId;
	std::vector<float> mCenter;
	float mDiameter;
	float mAccumulatedAge = 0.0f;
	EAgeType mAgeType;

	FPlantData mData;

	std::vector<AgeTypeDataEntry> mAgeTypesData;

	bool bProducedChild = false;
	bool bDieOnNextAge = false;
};
