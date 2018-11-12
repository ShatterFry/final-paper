#include <AppManager.h>
#include <AgeType.h>

AppManager::AppManager()
{
	std::shared_ptr<AgeTypeData> ageTypesData = std::make_shared<AgeTypeData>();
	ageTypesData->reserve(static_cast<int>(EAgeType::MAX) - 1);

	//se.SetId(AgeType::seId);
	//se.SetName("se");
	AgeTypeDataEntry seDataEntry;
	seDataEntry.minAge = 1;
	seDataEntry.maxAge = 1;
	ageTypesData->emplace_back(seDataEntry);

	//p.SetId(AgeType::pId);
	//p.SetName("p");
	AgeTypeDataEntry pDataEntry;
	pDataEntry.minAge = 1;
	pDataEntry.maxAge = 1;
	ageTypesData->emplace_back(pDataEntry);

	//j.SetId(AgeType::jId);
	//j.SetName("j");
	AgeTypeDataEntry jDataEntry;
	jDataEntry.minAge = 1;
	jDataEntry.maxAge = 1;
	ageTypesData->emplace_back(jDataEntry);

	//im.SetId(AgeType::imId);
	//im.SetName("im");
	AgeTypeDataEntry imDataEntry;
	imDataEntry.minAge = 1;
	imDataEntry.maxAge = 3;
	ageTypesData->emplace_back(imDataEntry);

	//v.SetId(AgeType::vId);
	//v.SetName("v");
	AgeTypeDataEntry vDataEntry;
	vDataEntry.minAge = 1;
	vDataEntry.maxAge = 5;
	ageTypesData->emplace_back(vDataEntry);

	//g1.SetId(AgeType::g1Id);
	//g1.SetName("g1");
	AgeTypeDataEntry g1DataEntry;
	g1DataEntry.minAge = 1;
	g1DataEntry.maxAge = 4;
	ageTypesData->emplace_back(g1DataEntry);

	//g2.SetId(AgeType::g2Id);
	//g2.SetName("g2");
	AgeTypeDataEntry g2DataEntry;
	g2DataEntry.minAge = 5;
	g2DataEntry.maxAge = 5;
	ageTypesData->emplace_back(g2DataEntry);

	//g3.SetId(AgeType::g3Id);
	//g3.SetName("g3");
	AgeTypeDataEntry g3DataEntry;
	g3DataEntry.minAge = 1;
	g3DataEntry.maxAge = 2;
	ageTypesData->emplace_back(g3DataEntry);

	//ss.SetId(AgeType::ssId);
	//ss.SetName("ss");
	AgeTypeDataEntry ssDataEntry;
	ssDataEntry.minAge = 1;
	ssDataEntry.maxAge = 2;
	ageTypesData->emplace_back(ssDataEntry);

	//s.SetId(AgeType::sId);
	//s.SetName("s");
	AgeTypeDataEntry sDataEntry;
	sDataEntry.minAge = 1;
	sDataEntry.maxAge = 2;
	ageTypesData->emplace_back(sDataEntry);

	this->PlantsAgeTypeData.emplace_back(ageTypesData);
}