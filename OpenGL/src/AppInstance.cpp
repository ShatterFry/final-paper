#include "AppInstance.h"
#include "AppManager.h"

AppInstance* AppInstance::mInstance;

AppInstance* GetAppInstance()
{
	return AppInstance::GetInstance();
}

AppManager* AppInstance::GetAppManager()
{
	return AppManager::GetInstance();
}

AppInstance* AppInstance::GetInstance()
{
	//AppInstance* AppInstance::mInstance;

	if (!mInstance)
	{
		mInstance = new AppInstance();
	}

	return mInstance;
}

AppInstance::AppInstance()
{

}

AppInstance::~AppInstance()
{
	if (mInstance)
	{
		delete mInstance;
	}
}