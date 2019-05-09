#pragma once
#include "AgeTypeDataEntry.h"

class AgeTypeData
{
public:
	AgeTypeData();
	std::vector<AgeTypeDataEntry>& GetData() { return mData; }
private:
	std::vector<AgeTypeDataEntry> mData;
};
