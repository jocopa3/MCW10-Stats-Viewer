// Leftover code from Zenova Launcher

#include <Windows.h>
#include <Shlobj.h>
#include <aclapi.h>
#include <shellapi.h>
#include <fstream>
#include <string>
#include <map>
#include <unordered_map>

#include "AppUtils.h"
#include "MinecraftAppLauncher.h"
#include "ProcessUtils.h"
#include "utils.h"
#include "MemoryReader.h"

#include "ZenovaLauncher.h"

AppUtils::AppDebugger* app;
std::wofstream LOGFILE;

bool debugMode = false;

void CreateMinecraftInstance()
{
	if (!app)
	{
		std::wstring AppFullName = AppUtils::GetMinecraftPackageId();
		app = new AppUtils::AppDebugger(AppFullName);
	}
}

void closeLog()
{
	if (LOGFILE.is_open())
		LOGFILE.close();
}

void openLog()
{
	if (!LOGFILE.is_open())
	{
		LOGFILE.open(Util::GetCurrentDirectory() + L"\\Log.txt", std::ofstream::out | std::ofstream::app);
		LOGFILE << L"-----------------------------------------------------------------" << L"\n";
		LOGFILE << L"ZenovaLauncher.dll\n" << std::endl;
	}
}

void LaunchMinecraft(bool forceRestart)
{
	HRESULT hresult = S_OK;

	if (!app) return;

	if (SUCCEEDED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)))
	{
		if (app->GetPackageExecutionState() != PES_UNKNOWN)
		{
			MinecraftAppLauncher::showMessageBox(app);
			/*
			if (forceRestart)
			{
				app->TerminateAllProcesses();
				hresult = app->GetHRESULT();
				if (hresult != S_OK)
				{
					//std::cout << "Failed to restart Minecraft, HRESULT: " << hresult << std::endl;
					//system("PAUSE");
					return;
				}
			}
			else
				return;
			*/
		}

		std::wstring ModLoaderPath = Util::GetCurrentDirectory();
		if (ModLoaderPath.length() == 0)
		{
			if (LOGFILE.is_open()) LOGFILE << L"Failed to get the current directory" << std::endl;
			//system("PAUSE");
			CoUninitialize();
			return;//  E_FAIL;
		}

		ModLoaderPath += L"ZenovaModLoader.exe";
		if (!PathFileExists(ModLoaderPath.c_str()))
		{
			if (LOGFILE.is_open()) LOGFILE << L"Couldn't find ZenovaModLoader.exe" << std::endl;
			//system("PAUSE");
			CoUninitialize();
			return;//  E_FAIL;
		}
		
		app->EnableDebugging(ModLoaderPath);
		hresult = app->GetHRESULT();
		if (hresult != S_OK)
		{
			if (LOGFILE.is_open()) LOGFILE << L"Could not enable debugging, HRESULT: " << hresult << std::endl;
			//system("PAUSE");
			CoUninitialize();
			return;//  hresult;
		}

		DWORD dwProcessId = 0;
		std::wstring ApplicationId = AppUtils::GetMinecraftApplicationId();
		if (ApplicationId.length() == 0) {
			if (LOGFILE.is_open()) LOGFILE << L"Failed to get Minecraft's Application ID" << std::endl;
			return;// E_FAIL;
		}

		if (debugMode) closeLog(); // Let the modloader open the log
		hresult = AppUtils::LaunchApplication(ApplicationId.c_str(), &dwProcessId);
		if (debugMode) openLog();

		if (hresult != S_OK)
		{
			if (LOGFILE.is_open()) LOGFILE << L"Failed to launch Minecraft, HRESULT: " << hresult << std::endl;
			//system("PAUSE");
			CoUninitialize();
			return;
		}
		else
		{
			if (LOGFILE.is_open()) LOGFILE << L"Sucessfully launched Minecraft with mods!" << std::endl;
		}

		app->DisableDebugging();
		hresult = app->GetHRESULT();
		if (hresult != S_OK)
		{
			if (LOGFILE.is_open()) LOGFILE << L"Could not disable debugging, HRESULT: " << hresult << std::endl;
			//system("PAUSE");
			CoUninitialize();
			return;//  hresult;
		}

		//system("PAUSE");
		CoUninitialize();
	}

	return;//  S_OK;
}

int GetMinecraftExecutionState()
{
	// 5 is a custom enum specifying there was an error
	PACKAGE_EXECUTION_STATE executionState = (PACKAGE_EXECUTION_STATE)5;

	if (!app) return executionState;

	if (SUCCEEDED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)))
	{
		executionState = app->GetPackageExecutionState();

		CoUninitialize();
	}

	return executionState;//  S_OK;
}

// Opens the Minecraft AppData folder in Windows Explorer
void OpenMinecraftFolder()
{
	std::wstring minecraftFolder = Util::GetMinecraftAppDataPath();
	ShellExecute(NULL, L"open", minecraftFolder.c_str(), NULL, NULL, SW_SHOWDEFAULT);
}

// Opens the Minecraft assets AppData folder in Windows Explorer
void OpenAssetFolder()
{
	std::wstring minecraftFolder = Util::GetMinecraftAppDataPath()+L"assets\\";
	CreateDirectory(minecraftFolder.c_str(), NULL);
	ShellExecute(NULL, L"open", minecraftFolder.c_str(), NULL, NULL, SW_SHOWDEFAULT);
}

// Opens the AppData\Zenova\Mods folder in Windows Explorer
void OpenModsFolder()
{
	std::wstring modsFolder = Util::GetCommonFolder(FOLDERID_RoamingAppData) + L"\\Zenova\\Mods";
	ShellExecute(NULL, L"open", modsFolder.c_str(), NULL, NULL, SW_SHOWDEFAULT);
}

void SetDebugging(bool DebugMode)
{
	debugMode = DebugMode;
	if (debugMode) openLog();
	else closeLog();
}

Callback Handler = 0;

void StateChangeCallback(PACKAGE_EXECUTION_STATE state)
{
	Handler(state);
	if (state == PES_RUNNING)
	{
		initStuff();
	} else
	{
		uninitStuff();
	}
}

void SetStateChangeCallback(Callback handler)
{
	Handler = handler;

	if (!app) return;

	app->setStateChangeCallback(&StateChangeCallback);
}

void UnregisterStateChanges()
{
	if (!app) return;

	app->stopListening();
}