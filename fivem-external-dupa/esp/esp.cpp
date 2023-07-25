#include "esp.h"
#include <thread>
#include <chrono>
i_esp& i_esp::esp()
{
	static i_esp ins;
	return ins;
}

bool i_esp::IsOnScreen(D3DXVECTOR2 coords)
{
	if (coords.x < 0.1f || coords.y < 0.1 || coords.x > ImGui::GetIO().DisplaySize.x || coords.y > ImGui::GetIO().DisplaySize.y)
	{
		return false;
	}
	else {
		return true;
	}

}


void i_esp::draw_skeleton(uintptr_t ped_base) {
	int bone_positions[][2] = {
		{ 0, 7 },
		{ 7, 6 },
		{ 7, 5 },
		{ 7, 8 },
		{ 8, 3 },
		{ 8, 4 }
	};
	for (int i = 0; i < 6; ++i) {
		D3DXVECTOR2 screen_1, screen_2;
		auto bone_1 = i_sdk::sdk().get_bone_position(ped_base, bone_positions[i][0]),
			bone_2 = i_sdk::sdk().get_bone_position(ped_base, bone_positions[i][1]);
		if (bone_1 != D3DXVECTOR3(0, 0, 0) && bone_2 != D3DXVECTOR3(0, 0, 0) && ((screen_1 = i_sdk::sdk().world_to_screen(bone_1)) != D3DXVECTOR2(0, 0)) && ((screen_2 = i_sdk::sdk().world_to_screen(bone_2)) != D3DXVECTOR2(0, 0))) {
			ImGui::GetForegroundDrawList()->AddLine(ImVec2(screen_1.x, screen_1.y), ImVec2(screen_2.x, screen_2.y), skeletoncolor, 1);
		}
	}
}

void i_esp::esp_init()
{
	uintptr_t ClosestPlayer = 0;
	uintptr_t ReplayInterface = i_dupa::skid().i_replay;
	uintptr_t localplayer = i_dupa::skid().i_localplayer;
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

			if (Ped == localplayer) continue;

			if (ignoreped)
			{
				if (!i_memory::reeq().Read<uintptr_t>(Ped + i_dupa::skid().playerinfo))
					continue;
			}


			float HealthPed = i_memory::reeq().Read<float>(Ped + 0x280);
			float ArmorPed = i_memory::reeq().Read<float>(Ped + 0x1530);

			if (ignoredead)
			{
				if (HealthPed == 0)
					continue;
			}




			D3DXVECTOR3 GetCordLocal = i_memory::reeq().Read<D3DXVECTOR3>(localplayer + 0x90);
			D3DXVECTOR3 GetCordPed = i_memory::reeq().Read<D3DXVECTOR3>(Ped + 0x90);
			D3DXVECTOR3 DistanceCalculation = (GetCordLocal - GetCordPed);

			auto bone_pos = i_sdk::sdk().get_bone_position(Ped, 0);
			auto RightFoot = i_sdk::sdk().get_bone_position(Ped, 4);
			auto LeftFoot = i_sdk::sdk().get_bone_position(Ped, 3);
			auto Pelvis_ = i_sdk::sdk().get_bone_position(Ped, 5);
			auto bone_pos3 = i_sdk::sdk().get_bone_position(Ped, 7);
			D3DXVECTOR2 screen = i_sdk::sdk().world_to_screen(bone_pos);
			if (!IsOnScreen(screen)) continue;
			D3DXVECTOR2 screen2 = i_sdk::sdk().world_to_screen(RightFoot);
			if (!IsOnScreen(screen2)) continue;
			D3DXVECTOR2 screen4 = i_sdk::sdk().world_to_screen(LeftFoot);
			if (!IsOnScreen(screen4)) continue;
			D3DXVECTOR2 screen3 = i_sdk::sdk().world_to_screen(bone_pos3);
			if (!IsOnScreen(screen3)) continue;
			D3DXVECTOR2 Pelvis = i_sdk::sdk().world_to_screen(Pelvis_);
			if (!IsOnScreen(Pelvis)) continue;


			float Foot_Middle = (screen4.y + screen2.y) / 2.f;
			float Height = abs(screen.y - Foot_Middle) * 1.35f;
			float Width = (Height / 1.75f);
			float HealthDistance = Width / 16;


			uint64_t PedModelInfo = i_memory::reeq().Read<uint64_t>(Ped + 0x20);
			D3DXVECTOR3 mins = i_memory::reeq().Read<D3DXVECTOR3>(PedModelInfo + 0x30);
			D3DXVECTOR3 maxs = i_memory::reeq().Read<D3DXVECTOR3>(PedModelInfo + 0x40);
			D3DXVECTOR2 Ped_Location = i_sdk::sdk().world_to_screen(GetCordPed);
			double Distance = sqrtf(DistanceCalculation.x * DistanceCalculation.x + DistanceCalculation.y * DistanceCalculation.y + DistanceCalculation.z * DistanceCalculation.z);

			if (fixedesp)
			{


				D3DXVECTOR3 min = (GetCordPed + mins);
				D3DXVECTOR3 max = (GetCordPed + maxs);
				D3DXVECTOR3 points[] = {
						D3DXVECTOR3(min.x, min.y, min.z),
						D3DXVECTOR3(min.x, max.y, min.z),
						D3DXVECTOR3(max.x, max.y, min.z),
						D3DXVECTOR3(max.x, min.y, min.z),
						D3DXVECTOR3(max.x, max.y, max.z),
						D3DXVECTOR3(min.x, max.y, max.z),
						D3DXVECTOR3(min.x, min.y, max.z),
						D3DXVECTOR3(max.x, min.y, max.z)
				};

				D3DXVECTOR2 w2s_points[8];
				auto index = 0;
				for (D3DXVECTOR3 point : points) {
					w2s_points[index] = i_sdk::sdk().world_to_screen(point);
					if (w2s_points[index].x == 0.f && w2s_points[index].y == 0.f)
						continue;
					index++;
				}

				float x = w2s_points[0].x;
				float y = w2s_points[0].y;
				float width = w2s_points[0].x;
				float height = w2s_points[0].y;
				for (auto point : w2s_points) {
					if (x > point.x)
						x = point.x;
					if (width < point.x)
						width = point.x;
					if (y > point.y)
						y = point.y;
					if (height < point.y)
						height = point.y;
				}

				int out_x = x;
				int out_y = y;
				int out_w = width - x;
				int out_h = height - y;
				Height = out_h;
				Width = out_w;

				Width = (Height / 1.75f);
				if (((Ped_Location.x + Width / 2) - (Ped_Location.x - Width / 2)) > 600)
					continue;
			}
			if (Distance <= distanceint)
			{
				auto color = ImColor(255, 255, 255);
				uint64_t playerinfos = i_memory::reeq().Read<uint64_t>(Ped + i_dupa::skid().playerinfo);
				int playeridaddr = i_memory::reeq().Read<int>(playerinfos + 0x00);
				std::string playeridstr = std::to_string(playeridaddr);
				auto it = std::find(i_esp::esp().friends.begin(), i_esp::esp().friends.end(), playeridstr);
				if (it != i_esp::esp().friends.end())
					color = ImColor(0, 255, 0);
				else
					color = ImColor(255, 255, 255);

				std::string str = "";

				if (weapon)
				{

				}
				if (distance) {

					int Ddistance = sqrtf(DistanceCalculation.x * DistanceCalculation.x + DistanceCalculation.y * DistanceCalculation.y + DistanceCalculation.z * DistanceCalculation.z);

					std::string strr = " | " + std::to_string(Ddistance) + " m";
					if (!weapon)
						strr = "[ " + std::to_string(Ddistance) + " m " + " ]";
					str.append(strr);

				}
				if (distance || weapon)
				{

					i_overlay::here().DrawOutlinedText(ImGui::GetFont(), str, ImVec2(Ped_Location.x, Ped_Location.y + Height / 2 + 5), 15.0f, ImColor(255, 255, 255), true);

				}
				if (box)
				{
					ImGui::GetForegroundDrawList()->AddRect(ImVec2(Ped_Location.x - Width / 2, Ped_Location.y - Height / 2), ImVec2(Ped_Location.x + Width / 2, Ped_Location.y + Height / 2), color, 0);

					//ImGui::GetForegroundDrawList()->AddRect(ImVec2(Ped_Location.x - Width / 2, Ped_Location.y - Height / 2), ImVec2(Ped_Location.x + Width / 2, Ped_Location.y + Height / 2), ImColor(255, 255, 255), 0);
					//DrawBox(Ped_Location.x - (CornerWidth2 / 2), screen.y, CornerWidth2, CornerHeight, ImColor(255, 255, 255), 0, 0.5);
					//ImGui::GetForegroundDrawList()->AddRect(ImVec2(Ped_Location.x - Width / 2, Ped_Location.y - Height / 2), ImVec2(Ped_Location.x + Width / 2, Ped_Location.y + Height / 2), ImColor(0, 0, 0, 255), 1, 1, 1);
				}
				if (lines)
				{
					ImGui::GetForegroundDrawList()->AddLine(ImVec2(i_sdk::sdk().WindowSize.x / 2, i_sdk::sdk().WindowSize.y / 2), ImVec2(Ped_Location.x, Ped_Location.y), color, 1);
					// WindowSize
				}

				if (heathbar)
				{
					float HealthPercentage = Height / 100 * (HealthPed - 100);
					float HealthPercentagee = Height / 100 * 100;

					float fixedhealth = HealthPed / 2;
					float fixedhealthh = 100;

					ImColor	col = ImColor(14, 110, 17, 255);
					ImColor coll = ImColor(0, 0, 0, 255);

					float  caca = (Width) / 8.f;
					if (Distance < 20)
					{
						caca = (Width) / 12.f;
					}
					float HealthPercentageeee = Width / 100 * (HealthPed - 100);
					float HealthPercentageee = Width / 100 * 100;
					if (fixedhealth != 0)


						if (HealthBar_type == 0)
						{
							i_overlay::here().DrawHealthBar(ImVec2((Ped_Location.x + Width / 1.70), Ped_Location.y + Height / 2), ImVec2(3, HealthPercentagee), coll);

							i_overlay::here().DrawHealthBar(ImVec2((Ped_Location.x + Width / 1.70), Ped_Location.y + Height / 2), ImVec2(3, HealthPercentage), col);
						}
						else if (HealthBar_type == 1)
						{
							if (fixedhealth != 0)
								i_overlay::here().DrawHealthBar(ImVec2((Ped_Location.x - Width / 1.70) - HealthDistance, Ped_Location.y + Height / 2), ImVec2(3, HealthPercentagee), coll);
							i_overlay::here().DrawHealthBar(ImVec2((Ped_Location.x - Width / 1.70) - HealthDistance, Ped_Location.y + Height / 2), ImVec2(3, HealthPercentage), col);

						}

						else if (HealthBar_type == 2)
						{

							i_overlay::here().DrawHealthBarhorizontal(ImVec2((Ped_Location.x - Width / 2), (Ped_Location.y - Height / 2) - 10), ImVec2(Ped_Location.x - Width / 2, (Ped_Location.y - Height / 2) - 10), ImVec2(3, HealthPercentageee), coll);
							i_overlay::here().DrawHealthBarhorizontal(ImVec2((Ped_Location.x - Width / 2), (Ped_Location.y - Height / 2) - 10), ImVec2(Ped_Location.x - Width / 2, (Ped_Location.y - Height / 2) - 10), ImVec2(3, HealthPercentageeee), col);

						}

				}
				if (skeleton)
				{
					draw_skeleton(Ped);
				}
				if (armorbar)
				{

					float HealthPercentage = Height / 100 * (ArmorPed);
					float HealthPercentagee = Height / 100 * 100;

					float fixedhealth = ArmorPed;
					float fixedhealthh = 100;

					ImColor col = ImColor(10, 145, 255, 255);
					ImColor coll = ImColor(0, 0, 0, 255);

					float  caca = (Width) / 8.f;
					if (Distance < 20)
					{
						caca = (Width) / 12.f;
					}
					float HealthPercentageeee = Width / 100 * (ArmorPed);
					float HealthPercentageee = Width / 100 * 100;
					if (fixedhealth != 0)

						if (HealthBar_type == 0)
						{
							i_overlay::here().DrawHealthBar(ImVec2((Ped_Location.x + Width / 1.80), Ped_Location.y + Height / 2), ImVec2(3, HealthPercentagee), coll);

							i_overlay::here().DrawHealthBar(ImVec2((Ped_Location.x + Width / 1.80), Ped_Location.y + Height / 2), ImVec2(3, HealthPercentage), col);
						}
						else if (HealthBar_type == 1)
						{
							if (fixedhealth != 0)
								i_overlay::here().DrawHealthBar(ImVec2((Ped_Location.x - Width / 1.80) - HealthDistance, Ped_Location.y + Height / 2), ImVec2(3, HealthPercentagee), coll);
							i_overlay::here().DrawHealthBar(ImVec2((Ped_Location.x - Width / 1.80) - HealthDistance, Ped_Location.y + Height / 2), ImVec2(3, HealthPercentage), col);

						}
						else if (HealthBar_type == 2)
						{

							i_overlay::here().DrawHealthBarhorizontal(ImVec2((Ped_Location.x - Width / 2), (Ped_Location.y - Height / 2) - 20), ImVec2(Ped_Location.x - Width / 2, (Ped_Location.y - Height / 2) - 20), ImVec2(3, HealthPercentageee), coll);
							i_overlay::here().DrawHealthBarhorizontal(ImVec2((Ped_Location.x - Width / 2), (Ped_Location.y - Height / 2) - 20), ImVec2(Ped_Location.x - Width / 2, (Ped_Location.y - Height / 2) - 20), ImVec2(3, HealthPercentageeee), col);

						}

				}

			}

		}
	
	}
}

void i_esp::vehicleesp()
{

	uintptr_t ReplayInterface = i_dupa::skid().i_replay;

	if (ReplayInterface)
	{
		uintptr_t VehicleInterface = i_memory::reeq().Read<DWORD64>(ReplayInterface + 0x10);
		uintptr_t  VehucleList = i_memory::reeq().Read<DWORD64>(VehicleInterface + 0x180);
		int vehiclelist = i_memory::reeq().Read<int>(VehicleInterface + 0x188);

		for (int i = 0U; i < vehiclelist; i++)
		{

			uintptr_t Vehicle = i_memory::reeq().Read<uintptr_t>(VehucleList + (i * 0x10));
			if (!Vehicle) continue;

			D3DXVECTOR3 GetCorrdinateVehicle = i_memory::reeq().Read<D3DXVECTOR3>(Vehicle + 0x90);

			D3DXVECTOR2 VehicleLocation = i_sdk::sdk().world_to_screen(GetCorrdinateVehicle);




			if (vehiclesnapline)
			{
				i_overlay::here().DrawLine(ImVec2(GetSystemMetrics(SM_CXSCREEN) / 2, 0), ImVec2(VehicleLocation.x, VehicleLocation.y), ImGui::ColorConvertFloat4ToU32(ImColor(255, 255, 255)), 1.f);
			}




		}

	}


}

void i_esp::start()
{
	// some stuff

	if (i_dupa::skid().i_localplayer)
	{
		esp_init();
	}
}