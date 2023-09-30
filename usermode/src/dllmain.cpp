// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.hpp"

bool main()
{
	printf("re cotei");

	nlohmann::json test{};
	test["esti prost"] = true;

	std::getchar();
	std::getchar();

	return true;
}