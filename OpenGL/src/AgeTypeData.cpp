#include "AgeTypeData.h"

AgeTypeData::AgeTypeData()
{
	int ageTypesNum = static_cast<int>(EAgeType::MAX) - 1;
	mData.reserve(ageTypesNum);

	for (int i = 0; i < ageTypesNum; ++i)
	{
		AgeTypeDataEntry entry;
		entry.mType = static_cast<EAgeType>(i + 1);
		entry.mMinAge = 0;
		entry.mMaxAge = 0;
		mData.emplace_back(entry);
	}
}
