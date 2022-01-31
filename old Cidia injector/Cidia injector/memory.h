#pragma once

#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <memory>
#include <string_view>
#include <vector>
#include <comdef.h>

namespace Memory
{
	struct HandleDisposer
	{
		using pointer = HANDLE;
		void operator()(HANDLE handle) const
		{
			if (handle != NULL || handle != INVALID_HANDLE_VALUE)
			{
				CloseHandle(handle);
			}
		}
	};

	using unique_handle = std::unique_ptr<HANDLE, HandleDisposer>;

	int GetProcessID(PCSTR name)
	{
		DWORD pid = -1;

		PROCESSENTRY32 entry;
		entry.dwSize = sizeof(PROCESSENTRY32);

		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

		if (Process32First(snapshot, &entry) == TRUE)
		{
			while (Process32Next(snapshot, &entry) == TRUE)
			{
				_bstr_t b(entry.szExeFile);

				if (_stricmp(b, name) == 0)
				{
					HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);

					pid = GetProcessId(hProcess);

					CloseHandle(hProcess);
					return pid;
				}
			}
			CloseHandle(snapshot);
		}

		CloseHandle(snapshot);
		return pid;
	}


	static uint64_t GetModuleBase(std::string module)
	{
		std::uint32_t pid = GetProcessID(module.c_str());
		auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
		auto entry = MODULEENTRY32{};
		entry.dwSize = sizeof(entry);

		while (Module32Next(snapshot, &entry))
		{
			_bstr_t b(entry.szModule);
			if (!strcmp(b, module.c_str()))
			{
#if _DEBUG
				printf("%s = 0x%llx\n", entry.szModule, (uint64_t)entry.modBaseAddr);
#endif
				CloseHandle(snapshot);
				return reinterpret_cast<uint64_t>(entry.modBaseAddr);
			}
		}

		CloseHandle(snapshot);
		return 0;
	}

	static HMODULE GetThisModuleHandle(std::string module)
	{
		std::uint32_t pid = GetProcessID(module.c_str());
		auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
		auto entry = MODULEENTRY32{};
		entry.dwSize = sizeof(entry);

		while (Module32Next(snapshot, &entry))
		{
			_bstr_t b(entry.szModule);
			if (!strcmp(b, module.c_str()))
			{
				CloseHandle(snapshot);
				return entry.hModule;
			}
		}

		CloseHandle(snapshot);
		return 0;
	}

	static uint64_t PatternScan(void* module, const char* signature)
	{
		static auto pattern_to_byte = [](const char* pattern)
		{
			auto bytes = std::vector<int>{};
			auto start = const_cast<char*>(pattern);
			auto end = const_cast<char*>(pattern) + strlen(pattern);

			for (auto current = start; current < end; ++current)
				if (*current == '?')
				{
					++current;

					if (*current == '?')
						++current;

					bytes.push_back(-1);
				}
				else
					bytes.push_back(strtoul(current, &current, 16));

			return bytes;
		};

		auto dosHeader = (PIMAGE_DOS_HEADER)module;
		auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)module + dosHeader->e_lfanew);

		auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
		auto patternBytes = pattern_to_byte(signature);
		auto scanBytes = reinterpret_cast<std::uint8_t*>(module);

		auto s = patternBytes.size();
		auto d = patternBytes.data();

		for (auto i = 0ul; i < sizeOfImage - s; ++i)
		{
			bool found = true;
			for (auto j = 0ul; j < s; ++j)
			{
				if (scanBytes[i + j] != d[j] && d[j] != -1)
				{
					found = false;
					break;
				}
			}
			if (found)
				return reinterpret_cast<uint64_t>(&scanBytes[i]);
		}

		return 0; // nullptr
	}
}