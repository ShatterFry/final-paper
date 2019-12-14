#pragma once

#include <string>
#include <vector>

#include "AgeType.h"

struct AgeTypeDataEntry {
public:
	EAgeType mType = EAgeType::MAX;

	int mMinAge;
	int mMaxAge;
};