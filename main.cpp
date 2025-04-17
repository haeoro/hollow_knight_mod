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
int pId = 4688;

struct valueOffsets
{
	std::vector<lInt> moneyOffsets =
	{
		0x90, 0xE08, 0x88, 0x220,
		0x18, 0x218, 0x1C4
	};
};

struct moduleNames
{
};

lInt buildMemoryAddr(lInt baseAddr) 
{
	offsets offset;
	HANDLE mHandleVM = OpenProcess(
		PROCESS_VM_READ,
		FALSE,
		pId
	);
	lInt newAddr = 0;
	for (int i = 0; i < moneyOffsets.size(); i++) 
	{
		BOOL getTrueAddr = ReadProcessMemory
		(
			mHandleVM,
			(LPCVOID)baseAddr,
			(LPVOID)&newAddr,
			sizeof(lInt),
			NULL
		);
		baseAddr = newAddr + moneyOffsets[i];
	}
	lInt finalAddr = newAddr + moneyOffsets[moneyOffsets.size() - 1];
	return finalAddr;
}

void userInterface(lInt baseAddr) 
{
	int userInputMain;
	std::cout << "\tHollow Knight Patcher | Made by datarec";
	std::cout << "\n\n\t* * * * * * * * * * * * * * * * * * * * *"
		"\n\t*                                       *"
		"\n\t*    1) Max Money                       *"
		"\n\t*    2) Infinite                        *"
		"\n\t*                                       *"
		"\n\t*                                       *\n\n"
		;
	std::cout << "\t>> ";
	std::cin >> userInputMain;
	if (userInputMain == 1) 
	{
		buildMemoryAddr(baseAddr);
	}
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
	if (GetLastError() == 87)
	{
		std::cout << "Invalid process ID.";
		exit(1);
	}
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
	userInterface(baseAddr);
}
