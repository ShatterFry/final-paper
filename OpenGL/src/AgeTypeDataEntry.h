#pragma once

#include <string>
#include "AgeType.h"
#include <vector>

//using AgeTypeData = std::vector<struct AgeTypeDataEntry>;

struct AgeTypeDataEntry {
public:
	//std::string name;
	EAgeType mType;
	int mMinAge;
	int mMaxAge;
};
