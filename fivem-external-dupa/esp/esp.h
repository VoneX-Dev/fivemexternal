#pragma once

#include "../sdk/sdk.h"
#include <Windows.h>
#include "../main/fivem-external.h"
#include "../overlay/overlay.h"



class i_esp {
public:
    static i_esp& esp();

    void esp_init();
    bool IsOnScreen(D3DXVECTOR2 coords);
    void start();
    void draw_skeleton(uintptr_t ped_base);
    void vehicleesp();
    bool ignoreped = false;
    bool ignoredead = false;

    bool box = false;
    bool cornered = false;
    bool heathbar = false;
    bool armorbar = false;
    bool distance = false;
    bool weapon = false;
    bool playerid = false;
    bool lines = false;

    bool skeleton = false;
    bool fixedesp = true;
    bool vehiclesnapline = false;

    std::vector<std::string> friends;

    ImColor boxcolor = (255, 255, 255);
    ImColor FovColor = (255, 255, 255);
    ImColor LineColor = (255, 255, 255);
    ImColor skeletoncolor = ImColor(157, 176, 242);

    int distanceint = 90;
    int HealthBar_type = 0;

};

