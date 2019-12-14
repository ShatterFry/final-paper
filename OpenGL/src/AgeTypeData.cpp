#include "AgeTypeData.h"

AgeTypeData::AgeTypeData()
{
	int ageTypesNum = static_cast<int>(EAgeType::MAX);

	mData.reserve(ageTypesNum);

	for (int i = 0; i < ageTypesNum; ++i)
	{
		AgeTypeDataEntry entry;
		entry.mType = static_cast<EAgeType>(i);
		entry.mMinAge = 0;
		entry.mMaxAge = 0;
		mData.emplace_back(entry);
	}
}
