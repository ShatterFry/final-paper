#pragma once

#include <memory>
#include <vector>
#include <AgeTypeDataEntry.h>

class AppManager
{
public:
	AppManager();
	std::shared_ptr<AgeTypeData>& GetAgeTypeData(int plantId) { return PlantsAgeTypeData[plantId]; }

private:
	std::vector<std::shared_ptr<AgeTypeData>> PlantsAgeTypeData;
};