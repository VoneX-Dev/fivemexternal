#include "fivem-external.h"
#include <iostream>
#include "../Hotkey/hotkey.h"


i_dupa& i_dupa::skid()
{
	static i_dupa ins;
	return ins;
}
 



void i_overlay::render()
{

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	i_aim::aim().start();
	i_esp::esp().start();
	i_exploits::exp().start();
	Hotkey::hot().start();

	if (i_aim::aim().showfov)
	{
		ImGui::GetForegroundDrawList()->AddCircle(ImVec2(ScreenCenterX, ScreenCenterY), i_aim::aim().fov, IM_COL32_WHITE, 10000, 1);
	}



	if ((GetAsyncKeyState)(VK_INSERT) & 1)
	{
		showmenu = !showmenu;
	}

	if (showmenu)
	{
		menu();
		SetWindowLongA(MyWnd, GWL_EXSTYLE, WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW);
		
		
	}
	else
	{
		SetWindowLongA(MyWnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW);
	}
	
	ImGui::GetIO().MouseDrawCursor = showmenu;
	ImGui::GetIO().WantCaptureKeyboard = showmenu;
	ImGui::EndFrame();
	p_Device->SetRenderState(D3DRS_ZENABLE, false);
	p_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	p_Device->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
	p_Device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

	if (p_Device->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		p_Device->EndScene();
	}

	HRESULT result = p_Device->Present(NULL, NULL, NULL, NULL);

	if (result == D3DERR_DEVICELOST && p_Device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
	{
		ImGui_ImplDX9_InvalidateDeviceObjects();
		p_Device->Reset(&d3dpp);
		ImGui_ImplDX9_CreateDeviceObjects();
	}
}

void i_dupa::logs()
{
	printf("Loaded!", i_world);
	printf("\nOpen Key : insert", i_replay);
	
}




void i_dupa::start()
{

	i_memory::reeq().initialize();
	if (i_memory::reeq().find_build(L"FiveM_b2545_GTAProcess.exe"))
	{
		i_dupa::skid().isbuild2802 = false;
		i_memory::reeq().base = i_memory::reeq().get_base_address("FiveM_b2545_GTAProcess.exe");
		i_dupa::skid().i_world = i_memory::reeq().Read<uintptr_t>(i_memory::reeq().base + 0x25667E8);
		i_dupa::skid().i_replay = i_memory::reeq().Read<uintptr_t>(i_memory::reeq().base + 0x1F2E7A8);
		i_dupa::skid().i_viewport = i_memory::reeq().Read<uintptr_t>(i_memory::reeq().base + 0x1FD6F70);
		i_dupa::skid().i_camera = 0x1FD7B48; 
		i_dupa::skid().i_veh = 0x0D30;
		i_dupa::skid().i_handling = 0x938;
		i_dupa::skid().i_enginehealth = 0x908;
		i_dupa::skid().i_localplayer = i_memory::reeq().Read<uintptr_t>(i_world + 0x8);
		playerinfo = 0x10C8;
		i_dupa::skid().logs();
		i_overlay::here().initialize();
	}
	else if (i_memory::reeq().find_build(L"FiveM_b2699_GTAProcess.exe"))
	{
		i_dupa::skid().isbuild2802 = false;
		i_memory::reeq().base = i_memory::reeq().get_base_address("FiveM_b2699_GTAProcess.exe");
		i_dupa::skid().i_world = i_memory::reeq().Read<uintptr_t>(i_memory::reeq().base + 0x26684D8);
		i_dupa::skid().i_replay = i_memory::reeq().Read<uintptr_t>(i_memory::reeq().base + 0x20304C8);
		i_dupa::skid().i_viewport = i_memory::reeq().Read<uintptr_t>(i_memory::reeq().base + 0x20D8C90);
		i_dupa::skid().i_camera = 0x20D9868;
		i_dupa::skid().i_veh = 0x0D30;
		i_dupa::skid().i_handling = 0x938;
		i_dupa::skid().i_enginehealth = 0x908;
		i_dupa::skid().i_localplayer = i_memory::reeq().Read<uintptr_t>(i_world + 0x8);
		playerinfo = 0x10C8;
		i_dupa::skid().logs();
		i_overlay::here().initialize();

	}
	else if (i_memory::reeq().find_build(L"FiveM_GTAProcess.exe"))
	{
		i_dupa::skid().isbuild2802 = false;
		i_memory::reeq().base = i_memory::reeq().get_base_address("FiveM_GTAProcess.exe");
		i_dupa::skid().i_world = i_memory::reeq().Read<uintptr_t>(i_memory::reeq().base + 0x247F840);
		i_dupa::skid().i_replay = i_memory::reeq().Read<uintptr_t>(i_memory::reeq().base + 0x1EFD4C8);
		i_dupa::skid().i_viewport = i_memory::reeq().Read<uintptr_t>(i_memory::reeq().base + 0x2087780);
		i_dupa::skid().i_camera = 0x20D9868;
		i_dupa::skid().i_veh = 0x0D28;
		i_dupa::skid().i_enginehealth = 0x908;
		i_dupa::skid().i_handling = 0x938;

		i_dupa::skid().i_localplayer = i_memory::reeq().Read<uintptr_t>(i_world + 0x8);
		playerinfo = 0x10C8;
		i_dupa::skid().logs();
		i_overlay::here().initialize();

	}
	else if (i_memory::reeq().find_build(L"FiveM_b2060_GTAProcess.exe"))
	{
		i_dupa::skid().isbuild2802 = false;
		i_memory::reeq().base = i_memory::reeq().get_base_address("FiveM_b2060_GTAProcess.exe");
		i_dupa::skid().i_world = i_memory::reeq().Read<uintptr_t>(i_memory::reeq().base + 0x24C8858);
		i_dupa::skid().i_replay = i_memory::reeq().Read<uintptr_t>(i_memory::reeq().base + 0x1EC3828);
		i_dupa::skid().i_viewport = i_memory::reeq().Read<uintptr_t>(i_memory::reeq().base + 0x2087780);
		i_dupa::skid().i_camera = 0x20D9868;
		i_dupa::skid().i_veh = 0x0D28;
		i_dupa::skid().i_handling = 0x938;
		i_dupa::skid().i_enginehealth = 0x908;
		i_dupa::skid().i_localplayer = i_memory::reeq().Read<uintptr_t>(i_world + 0x8);
		playerinfo = 0x10C8;
		i_dupa::skid().logs();
		i_overlay::here().initialize();

	}
	else if (i_memory::reeq().find_build(L"FiveM_b2372_GTAProcess.exe"))
	{
		i_dupa::skid().isbuild2802 = false;
		i_memory::reeq().base = i_memory::reeq().get_base_address("FiveM_b2372_GTAProcess.exe");
		i_dupa::skid().i_world = i_memory::reeq().Read<uintptr_t>(i_memory::reeq().base + 0x252DCD8);
		i_dupa::skid().i_replay = i_memory::reeq().Read<uintptr_t>(i_memory::reeq().base + 0x1F05208);
		i_dupa::skid().i_viewport = i_memory::reeq().Read<uintptr_t>(i_memory::reeq().base + 0x1F9E9F0);
		i_dupa::skid().i_camera = 0x1FD8570;
		i_dupa::skid().i_veh = 0x0D30;
		i_dupa::skid().i_handling = 0x938;
		i_dupa::skid().i_enginehealth = 0x908;
		i_dupa::skid().i_localplayer = i_memory::reeq().Read<uintptr_t>(i_world + 0x8);
		playerinfo = 0x10C8;
		i_dupa::skid().logs();
		i_overlay::here().initialize();
	}
	else if (i_memory::reeq().find_build(L"FiveM_b2612_GTAProcess.exe"))
	{
		i_dupa::skid().isbuild2802 = false;
		i_memory::reeq().base = i_memory::reeq().get_base_address("FiveM_b2612_GTAProcess.exe");
		i_dupa::skid().i_world = i_memory::reeq().Read<uintptr_t>(i_memory::reeq().base + 0x2567DB0);
		i_dupa::skid().i_replay = i_memory::reeq().Read<uintptr_t>(i_memory::reeq().base + 0x1F77EF0);
		i_dupa::skid().i_viewport = i_memory::reeq().Read<uintptr_t>(i_memory::reeq().base + 0x1FD8570);
		i_dupa::skid().i_camera = i_memory::reeq().Read<DWORD64>(i_memory::reeq().base + 0x20D9868);
		i_dupa::skid().i_veh = 0x0D30;
		i_dupa::skid().i_handling = 0x938;
		i_dupa::skid().i_enginehealth = 0x908;
		i_dupa::skid().i_localplayer = i_memory::reeq().Read<uintptr_t>(i_world + 0x8);
		playerinfo = 0x10C8;
		i_dupa::skid().logs();
		i_overlay::here().initialize();
	}
	else if (i_memory::reeq().find_build(L"FiveM_b2189_GTAProcess.exe"))
	{
		i_dupa::skid().isbuild2802 = false;
		i_memory::reeq().base = i_memory::reeq().get_base_address("FiveM_b2189_GTAProcess.exe");
		i_dupa::skid().i_world = i_memory::reeq().Read<uintptr_t>(i_memory::reeq().base + 0x24E6D90);
		i_dupa::skid().i_replay = i_memory::reeq().Read<uintptr_t>(i_memory::reeq().base + 0x1EE18A8);
		i_dupa::skid().i_viewport = i_memory::reeq().Read<uintptr_t>(i_memory::reeq().base + 0x1F888C0);
		i_dupa::skid().i_camera = i_memory::reeq().Read<DWORD64>(i_memory::reeq().base + 0x0);
		i_dupa::skid().i_veh = 0x0D30;
		i_dupa::skid().i_handling = 0x938;
		i_dupa::skid().i_enginehealth = 0x908;
		i_dupa::skid().i_localplayer = i_memory::reeq().Read<uintptr_t>(i_world + 0x8);
		playerinfo = 0x10C8;
		i_dupa::skid().logs();
		i_overlay::here().initialize();
	}
	else if (i_memory::reeq().find_build(L"FiveM_b2802_GTAProcess.exe"))
	{
		i_dupa::skid().isbuild2802 = true;
		i_memory::reeq().base = i_memory::reeq().get_base_address("FiveM_b2802_GTAProcess.exe");
		i_dupa::skid().i_world = i_memory::reeq().Read<uintptr_t>(i_memory::reeq().base + 0x254D448);
		i_dupa::skid().i_replay = i_memory::reeq().Read<uintptr_t>(i_memory::reeq().base + 0x1F5B820);
		i_dupa::skid().i_viewport = i_memory::reeq().Read<uintptr_t>(i_memory::reeq().base + 0x1FBC100);
		i_dupa::skid().i_camera = i_memory::reeq().Read<DWORD64>(i_memory::reeq().base + 0x0);
		i_dupa::skid().i_veh = 0x0D10;
		i_dupa::skid().i_handling = 0x918;
		i_dupa::skid().i_enginehealth = 0x8E8;
		i_dupa::skid().i_localplayer = i_memory::reeq().Read<uintptr_t>(i_world + 0x8);
		playerinfo = 0x10A8;
		i_dupa::skid().logs();
		i_overlay::here().initialize();
	}



	
}
