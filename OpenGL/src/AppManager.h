#pragma once

#include <memory>
#include <vector>

#include "AgeTypeDataEntry.h"
#include "EcoScaleDataEntry.h"
#include "AgeTypeData.h"
#include "vendor/sqlite/sqlite3.h"

enum class EDataSource { YAML, SQLITE, MAX };

class AppManager
{
public:
	static AppManager* GetInstance();

	AgeTypeData& GetAgeTypeData(int plantId) { return mAgeTypeData[plantId]; }

	void SetDataSource(EDataSource inSource) { mDataSource = inSource; }

	void SyncExternalData();

	int OpenDB(const std::string& dbName);

	int ExecuteSqlStatement(const std::string& inSqlStatement, int(*inCallback)(void*, int, char**, char**), void* inCallbackFirstArg);

	int DropTable(const std::string& inTableName);
	int DropTables(const std::vector<std::string>& inTableNames);

    int CreateTable(const std::string& inTableName);

	int InsertTestRows(const std::string& inTableName);

	int SelectAllTableData(const std::string& inTableName, int(*inCallback)(void*, int, char**, char**), void* inCallbackFirstArg = nullptr);

	int CloseDatabase();

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

	sqlite3* mDatabaseInstance;
};
