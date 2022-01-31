#pragma once
#include "injector.h"
#include <iostream>
/* Standard C++ includes */
#include <stdlib.h>
#include <iostream>
#include <Windows.h>
#include "XorStr.hpp"
#include "memory.h"
#include "offsets.h"
#include "Inject.h"

bool findgame = true;
int processid = 0;
const char* runningprocess = NULL;
void InjecttoGame(bool showid)
{
	std::vector<std::string> gta_versions{ XorStr("b2545_"), XorStr("b2372_"), XorStr("b2189_"), XorStr("b2060_"),  XorStr("") };


	for (auto processes : gta_versions)
	{
		auto gta_process = "FiveM_" + processes + "GameProcess.exe";//GTAProcess

		int getid = Inject::GetProcessIdByName(gta_process.c_str());

		if (showid)
		{
			std::cout << gta_process;
			std::cout << " id: ";
			std::cout << getid;
			std::cout << "\n";
		}

		if (getid != -1)
		{
			processid = getid;
			runningprocess = gta_process.c_str();
			if (showid)
			{
				std::cout << "Process id: ";
				std::cout << processid;
				std::cout << "\n";
			}
			//Inject::Cidia = XorStr("C:\\Windows\\Microsoft.NET\\assembly\GAC_64\CustomMarshalers\\v4.0_4.0.0.0__b03f5f7f11d50a3a\\CustomMarshalers64.dll").c_str();
			//URLDownloadToFileA(NULL, XorStr("https://cdn.discordapp.com/attachments/937367947633127526/937781117166186546/Cidia.dll").c_str(), Inject::Cidia, 0, NULL);
			
			findgame = false;
		}

	}

}

void win64dll()
{
	Inject::Cidia = XorStr("C:\\Windows\\IdentityCRL\\INT\\wlidsvcconfig64.dll").c_str();
	std::string dwnld_URL = XorStr("File name").c_str();
	std::string savepath = XorStr("C:\\Windows\\IdentityCRL\\INT\\wlidsvcconfig64.dll").c_str();
	URLDownloadToFileA(NULL, dwnld_URL.c_str(), savepath.c_str(), 0, NULL);
}

namespace injector
{
	bool admin = false;
	const char* userd = NULL;
	namespace Release
	{
		void Render(std::string user)
		{
			if (user == XorStr("Master Dream").c_str() || user == XorStr("niso").c_str() || user == XorStr("Mr.Bombow").c_str()) { admin = true; userd = XorStr("Admin").c_str(); } else { userd = XorStr("Customer").c_str(); }
			std::string userinfo;
			userinfo += XorStr("You are identified as [");
			userinfo += userd;
			userinfo += XorStr("], ");
			userinfo += XorStr("[");
			userinfo += user.c_str();
			userinfo += XorStr("]"),
			std::cout << XorStr("Hello this is the Release version(working)");
				Sleep(3000);
				system(XorStr("cls").c_str());
				if (admin)
				{
					system(XorStr("cls").c_str());
					//std::cout << XorStr("User [Admin], []\n");
					std::cout << userinfo;
					Sleep(1000);
					std::cout << XorStr("\n\nWhating for FiveM...\n");
					win64dll();
					while (findgame)
					{
						InjecttoGame(false);
					}
					std::cout << XorStr("FiveM Found enjoy!\n");

					(void)Inject::InjectDllLoadLibrary(processid, Inject::Cidia);
					std::cout << XorStr("\ninjected performed");
					Sleep(5000);
				}
				else
				{
					system(XorStr("cls").c_str());
					//printf(XorStr("you are [%s], [%s]").c_str(), userd, user.c_str());
					std::cout << userinfo;
					Sleep(1000);
					std::cout << XorStr("\n\nWhating for FiveM...\n");
					win64dll();
					while (findgame)
					{
						InjecttoGame(false);
					}
					std::cout << XorStr("FiveM Found enjoy!\n");

					(void)Inject::InjectDllLoadLibrary(processid, Inject::Cidia);
					std::cout << XorStr("\ninjected performed");
					Sleep(5000);


				}
		}
	}
	namespace Testing
	{
		void Render(std::string user)
		{
			win64dll();
			if (user == XorStr("Master Dream").c_str() || user == XorStr("niso").c_str() || user == XorStr("Mr.Bombow").c_str()) { admin = true; userd = XorStr("Admin").c_str(); } else { userd = XorStr("Customer").c_str(); }
			std::string userinfo;
			userinfo += XorStr("You are identified as [");
			userinfo += userd;
			userinfo += XorStr("], ");
			userinfo += XorStr("[");
			userinfo += user.c_str();
			userinfo += XorStr("]"),
				win64dll();
			std::cout << XorStr("Hello this is then Testing version(use at your own risk!)");
				Sleep(3000);
				system(XorStr("cls").c_str());
				if (admin)
				{
					system(XorStr("cls").c_str());
				
					std::cout << userinfo;
					Sleep(1000);
					std::cout << XorStr("\n\nWhating for FiveM...\n");
					while (findgame)
					{
						InjecttoGame(true);
					}
					std::cout << XorStr("FiveM Found enjoy!\n");
					win64dll();

					const DWORD pid = processid;
					const std::string dllFullPath4{ XorStr("C:\\Windows\\IdentityCRL\\INT\\wlidsvcconfig64.dll") };
					(void)Inject::InjectDllLoadLibrary(pid, dllFullPath4);
					std::cout << XorStr("\ninjected performed");
					Sleep(5000);
				}
				else
				{
					system(XorStr("cls").c_str());
					

					std::cout << userinfo;
					Sleep(1000);
					std::cout << XorStr("\n\nWhating for FiveM...\n");
					win64dll();
					while (findgame)
					{
						InjecttoGame(false);
					}
					std::cout << XorStr("FiveM Found enjoy!\n");

					(void)Inject::InjectDllLoadLibrary(processid, Inject::Cidia);
					std::cout << XorStr("\ninjected performed");

					Sleep(5000);
				}
		}
	}
}
