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

// #include "AgeType.h"
#include "AgeTypeData.h"
#include "AppInstance.h"

#include "vendor/sqlite/sqlite3.h"

#include "CommonDataTypes.h"

#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 540

#include <sstream>
#include <set>

#include "AgeTypesTable.h"

#include "GlobalAppStructs.h"
#include "GlobalAppDefines.h"

//#include <filesystem>
#include "OpenGLSettings.h"

void FillOutPlantsData(std::vector<Plant>& plants, int plantId);
void ScaleToReal(std::vector<Plant>& inPlants, const float scale);

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

void OnGlfwError(int id, const char* description)
{
	std::cout << description << std::endl;
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

	appMgr->OpenDB("myDataBase");
	appMgr->DropTables({ plants_TableName, ageTypes_TableName, ecoScales_TableName });

	appMgr->CreateTable(plants_TableName);
	appMgr->CreateTable(ageTypes_TableName);
	appMgr->CreateTable(ecoScales_TableName);

	appMgr->InsertTestRows(plants_TableName);
	appMgr->InsertTestRows(ageTypes_TableName);
	appMgr->InsertTestRows(ecoScales_TableName);

	std::map<int, FPlantsTableRow> plantsTableRows;
	std::map<int, FAgeTypesTableRowData> ageTypesRows;
	std::map<int, FEcoScaleTableRowData> ecoScalesTableRows;

	appMgr->SelectAllTableData(plants_TableName, printTableData);
	appMgr->SelectAllTableData(ageTypes_TableName, printTableData);
	appMgr->SelectAllTableData(ecoScales_TableName, printTableData);

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

	appMgr->SelectAllTableData(plants_TableName, getPlantsTableData, &savePlantsTableRow);

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

	appMgr->SelectAllTableData(ageTypes_TableName, getAgeTypesTableRow, &saveAgeTypesTableRowData);

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

		appMgr->SelectAllTableData(ecoScales_TableName, tableRowSelectCallback, &customCallback);
	}

	GLFWwindow* window;

	glfwSetErrorCallback(&OnGlfwError);

	// Initialize the library
	if (!glfwInit())
	{
		std::cout << "GLFW is not initialized!" << std::endl;
		return -1;
	}

	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

	const char* glVersion = (char*)glGetString(GL_VERSION);
	const std::string glVersionStr = glVersion;
	std::cout << "GL_VERSION: " << glVersionStr << std::endl;

	const std::string openglTargetVersionStr = "3.1.0";
	OpenGLSettings::SetVersionString(openglTargetVersionStr);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error!" << std::endl;
	}

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

		const char* glShadingLanguageVersion = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
		const std::string glShadingLanguageVersionStr = glShadingLanguageVersion;
		std::cout << "GL_SHADING_LANGUAGE_VERSION: " << glShadingLanguageVersionStr << std::endl;
		const std::string targetSLVersion = "1.40";
		OpenGLSettings::SetSLVersionString(targetSLVersion);

		const int versionMajor = OpenGLSettings::GetMajorVersion();
		const int versionMinor = OpenGLSettings::GetMinorVersion();

		std::string shaderFileName = "Basic.shader";
		if (versionMajor <= 3 && versionMinor < 3)
		{
			shaderFileName = "Basic_310.shader";
		}
		std::string shaderPath = "res/shaders/" + shaderFileName;
#ifdef PLATFORM_LINUX
		shaderPath = "../../OpenGL/res/shaders/" + shaderFileName;
#endif
		std::shared_ptr<Shader> shader = std::make_shared<Shader>(shaderPath);
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
		bool show_another_window = true;
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

				ImGui::InputText("Input text", testStr0, testStrLength);

				ImGui::Combo("Combo", &currentComboItem, comboItems, 5);

				if (ImGui::BeginCombo("Combo 2", currentComboItem2))
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
				}

				ImGui::Selectable("1. I am selectable", &selection[0]);

				ImGui::ListBox("ListBox", &currentListBoxItem, listBoxItems, 3);

				const int multilineTextCharsPerRow = 10;
				const int multilineTextRowsNum = 50;
				const int multilineTextCharsNum = multilineTextCharsPerRow * multilineTextRowsNum;
				const ImVec2 multilineTextSize(-1.0f, ImGui::GetTextLineHeight() * multilineTextRowsNum);
				char multilineText[multilineTextCharsNum] =
					"5.0	10.0\n"
					"15.25\t16.27\n"
					"25.10	45.11\n";
				ImGui::InputTextMultiline("Coordinates", multilineText,
					multilineTextCharsNum * sizeof(char));

				if (ImGui::Button("Parse"))
				{
					std::cout << "Parsing Numbers!" << std::endl;

					int lineBegin = 0;
					int firstNumSeparator = 0;
					int lineEnd = 0;
					int i = 0;
					std::string firstNumStr;
					std::string secondNumStr;
					float firstNum = 0.0f;
					float secondNum = 0.0f;

					while (lineBegin < multilineTextCharsNum && lineEnd < multilineTextCharsNum
						&& i< multilineTextCharsNum)
					{
						if (multilineText[i] == '\t')
						{
							firstNumSeparator = i;
						}

						if (multilineText[i] == '\n')
						{
							lineEnd = i;

							for (int j = lineBegin; j < firstNumSeparator; ++j)
							{
								firstNumStr += multilineText[j];
							}
							firstNum = atof(firstNumStr.c_str());
							std::cout << "First Num = " << firstNum << std::endl;

							for (int j = firstNumSeparator + 1; j < lineEnd; ++j)
							{
								secondNumStr += multilineText[j];
							}
							secondNum = atof(secondNumStr.c_str());
							std::cout << "Second Num = " << secondNum << std::endl;

							lineBegin = lineEnd + 1;
							lineEnd = lineBegin;
							firstNumStr = "";
							secondNumStr = "";
						}

						++i;
					}
				}
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

						Plant childPlant(it->GetId(), { childX, childY, childZ }, seRadius,
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

	//sqlite3_close(dataBase);
	appMgr->CloseDatabase();

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
