#include <Windows.h>
#include "../main/fivem-external.h"
#include <thread>
#include <chrono>
int main()
{
	i_dupa::skid().start();
	return i_overlay::here().Message.wParam;
}