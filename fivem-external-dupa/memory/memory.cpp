#include "memory.h"
#include <iostream>
#include "skStr.h"
i_memory& i_memory::reeq()
{
	static i_memory ins;
	return ins;
}


void i_memory::initialize()
{

	window = FindWindowA("grcWindow", 0);
	if (!window)
	{
		std::cout << "Fivem not found (start fivem before load)";
	}
	GetWindowThreadProcessId(window, &pid);
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, pid);


}



bool i_memory::find_build(const wchar_t* processName) {

	bool exists = false;
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry))
		while (Process32Next(snapshot, &entry))
			if (!_wcsicmp(entry.szExeFile, processName))
				exists = true;

	CloseHandle(snapshot);
	return exists;
}



uintptr_t i_memory::get_base_address(std::string a1)
{
	std::wstring pname = std::wstring(a1.begin(), a1.end());
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
	if (hSnap == INVALID_HANDLE_VALUE)
		return 0;
	MODULEENTRY32 ent;
	ent.dwSize = sizeof(ent);
	if (Module32First(hSnap, &ent))
	{
		do
		{
			if (!_wcsicmp(ent.szModule, pname.c_str()))
			{
				std::wcout << "Build :" << ent.szModule << std::endl;
				CloseHandle(hSnap);
				return (uintptr_t)ent.modBaseAddr;
			}
		} while (Module32Next(hSnap, &ent));
	}
	CloseHandle(hSnap);
	return 0;
}


HWND i_memory::get_process_wnd(uint32_t pid)
{
	std::pair<HWND, uint32_t> params = { 0, pid };
	BOOL bResult = EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL {
		auto pParams = (std::pair<HWND, uint32_t>*)(lParam);
	uint32_t processId = 0;

	if (GetWindowThreadProcessId(hwnd, reinterpret_cast<LPDWORD>(&processId)) && processId == pParams->second) {
		SetLastError((uint32_t)-1);
		pParams->first = hwnd;
		return FALSE;
	}

	return TRUE;

		}, (LPARAM)&params);

	if (!bResult && GetLastError() == -1 && params.first)
		return params.first;

	return NULL;
}
