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

int pId = NULL;
lInt moneyLocation = 0x00497DE8;
short userSelection;

void menuOfItems() 
{
	std::cout << "\tHollow Knight Patcher | Made by datarec";
	std::cout << "\n\n\t* * * * * * * * * * * * * * * * * * * * *"
		"\n\t*                                       *"
		"\n\t*    1) Set Currency                    *"
		"\n\t*    2) Infinite Health                 *"
		"\n\t*                                       *"
		"\n\t*                                       *\n\n";
}

void patchValue(lInt baseAddr) 
{
	system("cls");
	menuOfItems();
	int val;
	std::cout << "\tSETCURRENCY> ";
	std::cin >> val;

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
		(LPCVOID)&val,
		sizeof(int),
		NULL
	);
	if (GetLastError() == 0 || GetLastError() == 6)
	{
		int pSuccess = MessageBox(NULL, L"Patch successful!", L"Information", MB_ICONINFORMATION);
	}
	std::cout << GetLastError();
}

void buildMemoryAddr(lInt baseAddr) 
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
	baseAddr = newAddr + moneyOffsets[moneyOffsets.size() - 1];
	patchValue(baseAddr);
}

void resolveBaseAddress(wchar_t baseModuleName[], HANDLE mHandle)
{
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
	lInt baseAddr = (lInt)mInfo.modBaseAddr + moneyLocation;
	buildMemoryAddr(baseAddr);
}

void getBaseAddr() 
{
	HANDLE mHandle = CreateToolhelp32Snapshot
	(
		TH32CS_SNAPMODULE,
		pId
	);
	if (GetLastError() == 87)
	{
		int pSuccess = MessageBox(NULL, L"Invalid Process ID.", NULL, MB_OK);
		exit(1);
	}

	// call the resolve function with the TRUE basemodname

	if (userSelection == 1) 
	{
		wchar_t baseModuleName[] = L"mono-2.0-bdwgc.dll";
		resolveBaseAddress(baseModuleName, mHandle);
	}
	else if (userSelection == 2) 
	{
		// parse through values in accordance to health, not currency.
		std::cout << "FUck, not available to user yet.";
		exit(1);
	}
}

void getPid()
{
	int foundPid = 0;
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
			pId = pInfo.th32ProcessID;
			foundPid = 1;
			CloseHandle(pidHandle);
			break;
		}
	}
	if (foundPid == 0) 
	{
		MessageBox(NULL, L"Game is not open.", NULL, NULL);
		return;
	}
	getBaseAddr();
}

int main()
{
	system("cls");
	int userInputMain;
	menuOfItems();
	std::cout << "\t>> ";
	std::cin >> userInputMain;
	userSelection = userInputMain;
	getPid();
	main();
}
