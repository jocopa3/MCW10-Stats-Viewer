#include <Windows.h>
#include <Tlhelp32.h>
#include <ShObjIdl.h>
#include <atlbase.h>

#include "ThreadWorker.h"
#include "ProcessUtils.h"

#include "MinecraftAppLauncher.h"

HRESULT MinecraftAppLauncher::showMessageBox(AppUtils::AppDebugger* app) {
	HRESULT hresult;

	switch (MessageBox(NULL, L"Minecraft is already running! Would you like to restart the game?",
		L"Zenova Modloader", MB_YESNO | MB_ICONEXCLAMATION))
	{
	case IDYES:
		app->TerminateAllProcesses();
		hresult = app->GetHRESULT();
		if (hresult != S_OK)
		{
			//std::cout << "Failed to restart Minecraft, HRESULT: " << hresult << std::endl;
			//system("PAUSE");
			return hresult;
		}
		break;
	case IDNO:
		//std::cout << "Was not allowed to relaunch Minecraft, cannot inject mods" << std::endl;
		//system("PAUSE");
		return E_ABORT;
	}
	return E_ABORT;
}

HRESULT MinecraftAppLauncher::LaunchMinecraft(PDWORD dwProcessId)
{
	HRESULT hResult = S_OK;
	if (SUCCEEDED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)))
	{
		hResult = MinecraftAppLauncher::LaunchApplication(dwProcessId);

		CoUninitialize();
	}

	if (*dwProcessId == 0)
	{
		hResult = E_APPLICATION_ACTIVATION_TIMED_OUT;
	}

	return hResult;
}

/* Launches Minecraft and invokes mod injection */
HRESULT MinecraftAppLauncher::LaunchApplication(PDWORD processId_ptr)
{
	CComPtr<IApplicationActivationManager> spAppActivationManager;
	HRESULT result = E_INVALIDARG;

	/* Initialize IApplicationActivationManager */
	result = CoCreateInstance(CLSID_ApplicationActivationManager,
		NULL,
		CLSCTX_LOCAL_SERVER,
		IID_IApplicationActivationManager,
		(LPVOID*)&spAppActivationManager);

	if (!SUCCEEDED(result)) return result;

	/* This call ensures that the app is launched as the foreground window */
	result = CoAllowSetForegroundWindow(spAppActivationManager, NULL);

	/* Launch the app */
	if (!SUCCEEDED(result)) return result;

	result = spAppActivationManager->ActivateApplication(MINECRAFT_APP_NAME, NULL, AO_NONE, processId_ptr);

	return result;
}

/* Gets the running processId of Minecraft */
DWORD MinecraftAppLauncher::GetProcessId(const std::wstring& processName)
{
	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);

	HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (processesSnapshot == INVALID_HANDLE_VALUE)
		return 0;

	Process32First(processesSnapshot, &processInfo);
	if (!processName.compare(processInfo.szExeFile))
	{
		CloseHandle(processesSnapshot);
		return processInfo.th32ProcessID;
	}

	while (Process32Next(processesSnapshot, &processInfo))
	{
		if (!processName.compare(processInfo.szExeFile))
		{
			CloseHandle(processesSnapshot);
			return processInfo.th32ProcessID;
		}
	}

	CloseHandle(processesSnapshot);
	return 0;
}