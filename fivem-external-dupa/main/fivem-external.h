#pragma once
#include <Windows.h>
#include <string>
#include "../overlay/overlay.h"

class i_dupa {
public:
	static i_dupa& skid();

	void start();

	uintptr_t i_world;
	uintptr_t i_replay;
	uintptr_t i_viewport;
	uintptr_t i_localplayer;
	uintptr_t i_veh;
	uintptr_t i_enginehealth;
	uintptr_t i_handling;
	uintptr_t playerinfo;
	DWORD64 i_camera;
	bool isbuild2802;
	void test();
	void logs();

	void entity_list();


};
