#include "sdk.h"
i_sdk& i_sdk::sdk()
{
	static i_sdk ins;
	return ins;
}

D3DXVECTOR3 i_sdk::get_bone_position(uintptr_t ped_ptr, int bone_id) {
	auto matrix = i_memory::reeq().Read<D3DXMATRIX>(ped_ptr + 0x60);
	D3DXVECTOR3 bone;
	if (i_dupa::skid().isbuild2802 == false)
	{
		bone = i_memory::reeq().Read<D3DXVECTOR3>(ped_ptr + (0x430 + bone_id * 0x10));
	}
	if (i_dupa::skid().isbuild2802 == true)
	{
		bone = i_memory::reeq().Read<D3DXVECTOR3>(ped_ptr + (0x410 + 0x10 * bone_id));
	}


	D3DXVECTOR4 transform;
	D3DXVec3Transform(&transform, &bone, &matrix);
	return D3DXVECTOR3(transform.x, transform.y, transform.z);
}

D3DXVECTOR2 i_sdk::world_to_screen(D3DXVECTOR3 world_pos) {
	static uintptr_t viewport = i_dupa::skid().i_viewport;
	auto viewmatrix = i_memory::reeq().Read<D3DXMATRIX>(viewport + 0x24C);

	D3DXMatrixTranspose(&viewmatrix, &viewmatrix);
	auto vec_x = D3DXVECTOR4(viewmatrix._21, viewmatrix._22, viewmatrix._23, viewmatrix._24),
		vec_y = D3DXVECTOR4(viewmatrix._31, viewmatrix._32, viewmatrix._33, viewmatrix._34),
		vec_z = D3DXVECTOR4(viewmatrix._41, viewmatrix._42, viewmatrix._43, viewmatrix._44);

	auto screen_pos = D3DXVECTOR3((vec_x.x * world_pos.x) + (vec_x.y * world_pos.y) + (vec_x.z * world_pos.z) + vec_x.w,
		(vec_y.x * world_pos.x) + (vec_y.y * world_pos.y) + (vec_y.z * world_pos.z) + vec_y.w,
		(vec_z.x * world_pos.x) + (vec_z.y * world_pos.y) + (vec_z.z * world_pos.z) + vec_z.w);

	if (screen_pos.z <= 0.1f)
		return D3DXVECTOR2(0, 0);

	screen_pos.z = 1.0f / screen_pos.z;
	screen_pos.x *= screen_pos.z;
	screen_pos.y *= screen_pos.z;

	auto width = GetSystemMetrics(SM_CXSCREEN);
	auto height = GetSystemMetrics(SM_CYSCREEN);
	float x_temp = width / 2;
	float y_temp = height / 2;

	screen_pos.x += x_temp + float(0.5f * screen_pos.x * width + 0.5f);
	screen_pos.y = y_temp - float(0.5f * screen_pos.y * height + 0.5f);
	return D3DXVECTOR2(screen_pos.x, screen_pos.y);
}



