#include "MemoryReader.h"
#include "ProcessUtils.h"
#include <memory>
#include <vector>

namespace Util {

	ProcessReader::ProcessReader() {}

	ProcessReader::~ProcessReader()
	{
		close();
	}

	bool ProcessReader::openProcess(std::wstring processName)
	{
		processId = GetProcessId(processName.c_str());

		return openProcess(processId);
	}

	bool ProcessReader::openProcess(DWORD procId)
	{
		processId = procId;

		if (processId == 0)
		{
			if (processHandle != 0)
				CloseHandle(processHandle);

			processHandle = 0;
			baseAddress = 0;

			return false;
		}

		processHandle = OpenProcessWithDebug();
		baseAddress = GetProcessBaseAddress();

		return true;
	}

	void ProcessReader::close()
	{
		if (processHandle != 0)
			CloseHandle(processHandle);

		processId = 0;
		processHandle = 0;
		baseAddress = 0;
	}

	bool ProcessReader::isOpen()
	{
		return processId != 0;
	}

	HANDLE ProcessReader::getHandle()
	{
		return processHandle;
	}

	DWORD_PTR ProcessReader::getBaseAddress()
	{
		return baseAddress;
	}

	DWORD ProcessReader::getLastError()
	{
		return lastError;
	}

	bool ProcessReader::readBool(DWORD_PTR offset)
	{
		bool buffer = 0;

		readBytes(offset, &buffer, sizeof(buffer));

		return buffer;
	}

	byte ProcessReader::readByte(DWORD_PTR offset)
	{
		byte buffer = 0;

		readBytes(offset, &buffer, sizeof(buffer));

		return buffer;
	}

	short ProcessReader::readShort(DWORD_PTR offset)
	{
		short buffer = 0;

		readBytes(offset, &buffer, sizeof(buffer));

		return buffer;
	}

	int ProcessReader::readInt(DWORD_PTR offset)
	{
		int buffer = 0;

		readBytes(offset, &buffer, sizeof(buffer));

		return buffer;
	}

	float ProcessReader::readFloat(DWORD_PTR offset)
	{
		float buffer = 0;

		readBytes(offset, &buffer, sizeof(buffer));

		return buffer;
	}

	DWORD_PTR ProcessReader::readPointer(DWORD_PTR offset)
	{
		DWORD_PTR buffer = 0;

		readBytes(offset, &buffer, sizeof(buffer));

		return buffer;
	}

	long long ProcessReader::readLong(DWORD_PTR offset)
	{
		long long buffer = 0;

		readBytes(offset, &buffer, sizeof(buffer));

		return buffer;
	}

	double ProcessReader::readDouble(DWORD_PTR offset)
	{
		double buffer = 0;

		readBytes(offset, &buffer, sizeof(buffer));

		return buffer;
	}

	void ProcessReader::readVec2(DWORD_PTR offset, Vec2* out)
	{
		readBytes(offset, out, sizeof(Vec2));
	}

	void ProcessReader::readVec3(DWORD_PTR offset, Vec3* out)
	{
		readBytes(offset, out, sizeof(Vec3));
	}

	void ProcessReader::readVec4(DWORD_PTR offset, Vec4* out)
	{
		readBytes(offset, out, sizeof(Vec4));
	}

	void ProcessReader::readIntVec2(DWORD_PTR offset, IntVec2* out)
	{
		readBytes(offset, out, sizeof(IntVec2));
	}

	void ProcessReader::readIntVec3(DWORD_PTR offset, IntVec3* out)
	{
		readBytes(offset, out, sizeof(IntVec3));
	}

	void ProcessReader::readIntVec4(DWORD_PTR offset, IntVec4* out)
	{
		readBytes(offset, out, sizeof(IntVec4));
	}
	
	// From: http://stackoverflow.com/a/1031773
	BOOL ProcessReader::is_utf8(const char * string)
	{
		if (!string)
			return 0;

		const unsigned char * bytes = (const unsigned char *)string;
		while (*bytes)
		{
			if ((// ASCII
				 // use bytes[0] <= 0x7F to allow ASCII control characters
				bytes[0] == 0x09 ||
				bytes[0] == 0x0A ||
				bytes[0] == 0x0D ||
				(0x20 <= bytes[0] && bytes[0] <= 0x7E)
				)
				) {
				bytes += 1;
				continue;
			}

			if ((// non-overlong 2-byte
				(0xC2 <= bytes[0] && bytes[0] <= 0xDF) &&
				(0x80 <= bytes[1] && bytes[1] <= 0xBF)
				)
				) {
				bytes += 2;
				continue;
			}

			if ((// excluding overlongs
				bytes[0] == 0xE0 &&
				(0xA0 <= bytes[1] && bytes[1] <= 0xBF) &&
				(0x80 <= bytes[2] && bytes[2] <= 0xBF)
				) ||
				(// straight 3-byte
					((0xE1 <= bytes[0] && bytes[0] <= 0xEC) ||
						bytes[0] == 0xEE ||
						bytes[0] == 0xEF) &&
					(0x80 <= bytes[1] && bytes[1] <= 0xBF) &&
					(0x80 <= bytes[2] && bytes[2] <= 0xBF)
					) ||
				(// excluding surrogates
					bytes[0] == 0xED &&
					(0x80 <= bytes[1] && bytes[1] <= 0x9F) &&
					(0x80 <= bytes[2] && bytes[2] <= 0xBF)
					)
				) {
				bytes += 3;
				continue;
			}

			if ((// planes 1-3
				bytes[0] == 0xF0 &&
				(0x90 <= bytes[1] && bytes[1] <= 0xBF) &&
				(0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
				(0x80 <= bytes[3] && bytes[3] <= 0xBF)
				) ||
				(// planes 4-15
					(0xF1 <= bytes[0] && bytes[0] <= 0xF3) &&
					(0x80 <= bytes[1] && bytes[1] <= 0xBF) &&
					(0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
					(0x80 <= bytes[3] && bytes[3] <= 0xBF)
					) ||
				(// plane 16
					bytes[0] == 0xF4 &&
					(0x80 <= bytes[1] && bytes[1] <= 0x8F) &&
					(0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
					(0x80 <= bytes[3] && bytes[3] <= 0xBF)
					)
				) {
				bytes += 4;
				continue;
			}

			return 0;
		}

		return 1;
	}

	BOOL ProcessReader::isUTF8(DWORD_PTR offset, int len)
	{
		auto buffer = std::make_unique<char[]>(len);

		if (readBytes(offset, buffer.get(), len))
		{
			return is_utf8(buffer.get());
		}

		return FALSE;
	}
	
	// Used to read 8-byte char strings; also used to read UTF8 strings
	void ProcessReader::readAsciiString(DWORD_PTR offset, char* string, int maxChars)
	{
		int strLen = readInt(offset + 0x10);

		// Detect if string data is stored in the string struct or somewhere else
		if (strLen < 16)
			readBytes(offset, string, strLen < maxChars ? strLen : maxChars);
		else
			readBytes(readPointer(offset), string, strLen < maxChars ? strLen : maxChars);
	}

	// Incomplete and never used; originally to be used to read wide-char (16-byte) chars
	void ProcessReader::readUnicodeString(DWORD_PTR offset, wchar_t* string, int maxChars)
	{
		readBytes(offset, string, maxChars);
	}

	// Reads bytes given
	bool ProcessReader::readBytes(DWORD_PTR offset, LPVOID buffer, SIZE_T bytesToRead)
	{
		if (processHandle == 0 || baseAddress == 0 || offset == 0)
			return false;

		if (offset < 0x1000000 && offset > 0x100000)
			offset += baseAddress;

		SIZE_T bytesRead;
		BOOL result = ReadProcessMemory(processHandle, (LPCVOID)offset, buffer, bytesToRead, &bytesRead);

		// Error checking
		lastError = GetLastError();
		
		// Possibly redundant?
		//if (lastError == S_OK && bytesToRead != bytesRead)
			//lastError = E_FAIL;

		//result |= lastError == S_OK;

		if (!result)
			ZeroMemory(buffer, bytesToRead);

		return result;
	}

	// Does not work when there are multiple instances of the same process
	// I only care about windows apps which only have one running process at a time
	DWORD ProcessReader::GetProcessId(const std::wstring& processName)
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

	BOOL ProcessReader::SetDebugPrivilege(BOOL bEnable)
	{
		HANDLE hToken = nullptr;
		LUID luid;

		if (!OpenProcessToken(processHandle, TOKEN_ADJUST_PRIVILEGES, &hToken))
		{
			lastError = GetLastError();
			return FALSE;
		}
		if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid))
		{
			lastError = GetLastError();
			return FALSE;
		}

		TOKEN_PRIVILEGES tokenPriv;
		tokenPriv.PrivilegeCount = 1;
		tokenPriv.Privileges[0].Luid = luid;
		tokenPriv.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;

		if (!AdjustTokenPrivileges(hToken, FALSE, &tokenPriv, sizeof(TOKEN_PRIVILEGES), NULL, NULL))
		{
			lastError = GetLastError();
			return FALSE;
		}

		return TRUE;
	}

	DWORD_PTR ProcessReader::GetProcessBaseAddress()
	{
		DWORD_PTR baseAddress = 0;
		HMODULE *moduleArray;
		LPBYTE moduleArrayBytes;
		DWORD bytesRequired;

		if (processHandle)
		{
			if (EnumProcessModules(processHandle, NULL, 0, &bytesRequired))
			{
				if (bytesRequired)
				{
					moduleArrayBytes = (LPBYTE)LocalAlloc(LPTR, bytesRequired);

					if (moduleArrayBytes)
					{
						unsigned int moduleCount;

						moduleCount = bytesRequired / sizeof(HMODULE);
						moduleArray = (HMODULE *)moduleArrayBytes;

						if (EnumProcessModules(processHandle, moduleArray, bytesRequired, &bytesRequired))
						{
							baseAddress = (DWORD_PTR)moduleArray[0];
						}

						LocalFree(moduleArrayBytes);
					}
				}
			}
		}

		return baseAddress;
	}

	HANDLE ProcessReader::OpenProcessWithDebug()
	{
		processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
		SetDebugPrivilege(TRUE);

		return processHandle;
	}
}