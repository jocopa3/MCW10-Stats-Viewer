#pragma once

#include <Windows.h>
#include <string>
#include <iostream>
#include <stdint.h>
#include <ShObjIdl.h>
#include <Shlobj.h>
#include <tlhelp32.h>
#include <Psapi.h>
#include "MinecraftDataTypes.h"

namespace Util {

	class ProcessReader {
	public:
		ProcessReader();
		~ProcessReader();

		// Process utils
		bool openProcess(std::wstring processName);
		bool openProcess(DWORD processId);
		void close();
		bool isOpen();

		HANDLE getHandle();
		DWORD_PTR getBaseAddress();
		DWORD getLastError();

		// Read Basic Data Types
		bool readBool(DWORD_PTR offset);
		byte readByte(DWORD_PTR offset);
		short readShort(DWORD_PTR offset);
		int readInt(DWORD_PTR offset);
		float readFloat(DWORD_PTR offset);
		DWORD_PTR readPointer(DWORD_PTR offset);
		long long readLong(DWORD_PTR offset);
		double readDouble(DWORD_PTR offset);

		// Vector readers
		void readVec2(DWORD_PTR offset, Vec2* out);
		void readVec3(DWORD_PTR offset, Vec3* out);
		void readVec4(DWORD_PTR offset, Vec4* out);
		void readIntVec2(DWORD_PTR offset, IntVec2* out);
		void readIntVec3(DWORD_PTR offset, IntVec3* out);
		void readIntVec4(DWORD_PTR offset, IntVec4* out);

		// String readers
		BOOL isUTF8(DWORD_PTR offset, int len);
		void readAsciiString(DWORD_PTR offset, char* string, int maxChars);
		void readUnicodeString(DWORD_PTR offset, wchar_t* string, int maxChars);

		// Generic multi-byte reader
		bool readBytes(DWORD_PTR offset, LPVOID buffer, SIZE_T bytesToRead);

	private:
		DWORD GetProcessId(const std::wstring& processName);
		BOOL SetDebugPrivilege(BOOL bEnable);
		DWORD_PTR GetProcessBaseAddress();
		HANDLE OpenProcessWithDebug();

		BOOL is_utf8(const char * string);

		DWORD lastError;
		HANDLE processHandle;
		DWORD processId;
		DWORD_PTR baseAddress;
	};
}
