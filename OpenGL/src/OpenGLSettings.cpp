#include "OpenGLSettings.h"

std::string OpenGLSettings::mVersionString = "";
std::string OpenGLSettings::mSLVersionString = "";

int OpenGLSettings::mMajorVersion = 0;
int OpenGLSettings::mMinorVersion = 0;
int OpenGLSettings::mPatchVersion = 0;

int OpenGLSettings::mMajorSLVersion = 0;
int OpenGLSettings::mMinorSLVersion = 0;
int OpenGLSettings::mPatchSLVersion = 0;

void OpenGLSettings::SetVersionString(const std::string& inVersionString)
{
	mVersionString = inVersionString;
	ParseVersionString();
}

void OpenGLSettings::SetSLVersionString(const std::string& inVersionString)
{
	mSLVersionString = inVersionString;
	ParseSLVersionString();
}

int OpenGLSettings::ParseMajorVersion(const std::string& inStr)
{
	int firstDelimIndex = -1;

	for (int i = 0; i < inStr.length(); ++i)
	{
		if (inStr[i] == '.')
		{
			firstDelimIndex = i;
		}
	}

	return std::stoi(inStr.substr(0, firstDelimIndex));
}

int OpenGLSettings::ParseMinorVersion(const std::string& inStr)
{
	int firstDelimIndex = -1;
	int secondDelimIndex = -1;
	int delimsCount = 0;

	for (int i = 0; i < inStr.length(); ++i)
	{
		if (inStr[i] == '.')
		{
			++delimsCount;
			switch (delimsCount)
			{
				case 1:
				{
					firstDelimIndex = i;
					break;
				}
				case 2:
				{
					secondDelimIndex = i;
					break;
				}
				default: break;
			}
		}
	}

	return std::stoi(inStr.substr(firstDelimIndex + 1, secondDelimIndex - firstDelimIndex - 1));
}

int OpenGLSettings::ParsePatchVersion(const std::string& inStr)
{
	int secondDelimIndex = -1;
	int delimsCount = 0;

	for (int i = 0; i < inStr.length(); ++i)
	{
		if (inStr[i] == '.')
		{
			++delimsCount;
			switch (delimsCount)
			{
			case 2:
			{
				secondDelimIndex = i;
				break;
			}
			default: break;
			}
		}
	}

	return secondDelimIndex == -1 ? -1 : std::stoi(inStr.substr(secondDelimIndex + 1));
}

void OpenGLSettings::ParseVersionString()
{
	mMajorVersion = ParseMajorVersion(mVersionString);
	mMinorVersion = ParseMinorVersion(mVersionString);
	mPatchVersion = ParsePatchVersion(mVersionString);
}

void OpenGLSettings::ParseSLVersionString()
{
	mMajorSLVersion = ParseMajorVersion(mSLVersionString);
	mMinorSLVersion = ParseMinorVersion(mSLVersionString);
	mPatchSLVersion = ParsePatchVersion(mSLVersionString);
}