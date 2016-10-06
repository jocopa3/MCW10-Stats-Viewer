#include "Windows.h"
#include "ZenovaLauncher.h"

void Initialize()
{
	CreateMinecraftInstance();
	initStuff();
}

void Uninitialize()
{ 
	uninitStuff();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		Initialize();
		break;
	case DLL_THREAD_ATTACH: break;
	case DLL_THREAD_DETACH: break;
	case DLL_PROCESS_DETACH: 
		Uninitialize();
		break;
	}
	return TRUE;
}

