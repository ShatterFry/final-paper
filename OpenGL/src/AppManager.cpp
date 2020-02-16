#include "AppManager.h"
#include "AgeType.h"

#include <fstream>
#include <algorithm>
#include <string>
#include <iostream>
#include <map>

#include <cassert>

#include "GlobalAppDefines.h"
#include "GlobalAppStructs.h"

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

std::string ecoScalesTable_ColumnType("int");

std::string rowid_ColumnName("rowid");

std::string plantName_ColumnName("plantName");
std::string plantName_ColumnType("varchar(50)");

std::string ageBoundaryColumnType("int");

std::string se_Min_ColumnName("seMin");
std::string se_Max_ColumnName("seMax");

std::string p_Min_ColumnName("pMin");
std::string p_Max_ColumnName("pMax");

std::string j_Min_ColumnName("jMin");
std::string j_Max_ColumnName("jMax");

std::string im_Min_ColumnName("imMin");
std::string im_Max_ColumnName("imMax");

std::string v_Min_ColumnName("vMin");
std::string v_Max_ColumnName("vMax");

std::string g1_Min_ColumnName("g1Min");
std::string g1_Max_ColumnName("g1Max");

std::string g2_Min_ColumnName("g2Min");
std::string g2_Max_ColumnName("g2Max");

std::string g3_Min_ColumnName("g3Min");
std::string g3_Max_ColumnName("g3Max");

std::string ss_Min_ColumnNane("ssMin");
std::string ss_Max_ColumnName("ssMax");

std::string s_Min_ColumnName("sMin");
std::string s_Max_ColumnName("sMax");


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

	AgeTypeDataEntry& seDataEntry = ageTypesDataGenerated.GetData()[static_cast<int>(EAgeType::se)];
	seDataEntry.mMinAge = 1;
	seDataEntry.mMaxAge = 1;

	AgeTypeDataEntry& pDataEntry = ageTypesDataGenerated.GetData()[static_cast<int>(EAgeType::p)];
	pDataEntry.mMinAge = 1;
	pDataEntry.mMaxAge = 1;

	AgeTypeDataEntry& jDataEntry = ageTypesDataGenerated.GetData()[static_cast<int>(EAgeType::j)];
	jDataEntry.mMinAge = 1;
	jDataEntry.mMaxAge = 1;

	AgeTypeDataEntry& imDataEntry = ageTypesDataGenerated.GetData()[static_cast<int>(EAgeType::im)];
	imDataEntry.mMinAge = 1;
	imDataEntry.mMaxAge = 3;

	AgeTypeDataEntry& vDataEntry = ageTypesDataGenerated.GetData()[static_cast<int>(EAgeType::v)];
	vDataEntry.mMinAge = 1;
	vDataEntry.mMaxAge = 5;

	AgeTypeDataEntry& g1DataEntry = ageTypesDataGenerated.GetData()[static_cast<int>(EAgeType::g1)];
	g1DataEntry.mMinAge = 1;
	g1DataEntry.mMaxAge = 4;

	AgeTypeDataEntry& g2DataEntry = ageTypesDataGenerated.GetData()[static_cast<int>(EAgeType::g2)];
	g2DataEntry.mMinAge = 5;
	g2DataEntry.mMaxAge = 5;

	AgeTypeDataEntry& g3DataEntry = ageTypesDataGenerated.GetData()[static_cast<int>(EAgeType::g3)];
	g3DataEntry.mMinAge = 1;
	g3DataEntry.mMaxAge = 2;

	AgeTypeDataEntry& ssDataEntry = ageTypesDataGenerated.GetData()[static_cast<int>(EAgeType::ss)];
	ssDataEntry.mMinAge = 1;
	ssDataEntry.mMaxAge = 2;

	AgeTypeDataEntry& sDataEntry = ageTypesDataGenerated.GetData()[static_cast<int>(EAgeType::s)];
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

int AppManager::OpenDB(const std::string& dbName)
{
	std::string fullDatabaseName = dbName + ".dblite";

	int result = sqlite3_open(fullDatabaseName.c_str(), &mDatabaseInstance);

	if (result != SQLITE_OK)
	{
		std::cout << "Can't create/open database: " << sqlite3_errmsg(mDatabaseInstance) << std::endl;
		sqlite3_close(mDatabaseInstance);
	}

	return result;
}

int AppManager::ExecuteSqlStatement(const std::string& inSqlStatement, int(*inCallback)(void*, int, char**, char**), void* inCallbackFirstArg)
{
	char* errorMessage;

	int result = sqlite3_exec(mDatabaseInstance, inSqlStatement.c_str(), inCallback, inCallbackFirstArg, &errorMessage);

	if (result != SQLITE_OK)
	{
		std::cout << "SQL error: " << errorMessage << std::endl;
		sqlite3_free(errorMessage);
	}

	return result;
}

int AppManager::DropTable(const std::string& inTableName)
{
	std::string sqlStatement("DROP TABLE IF EXISTS " + inTableName + ";");
	return ExecuteSqlStatement(sqlStatement.c_str(), nullptr, nullptr);
}

int AppManager::DropTables(const std::vector<std::string>& inTableNames)
{
	int result = SQLITE_OK;

	for (int i = 0; i < inTableNames.size(); ++i)
	{
		int singleResult = DropTable(inTableNames[i]);

		if (singleResult != SQLITE_OK)
		{
			return singleResult;
		}
	}

	return result;
}

int AppManager::CreateTable(const std::string& inTableName)
{
	std::cout << "AppManager::CreateTable" << std::endl;

	std::string sqlStatement;
	char* sqlErrorMessage;

	if (inTableName == plants_TableName)
	{
		sqlStatement = std::string( "CREATE TABLE IF NOT EXISTS " + plants_TableName + "(" +
			plantName_ColumnName + " " +
			plantName_ColumnType + ");" );
	}
	else if (inTableName == ageTypes_TableName)
	{
		sqlStatement = std::string("CREATE TABLE IF NOT EXISTS " + ageTypes_TableName + "(");

		for (int i = 0; i < static_cast<int>(EAgeType::MAX); ++i)
		{
			EAgeType ageType = static_cast<EAgeType>(i);
			const std::string& ageTypeAbbrev = GetAgeTypeAbbrev(static_cast<EAgeType>(i));

			sqlStatement += ageTypeAbbrev + std::string("Min ");
			sqlStatement += ageBoundaryColumnType;
			sqlStatement += std::string(", ");

			sqlStatement += ageTypeAbbrev + std::string("Max ");
			sqlStatement += ageBoundaryColumnType;
			if ( ageType != static_cast<EAgeType>(static_cast<int>(EAgeType::MAX) - 1) )
			{
				sqlStatement += std::string(", ");
			}
		}

		sqlStatement += ");";
	}
	else if (inTableName == ecoScales_TableName)
	{
		sqlStatement = std::string("CREATE TABLE IF NOT EXISTS " + ecoScales_TableName + "(");

		for (int i = 0; i < static_cast<int>(EEcoScaleType::MAX); ++i)
		{
			EEcoScaleType type = static_cast<EEcoScaleType>(i);
			const std::string typeAbbrev = GetEcoScaleAbbreviation(type);

			sqlStatement += (typeAbbrev + "Min ");
			sqlStatement += ecoScalesTable_ColumnType + ", ";

			sqlStatement += (typeAbbrev + "Max ");
			sqlStatement += ecoScalesTable_ColumnType;

			if (i != (static_cast<int>(EEcoScaleType::MAX) - 1))
			{
				sqlStatement += ", ";
			}
		}

		sqlStatement += std::string(");");
	}

	return ExecuteSqlStatement(sqlStatement.c_str(), nullptr, nullptr);
}

int AppManager::InsertTestRows(const std::string& inTableName)
{
	std::cout << "AppManager::InsertTestRows" << std::endl;

	std::string sqlStatement;
	char* errorMessage;

	if (inTableName == plants_TableName)
	{
		std::map<int, FPlantsTableRow> rowsToInsert;

		FPlantsTableRow row1;
		row1.PlantName = std::string("Halocnemum strobilaceum (Pall.) M.Bieb.");
		rowsToInsert.emplace(1, row1);

		FPlantsTableRow row2;
		row2.PlantName = std::string("Suaeda maritima (L.) Dumort.");
		rowsToInsert.emplace(2, row2);

		FPlantsTableRow row3;
		row3.PlantName = std::string("Eremopyrum orientale (L.) Jaub. & Spach");
		rowsToInsert.emplace(3, row3);

		for (const std::pair<int, FPlantsTableRow> elem : rowsToInsert)
		{
			sqlStatement += std::string("INSERT INTO " + plants_TableName + "(" +
				rowid_ColumnName + ", " + plantName_ColumnName +
				") VALUES(" + std::to_string(elem.first) + ", " + "'" +
				elem.second.PlantName + "'" + ");");
		}
	}
	else if (inTableName == ageTypes_TableName)
	{
		for (int i = 1; i < 4; ++i)
		{
			sqlStatement += std::string("INSERT INTO " + ageTypes_TableName + "(" + rowid_ColumnName + ", ");

			for (int j = 0; j < static_cast<int>(EAgeType::MAX); ++j)
			{
				EAgeType ageType = static_cast<EAgeType>(j);
				std::string ageTypeAbbrev = GetAgeTypeAbbrev(ageType);

				sqlStatement += (ageTypeAbbrev + std::string("Min, "));
				sqlStatement += (ageTypeAbbrev + std::string("Max"));

				if ( ageType != static_cast<EAgeType>(static_cast<int>(EAgeType::MAX) - 1) )
				{
					sqlStatement += ", ";
				}
			}

			sqlStatement += (std::string(") VALUES(") + std::to_string(i) + ", ");

			for (int j = 0; j < static_cast<int>(EAgeType::MAX); ++j)
			{
				int minAge = 6;
				int maxAge = 9;

				EAgeType ageType = static_cast<EAgeType>(j);

				switch (ageType)
				{
					case EAgeType::se:
						minAge = 1;
						maxAge = 1;
						break;
					case EAgeType::p:
						minAge = 1;
						maxAge = 1;
						break;
					case EAgeType::j:
						minAge = 1;
						maxAge = 1;
						break;
					case EAgeType::im:
						minAge = 1;
						maxAge = 3;
						break;
					case EAgeType::v:
						minAge = 1;
						maxAge = 5;
						break;
					case EAgeType::g1:
						minAge = 1;
						maxAge = 4;
						break;
					case EAgeType::g2:
						minAge = 5;
						maxAge = 5;
						break;
					case EAgeType::g3:
						minAge = 1;
						maxAge = 2;
						break;
					case EAgeType::ss:
						minAge = 1;
						maxAge = 2;
						break;
					case EAgeType::s:
						minAge = 1;
						maxAge = 2;
						break;
					default:
						minAge = 0;
						maxAge = 0;
						break;
				}

				sqlStatement += ( std::to_string(minAge) + ", " + std::to_string(maxAge) );

				if (ageType != static_cast<EAgeType>(static_cast<int>(EAgeType::MAX) - 1))
				{
					sqlStatement += ", ";
				}
			}

			sqlStatement += ");";
		}
	}
	else if (inTableName == ecoScales_TableName)
	{
		std::vector<int> halocnemumEcoScales =
		{
			1, 7, // OM
			1, 13, // HD
			10, 19, // TR
			9, 13, // RC
			1, 3 // LC
		};

		std::vector<int> suaedaEcoScales = 
		{
			5, 11, // OM
			2, 18, // HD
			10, 18, // TR
			5, 13, // RC
			1, 4 // LC
		};

		std::vector<int> eremopyrumEcoScales = 
		{
			2, 8, // OM
			1, 8, // HD
			7, 16, // TR
			0, 0, // RC
			1, 3 // LC
		};

		std::vector<std::vector<int>> speciesEcoScales = {halocnemumEcoScales, suaedaEcoScales,
			eremopyrumEcoScales};

		for (int i = 1; i < 4; ++i)
		{
			sqlStatement += std::string("INSERT INTO " + ecoScales_TableName + "(" + rowid_ColumnName +
				", ");

			for (int j = 0; j < static_cast<int>(EEcoScaleType::MAX); ++j)
			{
				EEcoScaleType type = static_cast<EEcoScaleType>(j);

				const std::string typeAbbrev = GetEcoScaleAbbreviation(type);

				sqlStatement += typeAbbrev + "Min, ";
				sqlStatement += typeAbbrev + "Max";

				if (j != (static_cast<int>(EEcoScaleType::MAX) - 1))
				{
					sqlStatement += ", ";
				}
			}

			sqlStatement += ") VALUES(" + std::to_string(i) + ", ";

			for (int j = 0; j < static_cast<int>(EEcoScaleType::MAX) * 2; j += 2)
			{
				EEcoScaleType ecoScaleType = static_cast<EEcoScaleType>(j / 2);

				int minValue = speciesEcoScales[i - 1][j];
				int maxValue = speciesEcoScales[i - 1][j + 1];

				/*switch (ecoScaleType)
				{
				case EEcoScaleType::OM:
					minValue = 1;
					maxValue = 7;
					break;
				case EEcoScaleType::HD:
					minValue = 1;
					maxValue = 13;
					break;
				case EEcoScaleType::TR:
					minValue = 10;
					maxValue = 19;
					break;
				case EEcoScaleType::RC:
					minValue = 9;
					maxValue = 13;
					break;
				case EEcoScaleType::LC:
					minValue = 1;
					maxValue = 3;
					break;
				default:
					break;
				}*/

				sqlStatement += std::to_string(minValue) + ", " + std::to_string(maxValue);

				if (j != (static_cast<int>(EEcoScaleType::MAX) * 2 - 2))
				{
					sqlStatement += ", ";
				}
			}

			sqlStatement += ");";
		}
	}

	return ExecuteSqlStatement(sqlStatement.c_str(), nullptr, nullptr);
}

int AppManager::SelectAllTableData(const std::string& inTableName, int(*inCallback)(void*, int, char**, char**), void* inCallbackFirstArg)
{
	std::cout << "AppManager::SelectAllTableData" << std::endl;

	std::string sqlStatement;
	char* sqlErrorMsg;

	if (inTableName == plants_TableName)
	{
		sqlStatement = std::string("SELECT " + rowid_ColumnName + ", " + plantName_ColumnName +
			" FROM " + plants_TableName + ";");
	}
	else if (inTableName == ageTypes_TableName)
	{
		sqlStatement = std::string("SELECT " + rowid_ColumnName + ", ");

		for (int i = 0; i < static_cast<int>(EAgeType::MAX); ++i)
		{
			EAgeType ageType = static_cast<EAgeType>(i);
			sqlStatement += GetAgeTypeAbbrev(ageType) + "Min, ";

			if ( ageType == static_cast<EAgeType>(static_cast<int>(EAgeType::MAX) - 1) )
			{
				sqlStatement += GetAgeTypeAbbrev(ageType) + "Max ";
			}
			else
			{
				sqlStatement += GetAgeTypeAbbrev(ageType) + "Max, ";
			}
		}

		sqlStatement += "FROM " + ageTypes_TableName + ";";
	}
	else if (inTableName == ecoScales_TableName)
	{
		sqlStatement += "SELECT " + rowid_ColumnName + ", ";

		for (int i = 0; i < static_cast<int>(EEcoScaleType::MAX); ++i)
		{
			EEcoScaleType type = static_cast<EEcoScaleType>(i);
			const std::string typeAbbrev = GetEcoScaleAbbreviation(type);

			sqlStatement += typeAbbrev + "Min, ";
			sqlStatement += typeAbbrev + "Max";

			if ( i != (static_cast<int>(EEcoScaleType::MAX) - 1) )
			{
				sqlStatement += ", ";
			}
		}

		sqlStatement += " FROM " + ecoScales_TableName + ";";
	}

	return ExecuteSqlStatement(sqlStatement.c_str(), inCallback, inCallbackFirstArg);
}

int AppManager::CloseDatabase()
{
	return sqlite3_close(mDatabaseInstance);
}