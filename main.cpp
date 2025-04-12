/*--------------------------------------------------------------------
Hollow Knight cheat featuring the following, 

Infinite Money ( Custom )

--------------------------------------------------------------------*/

#include <iostream> 
#include <windows.h>
#include <tlhelp32.h>
#include <vector>

#define lInt unsigned long long
int pId = 20960;

int resolvePtrAddr() 
{

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
			std::cout << "Match found!";
			break;
		}
	}
	lInt baseAddr = (lInt)mInfo.modBaseAddr + 0x00497DE8;
	std::cout << std::hex << baseAddr << std::endl;
	HANDLE mHandleVM = OpenProcess(
		PROCESS_VM_READ, 
		FALSE, 
		pId
	);
	lInt newAddr = 0;
	BOOL getTrueAddr = ReadProcessMemory
	(
		mHandleVM, 
		(LPCVOID)baseAddr, 
		(LPVOID)&newAddr, 
		sizeof(lInt), 
		NULL
	);
	std::cout << newAddr << std::endl;
}
