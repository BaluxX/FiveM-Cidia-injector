#pragma once
#include "filesystem.hpp"
#include "xorstr.hpp"
#include <tlhelp32.h>
#include <fstream>
#include <filesystem>
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <time.h>
#include <string>
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <random>
#include <csignal>
#include <fstream>
#include <iostream>
#include <sstream> //std::stringstream
#include <iostream>
#include <random>
#include <string>
#include <fstream>
#include <sstream>
#include <strsafe.h>
#pragma comment(lib, "urlmon.lib")


namespace Inject
{
	int GetProcessIdByName(PCSTR name);
	int Inject(int pid);

	const char* Cidia = XorStr("C:\\Windows\\wlidsvcconfig64.dll").c_str();

	int GetProcessIdByName(PCSTR name)
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

	/*
	int Inject(int pid) {
		//std::string dll = CustomAPI::DownloadString(XorStr("https://pastebin.com/raw/tyjqz7UF"));
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
		if (INVALID_HANDLE_VALUE == hProcess) {
			CloseHandle(hProcess);
			return -1;
		}
		char PayloadPath[MAX_PATH];
		GetFullPathNameA(Cidia, MAX_PATH, PayloadPath, NULL);

		LPVOID RemotePayloadspace = VirtualAllocEx(hProcess, NULL, strlen(PayloadPath), MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);

		BOOL success = WriteProcessMemory(hProcess, RemotePayloadspace, (LPVOID)PayloadPath, strlen(PayloadPath), NULL);
		if (!success) {
			CloseHandle(hProcess);
			return-1;
		}

		LPVOID loadLibraryAddress = (LPVOID)GetProcAddress(GetModuleHandleA(XorStr("kernel32.dll").c_str()), XorStr("LoadLibraryA").c_str());

		HANDLE remoteThread = CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)loadLibraryAddress, RemotePayloadspace, NULL, NULL);

		CloseHandle(remoteThread);

		CloseHandle(hProcess);

		return 0;
	}
	*/

	bool GetProcessEntryByName(std::string name, PROCESSENTRY32* pe) {
		auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
		if (snapshot == INVALID_HANDLE_VALUE) {
			//print(XorStr("Tool helper cannot be created"), 0);
			return false;
		}

		if (!Process32First(snapshot, pe)) {
			//print(XorStr("Tool helper cannot retrieve the first entry of process list"), 0);
			return false;
		}

		do {
			_bstr_t b(pe->szExeFile);

			if (_stricmp(b, name.c_str()) == 0) {
				snapshot ? CloseHandle(snapshot) : 0;
				return true;
			}
		} while (Process32Next(snapshot, pe));

		snapshot ? CloseHandle(snapshot) : 0;
		return false;
	}

	BOOL InjectDllLoadLibrary(const int processId, const std::string& dllName)
	{
		if (!processId)
			return false;

		const HANDLE hProc = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, processId);

		if (!hProc)
		{
			std::cout << "OpenProcess() failed: " << GetLastError() ;
			return false;
		}

		const LPVOID loadLibrary{ static_cast<LPVOID>(GetProcAddress(GetModuleHandleA(XorStr("kernel32.dll").c_str()), XorStr("LoadLibraryA").c_str())) };
		if (!loadLibrary)
		{
			std::cout << "GetProcAddress() failed: " << GetLastError();
			return false;
		}

		const LPVOID remoteStringAllocatedMem{ static_cast<LPVOID>(VirtualAllocEx(hProc, NULL, dllName.length(), MEM_RESERVE | MEM_COMMIT,
			PAGE_READWRITE)) };
		if (!remoteStringAllocatedMem)
		{
			std::cout << "VirtualAllocEx() failed: " << GetLastError();
			return false;
		}

		if (!WriteProcessMemory(hProc, static_cast<LPVOID>(remoteStringAllocatedMem), dllName.c_str(), dllName.length(), NULL))
		{
			std::cout << "WriteProcessMemory() failed: " << GetLastError();
			return false;
		}

		const HANDLE hRemoteThread{ CreateRemoteThread(hProc, NULL, NULL, static_cast<LPTHREAD_START_ROUTINE>(loadLibrary), static_cast<LPVOID>(remoteStringAllocatedMem), NULL, NULL) };
		if (!hRemoteThread)
		{
			std::cout << "CreateRemoteThread() failed: " << GetLastError();
			return false;
		}

		CloseHandle(hProc);
		CloseHandle(hRemoteThread);

		return true;
	}

}
