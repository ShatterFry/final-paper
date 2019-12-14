#pragma once

class AppInstance* GetAppInstance();

class AppInstance
{
public:
	static AppInstance* GetInstance();
	static class AppManager* GetAppManager();

private:
	static AppInstance* mInstance;

	AppInstance();
	~AppInstance();
};