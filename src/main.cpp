/*--------------------------------------------------------------------

Hollow Knight cheat featuring the following,

Infinite Money ( Custom Value )
Increased Health ( 998 Lives )

ovh.feminine216@passinbox.com

--------------------------------------------------------------------*/

#include <iostream> 
#include <windows.h> 
#include <tlhelp32.h> 
#include <vector>

#define lInt unsigned long long int

lInt memoryLocation = 0;

struct moduleData
{
	int strtOffsetM = 0x00497DE8;
	int strtOffsetH = 0x019D4478;
	WCHAR baseModNameM[40] = L"mono-2.0-bdwgc.dll";
	WCHAR baseModNameH[40] = L"UnityPlayer.dll";
};

void patchValue(lInt baseAddr, int inputVal, int pId)
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
	int lastError = GetLastError();
	if (lastError == 0 || lastError == 6)
	{
		LPCTSTR successMsg = L"Patch successful!";
		LPCTSTR infoMsg = L"Information";

		int pSuccess = MessageBox(0, successMsg, infoMsg, MB_ICONINFORMATION);
	}
	else if (lastError == 998)
	{
		MessageBox(0, L"Offsets are out of date or incorrect. Please wait for an update.", L"Error code: 998", 0);
	}
	std::cout << GetLastError() << std::endl;
}

DWORD enterValue(int userSelection)
{
	int inputVal = 0;
	if (userSelection == 1)
	{
		system("cls");
		std::cout << "\tSETCURRENCY$ ";
		std::cin >> inputVal;
	}
	else if (userSelection == 2)
	{
		system("cls");
		std::cout << "\tSETHEALTH$ ";
		std::cin >> inputVal;
	}
	return inputVal;
}

lInt buildMemoryAddr(lInt baseAddr, int pId, int selection)
{
	std::vector<lInt> moneyOffsets = {
		0x90, 0xE08, 0x88, 0x220,
		0x18, 0x218, 0x1C4
	};

	std::vector<lInt> healthOffsets =
	{
		0xE00, 0x940, 0x128, 0x70,
		0x28, 0x28, 0x190
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
		if (selection == 1)
		{
			baseAddr = newAddr + moneyOffsets[i];
		}
		else if (selection == 2)
		{
			baseAddr = newAddr + healthOffsets[i];
		}
	}
	return baseAddr;
}

uintptr_t resolveBaseAddress(int pid, WCHAR baseModuleName[])
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
		int mCheck = wcscmp(mInfo.szModule, baseModuleName);
		if (mCheck == 0)
		{
			CloseHandle(mHandle);
			break;
		}
	}
	uintptr_t baseAddr = (lInt)mInfo.modBaseAddr;
	return baseAddr;
}

DWORD getPid()
{
	int foundPid = 0;
	WCHAR processName[] = L"hollow_knight.exe";
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
			foundPid = 1;
			CloseHandle(pidHandle);
			break;
		}
	}
	if (foundPid == 0)
	{
		LPCTSTR gameNotOpen = L"Game is not open.";
		MessageBox(NULL, gameNotOpen, NULL, MB_ICONERROR);
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

int build(int pid, int selection, WCHAR module[])
{
	lInt baseAddr = resolveBaseAddress(pid, module);
	lInt memoryAddr = buildMemoryAddr(baseAddr, pid, selection);
	int value = enterValue(selection);
	patchValue(memoryAddr, value, pid);
	return 0;
}

int main()
{
	struct moduleData modules;

	int selection = menuOfItems();
	int pId = getPid();

	if (pId == 1)
	{
		return 1;
	}
	
	switch (selection) 
	{
	case 1:
		build(pId, selection, modules.baseModNameM);
	
	case 2:
		build(pId, selection, modules.baseModNameH);
	}
	return 0;
}