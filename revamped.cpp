/*--------------------------------------------------------------------
Hollow Knight cheat featuring the following,

Infinite Money ( Custom )
Infinite Health

ovh.feminine216@passinbox.com
--------------------------------------------------------------------*/

#include <iostream> 
#include <windows.h> 
#include <tlhelp32.h> 
#include <vector>

#define lInt unsigned long long int

lInt memoryLocation = 0;

struct moduleName
{
	int moneyAddress = 0x00497DE8;
	int healthAddress = 0x019D4478;
	std::string moneyModule = "mono-2.0-bdwgc.dll";
	std::string healthModule = "UnityPlayer.dll";
};

void patchValue(lInt baseAddr, int inputVal)
{
	HANDLE wOpHandle = OpenProcess
	(
		PROCESS_VM_WRITE | PROCESS_VM_OPERATION,
		FALSE,
		pId
	);
	BOOL writeOperation = WriteProcessMemory
	(
		wOpHandle,
		(LPVOID)baseAddr,
		(LPCVOID)&inputVal,
		sizeof(int),
		NULL
	);
	if (GetLastError() == 0 || GetLastError() == 6)
	{
		int pSuccess = MessageBox(NULL, "Patch successful!", "Information", MB_ICONINFORMATION);
	}
	std::cout << GetLastError();
}

void enterValue(lInt baseAddr) 
{
	int inputVal;
	if (userSelection == 1) 
	{
    	system("cls");
    	std::cout << "\tSETCURRENCY$ ";
    	std::cin >> inputVal;
    	std::cout << inputVal;
    }
	else if (userSelection == 2) 
	{
		system("cls");
		std::cout << "\tSETHEALTH$ ";
		std::cin >> inputVal;
	}
	std::cout << inputVal;
	patchValue(baseAddr, inputVal);
}

void buildMemoryAddr(uintptr_t baseAddr)
{
	std::vector<lInt> moneyOffsets =
	{
		0x90,
		0xE08,
		0x88,
		0x220,
		0x18,
		0x218,
		0x1C4
	};
	std::vector<lInt> healthOffsets =
	{
		0xE00,
		0x940,
		0x128,
		0x70,
		0x28,
		0x28,
		0x190
	};

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
		if (userSelection == 1) 
		{
			baseAddr = newAddr + moneyOffsets[i];
		}
		else if (userSelection == 2) 
		{
			baseAddr = newAddr + healthOffsets[i];
		}
	}
	enterValue(baseAddr);
}

// Use a pointer to struct to get the baseModuleName.
// Dependencies include, PID, baseModuleName

uintptr_t resolveBaseAddress(int pid, char* baseModuleName)
{

	HANDLE mHandle = CreateToolhelp32Snapshot
	(
		TH32CS_SNAPMODULE,
		pid
	);

	MODULEENTRY32 mInfo;
	mInfo.dwSize = sizeof(MODULEENTRY32);
	while (Module32Next(mHandle, &mInfo))
	{
		int mCheck = strcmp(mInfo.szModule, *baseModuleName);
		if (mCheck == 0)
		{
			CloseHandle(mHandle);
			break;
		}
	}
	uintptr_t baseAddr = (lInt)mInfo.modBaseAddr + memoryLocation;
	return baseAddr;
}

DWORD getPid()
{
	int foundPid = 0;
    char processName[] = "Hollow Knight.exe";
	PROCESSENTRY32 pInfo;
	pInfo.dwSize = sizeof(PROCESSENTRY32);
	HANDLE pidHandle = CreateToolhelp32Snapshot
	(
		TH32CS_SNAPPROCESS,
		0
	);
	while (Process32Next(pidHandle, &pInfo))
	{
		int checkPidName = strcmp(pInfo.szExeFile, processName);
		if (checkPidName == 0)
		{
		

			CloseHandle(pidHandle);
			break;
		}
	}
	if (foundPid == 0)
	{
		MessageBox(NULL, "Game is not open.", NULL, MB_ICONERROR);
		return 1;
	}
	return pInfo.th32ProcessID;
}

int menuOfItems()
{
	int selection;
	std::cout << "\tHollow Knight Patcher | Made by datarec";
	std::cout << "\n\n\t* * * * * * * * * * * * * * * * * * * * *"
		"\n\t*                                       *"
		"\n\t*    1) Set Currency                    *"
		"\n\t*    2) Set Health                      *"
		"\n\t*                                       *"
		"\n\t*                                       *\n\n";
	std::cout << "\t>> ";
	std::cin >> selection;
	return selection;
}

int main()
{	
	struct moduleName modules;
	struct moduleName *ptrModules = modules;
	system("cls");
	int selection = menuOfItems();
	int pId = getPid();
	switch (selection)
	{
		case 1: uintptr_t baseAddr = resolveBaseAddress(pId, ptrModules->moneyModule);
		case 2: uintptr_t baseAddr = resolveBaseAddress(pId, ptrModules->healthModule);
	}
}
