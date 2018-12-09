#include <AppManager.h>
#include <AgeType.h>

AppManager::AppManager()
{
	PlantsNames.emplace_back(std::string("Halocnemum strobilaceum (Pall.) M.Bieb."));

	std::vector<AgeTypeDataEntry> ageTypesData;
	ageTypesData.reserve(static_cast<int>(EAgeType::MAX) - 1);

	AgeTypeDataEntry seDataEntry;
	seDataEntry.Type = EAgeType::se;
	seDataEntry.minAge = 1;
	seDataEntry.maxAge = 1;
	ageTypesData.emplace_back(seDataEntry);

	AgeTypeDataEntry pDataEntry;
	pDataEntry.Type = EAgeType::p;
	pDataEntry.minAge = 1;
	pDataEntry.maxAge = 1;
	ageTypesData.emplace_back(pDataEntry);

	AgeTypeDataEntry jDataEntry;
	jDataEntry.Type = EAgeType::j;
	jDataEntry.minAge = 1;
	jDataEntry.maxAge = 1;
	ageTypesData.emplace_back(jDataEntry);

	AgeTypeDataEntry imDataEntry;
	imDataEntry.Type = EAgeType::im;
	imDataEntry.minAge = 1;
	imDataEntry.maxAge = 3;
	ageTypesData.emplace_back(imDataEntry);

	AgeTypeDataEntry vDataEntry;
	vDataEntry.minAge = 1;
	vDataEntry.maxAge = 5;
	ageTypesData.emplace_back(vDataEntry);

	AgeTypeDataEntry g1DataEntry;
	g1DataEntry.minAge = 1;
	g1DataEntry.maxAge = 4;
	ageTypesData.emplace_back(g1DataEntry);

	AgeTypeDataEntry g2DataEntry;
	g2DataEntry.minAge = 5;
	g2DataEntry.maxAge = 5;
	ageTypesData.emplace_back(g2DataEntry);

	AgeTypeDataEntry g3DataEntry;
	g3DataEntry.minAge = 1;
	g3DataEntry.maxAge = 2;
	ageTypesData.emplace_back(g3DataEntry);

	AgeTypeDataEntry ssDataEntry;
	ssDataEntry.minAge = 1;
	ssDataEntry.maxAge = 2;
	ageTypesData.emplace_back(ssDataEntry);

	AgeTypeDataEntry sDataEntry;
	sDataEntry.minAge = 1;
	sDataEntry.maxAge = 2;
	ageTypesData.emplace_back(sDataEntry);

	PlantsAgeTypeData.emplace_back(ageTypesData);

	std::vector<EcoScaleDataEntry> ecoScaleData;
	ecoScaleData.reserve(static_cast<int>(EEcoScaleType::MAX) - 1);

	EcoScaleDataEntry omDataEntry;
	omDataEntry.Type = EEcoScaleType::OM;
	omDataEntry.Min = 1;
	omDataEntry.Max = 7;
	ecoScaleData.emplace_back(omDataEntry);

	EcoScaleDataEntry hdDataEntry;
	hdDataEntry.Type = EEcoScaleType::HD;
	hdDataEntry.Min = 1;
	hdDataEntry.Max = 13;
	ecoScaleData.emplace_back(hdDataEntry);

	EcoScaleDataEntry trDataEntry;
	trDataEntry.Type = EEcoScaleType::TR;
	trDataEntry.Min = 10;
	trDataEntry.Max = 19;
	ecoScaleData.emplace_back(trDataEntry);

	EcoScaleDataEntry rcDataEntry;
	rcDataEntry.Type = EEcoScaleType::RC;
	rcDataEntry.Min = 9;
	rcDataEntry.Max = 13;
	ecoScaleData.emplace_back(rcDataEntry);

	EcoScaleDataEntry lcDataEntry;
	lcDataEntry.Type = EEcoScaleType::LC;
	lcDataEntry.Min = 1;
	lcDataEntry.Max = 3;
	ecoScaleData.emplace_back(lcDataEntry);

	PlantsEcoScaleData.emplace_back(ecoScaleData);

	PlantsNames.emplace_back(std::string("Suaeda maritima (L.) Dumort."));

	ecoScaleData.clear();

	omDataEntry.Min = 5;
	omDataEntry.Max = 11;
	ecoScaleData.emplace_back(omDataEntry);

	hdDataEntry.Min = 2;
	hdDataEntry.Max = 18;
	ecoScaleData.emplace_back(hdDataEntry);

	trDataEntry.Min = 10;
	trDataEntry.Max = 18;
	ecoScaleData.emplace_back(trDataEntry);

	rcDataEntry.Min = 5;
	rcDataEntry.Max = 13;
	ecoScaleData.emplace_back(rcDataEntry);

	lcDataEntry.Min = 1;
	lcDataEntry.Max = 4;
	ecoScaleData.emplace_back(lcDataEntry);

	PlantsEcoScaleData.emplace_back(ecoScaleData);

	//PlantsNames.emplace_back(std::string("Stipa capillata L."));
	PlantsNames.emplace_back(std::string("Eremopyrum orientale (L.) Jaub. & Spach"));

	ecoScaleData.clear();

	omDataEntry.Min = 2;
	omDataEntry.Max = 8;

	hdDataEntry.Min = 1;
	hdDataEntry.Max = 8;

	trDataEntry.Min = 7;
	trDataEntry.Max = 16;

	rcDataEntry.Min = 0;
	rcDataEntry.Max = 0;

	lcDataEntry.Min = 1;
	lcDataEntry.Max = 3;

	ecoScaleData.emplace_back(omDataEntry);
	ecoScaleData.emplace_back(hdDataEntry);
	ecoScaleData.emplace_back(trDataEntry);
	ecoScaleData.emplace_back(rcDataEntry);
	ecoScaleData.emplace_back(lcDataEntry);
}