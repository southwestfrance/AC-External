// AssaultCube Trainer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Windows.h"
#include <vector>
#include "proc.h"
#include "mem.h"

void printMenu()
{
	system("cls");
}

int main()
{
	HANDLE hprocess = 0;

	uintptr_t moduleBase = 0, localPlayerPtr = 0, healthAddr = 0, ammoAddr = 0, playerSpeedAddr = 0, nameAddr = 0, crouchHeightAddr = 0;
	bool bHealth = false, bAmmo = false, bPlayerSpeed = false, bName = false, bCrouchHeight = false, bRecoil = false;

	const int newValue = 1337;

	DWORD procId = GetProcId(L"ac_client.exe");

	if (procId)
	{

		hprocess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

		moduleBase = GetModuleBaseAddress(procId, L"ac_client.exe");

		localPlayerPtr = moduleBase + 0x10f4f4;

		healthAddr = FindDMAAddy(hprocess, localPlayerPtr, { 0xf8 });

	}

	else
	{
		std::cout << "Process not found, press enter to exit\n";
		getchar();
		return 0;
	}

	DWORD dwExit = 0;

	std::cout << "test";

	while (GetExitCodeProcess(hprocess, &dwExit) && dwExit == STILL_ACTIVE)
	{
		if (GetAsyncKeyState(VK_NUMPAD0) & 1)
		{
			bHealth = !bHealth;
		}

		if (GetAsyncKeyState(VK_NUMPAD1) & 1)
		{
			bAmmo = !bAmmo;
			
			if (bAmmo)
			{
				//ff 06 = inc [esi]
				mem::PatchEx((BYTE*)(moduleBase + 0x637e9), (BYTE*)"\xFF\x06", 2, hprocess);
			}
			else
			{
				//ff 0E = dec [esi]
				mem::PatchEx((BYTE*)(moduleBase + 0x637e9), (BYTE*)"\xFF\x0E", 2, hprocess);
			}
	
		}

		if (GetAsyncKeyState(VK_NUMPAD2) & 1)
		{
			bPlayerSpeed = !bPlayerSpeed;
		}

		if (GetAsyncKeyState(VK_NUMPAD3) & 1)
		{
			bRecoil = !bRecoil;

			if (bRecoil)
			{
				mem::NopEx((BYTE*)(moduleBase + 0x63786), 10, hprocess);
			}
			else
			{
				mem::PatchEx((BYTE*)(moduleBase + 0x63786), (BYTE*)"\x50\x8D\x4C\x24\x1C\x51\x8B\xCE\xFF\xD2", 10, hprocess);
			}
		}
		if (bHealth)
		{
			mem::PatchEx((BYTE*)healthAddr, (BYTE*)&newValue, sizeof(newValue), hprocess);
		}

		Sleep(10);
	}

	std::cout << "Process not found, press enter to exit\n";
	getchar();
	return 0;
}
