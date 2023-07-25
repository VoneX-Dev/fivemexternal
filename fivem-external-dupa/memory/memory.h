#pragma once
#include <Windows.h>
#include <string>
#include <TlHelp32.h>
#include <Psapi.h> 
#include "auth.hpp"
#include "skStr.h"

class i_memory {
public:
    static i_memory& reeq();

    HANDLE hProcess;
    DWORD pid;
    uintptr_t base;

    void initialize();
    
    HWND window;
    uintptr_t get_base_address(std::string a1);
    HWND get_process_wnd(uint32_t pid);
    bool find_build(const wchar_t* processName);
   
    template <class t>
    t Read(uintptr_t address) {
        t read;
        ReadProcessMemory(hProcess, (LPVOID)address, &read, sizeof(t), NULL);
        return read;
    }

    template <class T>
    void Write(uintptr_t address, T value) {
        WriteProcessMemory(hProcess, (LPVOID)address, &value, sizeof(T), NULL);
    }
    


   
};


