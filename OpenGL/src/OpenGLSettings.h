#pragma once
#include <string>

class OpenGLSettings
{
public:
	static void SetVersionString(const std::string& inVersionString);
	static void SetSLVersionString(const std::string& inVersionString);

	static int GetMajorVersion() { return mMajorVersion; }
	static int GetMinorVersion() { return mMinorVersion; }
	static int GetPatchVersion() { return mPatchVersion; }

	static int GetMajorSLVersion() { return mMajorSLVersion; }
	static int GetMinorSLVersion() { return mMinorSLVersion; }
	static int GetPatchSLVersion() { return mPatchSLVersion; }

private:
	static std::string mVersionString;
	static int mMajorVersion;
	static int mMinorVersion;
	static int mPatchVersion;

	static std::string mSLVersionString;
	static int mMajorSLVersion;
	static int mMinorSLVersion;
	static int mPatchSLVersion;

	static void ParseVersionString();
	static void ParseSLVersionString();

	static int ParseMajorVersion(const std::string& inStr);
	static int ParseMinorVersion(const std::string& inStr);
	static int ParsePatchVersion(const std::string& inStr);
};