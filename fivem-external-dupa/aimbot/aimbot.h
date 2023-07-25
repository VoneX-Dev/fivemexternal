#pragma once

#include <Windows.h>
#include "../sdk/sdk.h"
#include "../main/fivem-external.h"


class i_aim {
public:
    static i_aim& aim();

    uintptr_t BestTarget();
    void aimbot(uintptr_t Ped);
    void MemoryAim(D3DXVECTOR3 point, uintptr_t Ped);
    void HookMemoryAim();


    void start();

    int fov = 0;
    int smooth = 0;
    int distance = 50;
    int aimbottype = 0;
    int predictionint = 0;
    
    bool enable = false;
    bool aimenable = false;
    bool memory = false;
    bool prediction = false;

    bool showfov = false;

    bool ignoreped = false;
    bool ignoredeath = false;
    bool ignoredown = false;
    bool targetfriend = false;
    int boneid = 0;
    int aimkey = 0;

    

};
