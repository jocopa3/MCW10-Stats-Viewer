#ifndef __APPLAUNCHERMANAGER_H_
#define __APPLAUNCHERMANAGER_H_

#include <iostream>
#include "AppUtils.h"

#define MINECRAFT_APP_NAME L"Microsoft.MinecraftUWP_8wekyb3d8bbwe!App"
#define MINECRAFT_MODULE_NAME L"Minecraft.Win10.DX11.exe"

namespace MinecraftAppLauncher
{
	HRESULT showMessageBox(AppUtils::AppDebugger* app);
	/* Launches Minecraft and invokes mod injection */
	HRESULT LaunchApplication(PDWORD processId_ptr);
	/* Gets the running processId of Minecraft */
	DWORD GetProcessId(const std::wstring& processName);

	HRESULT LaunchMinecraft(PDWORD processId);
};

#endif // __APPLAUNCHERMANAGER_H_
