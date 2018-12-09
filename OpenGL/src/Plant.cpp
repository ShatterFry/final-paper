#include <Plant.h>

Plant::Plant(const int id, const std::vector<float>& center, const float diameter, const EAgeType ageType)
{
	mId = id;
	mCenter = std::move(center);
	mDiameter = diameter;
	mAgeType = ageType;
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
	return mCenter;
}

void Plant::Print() const
{
	std::cout << "Age Type = " << static_cast<int>(mAgeType) << std::endl;
	std::cout << "X = " << mCenter[0] << std::endl;
	std::cout << "Y = " << mCenter[1] << std::endl;
	std::cout << "Z = " << mCenter[2] << std::endl;
	std::cout << "Diameter = " << GetDiameter() << std::endl;
}