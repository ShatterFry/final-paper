#ifndef _USE_MATH_DEFINES
    #define _USE_MATH_DEFINES
#endif

#ifdef __STRICT_ANSI__
    #undef __STRICT_ANSI__
#endif

#include <cmath>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <iostream>
#include <vector>
#include <map>
#include <cfenv>
#include <algorithm>
#include <random>

#include "Renderer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"

#include "AgeTypeDataEntry.h"
#include "Plant.h"

#include "Point.h"
#include "Circle.h"
#include "Rectangle.h"
#include "Aliases.h"
#include "Grid.h"
#include "AppManager.h"

#include "AgeTypeData.h"
#include "AppInstance.h"

#include "vendor/sqlite/sqlite3.h"

#include "CommonDataTypes.h"

#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 540

#include <sstream>
#include <set>

#include "AgeTypesTable.h"

struct FPlantsTableRow
{
	std::string PlantName;
};

struct FAgeTypesTableRowData
{
	std::vector<AgeTypeDataEntry> AgeTypesData;
};

struct FEcoScaleTableRowData
{
	std::vector<EcoScaleDataEntry> EcoScalesData;
};

void FillOutPlantsData(std::vector<Plant>& plants, int plantId);
void ScaleToReal(std::vector<Plant>& inPlants, const float scale);

std::string plants_TableName("plants");
std::string ageTypes_TableName("ageTypes");
std::string ecoScales_TableName("ecoScales");

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

void ExecuteSql(sqlite3* db, const char* statement, int(*callback)(void*, int, char**, char**), void* firstArg, char** errMsg)
{
	if (sqlite3_exec(db, statement, callback, firstArg, errMsg) != SQLITE_OK)
	{
		std::cout << "SQL error: " << *errMsg << std::endl;
		sqlite3_free(*errMsg);
	}
}

void InsertTestRows(sqlite3* inDataBase, const std::string& inTableName)
{
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

	ExecuteSql(inDataBase, sqlStatement.c_str(), nullptr, nullptr, &errorMessage);
}

void CreateTable(sqlite3* inDataBase, const std::string& inTableName)
{
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

	ExecuteSql(inDataBase, sqlStatement.c_str(), nullptr, nullptr, &sqlErrorMessage);
}

void SelectAllTableData(sqlite3* inDataBase, const std::string& inTableName,
	int(*inCallback)(void*, int, char**, char**), void* inCallbackFirstArg = nullptr)
{
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

	ExecuteSql(inDataBase, sqlStatement.c_str(), inCallback, inCallbackFirstArg, &sqlErrorMsg);
}

float CalcEcoNiche(const std::vector<int>& specieIds,
	const std::map<int, FEcoScaleTableRowData>& inEcoScalesData, int selectedSpecieId)
{
	std::map<int, std::map<EEcoScaleType, float>> scalesAvgBySpecie;

	for (int id : specieIds)
	{
		std::map<EEcoScaleType, float> curSpecieScalesAvg;
		const std::vector<EcoScaleDataEntry>& ecoScalesData = inEcoScalesData.at(id).EcoScalesData;

		for (int i = 0; i < ecoScalesData.size(); ++i)
		{
			EEcoScaleType ecoScaleType = static_cast<EEcoScaleType>(i);
			const EcoScaleDataEntry& entry = ecoScalesData[i];
			curSpecieScalesAvg.emplace( ecoScaleType, (entry.mMin + entry.mMax) * 0.5f );
		}

		scalesAvgBySpecie.emplace(id, curSpecieScalesAvg);
	}

	std::map<EEcoScaleType, float> cenopopulationScalesAvg;

	for (int i = 0; i < static_cast<int>(EEcoScaleType::MAX); ++i)
	{
		EEcoScaleType ecoScaleType = static_cast<EEcoScaleType>(i);
		cenopopulationScalesAvg[ecoScaleType] = 0.0f;
	}

	for (int i = 0; i < static_cast<int>(EEcoScaleType::MAX); ++i)
	{
		EEcoScaleType ecoScaleType = static_cast<EEcoScaleType>(i);

		for (int id : specieIds)
		{
			cenopopulationScalesAvg[ecoScaleType] += scalesAvgBySpecie[id][ecoScaleType];
		}

		cenopopulationScalesAvg[ecoScaleType] /= scalesAvgBySpecie.size();
	}

	std::map<EEcoScaleType, float> scalesRealised;
	const std::vector<EcoScaleDataEntry>& selectedSpecieScalesData =
		inEcoScalesData.at(selectedSpecieId).EcoScalesData;

	for (int i = 0; i < static_cast<int>(EEcoScaleType::MAX); ++i)
	{
		EEcoScaleType ecoScaleType = static_cast<EEcoScaleType>(i);
		EcoScaleDataEntry selectedSpecieEntry = selectedSpecieScalesData[i];
		
		scalesRealised.emplace( ecoScaleType,
			(cenopopulationScalesAvg[ecoScaleType] - selectedSpecieEntry.mMin) /
			(selectedSpecieEntry.mMax - selectedSpecieEntry.mMin) );
	}

	float realisedEcoNiche = 0.0f;

	for (const std::pair<EEcoScaleType, float>& entry : scalesRealised)
	{
		realisedEcoNiche += entry.second;
	}

	realisedEcoNiche /= static_cast<int>(EEcoScaleType::MAX);

	return realisedEcoNiche;
}

int main(void)
{
	AgeTypesTable ageTypesTable;

	AppManager* appMgr = GetAppInstance()->GetAppManager();
	appMgr->SetDataSource(EDataSource::YAML);
	appMgr->SyncExternalData();

	enum class EModellingState { Active, Inactive, Paused };

	EModellingState modellingState = EModellingState::Inactive;

	std::vector<Plant> plants;
	std::shared_ptr<AgeRadiusVector> averageRadiuses = std::make_shared<AgeRadiusVector>(static_cast<int>(EAgeType::MAX), 0.0f);

	int halocnemumId = 0;

	int halocnemumId_DB = 1;
	int suaedaId_DB = 2;
	int eremopyrumId_DB = 3;

	FillOutPlantsData(plants, halocnemumId);

	//TODO - add setting map scale in menu
	float mapScale = 1.0f / 30.0f;
	bool bDataWasScaled = false;

	const int testStrLength = 128;
	char testStr0[testStrLength] = "Hello, world!";

	const char* comboItems[5] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE" };
	int currentComboItem = 0;

	const char* comboItems2[3] = {"FFF", "GGG", "HHH"};
	const char* currentComboItem2 = comboItems2[0];

	bool selection[5] = {false, true, false, false, false};

	const char* listBoxItems[3] = {"Apple", "Banana", "Cherry"};
	int currentListBoxItem = 0;

	sqlite3* dataBase;

	char* sqlErrorMsg = nullptr;

	if (sqlite3_open("myDataBase.dblite", &dataBase))
	{
		std::cout << "Can't create/open database: " << sqlite3_errmsg(dataBase) << std::endl;
		sqlite3_close(dataBase);
		return 0;
	}

	auto printTableData = [](void* firstArg, int colNum, char** colValues, char** colNames)
	{
	    std::cout << "Print Table Data" << std::endl;

        std::cout << "Column Num = " << colNum << std::endl;

		for (int i = 0; i < colNum; ++i)
		{
			std::cout << colNames[i] << " = " << (colValues[i] ? colValues[i] : "NULL") << std::endl;
		}

        std::cout << "\n";

		return 0;
	};


	auto dropTable = [&](const std::string& inTableName)
	{
		std::string sqlStm("DROP TABLE IF EXISTS " + inTableName + ";");
		ExecuteSql(dataBase, sqlStm.c_str(), nullptr, nullptr, &sqlErrorMsg);
	};

	dropTable(plants_TableName);
	dropTable(ageTypes_TableName);
	dropTable(ecoScales_TableName);

	CreateTable(dataBase, plants_TableName);
	CreateTable(dataBase, ageTypes_TableName);
	CreateTable(dataBase, ecoScales_TableName);

	InsertTestRows(dataBase, plants_TableName);
	InsertTestRows(dataBase, ageTypes_TableName);
	InsertTestRows(dataBase, ecoScales_TableName);

	std::map<int, FPlantsTableRow> plantsTableRows;
	std::map<int, FAgeTypesTableRowData> ageTypesRows;
	std::map<int, FEcoScaleTableRowData> ecoScalesTableRows;

	SelectAllTableData(dataBase, plants_TableName, printTableData);
	SelectAllTableData(dataBase, ageTypes_TableName, printTableData);
	SelectAllTableData(dataBase, ecoScales_TableName, printTableData);

	auto getPlantsTableData = [](void* firstArg, int num, char** colValue, char** colName)
	{
		std::function<void(const std::pair<int, FPlantsTableRow>&)>* funcToCall = static_cast<std::function<void(const std::pair<int, FPlantsTableRow>&)>*>(firstArg);

		std::pair<int, FPlantsTableRow> pair;
		std::stringstream(colValue[0]) >> pair.first;
		pair.second.PlantName = std::string(colValue[1]);

		(*funcToCall)(pair);

		return 0;
	};

	std::function<void(const std::pair<int, FPlantsTableRow>&)> savePlantsTableRow =
	[&](const std::pair<int, FPlantsTableRow>& pair)
	{
		plantsTableRows.emplace(pair);
	};

	SelectAllTableData(dataBase, plants_TableName, getPlantsTableData, &savePlantsTableRow);

	auto getAgeTypesTableRow = [](void* firstArg, int colNum, char** colValue, char** colName)
	{
		std::function<void(const std::pair<int, FAgeTypesTableRowData>&)>* customCallback =
			static_cast<std::function<void(const std::pair<int, FAgeTypesTableRowData>&)>*>(firstArg);

		std::pair<int, FAgeTypesTableRowData> rowData;

		std::stringstream(colValue[0]) >> rowData.first;

		for (int i = 1; i < (static_cast<int>(EAgeType::MAX) * 2) + 1; i += 2)
		{
			AgeTypeDataEntry entry;

			std::stringstream(colValue[i]) >> entry.mMinAge;
			std::stringstream(colValue[i + 1]) >> entry.mMaxAge;

			rowData.second.AgeTypesData.emplace_back(entry);
		}

		(*customCallback)(rowData);

		return 0;
	};

	std::function<void(const std::pair<int, FAgeTypesTableRowData>&)> saveAgeTypesTableRowData =
	[&](const std::pair<int, FAgeTypesTableRowData>& pair)
	{
		ageTypesRows.emplace(pair);
	};

	SelectAllTableData(dataBase, ageTypes_TableName, getAgeTypesTableRow, &saveAgeTypesTableRowData);

	{
		auto tableRowSelectCallback = [](void* firstArg, int colNum, char** colValue, char** colName)
		{
			std::function<void(const std::pair<int, FEcoScaleTableRowData>&)>* customCallback =
				static_cast<std::function<void(const std::pair<int, FEcoScaleTableRowData>&)>*>(firstArg);

			std::pair<int, FEcoScaleTableRowData> pair;

			std::stringstream(colValue[0]) >> pair.first;

			for (int i = 1; i < static_cast<int>(EEcoScaleType::MAX) * 2 + 1 ; i +=2)
			{
				EcoScaleDataEntry entry;

				std::stringstream(colValue[i]) >> entry.mMin;
				std::stringstream(colValue[i + 1]) >> entry.mMax;

				pair.second.EcoScalesData.emplace_back(entry);
			}

			(*customCallback)(pair);

			return 0;
		};

		std::function<void(const std::pair<int, FEcoScaleTableRowData>&)> customCallback =
		[&](const std::pair<int, FEcoScaleTableRowData>& entry)
		{
			ecoScalesTableRows.emplace(entry);
		};

		SelectAllTableData(dataBase, ecoScales_TableName, tableRowSelectCallback, &customCallback);
	}

	GLFWwindow *window;

	// Initialize the library
	if (!glfwInit())
	{
		std::cout << "GLFW is not initialized!" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(960, 540, "Modelling", NULL, NULL);

	if (!window)
	{
		std::cout << "GLFW Window is not created!" << std::endl;
		std::cin.get();

		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error!" << std::endl;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	{
		std::shared_ptr<std::vector<float>> positions = std::make_shared<std::vector<float>>();
		*positions = {
			-50.0f, -50.0f, 0.0f, 0.0f, //0
			50.0f, -50.0f, 1.0f, 0.0f, // 1
			50.0f, 50.0f, 1.0f, 1.0f, // 2
			-50.0f, 50.0f, 0.0f, 1.0f // 3
		};

		std::shared_ptr<UIntVector> indices = std::make_shared<UIntVector>();
		*indices = {
			0, 1, 2,
			2, 3, 0
		};

        {
            FFunctionCallback funcCB = []()
            {
                glEnable(GL_BLEND);
            };
            GLCall(funcCB);
        }

        {
            FFunctionCallback funcCB = []()
            {
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            };
            GLCall(funcCB);
        }

		VertexArray va;
		VertexBuffer<float> vb(positions);

		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);

		IndexBuffer ib(indices);

		glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

		std::shared_ptr<Shader> shader = std::make_shared<Shader>("res/shaders/Basic.shader");
		shader->Bind();
		shader->SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

		Texture texture("res/textures/ChernoLogo.png");
		texture.Bind();
		shader->SetUniform1i("u_Texture", 0);

		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		shader->Unbind();

		Renderer renderer;

		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init();
		ImGui::StyleColorsDark();

		glm::vec3 translationA(200, 200, 0);
		glm::vec3 translationB(400, 200, 0);
		glm::vec3 gridTranslation(0, 0, 0);
		glm::vec3 circleTranslation(0, 0, 0);

		float r = 0.0f;
		float increment = 0.05f;

		float initialTime = 0.0f;
		bool modellingActive = false;
		float actualEcoPercent = 0.498f;

		float accumulatedTime = 0.0f;
		float passedTime = 0.0f;
		bool shouldCalcRealisedNiche = true;

		std::vector<float> curtainGrowthCoeffs(static_cast<int>(EAgeType::MAX), 0);

		curtainGrowthCoeffs[static_cast<int>(EAgeType::se)] = 25.0f;
		curtainGrowthCoeffs[static_cast<int>(EAgeType::p)] = 0.2f;
		curtainGrowthCoeffs[static_cast<int>(EAgeType::j)] = 0.2f;
		curtainGrowthCoeffs[static_cast<int>(EAgeType::im)] = 0.2f;
		curtainGrowthCoeffs[static_cast<int>(EAgeType::v)] = 0.2f;
		curtainGrowthCoeffs[static_cast<int>(EAgeType::g1)] = 0.1f;
		curtainGrowthCoeffs[static_cast<int>(EAgeType::g2)] = 0.1f;
		curtainGrowthCoeffs[static_cast<int>(EAgeType::g3)] = 0.1f;
		curtainGrowthCoeffs[static_cast<int>(EAgeType::ss)] = 0.2f;
		curtainGrowthCoeffs[static_cast<int>(EAgeType::s)] = 0.2f;

		bool bProduceChildren = true;
		bool show_another_window = false;
		float plantsRepulseDistance = 0.1f;

		while (!glfwWindowShouldClose(window))
		{
			renderer.Clear();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
				glm::mat4 mvp = proj * view * model;
				shader->Bind();
				shader->SetUniformMat4f("u_MVP", mvp);
				shader->SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
				//EDrawTypes drawType = EDrawTypes::EDT_TRIANGLES;

				//renderer.Draw(va, ib, shader, drawType);
			}

			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
				glm::mat4 mvp = proj * view * model;
				shader->Bind();
				shader->SetUniformMat4f("u_MVP", mvp);
				shader->SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
				//EDrawTypes drawType = EDrawTypes::EDT_TRIANGLES;

				//renderer.Draw(va, ib, shader, drawType);
			}

			URectangle boundingRectangle = URectangle(Point2f(0.0f, 0.0f), Point2f(0.0f, 250.0f), Point2f(250.0f, 250.0f), Point2f(250.0f, 0.0f));
			std::shared_ptr<Grid> grid = std::make_shared<Grid>(boundingRectangle, 5, 5);
			grid->Draw(proj, view, gridTranslation, shader, renderer);

			if (r > 1.0f) {
				increment = -0.05f;
			}
			else if (r < 0.0f) {
				increment = 0.05f;
			}

			r += increment;

			{
				//ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, 960.0f);
				//ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, 960.0f);
				//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

				//ImGui::Text("1) Halocnemum strobilaceum (Pall.) M.Bieb.");
				//ImGui::Text("2) Suaeda maritima (L.) Dumort.");
				//ImGui::Text("3) Eremopyrum orientale (L.) Jaub. & Spach");

				for (const std::pair<int, FPlantsTableRow>& pair : plantsTableRows)
				{
					ImGui::Text(pair.second.PlantName.c_str());
				}

				ImGui::Separator();

				if (ImGui::Button("Start") && (modellingState != EModellingState::Active))
				{
					std::cout << "Started" << std::endl;

					modellingActive = true;
					modellingState = EModellingState::Active;

					initialTime = glfwGetTime();
				}

				ImGui::SameLine();
				if (ImGui::Button("Pause") && (modellingState == EModellingState::Active))
				{
					std::cout << "Paused" << std::endl;

					accumulatedTime += passedTime;

					modellingActive = false;
					modellingState = EModellingState::Paused;
				}

				ImGui::SameLine();
				if (ImGui::Button("Stop") && (modellingState != EModellingState::Inactive))
				{
					std::cout << "Stopped" << std::endl;

					FillOutPlantsData(plants, halocnemumId);
					ScaleToReal(plants, mapScale);
					accumulatedTime = 0.0f;

					modellingActive = false;
					modellingState = EModellingState::Inactive;
				}

				if (shouldCalcRealisedNiche)
				{
					shouldCalcRealisedNiche = false;
					actualEcoPercent = CalcEcoNiche({halocnemumId_DB, suaedaId_DB, eremopyrumId_DB},
						ecoScalesTableRows, halocnemumId_DB);
				}
				ImGui::Text("Implemented niche = %.1f", actualEcoPercent);

				ImGui::InputFloat("Scale", &mapScale);

				//ImGui::Checkbox("Produce Children", &bProduceChildren);

				ImGui::Text("Plants Repulse Distance");
				ImGui::InputFloat("", &plantsRepulseDistance);

				ImGui::Text("Curtain Growth Coeff");
				ImGui::InputFloat("SE", &curtainGrowthCoeffs[static_cast<int>(EAgeType::se)]);
				ImGui::InputFloat("P", &curtainGrowthCoeffs[static_cast<int>(EAgeType::p)]);
				ImGui::InputFloat("J", &curtainGrowthCoeffs[static_cast<int>(EAgeType::j)]);
				ImGui::InputFloat("IM", &curtainGrowthCoeffs[static_cast<int>(EAgeType::im)]);
				ImGui::InputFloat("V", &curtainGrowthCoeffs[static_cast<int>(EAgeType::v)]);
				ImGui::InputFloat("G1", &curtainGrowthCoeffs[static_cast<int>(EAgeType::g1)]);
				ImGui::InputFloat("G2", &curtainGrowthCoeffs[static_cast<int>(EAgeType::g2)]);
				ImGui::InputFloat("G3", &curtainGrowthCoeffs[static_cast<int>(EAgeType::g3)]);
				ImGui::InputFloat("SS", &curtainGrowthCoeffs[static_cast<int>(EAgeType::ss)]);
				ImGui::InputFloat("S", &curtainGrowthCoeffs[static_cast<int>(EAgeType::s)]);

				//ImGui::InputText("Input text", testStr0, testStrLength);

				//ImGui::Combo("Combo", &currentComboItem, comboItems, 5);

				/*if (ImGui::BeginCombo("Combo 2", currentComboItem2))
				{
					for (int i = 0; i < 3; ++i)
					{
						bool isSelected = (currentComboItem2 == comboItems2[i]);

						if (ImGui::Selectable(comboItems2[i], isSelected))
						{
							currentComboItem2 = comboItems2[i];
						}

						if (isSelected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}

					ImGui::EndCombo();
				}*/

				//ImGui::Selectable("1. I am selectable", &selection[0]);

				//ImGui::ListBox("ListBox", &currentListBoxItem, listBoxItems, 3);
			}

			if (show_another_window)
			{
				ImGui::Begin("INFO", &show_another_window);

				ImGui::Text("Hello from another window!");

				ImGui::Text("ID = %d", 112);

				if (ImGui::Button("Close Me"))
				{
					show_another_window = false;
				}

				ImGui::End();
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			if (modellingActive)
			{
				if (!bDataWasScaled)
				{
					bDataWasScaled = true;
					ScaleToReal(plants, mapScale);
				}

				passedTime = glfwGetTime() - initialTime + accumulatedTime;
				std::cout << passedTime << std::endl;

				float passedYears = passedTime / 3.0f;

				std::vector<int> plantsToDeleteIdx;
				std::vector<Plant> plantsToAdd;

				for (std::vector<Plant>::iterator it = plants.begin(); it != plants.end(); ++it)
				{
					if ((it->GetAgeType() == EAgeType::g1 ||
						it->GetAgeType() == EAgeType::g2 ||
						it->GetAgeType() == EAgeType::g3) &&
						!it->GetProducedChild() && bProduceChildren)
					{
						std::random_device rd;
						std::mt19937 rng(rd());
						std::uniform_real_distribution<> dis(0.0f, 5.0f);

						float childX = dis(rng);
						float childY = dis(rng);
						float childZ = it->GetCenter()[2];

						float seRadius = 0.005f;

						Plant childPlant(halocnemumId, { childX, childY, childZ }, seRadius,
							EAgeType::se);

						URectangle boundingRectangle = grid->GetBoundingRectangle();

						plantsToAdd.push_back(childPlant);
						it->SetProducedChild(true);
					}

					int index = static_cast<int>(it - plants.begin());

					AppInstance* appInstance = GetAppInstance();
					AppManager* appMgr = appInstance->GetAppManager();

					AgeTypeData& ageTypeData = appMgr->GetAgeTypeData(halocnemumId);

					const std::vector<AgeTypeDataEntry>& plantAgeData = ageTypeData.GetData();

					EAgeType ageType = it->GetAgeType();

					int minAgeTypeDuration = plantAgeData.at( static_cast<int>(ageType) ).mMinAge;
					int maxAgeTypeDuration = plantAgeData.at( static_cast<int>(ageType) ).mMaxAge;

					int resultingAgeDuration = minAgeTypeDuration + (maxAgeTypeDuration -
						minAgeTypeDuration) * actualEcoPercent;

					float ageIncrease = passedYears - it->GetAccumulatedAge();

					if (ageIncrease > resultingAgeDuration)
					{
						std::cout << "Switching ages!" << std::endl;

						it->SetProducedChild(false);

						EAgeType prevAgeType = it->GetAgeType();
						EAgeType newAgeType = static_cast<EAgeType>(static_cast<int>(prevAgeType) + 1);

						if (it->GetDieOnNextAge())
						{
							plantsToDeleteIdx.push_back(index);
						}
						else if (newAgeType != EAgeType::MAX)
						{
							it->SetAgeType(newAgeType);
							it->SetAccumulatedAge(it->GetAccumulatedAge() + ageIncrease);
							int ageTypesNum = static_cast<int>(EAgeType::MAX);

							float currentRadius = it->GetRadius();

							bool bPlantsRepulsion = false;

							auto calcDistanceBetweenPlants = [](const Plant& plant1, const Plant& plant2)
							{
								const std::vector<float> plant1Center = plant1.GetCenter();
								const std::vector<float> plant2Center = plant2.GetCenter();

								float distance =  sqrt(pow((plant2Center[0] - plant1Center[0]), 2) +
									pow((plant2Center[1] - plant1Center[1]), 2)) - 0.5f * 0.5f *
									plant2.GetDiameter() - 0.5f * 0.5f * plant1.GetDiameter();

								std::cout << "Distance between plants (d): " << distance << std::endl;
								std::cout << "1/(d^2) = " << (1.0f / distance) << std::endl;

								return distance;
							};

							float curtainGrowthIncrease;
							float maxAvailableCurtainIncrease = 0.0f;

							for (int i = 0; i < plants.size(); ++i)
							{

								if (i != index && calcDistanceBetweenPlants(*it, plants[i]) <
									plantsRepulseDistance)
								{
									bPlantsRepulsion = true;
									it->SetDieOnNextAge(true);
									break;
								}
							}

							if (bPlantsRepulsion)
							{
								curtainGrowthIncrease = 0.0f;
							}
							else
							{
								curtainGrowthIncrease = currentRadius *
									curtainGrowthCoeffs[static_cast<int>(prevAgeType)];
							}

							it->SetRadius(currentRadius + curtainGrowthIncrease);
						}
						else
						{
							plantsToDeleteIdx.push_back(index);
						}
					}
				}

				std::sort(plantsToDeleteIdx.begin(), plantsToDeleteIdx.end(), [](int a, int b)
				{
					return a > b;
				});

				for (std::vector<int>::iterator i = plantsToDeleteIdx.begin();
					i != plantsToDeleteIdx.end(); ++i)
				{
					std::cout << "Vector size = " << plants.size() << std::endl;
					std::cout << "Will delete element with index " << *i << std::endl;

					plants.erase(plants.begin() + *i);
				}

				for (int i = 0; i < plantsToAdd.size(); ++i)
				{
					plants.emplace_back(plantsToAdd[i]);
				}
			}

			if (modellingState == EModellingState::Active || modellingState == EModellingState::Paused)
			{
				for (std::vector<Plant>::iterator i = plants.begin(); i != plants.end(); ++i)
				{
					URectangle gridSquare = grid->GetBoundingRectangle();

					float gridXLeft = gridSquare.GetBottomLeftPoint()._x;
					float gridXRight = gridSquare.GetBottomRightPoint()._x;

					float gridYBottom = gridSquare.GetBottomLeftPoint()._y;
					float gridYTop = gridSquare.GetTopLeftPoint()._y;

					float windowX = gridXLeft + i->GetCenter()[0] * gridSquare.GetWidht() / 5;
					float windowY = gridYBottom + i->GetCenter()[1] * gridSquare.GetHeight() / 5;
					float windowZ = i->GetCenter()[2];
					float windowRadius = i->GetDiameter() * 0.5f * gridSquare.GetWidht() / 5;

					Circle circle(windowRadius, Point2f(windowX, windowY), 10);
					circle.Draw(proj, view, circleTranslation, shader, renderer);
				}
			}

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();

	sqlite3_close(dataBase);

	return 0;
}

void FillOutPlantsData(std::vector<Plant>& plants, int plantId)
{
	plants.clear();

	{
		FPlantData plantData;
		plantData.PlantId = plantId;
		plantData.Center = { 0.24243f, 4.50999f, 0.0f };
		plantData.Radius = 6.0f;
		plantData.AgeType = EAgeType::g1;

		Plant plant;
		plant.SetData(plantData);

		plants.emplace_back(plant);
	}

	{
		FPlantData plantData;
		plantData.PlantId = plantId;
		plantData.Center = { 0.27837f, 0.41184f, 0.0f };
		plantData.Radius = 6.5f;
		plantData.AgeType = EAgeType::g3;

		Plant plant;
		plant.SetData(plantData);
		plants.emplace_back(plant);
	}

	{
		FPlantData plantData;
		plantData.AgeType = EAgeType::g1;
		plantData.Center = { 0.36258f, 3.3966f, 0.0f };
		plantData.PlantId = plantId;
		plantData.Radius = 6.0f;

		Plant plant;
		plant.SetData(plantData);

		plants.emplace_back(plant);
	}

	{
		FPlantData plantData;

		plantData.PlantId = plantId;
		plantData.Radius = 6.0f;
		plantData.AgeType = EAgeType::g1;
		plantData.Center = { 0.62103f, 2.6586f, 0.0f };

		Plant plant;
		plant.SetData(plantData);

		plants.emplace_back(plant);
	}

	{
		FPlantData plantData;

		plantData.PlantId = plantId;
		plantData.AgeType = EAgeType::g2;
		plantData.Center = { 0.96021f, 3.41421f, 0.0f };
		plantData.Radius = 6.5f;

		Plant plant;
		plant.SetData(plantData);

		plants.emplace_back(plant);
	}

	{
		FPlantData plantData;
		plantData.PlantId = plantId;
		plantData.AgeType = EAgeType::g3;
		plantData.Center = { 1.15797f, 0.47283f, 0.0f };
		plantData.Radius = 7.5f;

		Plant plant;
		plant.SetData(plantData);

		plants.emplace_back(plant);
	}

	{
		FPlantData plantData;
		plantData.AgeType = EAgeType::j;
		plantData.Center = { 1.32927f, 3.0903f, 0.0f };
		plantData.PlantId = plantId;
		plantData.Radius = 1.0f;

		Plant plant;
		plant.SetData(plantData);

		plants.emplace_back(plant);
	}

	{
		FPlantData plantData;
		plantData.AgeType = EAgeType::j;
		plantData.Center = { 1.37949f, 3.23448f, 0.0f };
		plantData.PlantId = plantId;
		plantData.Radius = 1.0f;

		Plant plant;
		plant.SetData(plantData);

		plants.emplace_back(plant);
	}

	{
		FPlantData plantData;
		plantData.AgeType = EAgeType::im;
		plantData.Center = { 1.43334f, 3.4041f, 0.0f };
		plantData.PlantId = plantId;
		plantData.Radius = 1.5f;

		Plant plant;
		plant.SetData(plantData);

		plants.emplace_back(plant);
	}

	{
		FPlantData plantData;
		plantData.AgeType = EAgeType::j;
		plantData.Center = { 1.47093f, 3.08031f, 0.0f };
		plantData.PlantId = plantId;
		plantData.Radius = 1.0f;

		Plant plant;
		plant.SetData(plantData);

		plants.emplace_back(plant);
	}

	plants.push_back(Plant(plantId, { 1.63161f, 2.96799f, 0.0f }, 6.5f, EAgeType::g2));
	plants.push_back(Plant(plantId, { 1.70658f, 3.09102f, 0.0f }, 2.0f, EAgeType::v));
	plants.push_back(Plant(plantId, { 2.01927f, 0.69852f, 0.0f }, 7.5f, EAgeType::g3));
	plants.push_back(Plant(plantId, { 2.19657f, 3.59253f, 0.0f }, 6.0f, EAgeType::g1));
	plants.push_back(Plant(plantId, { 2.58516f, 2.65056f, 0.0f }, 6.0f, EAgeType::g2));
	plants.push_back(Plant(plantId, { 2.97834f, 0.49773f, 0.0f }, 5.5f, EAgeType::ss));
	plants.push_back(Plant(plantId, { 3.47964f, 0.69081f, 0.0f }, 6.0f, EAgeType::g3));
	plants.push_back(Plant(plantId, { 3.49155f, 2.62122f, 0.0f }, 5.0f, EAgeType::s));
	plants.push_back(Plant(plantId, { 3.97167f, 3.67674f, 0.0f }, 6.0f, EAgeType::g3));
	plants.push_back(Plant(plantId, { 4.36764f, 0.33888f, 0.0f }, 7.5f, EAgeType::ss));
}

void ScaleToReal(std::vector<Plant>& inPlants, const float scale)
{
	for (int i = 0; i < inPlants.size(); ++i)
	{
		inPlants[i].SetRadius(inPlants[i].GetRadius() * scale);
	}
}
