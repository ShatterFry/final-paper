#include "Plant.h"

Plant::Plant() : mId(0), mCenter({}), mDiameter(0.0f), mAgeType(EAgeType::MAX)
{

}

Plant::Plant(const int id, const std::vector<float>& center, const float radius, const EAgeType ageType)
{
	mData.PlantId = id;
	mData.Center = std::move(center);
	mData.Radius = radius;
	mData.AgeType = ageType;
}

/*Plant::Plant(Plant&& other)
	: mId(-1),
	mCenter({}),
	mDiameter(0.0f),
	mAgeType(EAgeType::NONE),
	mAgeTypesData({})
{
	 mId = other.mId;
	 mCenter = other.mCenter;
	 mDiameter = other.mDiameter;
	 mAgeType = other.mAgeType;
	 mAgeTypesData = other.mAgeTypesData;

	 other.mId = -1;
	 other.mCenter = {};
	 other.mDiameter = 0.0f;
	 other.mAgeType = EAgeType::NONE;
	 other.mAgeTypesData = {};
}*/

std::vector<float> Plant::GetCenter() const
{
	return mData.Center;
}

void Plant::Print() const
{
	std::cout << "Age Type = " << static_cast<int>(mData.AgeType) << std::endl;
	std::cout << "X = " << mData.Center[0] << std::endl;
	std::cout << "Y = " << mData.Center[1] << std::endl;
	std::cout << "Z = " << mData.Center[2] << std::endl;
	std::cout << "Radius = " << mData.Radius << std::endl;
	//std::cout << "Diameter = " << mData.Radius * 2.0f << std::endl;
}

void Plant::SetData(const FPlantData& data)
{
	mData = data;
}
