#pragma once
#include <Windows.h>
#include <string>

#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#include "fonts.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include "../sdk/sdk.h"
#include "../aimbot/aimbot.h"
#include "../esp/esp.h"
#include "../exploits/exploits.h"
#include "../memory/skStr.h"
#include "col.h"
#include "image.h"
#include <D3dx9tex.h>
#pragma comment(lib, "D3dx9")
class i_overlay {
public:
    static i_overlay& here();


	static std::int64_t __stdcall wndproc(HWND hwnd, unsigned int message, WPARAM wparam, LPARAM lparam);
	IDirect3D9Ex* p_Object = NULL;
	IDirect3DDevice9Ex* p_Device = NULL;
	D3DPRESENT_PARAMETERS p_Params = { NULL };
	HWND MyWnd = NULL;
	HWND GameWnd = NULL;
	MSG Message = { NULL };
	RECT GameRect = { NULL };
	D3DPRESENT_PARAMETERS d3dpp;
	ULONG Width = GetSystemMetrics(SM_CXSCREEN);
	ULONG Height = GetSystemMetrics(SM_CYSCREEN);
	DWORD ScreenCenterX;
	DWORD ScreenCenterY;
	IDirect3DTexture9* unknowncheatslogo = nullptr;
	ImFont* icons;
	ImFont* iconfont;
	ImFont* main_font;
	ImFont* main_font2;
	ImFont* global3;
	ImFont* LexendRegular;
	ImFont* ico_combo2;
	bool ret = false;
	int my_image_width = 0;
	int my_image_height = 0;
	int menuhot = 116;
	ImFont* global34;
	int selected_tab = 0;
	void CustomStyleColor();
	PDIRECT3DTEXTURE9 my_texture = NULL;
    void initialize();
	bool LoadTextureFromFile(const char* filename, PDIRECT3DTEXTURE9* out_texture, int* out_width, int* out_height);
    HRESULT init_wndparams(HWND hwnd);

    void cleanup_d3d();

    void setup_window();

	void style();

	void menu();

	void signal();

	void render();

	void DrawString(float fontSize, int x, int y, bool bCenter, bool stroke, const char* pText, ...);

	void DrawLine(const ImVec2& x, const ImVec2 y, ImU32 color, const FLOAT width);

	void DrawShieldbar(int x, int y, int w, int h, int thick, int m_health);

	void DrawLeftProgressBar(int x, int y, int w, int h, int thick, int m_health);

	static void DrawRectImGui(int x, int y, int w, int h, ImVec4 color, float thickness);

	void DrawBox(float X, float Y, float W, float H, const ImU32& color, int trounding, int thickness);

	void DrawHealthBar(ImVec2 pos, ImVec2 dim, ImColor col);
	

	void DrawHealthBarhorizontal(ImVec2 pos, ImVec2 pos2, ImVec2 dim, ImColor col);

	void DrawOutlinedText(ImFont* pFont, const std::string& text, const ImVec2& pos, float size, ImU32 color, bool center);
	void DrawFilledRect(int x, int y, int w, int h, RGBA* color);
	void RectFilled(float x, float y, float x1, float y1, ImColor color, float rounding, int rounding_corners_flags);

	void HotkeyButton(int aimkey, void* changekey, int status);

	bool showmenu = true;
};