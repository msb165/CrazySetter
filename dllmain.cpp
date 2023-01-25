#include "pch.h"
#include "IniReader/IniReader.h"
#include "injector/injector.hpp"
#include "injector/assembly.hpp"
#include "dllmain.h"


void ReadIni()
{
	CIniReader iniReader("");
	disaCostumerMsg = iniReader.ReadInteger("Gameplay", "DisableCostumerMsg", 0) != 0;
	gameFov = iniReader.ReadFloat("System", "GameFOV", -1);
	freezeTimer = iniReader.ReadInteger("Cheats", "FreezeTimer", 0);
	resX = iniReader.ReadInteger("System", "ResX", -1);
	resY = iniReader.ReadInteger("System", "ResY", -1);
	fpsLimit = iniReader.ReadInteger("Gameplay", "FPSLimit", -1);
}

void ApplyIniSettings()
{
	if (disaCostumerMsg)
	{
		injector::MakeNOP(0x47344E, 6, true);
	}

	if (gameFov > 0 && gameFov != -1)
	{
		injector::WriteMemory<float>(0x46ADCE, gameFov, true);
	}

	if (freezeTimer)
	{
		injector::MakeNOP(0x473363, 1, true);
	}

	if (resX != -1 || resY != -1)
	{
		float floatResX = (float)resX;
		float floatResY = (float)resY;

		injector::WriteMemory<uint32_t>(0x407A89, resX, true);
		injector::WriteMemory<uint32_t>(0x407A93, resY, true);

		// Aspect ratio
		injector::WriteMemory<float>(0x43176A, floatResX / floatResY, true);
		injector::WriteMemory<float>(0x46ADC9, floatResX / floatResY, true);
	}

	if (fpsLimit != -1)
	{
		injector::WriteMemory<uint8_t>(0x407E4B, fpsLimit, true);
	}
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD  reason, LPVOID lpReserved)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		if (injector::ReadMemory<uint32_t>(0x400130, true) == 0x21544C66)
		{
			ReadIni();
			ApplyIniSettings();
		}
		else
		{
			MessageBoxA(0, "Invalid game version", "Error", 0);
		}

	}
}

