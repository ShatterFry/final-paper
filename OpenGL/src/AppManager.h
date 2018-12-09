#pragma once

#include <memory>
#include <vector>
#include <AgeTypeDataEntry.h>
#include <EcoScaleDataEntry.h>

class AppManager
{
public:
	AppManager();
	std::vector<AgeTypeDataEntry>& GetAgeTypeData(int plantId) { return PlantsAgeTypeData[plantId]; }

private:
	std::vector<std::vector<AgeTypeDataEntry>> PlantsAgeTypeData;
	std::vector<std::vector<EcoScaleDataEntry>> PlantsEcoScaleData;
	std::vector<std::string> PlantsNames;
};