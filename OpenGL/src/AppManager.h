#pragma once

#include <memory>
#include <vector>

#include "AgeTypeDataEntry.h"
#include "EcoScaleDataEntry.h"
#include "AgeTypeData.h"

enum class EDataSource { YAML, SQLITE, MAX };

class AppManager
{
public:
	static AppManager* GetInstance();

	AgeTypeData& GetAgeTypeData(int plantId) { return mAgeTypeData[plantId]; }

	void SetDataSource(EDataSource inSource) { mDataSource = inSource; }

	void SyncExternalData();

private:
	std::vector<AgeTypeData> mAgeTypeData;
	std::vector<EcoScaleData> mEcoScaleData;
	std::vector<std::string> mPlantNames;

	static AppManager* mInstance;

	AppManager();
	~AppManager();

	void TrimCharacters(std::string& source, const std::string& charsToTrim);

	void SyncExternalDataFromYAML();

	EDataSource mDataSource = EDataSource::MAX;
};
