/*--------------------------------------------------------------------
Hollow Knight cheat featuring the following,

Infinite Money ( Custom )

ovh.feminine216@passinbox.com
--------------------------------------------------------------------*/

#include <iostream> 
#include <windows.h>
#include <tlhelp32.h>
#include <vector>

#define lInt unsigned long long int
int pId = 3228;

lInt buildMemoryAddr(lInt baseAddr) 
{
	std::vector<lInt> offsets = 
	{
		0x90, 
		0xE08,
		0x88,
		0x220, 
		0x18, 
		0x218, 
		0x1C4
	};
	HANDLE mHandleVM = OpenProcess(
		PROCESS_VM_READ,
		FALSE,
		pId
	);
	lInt newAddr = 0;
	for (int i = 0; i < offsets.size(); i++) 
	{
		BOOL getTrueAddr = ReadProcessMemory
		(
			mHandleVM,
			(LPCVOID)baseAddr,
			(LPVOID)&newAddr,
			sizeof(lInt),
			NULL
		);
		baseAddr = newAddr + offsets[i];
	}
	lInt finalAddr = newAddr + offsets[offsets.size() - 1];
	return finalAddr;
}

void userInterface() 
{
	SMALL_RECT xyCords;
	xyCords.Left = 222;
	xyCords.Right = 222;
	xyCords.Bottom = 222;
	xyCords.Top = 222;
	
	HWND WINAPI cHandle = GetConsoleWindow();
	BOOL WINAPI consoleWinInfo = SetConsoleWindowInfo
	(
		cHandle,
		TRUE, 
		&xyCords
	);
	std::cout << GetLastError();

	//std::cout << "\tHollow Knight Patcher | Made by Datarec";

}

int main()
{
	WCHAR module[] = L"mono-2.0-bdwgc.dll";
	MODULEENTRY32 mInfo;
	mInfo.dwSize = sizeof(MODULEENTRY32);
	HANDLE mHandle = CreateToolhelp32Snapshot
	(
		TH32CS_SNAPMODULE,
		pId
	);
	while (Module32Next(mHandle, &mInfo))
	{
		int mCheck = wcscmp(mInfo.szModule, module);
		if (mCheck == 0)
		{
			CloseHandle(mHandle);
			break;
		}
	}
	lInt baseAddr = (lInt)mInfo.modBaseAddr + 0x00497DE8;
	userInterface();
	//std::cout << std::hex << buildMemoryAddr(baseAddr);

}
