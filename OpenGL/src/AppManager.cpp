#include "AppManager.h"
#include "AgeType.h"

#include <fstream>
#include <algorithm>
#include <string>

const std::string startDocumentTag = "---";
const std::string endDocumentTag = "...";
const std::string symbolAfterTag = ":";
const std::string plantNameTag = "plant-name";
const std::string ageTypesDataTag = "age-type-data";
const std::string ageTypesDataEntryTag = "age-type-data-entry";
const std::string ageTypeIdTag = "age-type-id";
std::string minAgeTag = "min-age";
std::string maxAgeTag = "max-age";
std::string charactersToTrim = {' ', ':'};

AppManager* AppManager::mInstance;

AppManager::~AppManager()
{
	if (mInstance)
	{
		delete mInstance;
	}
}

AppManager* AppManager::GetInstance()
{
	if (!mInstance)
	{
		mInstance = new AppManager();
	}

	return mInstance;
}

void TrimCharacters(std::string& source, const std::string& charsToTrim)
{
	bool invalidCharFound = false;
	std::size_t firstValidCharPos = 0;
	std::size_t lastValidCharPos = 0;

	for (int i = 0; i < source.length(); ++i)
	{
		invalidCharFound = false;

		for (char elem : charsToTrim)
		{
			if (source[i] == elem)
			{
				invalidCharFound = true;
			}
		}

		if (!invalidCharFound)
		{
			firstValidCharPos = i;
			break;
		}
	}

	for (int j = source.length(); j >= 0; --j)
	{
		invalidCharFound = false;

		for (char elem : charsToTrim)
		{
			if (source[j] == elem)
			{
				invalidCharFound = true;
			}
		}

		if (!invalidCharFound)
		{
			lastValidCharPos = j;
			break;
		}
	}

	source = source.substr(firstValidCharPos, lastValidCharPos);
}

AppManager::AppManager()
{

}

void AppManager::SyncExternalData()
{
	switch (mDataSource)
	{
	case EDataSource::SQLITE:
		break;
	case EDataSource::YAML:
		SyncExternalDataFromYAML();
		break;
	default:
		break;
	}
}

void AppManager::SyncExternalDataFromYAML()
{
	std::ifstream inStream("data.yaml");

	auto trimCharacters = [&](std::string& source)
	{
		bool invalidCharFound = false;
		std::size_t firstValidCharPos = 0;
		std::size_t lastValidCharPos = 0;

		for (int i = 0; i < source.length(); ++i)
		{
			invalidCharFound = false;

			for (char elem : charactersToTrim)
			{
				if (source[i] == elem)
				{
					invalidCharFound = true;
				}
			}

			if (!invalidCharFound)
			{
				firstValidCharPos = i;
				break;
			}
		}

		for (int j = source.length(); j >= 0; --j)
		{
			invalidCharFound = false;

			for (char elem : charactersToTrim)
			{
				if (source[j] == elem)
				{
					invalidCharFound = true;
				}
			}

			if (!invalidCharFound)
			{
				lastValidCharPos = j;
				break;
			}
		}

		source = source.substr(firstValidCharPos, lastValidCharPos);
	};

	int ageTypeId = -1;
	int minAge = -1;
	int maxAge = -1;

	std::string lineFromFile;
	while (getline(inStream, lineFromFile))
	{
		std::size_t foundPos = lineFromFile.find(startDocumentTag);
		if (foundPos != std::string::npos)
		{
			AgeTypeData ageTypeDataFromFile;
			EcoScaleData ecoScaleDataFromFile;

			mAgeTypeData.emplace_back(ageTypeDataFromFile);
			mEcoScaleData.emplace_back(ecoScaleDataFromFile);
		}

		foundPos = lineFromFile.find(plantNameTag);
		if (foundPos != std::string::npos)
		{
			std::string plantName = lineFromFile.substr(foundPos + plantNameTag.length());
			trimCharacters(plantName);
			mPlantNames.emplace_back(plantName);
		}

		foundPos = lineFromFile.find(ageTypeIdTag + symbolAfterTag);
		if (foundPos != std::string::npos)
		{
			std::string ageType = lineFromFile.substr(foundPos + ageTypeIdTag.length() + symbolAfterTag.length());
			trimCharacters(ageType);
			ageTypeId = std::stoi(ageType);
		}

		foundPos = lineFromFile.find(minAgeTag + symbolAfterTag);
		if (foundPos != std::string::npos)
		{
			std::string minAgeStr = lineFromFile.substr(foundPos + minAgeTag.length() + symbolAfterTag.length());
			trimCharacters(minAgeStr);
			minAge = std::stoi(minAgeStr);
		}

		foundPos = lineFromFile.find(maxAgeTag + symbolAfterTag);
		if (foundPos != std::string::npos)
		{
			std::string maxAgeStr = lineFromFile.substr(foundPos + maxAgeTag.length() + symbolAfterTag.length());
			trimCharacters(maxAgeStr);
			maxAge = std::stoi(maxAgeStr);
		}

		if ((ageTypeId != -1) && (maxAge != -1))
		{
			AgeTypeDataEntry& entryToChange = mAgeTypeData[mAgeTypeData.size() - 1].GetData()[ageTypeId - 1];
			entryToChange.mMinAge = minAge;
			entryToChange.mMaxAge = maxAge;

			ageTypeId = -1;
			maxAge = -1;
		}
	}

	mPlantNames.emplace_back(std::string("Halocnemum strobilaceum (Pall.) M.Bieb."));

	AgeTypeData ageTypesDataGenerated;

	AgeTypeDataEntry& seDataEntry = ageTypesDataGenerated.GetData()[static_cast<int>(EAgeType::se) - 1];
	seDataEntry.mMinAge = 1;
	seDataEntry.mMaxAge = 1;

	AgeTypeDataEntry& pDataEntry = ageTypesDataGenerated.GetData()[static_cast<int>(EAgeType::p) - 1];
	pDataEntry.mMinAge = 1;
	pDataEntry.mMaxAge = 1;

	AgeTypeDataEntry& jDataEntry = ageTypesDataGenerated.GetData()[static_cast<int>(EAgeType::j) - 1];
	jDataEntry.mMinAge = 1;
	jDataEntry.mMaxAge = 1;

	AgeTypeDataEntry& imDataEntry = ageTypesDataGenerated.GetData()[static_cast<int>(EAgeType::im) - 1];
	imDataEntry.mMinAge = 1;
	imDataEntry.mMaxAge = 3;

	AgeTypeDataEntry& vDataEntry = ageTypesDataGenerated.GetData()[static_cast<int>(EAgeType::v) - 1];
	vDataEntry.mMinAge = 1;
	vDataEntry.mMaxAge = 5;

	AgeTypeDataEntry& g1DataEntry = ageTypesDataGenerated.GetData()[static_cast<int>(EAgeType::g1) - 1];
	g1DataEntry.mMinAge = 1;
	g1DataEntry.mMaxAge = 4;

	AgeTypeDataEntry& g2DataEntry = ageTypesDataGenerated.GetData()[static_cast<int>(EAgeType::g2) - 1];
	g2DataEntry.mMinAge = 5;
	g2DataEntry.mMaxAge = 5;

	AgeTypeDataEntry& g3DataEntry = ageTypesDataGenerated.GetData()[static_cast<int>(EAgeType::g3) - 1];
	g3DataEntry.mMinAge = 1;
	g3DataEntry.mMaxAge = 2;

	AgeTypeDataEntry& ssDataEntry = ageTypesDataGenerated.GetData()[static_cast<int>(EAgeType::ss) - 1];
	ssDataEntry.mMinAge = 1;
	ssDataEntry.mMaxAge = 2;

	AgeTypeDataEntry& sDataEntry = ageTypesDataGenerated.GetData()[static_cast<int>(EAgeType::s) - 1];
	sDataEntry.mMinAge = 1;
	sDataEntry.mMaxAge = 2;

	mAgeTypeData.emplace_back(ageTypesDataGenerated);

	std::vector<EcoScaleDataEntry> ecoScaleData;
	ecoScaleData.reserve(static_cast<int>(EEcoScaleType::MAX) - 1);

	EcoScaleDataEntry omDataEntry;
	omDataEntry.mType = EEcoScaleType::OM;
	omDataEntry.mMin = 1;
	omDataEntry.mMax = 7;
	ecoScaleData.emplace_back(omDataEntry);

	EcoScaleDataEntry hdDataEntry;
	hdDataEntry.mType = EEcoScaleType::HD;
	hdDataEntry.mMin = 1;
	hdDataEntry.mMax = 13;
	ecoScaleData.emplace_back(hdDataEntry);

	EcoScaleDataEntry trDataEntry;
	trDataEntry.mType = EEcoScaleType::TR;
	trDataEntry.mMin = 10;
	trDataEntry.mMax = 19;
	ecoScaleData.emplace_back(trDataEntry);

	EcoScaleDataEntry rcDataEntry;
	rcDataEntry.mType = EEcoScaleType::RC;
	rcDataEntry.mMin = 9;
	rcDataEntry.mMax = 13;
	ecoScaleData.emplace_back(rcDataEntry);

	EcoScaleDataEntry lcDataEntry;
	lcDataEntry.mType = EEcoScaleType::LC;
	lcDataEntry.mMin = 1;
	lcDataEntry.mMax = 3;
	ecoScaleData.emplace_back(lcDataEntry);

	mEcoScaleData.emplace_back(ecoScaleData);

	mPlantNames.emplace_back(std::string("Suaeda maritima (L.) Dumort."));

	AgeTypeData ageTypesData2;

	EcoScaleData ecoScaleData2;
	ecoScaleData2.reserve(static_cast<int>(EEcoScaleType::MAX) - 1);

	EcoScaleDataEntry omDataEntry2;
	omDataEntry2.mMin = 5;
	omDataEntry2.mMax = 11;
	ecoScaleData2.emplace_back(omDataEntry2);

	EcoScaleDataEntry hdDataEntry2;
	hdDataEntry2.mMin = 2;
	hdDataEntry2.mMax = 18;
	ecoScaleData2.emplace_back(hdDataEntry2);

	EcoScaleDataEntry trDataEntry2;
	trDataEntry2.mMin = 10;
	trDataEntry2.mMax = 18;
	ecoScaleData2.emplace_back(trDataEntry2);

	EcoScaleDataEntry rcDataEntry2;
	rcDataEntry2.mMin = 5;
	rcDataEntry2.mMax = 13;
	ecoScaleData2.emplace_back(rcDataEntry2);

	EcoScaleDataEntry lcDataEntry2;
	lcDataEntry2.mMin = 1;
	lcDataEntry2.mMax = 4;
	ecoScaleData2.emplace_back(lcDataEntry2);

	mAgeTypeData.emplace_back(ageTypesData2);
	mEcoScaleData.emplace_back(ecoScaleData2);

	mPlantNames.emplace_back(std::string("Eremopyrum orientale (L.) Jaub. & Spach"));

	AgeTypeData ageTypesData3;

	EcoScaleData ecoScaleData3;
	ecoScaleData3.reserve(static_cast<int>(EEcoScaleType::MAX) - 1);

	EcoScaleDataEntry omDataEntry3;
	omDataEntry3.mMin = 2;
	omDataEntry3.mMax = 8;

	EcoScaleDataEntry hdDataEntry3;
	hdDataEntry3.mMin = 1;
	hdDataEntry3.mMax = 8;

	EcoScaleDataEntry trDataEntry3;
	trDataEntry3.mMin = 7;
	trDataEntry3.mMax = 16;

	EcoScaleDataEntry rcDataEntry3;
	rcDataEntry3.mMin = 0;
	rcDataEntry3.mMax = 0;

	EcoScaleDataEntry lcDataEntry3;
	lcDataEntry3.mMin = 1;
	lcDataEntry3.mMax = 3;

	ecoScaleData3.emplace_back(omDataEntry3);
	ecoScaleData3.emplace_back(hdDataEntry3);
	ecoScaleData3.emplace_back(trDataEntry3);
	ecoScaleData3.emplace_back(rcDataEntry3);
	ecoScaleData3.emplace_back(lcDataEntry3);

	mAgeTypeData.emplace_back(ageTypesData3);
	mEcoScaleData.emplace_back(ecoScaleData3);

	//std::ofstream outFile("data.yaml");
	/*for (int i = 0; i < mPlantNames.size(); ++i)
	{
		outFile << "---" << std::endl;
		outFile << plantNameTag  << ": " << mPlantNames[i] << std::endl;

		outFile << ageTypesDataTag << ": " << std::endl;
		for (int j = 0; j < mAgeTypeData[i].size(); ++j)
		{
			outFile << "\tageTypesDataEntryTag" << ": " <<  << std::endl;
			outFile << "\t\tage-type-id: " << j + 1 << std::endl;
			outFile << "\t\tmin-age: " << mAgeTypeData[i][j].mMinAge << std::endl;
			outFile << "\t\tmax-age: " << mAgeTypeData[i][j].mMaxAge << std::endl;
		}

		outFile << "eco-scales-data:" << std::endl;
		for (int k = 0; k < mEcoScaleData[i].size(); ++k)
		{
			outFile << "\teco-scale-type: " << static_cast<int>(mEcoScaleData[i][k].mType) << std::endl;
			outFile << "\teco-scale-min: " << mEcoScaleData[i][k].mMin << std::endl;
			outFile << "\teco-scale-max: " << mEcoScaleData[i][k].mMax << std::endl;
		}

		outFile << "..." << std::endl;
	}*/
}
