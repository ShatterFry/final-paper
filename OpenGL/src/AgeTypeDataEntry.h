#pragma once

#include <string>

using AgeTypeData = std::vector<struct AgeTypeDataEntry>;

struct AgeTypeDataEntry {

public:
	//std::string name;
	int minAge;
	int maxAge;
};