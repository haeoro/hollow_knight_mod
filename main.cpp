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

struct baseInfo
{
	int pId = 0;
	int baseAddr = 0; 
	int valueModifySelection = 0;
};

struct memoryLocation 
{
	lInt moneyLocation = 0x00497DE8;
};

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
		333 // DEBUG
	);
	BOOL writeOperation = WriteProcessMemory
	(
		wOpHandle, 
		(LPVOID)finalAddr,
		(LPCVOID)&val,
		sizeof(int),
		NULL
	);
	if (GetLastError() == 0 || GetLastError() == 6);
	{
		int pSuccess = MessageBox(NULL, L"Patch successful! \n\n~ Inf Currency", L"SUCCESS!", MB_OK);
	}
}

void buildMemoryAddr(lInt baseAddr) 
{
	baseInfo bInfo;
	valueOffsets offset;
	HANDLE mHandleVM = OpenProcess(
		PROCESS_VM_READ,
		FALSE,
		bInfo.pId
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

int userInterface() 
{
	baseInfo bInfo;
	int userInputMain;
	std::cout << "\tHollow Knight Patcher | Made by datarec";
	std::cout << "\n\n\t* * * * * * * * * * * * * * * * * * * * *"
		"\n\t*                                       *"
		"\n\t*    1) Set Currency                    *"
		"\n\t*    2) Infinite Health                 *"
		"\n\t*                                       *"
		"\n\t*                                       *\n\n"
		;
	std::cout << "\t>> ";
	std::cin >> userInputMain;
	if (userInputMain == 1) 
	{
		bInfo.valueModifySelection = userInputMain;
	}
	else if (userInputMain == 2) 
	{
		bInfo.valueModifySelection = userInputMain;
	}
}

void getPid()
{
	baseInfo bInfo;
	wchar_t processName[] = L"Hollow Knight.exe";
	PROCESSENTRY32 pInfo;
	pInfo.dwSize = sizeof(PROCESSENTRY32);
	HANDLE pidHandle = CreateToolhelp32Snapshot
	(
		TH32CS_SNAPPROCESS, 
		0
	);
	while (Process32Next(pidHandle, &pInfo))
	{
		int checkPidName = wcscmp(pInfo.szExeFile, processName);
		if (checkPidName == 0) 
		{
			bInfo.pId = pInfo.th32ProcessID;
			CloseHandle(pidHandle);
		}
	}
}

void getBaseAddr() 
{
	memoryLocation mLocation;
	baseInfo bInfo;
	moduleNames module;
	MODULEENTRY32 mInfo;
	mInfo.dwSize = sizeof(MODULEENTRY32);
	HANDLE mHandle = CreateToolhelp32Snapshot
	(
		TH32CS_SNAPMODULE,
		bInfo.pId
	);
	if (GetLastError() == 87)
	{
		int pSuccess = MessageBox(NULL, L"Invalid Process ID.", NULL, MB_OK);
		exit(1);
	}
	if (bInfo.valueModifySelection == 1) 
	{
		while (Module32Next(mHandle, &mInfo))
		{
			int mCheck = wcscmp(mInfo.szModule, module.moneyBase);
			if (mCheck == 0)
			{
				CloseHandle(mHandle);
				break;
			}
		}
		lInt baseAddr = (lInt)mInfo.modBaseAddr + mLocation.moneyLocation;
	}
}

int main()
{
	system("cls");
	getPid();
	userInterface();
	
	main();
}
