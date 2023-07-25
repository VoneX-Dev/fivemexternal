#include "aimbot.h"
#include <thread>
#include <chrono>
#include "../sdk/vectorh.h"
#include <iostream>
#include "../Hotkey/hotkey.h"
i_aim& i_aim::aim()
{
	static i_aim ins;
	return ins;
}

uintptr_t i_aim::BestTarget()
{
	uintptr_t ClosestPlayer = 0;
	uintptr_t ReplayInterface = i_dupa::skid().i_replay;
	uintptr_t localplayer = i_dupa::skid().i_localplayer;
	float crosshair_dist;
	float closestdist = FLT_MAX;
	float dist;
	if (ReplayInterface)
	{
		uintptr_t PedReplayInterface = i_memory::reeq().Read<DWORD64>(ReplayInterface + 0x18);
		uintptr_t PedList = i_memory::reeq().Read<DWORD64>(PedReplayInterface + 0x100);
		int entitylist = i_memory::reeq().Read<int>(PedReplayInterface + 0x108);

		for (int i = 0U; i < entitylist; i++)
		{
			if (!PedList) continue;

			uintptr_t Ped = i_memory::reeq().Read<uintptr_t>(PedList + (i * 0x10));
			if (!Ped) continue;
			uint64_t playerinfos = i_memory::reeq().Read<uint64_t>(Ped + i_dupa::skid().playerinfo);
			int playeridaddr = i_memory::reeq().Read<int>(playerinfos + 0x88);
			std::string playeridstr = std::to_string(playeridaddr);
			auto it = std::find(i_esp::esp().friends.begin(), i_esp::esp().friends.end(), playeridstr);
			if (!targetfriend)
			{
				if (it != i_esp::esp().friends.end())
					continue; 
			}
			if (Ped == localplayer) continue;

			if (ignoreped)
			{
				if (!i_memory::reeq().Read<uintptr_t>(Ped + i_dupa::skid().playerinfo))
					continue;
			}


			float HealthPed = i_memory::reeq().Read<float>(Ped + 0x280);

			if (ignoredeath)
			{
				if (HealthPed == 0)
					continue;
			}

			if (ignoredown)
			{
				if (HealthPed == 150)
					continue;
			}

			D3DXVECTOR3 GetCordLocal= i_memory::reeq().Read<D3DXVECTOR3>(localplayer + 0x90);
			D3DXVECTOR3 GetCordPed = i_memory::reeq().Read<D3DXVECTOR3>(Ped + 0x90);
			D3DXVECTOR3 DistanceCalculation = (GetCordLocal - GetCordPed);
			double Distance = sqrtf(DistanceCalculation.x * DistanceCalculation.x + DistanceCalculation.y * DistanceCalculation.y + DistanceCalculation.z * DistanceCalculation.z);
			auto iop = i_sdk::sdk().get_bone_position(Ped, 0);
			D3DXVECTOR2 Head = i_sdk::sdk().world_to_screen(iop);
			float best_dist = FLT_MAX;
			float x = Head.x - GetSystemMetrics(SM_CXSCREEN) / 2.f;
			float y = Head.y - GetSystemMetrics(SM_CYSCREEN) / 2.f;
			crosshair_dist = sqrtf((x * x) + (y * y));


			if (Distance <= distance)
			{
				if (crosshair_dist < closestdist) {


					closestdist = crosshair_dist;
					ClosestPlayer = Ped;
				}
			}
		}
		return ClosestPlayer;
	}
}

void i_aim::aimbot(uintptr_t Ped)
{
	auto get_distance = [](double x1, double y1, double x2, double y2) {
		return sqrtf(pow(x2 - x1, 2.0) + pow(y2 - y1, 2.0));
	};
	int bones;
	if (boneid == 0)
	{
		bones = 0;
	}
	if (boneid == 1)
	{
		bones = 1;
	}

	auto bone_pos = i_sdk::sdk().get_bone_position(Ped, bones);
	D3DXVECTOR2 screen = i_sdk::sdk().world_to_screen(bone_pos);

	if (screen == D3DXVECTOR2(0, 0))
		return;


	auto center_x = GetSystemMetrics(SM_CXSCREEN) / 2;
	auto center_y = GetSystemMetrics(SM_CYSCREEN) / 2;
	auto fov1 = get_distance(center_x, center_y, screen.x, screen.y);

	float TargetX = 0;
	float TargetY = 0;

	if (screen.x != 0) {
		if (screen.x > center_x) {
			TargetX = -(center_x - screen.x);
			TargetX /= smooth - 0.5;
			if (TargetX + center_x > center_x * 2) TargetX = 0;
		}

		if (screen.x < center_x) {
			TargetX = screen.x - center_x;
			TargetX /= smooth - 0.5;
			if (TargetX + center_x < 0) TargetX = 0;
		}
	}

	if (screen.y != 0) {
		if (screen.y > center_y) {
			TargetY = -(center_y - screen.y);
			TargetY /= smooth - 0.5;
			if (TargetY + center_y > center_y * 2) TargetY = 0;
		}

		if (screen.y < center_y) {
			TargetY = screen.y - center_y;
			TargetY /= smooth - 0.5;
			if (TargetY + center_y < 0) TargetY = 0;
		}
	}

	if (fov1 < fov)
	{
		if ((GetAsyncKeyState)(Hotkey::hot().Aimbotkey) & 0x8000) {

			mouse_event(MOUSEEVENTF_MOVE, float(TargetX), float(TargetY), 0, 0);


		}
	}
}

void i_aim::MemoryAim(D3DXVECTOR3 point, uintptr_t Ped)
{
	DWORD64 addr = i_memory::reeq().Read<DWORD64>(i_memory::reeq().base + i_dupa::skid().i_camera);
	if (addr)
	{
		Vec3 view_angles = i_memory::reeq().Read<Vec3>(addr + 0x40);
		D3DXVECTOR3 camera_pos = i_memory::reeq().Read<D3DXVECTOR3>(addr + 0x60);
		D3DXVECTOR3 target_bone = point;
		Vec3 caca(camera_pos.x - target_bone.x, camera_pos.y - target_bone.y, camera_pos.z - target_bone.z);
		float  distance = caca.Length();

		Vec3 final_angle = Vec3();
		final_angle = calc_angle(camera_pos, target_bone, distance);
		final_angle.Normalize();
		Vec3 delta = final_angle - view_angles;
		delta.Normalize();
		Vec3 smoothed_angle = view_angles + (smooth ? delta / smooth : delta);
		smoothed_angle.Normalize();

		
		
		i_memory::reeq().Write<Vec3>(addr + 0x40, smoothed_angle);
		
	}

}

void i_aim::HookMemoryAim()
{
	auto get_distance = [](double x1, double y1, double x2, double y2) {
		return sqrtf(pow(x2 - x1, 2.0) + pow(y2 - y1, 2.0));
	};

	auto center_x = GetSystemMetrics(SM_CXSCREEN) / 2;
	auto center_y = GetSystemMetrics(SM_CYSCREEN) / 2;
	int bones2;
	if (boneid == 0)
	{
		bones2 = 0;
	}
	if (boneid == 1)
	{
		bones2 = 1;
	}

	uintptr_t entity = BestTarget();

	auto bone_pos = i_sdk::sdk().get_bone_position(entity, bones2);

	D3DXVECTOR2 screen = i_sdk::sdk().world_to_screen(bone_pos);

	auto fov2 = get_distance(center_x, center_y, screen.x, screen.y);
	if ((GetAsyncKeyState)(Hotkey::hot().Aimbotkey) & 0x8000) {
		if (fov2 < fov)
		{

			MemoryAim(bone_pos, entity);
		}
	}
}

void i_aim::start()
{
	if (i_dupa::skid().i_localplayer)
	{
		if (aimenable)
		{
			if (aimbottype == 0)
			{
				if (i_aim::aim().BestTarget())
				{
					aimbot(BestTarget());
				}
			}
			if (aimbottype == 1)
			{
				if (i_aim::aim().BestTarget())
				{
					HookMemoryAim();
				}
			}
		}
	}
}