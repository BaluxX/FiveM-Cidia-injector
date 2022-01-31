#pragma once
#include "include.h"
#include "injector.h"
namespace Cidia
{
	//print(XorStr("").c_str(), 1);
	bool Debug = true;
	void print(std::string text) { std::cout << text; }
	void Render()
	{
		print(XorStr("Hello World!").c_str());
		Sleep(300);
		system(XorStr("cls").c_str());
		if (Debug != false)
		{
			print(XorStr("Version(Testing) 31/01/2022\n").c_str());
			Sleep(2000);
			system(XorStr("cls").c_str());
			std::string user;
			print(XorStr(" Enter Username: ").c_str());
			std::getline(std::cin, user);
			system(XorStr("cls").c_str());
			injector::Testing::Render(user.c_str());
		}
		else
		{
			print(XorStr("Version(Release) 31/01/2022").c_str());
			Sleep(2000);
			system(XorStr("cls").c_str());
			std::string user;
			print(XorStr("\nEnter Username: ").c_str());
			std::getline(std::cin, user);
			injector::Release::Render(user.c_str());
		}
	}
}