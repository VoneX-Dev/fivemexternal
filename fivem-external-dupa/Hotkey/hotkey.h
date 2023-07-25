#pragma once
#include <Windows.h>
#include <string>


class Hotkey {
public:
	static  Hotkey& hot();

	void start();

	int key = 0;
	int Aimbotkey;
};
