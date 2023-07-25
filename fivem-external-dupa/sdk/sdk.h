#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#include "../memory/memory.h"
#include "../main/fivem-external.h"


class i_sdk {
public:
    static i_sdk& sdk();
    ImVec2 WindowSize = ImVec2(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
    ;

    D3DXVECTOR2 world_to_screen(D3DXVECTOR3 world_pos);
    D3DXVECTOR3 get_bone_position(uintptr_t ped_ptr, int bone_id);
};

