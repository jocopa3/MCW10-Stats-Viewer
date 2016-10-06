#ifndef __APPUTILS_H_
#define __APPUTILS_H_

/*
 * Features basic tools for dealing with Win10 apps and a wrapper for IPackageDebugSettings
 */

#include <Windows.h>
#include <iostream>
#include <string>
#include <ShObjIdl.h>
#include <atlbase.h>
#include <wrl.h>

using namespace Microsoft;

namespace AppUtils
{
	std::wstring GetMinecraftPackageId();
	std::wstring GetMinecraftApplicationId();
	HRESULT LaunchApplication(LPCWSTR packageFullName, PDWORD pdwProcessId);

	class StateChangeMonitor : public WRL::RuntimeClass<WRL::RuntimeClassFlags<WRL::ClassicCom>, IPackageExecutionStateChangeNotification> {
	public:
		StateChangeMonitor();

		virtual ~StateChangeMonitor();

		void initialize(const wchar_t *packageFullName, ATL::CComPtr<IPackageDebugSettings> packageDebugSettingsIn);
		HRESULT __stdcall OnStateChanged(const wchar_t *packageFullName, PACKAGE_EXECUTION_STATE state);
		PACKAGE_EXECUTION_STATE getState();
		void setCallback(void* CallbackPtr);

	private:
		void showStateMessageBox(PACKAGE_EXECUTION_STATE state);

		typedef void(*callbackfn)(PACKAGE_EXECUTION_STATE);
		
		callbackfn callbackPtr;
		PACKAGE_EXECUTION_STATE currentState;
		ATL::CComPtr<IPackageDebugSettings> packageDebugSettings;
		DWORD registrationId;
	};

	class AppDebugger
	{
	public:
		AppDebugger(const std::wstring& packageFullName);
		virtual ~AppDebugger();

		HRESULT GetHRESULT();

		void DisableDebugging();
		void EnableDebugging(std::wstring commandLineParameters);
		void Suspend();
		void Resume();
		void TerminateAllProcesses();

		PACKAGE_EXECUTION_STATE GetPackageExecutionState();
		void setStateChangeCallback(void* callbackPtr);
		void stopListening();

	private:
		std::wstring PackageFullName;
		HRESULT hResult;
		ATL::CComPtr<IPackageDebugSettings> debugSettings;
		WRL::ComPtr<StateChangeMonitor> stateChangeMonitor;
	};
};

#endif // __UTILS_H_
#pragma once
