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
	WCHAR moneyBase[20] = L"mono-2.0-bdwgc.dll";
};

void patchValue(lInt finalAddr) 
{
	int val = INT_MAX;
	HANDLE wOpHandle = OpenProcess
	(
		PROCESS_VM_WRITE | PROCESS_VM_OPERATION,
		FALSE, 
		pId
	);
	BOOL writeOperation = WriteProcessMemory
	(
		wOpHandle, 
		(LPVOID)finalAddr,
		(LPCVOID)&val,
		sizeof(int),
		NULL
	);
	if (GetLastError() == 0) 
	{
		std::cout << "\n\n\tPatches applied!";
	}
}

void buildMemoryAddr(lInt baseAddr) 
{
	valueOffsets offset;
	HANDLE mHandleVM = OpenProcess(
		PROCESS_VM_READ,
		FALSE,
		pId
	);
	lInt newAddr = 0;
	for (int i = 0; i < offset.moneyOffsets.size(); i++) 
	{
		BOOL getTrueAddr = ReadProcessMemory
		(
			mHandleVM,
			(LPCVOID)baseAddr,
			(LPVOID)&newAddr,
			sizeof(lInt),
			NULL
		);
		baseAddr = newAddr + offset.moneyOffsets[i];
	}
	lInt finalAddr = newAddr + offset.moneyOffsets[offset.moneyOffsets.size() - 1];
	patchValue(finalAddr);
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

void getBaseAddr()
{
	moduleNames module;
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
		int mCheck = wcscmp(mInfo.szModule, module.moneyBase);
		if (mCheck == 0)
		{
			CloseHandle(mHandle);
			break;
		}
	}
	lInt baseAddr = (lInt)mInfo.modBaseAddr + 0x00497DE8;
	userInterface(baseAddr);
}

int main()
{
	getBaseAddr();
}
