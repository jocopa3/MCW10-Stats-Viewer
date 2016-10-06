#pragma once

#include "stdafx.h"

#include "AppUtils.h"
#include "MinecraftDataTypes.h"
#include "MinecraftUtils.h"

/*
 * Definitions
 */

typedef int(__stdcall * Callback)(const int state);

/*
 * Internal Functions
 */

void CreateMinecraftInstance();

/*
 * Exported Functions
 */

EXPORT void LaunchMinecraft(bool forceRestart);
EXPORT int GetMinecraftExecutionState();
EXPORT void OpenAssetFolder();
EXPORT void OpenMinecraftFolder();
EXPORT void OpenModsFolder();
EXPORT void SetDebugging(bool DebugMode);
EXPORT void SetStateChangeCallback(Callback handler);
EXPORT void UnregisterStateChanges();