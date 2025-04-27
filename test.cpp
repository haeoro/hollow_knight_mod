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
lInt healthLocation = 0x019D4478;
short userSelection;
lInt baseAddrGlob = 0;

void menuOfItems() 
{
	std::cout << "\tHollow Knight Patcher | Made by datarec";
	std::cout << "\n\n\t* * * * * * * * * * * * * * * * * * * * *"
		"\n\t*                                       *"
		"\n\t*    1) Set Currency                    *"
		"\n\t*    2) Set Health                      *"
		"\n\t*                                       *"
		"\n\t*                                       *\n\n";
}

void patchValue() 
{
	system("cls");
	menuOfItems();
	lInt val;
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
		(LPVOID)baseAddrGlob,
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
	std::vector<lInt> healthOffsets = 
	{
		0xEE0, 0xE70, 0x128, 
		0x70,0x28, 0x28,
		0x190
	};
	HANDLE mHandleVM = OpenProcess(PROCESS_VM_READ, FALSE, pId);
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
		switch (userSelection) 
		{
			case 1:
				baseAddr = newAddr + moneyOffsets[i];
				std::cout << std::hex << newAddr << std::endl;
			case 2:
				baseAddr = newAddr + healthOffsets[i];
				std::cout << std::hex << newAddr << std::endl;
		}
	}
	switch (userSelection) 
	{
	case 1:
		baseAddrGlob = newAddr + moneyOffsets[moneyOffsets.size() - 1];
		exit(1); 
		patchValue();
	case 2:
		baseAddrGlob = newAddr + healthOffsets[healthOffsets.size() - 1];
		std::cout << std::hex << baseAddrGlob; // Debug
		exit(1);
		patchValue();
	}
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
	if (userSelection == 1) 
	{
		lInt baseAddr = (lInt)mInfo.modBaseAddr + moneyLocation;
		buildMemoryAddr(baseAddr);
	}
	else if (userSelection == 2) 
	{
		lInt baseAddr = (lInt)mInfo.modBaseAddr + healthLocation;
		buildMemoryAddr(baseAddr);
	}
}

void getBaseAddr()  
{
	HANDLE mHandle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pId);
	if (GetLastError() == 87)
	{
		int pSuccess = MessageBox(NULL, L"Invalid Process ID.", NULL, MB_ICONERROR);
		exit(1);
	}
	if (userSelection == 1) 
	{
		wchar_t baseModuleName[] = L"mono-2.0-bdwgc.dll";
		resolveBaseAddress(baseModuleName, mHandle);
	}
	else if (userSelection == 2) 
	{
		wchar_t baseModuleName[] = L"UnityPlayer.dll";
		resolveBaseAddress(baseModuleName, mHandle);
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
		MessageBox(NULL, L"Game is not open.", NULL, MB_ICONERROR);
		return;
	}
	getBaseAddr();
}

int main()
{
	system("cls");
	short userInputMain;
	menuOfItems();
	std::cout << "\t>> ";
	std::cin >> userInputMain;
	userSelection = userInputMain;
	getPid();
	main();
}
