#include "overlay.h"
#include <sstream>
#include "million.h"
#include "../Hotkey/hotkey.h"
i_overlay& i_overlay::here()
{
	static i_overlay ins;
	return ins;
}

bool i_overlay::LoadTextureFromFile(const char* filename, PDIRECT3DTEXTURE9* out_texture, int* out_width, int* out_height)
{
	// Load texture from disk
	PDIRECT3DTEXTURE9 texture;
	HRESULT hr = D3DXCreateTextureFromFileA(p_Device, filename, &texture);
	if (hr != S_OK)
		return false;

	// Retrieve description of the texture surface so we can access its size
	D3DSURFACE_DESC my_image_desc;
	texture->GetLevelDesc(0, &my_image_desc);
	*out_texture = texture;
	*out_width = (int)my_image_desc.Width;
	*out_height = (int)my_image_desc.Height;
	return true;
}

std::int64_t __stdcall i_overlay::wndproc(HWND hwnd, unsigned int message, WPARAM wparam, LPARAM lparam)
{
	
	std::int64_t ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	ImGui_ImplWin32_WndProcHandler(hwnd, message, wparam, lparam);
	if (i_overlay::here().showmenu)
		return 1;
	
	return DefWindowProcA(i_overlay::here().MyWnd,message, wparam, lparam);
}


HRESULT i_overlay::init_wndparams(HWND hwnd)
{
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &p_Object)))
		exit(3);

	ZeroMemory(&p_Params, sizeof(p_Params));
	p_Params.Windowed = TRUE;
	p_Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	p_Params.hDeviceWindow = hwnd;
	p_Params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	p_Params.BackBufferFormat = D3DFMT_A8R8G8B8;
	p_Params.BackBufferWidth = Width;
	p_Params.BackBufferHeight = Height;
	p_Params.EnableAutoDepthStencil = TRUE;
	p_Params.AutoDepthStencilFormat = D3DFMT_D16;
	p_Params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (FAILED(p_Object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &p_Params, 0, &p_Device))) {
		p_Object->Release();
		exit(4);
	}
	
	ImGui::CreateContext();
	
	ret = LoadTextureFromFile("unknown2.png", &my_texture, &my_image_width, &my_image_height);
	IM_ASSERT(ret);
	//D3DXCreateTextureFromFileInMemoryEx(p_Device, &skid, sizeof(skid), 407, 695, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &unknowncheatslogo);
	//ImGui::GetIO().Fonts->AddFontDefault();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.IniFilename = NULL;
	ImFontConfig font_config;
	font_config.OversampleH = 1; //or 2 is the same
	font_config.OversampleV = 1;
	font_config.PixelSnapH = 1;
	static const ImWchar ranges[] =
	{
		0x0020, 0x00FF, // Basic Latin + Latin Supplement
		0x0400, 0x044F, // Cyrillic
		0,
	};

	char windows_directory[MAX_PATH];
	
	std::string segoeui_dir = (std::string)windows_directory + "\\Fonts\\segoeui.ttf";
	
	global34 = io.Fonts->AddFontFromMemoryTTF(segoui, sizeof(segoui), 18, NULL, io.Fonts->GetGlyphRangesCyrillic());
	//ico_combo2 = io.Fonts->AddFontFromMemoryTTF(&icon, sizeof icon, 19, NULL, io.Fonts->GetGlyphRangesCyrillic());
	
	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	global3 = io.Fonts->AddFontFromMemoryTTF(segoui, sizeof(segoui), 21, NULL, io.Fonts->GetGlyphRangesCyrillic());

	//io.Fonts->AddFontFromMemoryCompressedBase85TTF(RobotoLight_compressed_data_base85, 18.0F);


	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX9_Init(p_Device);
	return S_OK;
}

void i_overlay::cleanup_d3d()
{
	if (p_Device != NULL) {
		p_Device->EndScene();
		p_Device->Release();
	}
	if (p_Object != NULL) {
		p_Object->Release();
	}
}

void set_window_target()
{
	while (true) {
		i_overlay::here().GameWnd = i_memory::reeq().get_process_wnd(i_memory::reeq().pid);
		if (i_overlay::here().GameWnd) {
			ZeroMemory(&i_overlay::here().GameRect, sizeof(i_overlay::here().GameRect));
			GetWindowRect(i_overlay::here().GameWnd, &i_overlay::here().GameRect);
			DWORD dwStyle = GetWindowLong(i_overlay::here().GameWnd, GWL_STYLE);
			if (dwStyle & WS_BORDER)
			{
				i_overlay::here().GameRect.top += 32;
				i_overlay::here().Height -= 39;
			}
			i_overlay::here().ScreenCenterX = i_overlay::here().Width / 2;
			i_overlay::here().ScreenCenterY = i_overlay::here().Height / 2;
			MoveWindow(i_overlay::here().MyWnd, i_overlay::here().GameRect.left, i_overlay::here().GameRect.top, i_overlay::here().Width, i_overlay::here().Height, true);
		}
	}
}
char Licence[50] = "";
void i_overlay::setup_window()
{
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)set_window_target, 0, 0, 0);
	WNDCLASSEXA wcex = {
		sizeof(WNDCLASSEXA),
		0,
		wndproc,
		0,
		0,
		nullptr,
		LoadIcon(nullptr, IDI_APPLICATION),
		LoadCursor(nullptr, IDC_ARROW),
		nullptr,
		nullptr,
		("Chrome"),
		LoadIcon(nullptr, IDI_APPLICATION)
	};

	RECT Rect;
	GetWindowRect(GetDesktopWindow(), &Rect);

	RegisterClassExA(&wcex);

	MyWnd = CreateWindowExA(NULL, ("Chrome"), ("Chrome"), WS_POPUP, Rect.left, Rect.top, Rect.right, Rect.bottom, NULL, NULL, wcex.hInstance, NULL);
	SetWindowLong(MyWnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW);
	SetLayeredWindowAttributes(MyWnd, RGB(0, 0, 0), 255, LWA_ALPHA);

	MARGINS margin = { -1 };
	DwmExtendFrameIntoClientArea(MyWnd, &margin);

	ShowWindow(MyWnd, SW_SHOW);
	UpdateWindow(MyWnd);
}

void i_overlay::style()
{
	ImGuiStyle* Style = &ImGui::GetStyle();
	ImVec4* Colors = Style->Colors;


	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();
	Style->WindowRounding = 4.f;
	Style->WindowBorderSize = 0;
	Style->Colors[ImGuiCol_WindowBg] = ImColor(23, 23, 23);
	Style->Colors[ImGuiCol_Border] = ImColor(23,23,23);
	Style->Colors[ImGuiCol_Button] = ImColor(15, 15, 15);
	Style->Colors[ImGuiCol_ButtonActive] = ImColor(23, 23, 23);
	Style->Colors[ImGuiCol_ButtonHovered] = ImColor(18, 18, 18);
}

void i_overlay::CustomStyleColor()
{
	ImGuiStyle& s = ImGui::GetStyle();


	s.WindowBorderSize = 0;
	
	s.PopupBorderSize = 0;
	s.FrameBorderSize = 1.0f;
	s.FrameRounding = 4.f;
	
}
int slider_int = 0;
int slider_int2 = 0;
int slider_int3s = 0;
bool test = false;
bool test2 = false;
bool test3 = false;
bool test4 = false;
const char* items[3] = { "head","chest", "legs" };

const char* const KeyNames[] = {
	"VK_LBUTTON",
	"VK_RBUTTON",
	"VK_CANCEL",
	"VK_MBUTTON",
	"VK_XBUTTON1",
	"VK_XBUTTON2",
	"VK_BACK",
	"VK_TAB",
	"VK_CLEAR",
	"VK_RETURN",
	"VK_SHIFT",
	"VK_CONTROL",
	"VK_MENU",
	"VK_PAUSE",
	"VK_CAPITAL",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
};

void i_overlay::menu()
{
	
	ImDrawList* draw;
	
	ImGui::SetNextWindowSize({ 500,365 });
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	CustomStyleColor();
	ImGui::Begin("designvx", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
	{
		ImVec2 pos;
		auto p = ImGui::GetWindowPos();
		pos = ImGui::GetWindowPos();
		draw = ImGui::GetWindowDrawList();

		draw->AddRectFilled(ImVec2(p.x + 0, p.y + 35), ImVec2(p.x + 500, p.y + 330), ImColor(16, 16, 16), 4);
		draw->AddLine(ImVec2(p.x, p.y + 35), ImVec2(p.x + 500, p.y + 35), ImColor(33, 33, 33), 0);
		draw->AddLine(ImVec2(p.x, p.y + 330), ImVec2(p.x + 500, p.y + 330), ImColor(33, 33, 33), 0);
		draw->AddText(global3, 21, ImVec2(p.x + 10, p.y + 5), ImColor(157, 176, 242, int(255)), "Cheat-Shop.com");
		draw->AddText(global3, 21, ImVec2(p.x + 10, p.y + 335), ImColor(157, 176, 242, int(255)), "Fivem");
		draw->AddText(global3, 21, ImVec2(p.x + 445, p.y + 335), ImColor(157, 176, 242, int(255)), "User");

		ImGui::PushFont(global34);
		ImGui::SetCursorPos(ImVec2(270, 0));
		ImGui::BeginGroup();
		if (figma_frame::tab("Aim", 0 == selected_tab)) selected_tab = 0;
		ImGui::SameLine();
		if (figma_frame::tab("Visuals", 1 == selected_tab)) selected_tab = 1;
		ImGui::SameLine();
		if (figma_frame::tab("Misc", 2 == selected_tab)) selected_tab = 2;
		ImGui::SameLine();
		if (figma_frame::tab("Config", 3 == selected_tab)) selected_tab = 3;
		ImGui::EndGroup();
		ImGui::PopFont();

		if (selected_tab == 0)
		{
			ImGui::PushFont(global34);
			ImGui::SetCursorPos({ 20, 70 });
			ImGui::MenuChild(("General"), ImVec2(225, 245));
			{
				ImGui::SetCursorPosY(10);
				ImGui::Checkbox("Enable", &i_aim::aim().aimenable);
				ImGui::Checkbox("Draw Fov", &i_aim::aim().showfov);
				ImGui::Checkbox("Ignore Ped", &i_aim::aim().ignoreped);
				ImGui::Checkbox("Ignore Death", &i_aim::aim().ignoredeath);

			}
			ImGui::EndChild();
			ImGui::PopFont();
			ImGui::PushFont(global34);
			ImGui::SetCursorPos({ 260, 70 });
			ImGui::MenuChild(("Other"), ImVec2(225, 245));
			{
				ImGui::SetCursorPos({ 5,10 });
				ImGui::SliderInt(("Fov"), &i_aim::aim().fov, 0.0f, 300);

				ImGui::SetCursorPos({ 5,45 });
				ImGui::SliderInt(("Smooth"), &i_aim::aim().smooth, 0, 50);

				ImGui::SetCursorPos({ 5,85 });
				ImGui::SliderInt(("Distance"), &i_aim::aim().distance, 0.0f, 1000);

				const char* AimType[]{ "Mouse","Memory" };
				ImGui::SetCursorPos({ 16,135 });
				ImGui::PushItemWidth(175);
				ImGui::Combo(((("Aim Type"))), &i_aim::aim().aimbottype, AimType, IM_ARRAYSIZE(AimType), ImGuiComboFlags_NoArrowButton);
			}
			ImGui::EndChild();
			ImGui::PopFont();
		}
		if (selected_tab == 1)
		{
			ImGui::PushFont(global34);
			ImGui::SetCursorPos({ 20, 70 });
			ImGui::MenuChild(("ESP"), ImVec2(225, 245));
			{
				ImGui::SetCursorPosY(10);
				ImGui::Checkbox("Box", &i_esp::esp().box);
				ImGui::Checkbox("Corner Box", &i_esp::esp().cornered);
				ImGui::Checkbox("Health Bar", &i_esp::esp().heathbar);
				ImGui::Checkbox("Armor Bar", &i_esp::esp().armorbar);
				ImGui::Checkbox("Distance", &i_esp::esp().distance);
				ImGui::Checkbox("Skeleton", &i_esp::esp().skeleton);
				ImGui::Checkbox("Ignore Ped", &i_esp::esp().ignoreped);
				ImGui::Checkbox("Ignore Death", &i_esp::esp().ignoredead);

			}
			ImGui::EndChild();
			ImGui::PopFont();
			ImGui::PushFont(global34);
			ImGui::SetCursorPos({ 260, 70 });
			ImGui::MenuChild(("Settings"), ImVec2(225, 245));
			{
				ImGui::SetCursorPos({ 5,10 });
				ImGui::SliderInt(("Distance"), &i_esp::esp().distanceint, 0.0f, 500);

				const char* HealthbarStyle[]{ "Right","Left","Up" };
				ImGui::SetCursorPos({ 15,170 });
				ImGui::Combo(((("Health Bar Pos"))), &i_esp::esp().HealthBar_type, HealthbarStyle, IM_ARRAYSIZE(HealthbarStyle), ImGuiComboFlags_NoArrowButton);
			}
			ImGui::EndChild();
			ImGui::PopFont();
		}
		if (selected_tab == 2)
		{
			ImGui::PushFont(global34);
			ImGui::SetCursorPos({ 20, 70 });
			ImGui::MenuChild(("Self"), ImVec2(225, 245));
			{
				ImGui::SetCursorPosY(10);
			
				
			
				ImGui::Checkbox("Infinite Stamina", &i_exploits::exp().infinitestamina);
				
				ImGui::Checkbox("Fov Changer", &i_exploits::exp().customfov);
				if (i_exploits::exp().customfov)
				{
					
					ImGui::SliderInt(("Fov"), &i_exploits::exp().fovvalue, 0.0f, 150);
				}

			}
			ImGui::EndChild();
			ImGui::PopFont();
			ImGui::PushFont(global34);
			ImGui::SetCursorPos({ 260, 70 });
			ImGui::MenuChild(("Weapon"), ImVec2(225, 245));
			{
				ImGui::SetCursorPosY(10);
				ImGui::Checkbox("No Recoil", &i_exploits::exp().norecoil);
			
				ImGui::Checkbox("No Reload", &i_exploits::exp().noreload);
				
			}
			ImGui::EndChild();
			ImGui::PopFont();
		}
		if (selected_tab == 3)
		{
			ImGui::PushFont(global34);
			ImGui::SetCursorPos({ 20, 70 });
			ImGui::MenuChild(("Config"), ImVec2(225, 245));
			{
				
				ImGui::SetCursorPos({ 15,10 });
				ImGui::PushItemWidth(190);
				ImGui::InputText("##cfgname", Licence, 155);
				ImGui::Text("                          ");
				ImGui::SetCursorPosX(15);

				if (ImGui::Button(("Save Config"), ImVec2(190, 25)))
				{
					FILE* p_stream4;

					// open file for (extended) writing
					fopen_s(&p_stream4, Licence, "w+");

					// set file pointer to 0
					fseek(p_stream4, 0, SEEK_SET);

					fwrite(&i_aim::aim().aimenable, sizeof(bool), 1, p_stream4);
					fwrite(&i_aim::aim().ignoredeath, sizeof(bool), 1, p_stream4);
					fwrite(&i_aim::aim().ignoreped, sizeof(bool), 1, p_stream4);
					fwrite(&i_aim::aim().showfov, sizeof(bool), 1, p_stream4);
					fwrite(&i_aim::aim().aimbottype, sizeof(bool), 1, p_stream4);
					fwrite(&i_aim::aim().distance, sizeof(bool), 1, p_stream4);
					fwrite(&i_aim::aim().fov, sizeof(bool), 1, p_stream4);
					fwrite(&i_aim::aim().smooth, sizeof(bool), 1, p_stream4);

					fwrite(&i_esp::esp().armorbar, sizeof(bool), 1, p_stream4);
					fwrite(&i_esp::esp().box, sizeof(bool), 1, p_stream4);
					fwrite(&i_esp::esp().cornered, sizeof(bool), 1, p_stream4);
					fwrite(&i_esp::esp().distance, sizeof(bool), 1, p_stream4);
					fwrite(&i_esp::esp().distanceint, sizeof(bool), 1, p_stream4);
					fwrite(&i_esp::esp().HealthBar_type, sizeof(bool), 1, p_stream4);
					fwrite(&i_esp::esp().heathbar, sizeof(bool), 1, p_stream4);
					fwrite(&i_esp::esp().weapon, sizeof(bool), 1, p_stream4);
					fwrite(&i_esp::esp().skeleton, sizeof(bool), 1, p_stream4);
					fwrite(&i_esp::esp().ignoreped, sizeof(bool), 1, p_stream4);
					fwrite(&i_esp::esp().ignoredead, sizeof(bool), 1, p_stream4);

					fwrite(&i_exploits::exp().boostvehicle, sizeof(bool), 1, p_stream4);
					fwrite(&i_exploits::exp().brakeforce, sizeof(bool), 1, p_stream4);
					fwrite(&i_exploits::exp().brakevalue, sizeof(bool), 1, p_stream4);
					fwrite(&i_exploits::exp().customammo, sizeof(bool), 1, p_stream4);
					fwrite(&i_exploits::exp().customfov, sizeof(bool), 1, p_stream4);
					fwrite(&i_exploits::exp().customspeed, sizeof(bool), 1, p_stream4);
					fwrite(&i_exploits::exp().damageamount, sizeof(bool), 1, p_stream4);
					fwrite(&i_exploits::exp().damageboos, sizeof(bool), 1, p_stream4);
					fwrite(&i_exploits::exp().explosiveammo, sizeof(bool), 1, p_stream4);
					fwrite(&i_exploits::exp().fovvalue, sizeof(bool), 1, p_stream4);
					fwrite(&i_exploits::exp().godmode, sizeof(bool), 1, p_stream4);
					fwrite(&i_exploits::exp().infiniteammo, sizeof(bool), 1, p_stream4);
					fwrite(&i_exploits::exp().infinitearmor, sizeof(bool), 1, p_stream4);
					fwrite(&i_exploits::exp().infinitestamina, sizeof(bool), 1, p_stream4);
					fwrite(&i_exploits::exp().invisible, sizeof(bool), 1, p_stream4);
					fwrite(&i_exploits::exp().norecoil, sizeof(bool), 1, p_stream4);
					fwrite(&i_exploits::exp().noreload, sizeof(bool), 1, p_stream4);
					fwrite(&i_exploits::exp().nospread, sizeof(bool), 1, p_stream4);
					fwrite(&i_exploits::exp().swimspeed2, sizeof(bool), 1, p_stream4);
					fwrite(&i_exploits::exp().testbool, sizeof(bool), 1, p_stream4);
					fwrite(&i_exploits::exp().rapidfire, sizeof(bool), 1, p_stream4);
					fwrite(&i_exploits::exp().playerspeed, sizeof(bool), 1, p_stream4);

					fclose(p_stream4);
				}
				ImGui::Text("                          ");

				ImGui::SetCursorPosX(15);

				if (ImGui::Button(("Load Config"), ImVec2(190, 25)))
				{
					FILE* p_stream4;

					// open file for (extended) reading
					fopen_s(&p_stream4, Licence, "r+");

					// set file pointer to 0
					fseek(p_stream4, 0, SEEK_SET);

					// read a value... 
					fread_s(&i_aim::aim().aimenable, sizeof(bool), sizeof(bool), 1, p_stream4);

					fread_s(&i_aim::aim().ignoredeath, sizeof(bool), sizeof(bool), 1, p_stream4);
					fread_s(&i_aim::aim().ignoreped, sizeof(bool), sizeof(bool), 1, p_stream4);
					fread_s(&i_aim::aim().showfov, sizeof(bool), sizeof(bool), 1, p_stream4);
					fread_s(&i_aim::aim().aimbottype, sizeof(bool), sizeof(bool), 1, p_stream4);
					fread_s(&i_aim::aim().distance, sizeof(bool), sizeof(bool), 1, p_stream4);
					fread_s(&i_aim::aim().fov, sizeof(bool), sizeof(bool), 1, p_stream4);
					fread_s(&i_aim::aim().smooth, sizeof(bool), sizeof(bool), 1, p_stream4);

					fread_s(&i_esp::esp().armorbar, sizeof(bool), sizeof(bool), 1, p_stream4);
					fread_s(&i_esp::esp().box, sizeof(bool), sizeof(bool), 1, p_stream4);
					fread_s(&i_esp::esp().cornered, sizeof(bool), sizeof(bool), 1, p_stream4);
					fread_s(&i_esp::esp().distance, sizeof(bool), sizeof(bool), 1, p_stream4);
					fread_s(&i_esp::esp().distanceint, sizeof(bool), sizeof(bool), 1, p_stream4);
					fread_s(&i_esp::esp().HealthBar_type, sizeof(bool), sizeof(bool), 1, p_stream4);
					fread_s(&i_esp::esp().heathbar, sizeof(bool), sizeof(bool), 1, p_stream4);
					fread_s(&i_esp::esp().weapon, sizeof(bool), sizeof(bool), 1, p_stream4);
					fread_s(&i_esp::esp().skeleton, sizeof(bool), sizeof(bool), 1, p_stream4);
					fread_s(&i_esp::esp().ignoreped, sizeof(bool), sizeof(bool), 1, p_stream4);
					fread_s(&i_esp::esp().ignoredead, sizeof(bool), sizeof(bool), 1, p_stream4);


					fread_s(&i_exploits::exp().boostvehicle, sizeof(bool), sizeof(bool), 1, p_stream4);
					fread_s(&i_exploits::exp().brakeforce, sizeof(bool), sizeof(bool), 1, p_stream4);
					fread_s(&i_exploits::exp().brakevalue, sizeof(bool), sizeof(bool), 1, p_stream4);
					fread_s(&i_exploits::exp().customammo, sizeof(bool), sizeof(bool), 1, p_stream4);
					fread_s(&i_exploits::exp().customfov, sizeof(bool), sizeof(bool), 1, p_stream4);
					fread_s(&i_exploits::exp().customspeed, sizeof(bool), sizeof(bool), 1, p_stream4);
					fread_s(&i_exploits::exp().damageamount, sizeof(bool), sizeof(bool), 1, p_stream4);
					fread_s(&i_exploits::exp().damageboos, sizeof(bool), sizeof(bool), 1, p_stream4);
					fread_s(&i_exploits::exp().explosiveammo, sizeof(bool), sizeof(bool), 1, p_stream4);
					fread_s(&i_exploits::exp().fovvalue, sizeof(bool), sizeof(bool), 1, p_stream4);
					fread_s(&i_exploits::exp().godmode, sizeof(bool), sizeof(bool), 1, p_stream4);
					fread_s(&i_exploits::exp().infiniteammo, sizeof(bool), sizeof(bool), 1, p_stream4);
					fread_s(&i_exploits::exp().infinitearmor, sizeof(bool), sizeof(bool), 1, p_stream4);
					fread_s(&i_exploits::exp().infinitestamina, sizeof(bool), sizeof(bool), 1, p_stream4);
					fread_s(&i_exploits::exp().invisible, sizeof(bool), sizeof(bool), 1, p_stream4);
					fread_s(&i_exploits::exp().norecoil, sizeof(bool), sizeof(bool), 1, p_stream4);
					fread_s(&i_exploits::exp().noreload, sizeof(bool), sizeof(bool), 1, p_stream4);
					fread_s(&i_exploits::exp().nospread, sizeof(bool), sizeof(bool), 1, p_stream4);
					fread_s(&i_exploits::exp().swimspeed2, sizeof(bool), sizeof(bool), 1, p_stream4);
					fread_s(&i_exploits::exp().testbool, sizeof(bool), sizeof(bool), 1, p_stream4);
					fread_s(&i_exploits::exp().rapidfire, sizeof(bool), sizeof(bool), 1, p_stream4);
					fread_s(&i_exploits::exp().playerspeed, sizeof(bool), sizeof(bool), 1, p_stream4);

					fclose(p_stream4);
				}

			}
			ImGui::EndChild();
			ImGui::PopFont();
			ImGui::PushFont(global34);
			ImGui::SetCursorPos({ 260, 70 });
			ImGui::MenuChild(("Binds"), ImVec2(225, 245));
			{
				ImGui::SetCursorPos({ 15,10 });
				ImGui::Combo(((("Hotkeys"))), &Hotkey::hot().key, KeyNames, IM_ARRAYSIZE(KeyNames), ImGuiComboFlags_NoArrowButton);
			}
			ImGui::EndChild();
			ImGui::PopFont();
		}
	}
	ImGui::End();
	ImGui::PopStyleVar();
}

void i_overlay::signal()
{
	ZeroMemory(&Message, sizeof(MSG));
	style();
	static RECT old_rc;
	while (Message.message != WM_QUIT)
	{
		if (PeekMessage(&Message, MyWnd, 0, 0, PM_REMOVE)) {
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}

		HWND hwnd_active = GetForegroundWindow();


		if (hwnd_active == GameWnd) {
			HWND hwndtest = GetWindow(hwnd_active, GW_HWNDPREV);
			SetWindowPos(MyWnd, hwndtest, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			
		}
		if (showmenu)
		{

		}
		RECT rc;
		POINT xy;

		ZeroMemory(&rc, sizeof(RECT));
		ZeroMemory(&xy, sizeof(POINT));
		GetClientRect(GameWnd, &rc);
		ClientToScreen(GameWnd, &xy);
		rc.left = xy.x;
		rc.top = xy.y;

		ImGuiIO& io = ImGui::GetIO();
		io.ImeWindowHandle = GameWnd;
		io.DeltaTime = 1.0f / 60.0f;

		POINT p;
		GetCursorPos(&p);
		io.MousePos.x = p.x - xy.x;
		io.MousePos.y = p.y - xy.y;
		if (GetAsyncKeyState(0x1)) {
			io.MouseDown[0] = true;
			io.MouseClicked[0] = true;
			io.MouseClickedPos[0].x = io.MousePos.x;
			io.MouseClickedPos[0].x = io.MousePos.y;
		}
		else io.MouseDown[0] = false;

		if (rc.left != old_rc.left || rc.right != old_rc.right || rc.top != old_rc.top || rc.bottom != old_rc.bottom) {

			old_rc = rc;

			Width = rc.right;
			Height = rc.bottom;

			p_Params.BackBufferWidth = Width;
			p_Params.BackBufferHeight = Height;
			SetWindowPos(MyWnd, (HWND)0, xy.x, xy.y, Width, Height, SWP_NOREDRAW);
			p_Device->Reset(&p_Params);
		}
		i_overlay::here().render();

		Sleep(10);
	}
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	i_overlay::here().cleanup_d3d();
	DestroyWindow(i_overlay::here().MyWnd);

	
}

std::wstring MBytesToWString(const char* lpcszString)
{
	int len = strlen(lpcszString);
	int unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, lpcszString, -1, NULL, 0);
	wchar_t* pUnicode = new wchar_t[unicodeLen + 1];
	memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
	::MultiByteToWideChar(CP_ACP, 0, lpcszString, -1, (LPWSTR)pUnicode, unicodeLen);
	std::wstring wString = (wchar_t*)pUnicode;
	delete[] pUnicode;
	return wString;
}

std::string WStringToUTF8(const wchar_t* lpwcszWString)
{
	char* pElementText;
	int iTextLen = ::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)lpwcszWString, -1, NULL, 0, NULL, NULL);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText, 0, (iTextLen + 1) * sizeof(char));
	::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)lpwcszWString, -1, pElementText, iTextLen, NULL, NULL);
	std::string strReturn(pElementText);
	delete[] pElementText;
	return strReturn;
}

void i_overlay::RectFilled(float x, float y, float x1, float y1, ImColor color, float rounding, int rounding_corners_flags)
{
	ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x1, y1), color, rounding, rounding_corners_flags);
}

void i_overlay::DrawHealthBarhorizontal(ImVec2 pos, ImVec2 pos2, ImVec2 dim, ImColor col)
{

	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(pos.x + dim.y, pos.y), ImVec2(pos2.x, pos.y), col, dim.x);

}

void i_overlay::DrawRectImGui(int x, int y, int w, int h, ImVec4 color, float thickness)
{
	ImGui::GetOverlayDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(color), 0, 0, thickness);
}

void i_overlay::DrawLeftProgressBar(int x, int y, int w, int h, int thick, int m_health)
{
	int G = (255 * m_health / 100);
	int R = 255 - G;
	RGBA healthcol = { R, G, 0, 255 };

	DrawFilledRect(x - (w / 2) - 3, y, thick, (h)*m_health / 100, &healthcol);
}

void i_overlay::DrawFilledRect(int x, int y, int w, int h, RGBA* color)
{
	ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 153.0, color->B / 51.0, color->A / 255.0)), 0, 0);
}
void i_overlay::DrawShieldbar(int x, int y, int w, int h, int thick, int m_health)
{
	int G = (255 * m_health / 100);
	int R = 255 - G;

	DrawFilledRect(x - (w / 2) - 3, y, thick, (h)*m_health / 100, &Col.darkblue);
}

void i_overlay::DrawHealthBar(ImVec2 pos, ImVec2 dim, ImColor col)
{


	ImGui::GetOverlayDrawList()->AddLine(pos, ImVec2(pos.x, pos.y - dim.y), col, dim.x);

}
void i_overlay::DrawLine(const ImVec2& x, const ImVec2 y, ImU32 color, const FLOAT width)
{
	ImGui::GetOverlayDrawList()->AddLine(x, y, color, width);
}

void i_overlay::DrawOutlinedText(ImFont* pFont, const std::string& text, const ImVec2& pos, float size, ImU32 color, bool center)
{
	std::stringstream stream(text);
	std::string line;

	float y = 0.0f;
	int index = 0;

	while (std::getline(stream, line))
	{
		ImVec2 textSize = pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());

		if (center)
		{

			ImGui::GetBackgroundDrawList()->AddText(pFont, size, ImVec2(pos.x - textSize.x / 2.0f, pos.y + textSize.y * index), ImGui::GetColorU32(color), line.c_str());
		}
		else
		{//

			ImGui::GetBackgroundDrawList()->AddText(pFont, size, ImVec2(pos.x, pos.y + textSize.y * index), ImGui::GetColorU32(color), line.c_str());
		}

		y = pos.y + textSize.y * (index + 1);
		index++;
	}
}

void i_overlay::DrawString(float fontSize, int x, int y, bool bCenter, bool stroke, const char* pText, ...)
{
	va_list va_alist;
	char buf[1024] = { 0 };
	va_start(va_alist, pText);
	_vsnprintf_s(buf, sizeof(buf), pText, va_alist);
	va_end(va_alist);
	std::string text = WStringToUTF8(MBytesToWString(buf).c_str());
	if (bCenter)
	{
		ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
		x = x - textSize.x / 4;
		y = y - textSize.y;
	}
	if (stroke)
	{
		ImGui::GetOverlayDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x + 1, y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
		ImGui::GetOverlayDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x - 1, y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
		ImGui::GetOverlayDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x + 1, y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
		ImGui::GetOverlayDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x - 1, y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
	}
	ImGui::GetOverlayDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x, y), ImColor(189, 175, 255), text.c_str());
}

void i_overlay::initialize()
{

	// some stuff xyzus
	setup_window();
	init_wndparams(MyWnd);
	Sleep(2000);
	signal();
}


