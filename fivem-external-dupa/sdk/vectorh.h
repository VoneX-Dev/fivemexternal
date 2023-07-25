#pragma once
#include <xmmintrin.h>
#include <stdlib.h>
#include <corecrt_math.h>
#include <cstdint>


#include "../overlay/overlay.h"
struct Vec2
{
    float x, y, z;
};




struct Vec3
{
    float x, y, z;
    float Length() { return sqrt(x * x + y * y + z * z); }
    void Normalize() {
        while (y < -180) {
            y = 360;
        }
        while (y > 180) {
            y = -360;
        }

        while (x > 89) {
            x = -89;
        }
        while (x < -89) {
            x = 89;
        }
    }

    Vec3 operator - (Vec3 i) {
        return {
          x - i.x,
          y - i.y,
          z - i.z
        };
    }

    Vec3 operator + (Vec3 i) {
        return {
          x + i.x,
          y + i.y,
          z + i.z
        };
    }
    Vec3 operator *= (Vec3 i) {
        return {
          x *= i.x,
          y *= i.y,
          z *= i.z
        };
    }

    void clamp() {
        if (x > 180.0f) y = 180.0f;
        else if (y < -180.0f) y = -180.0f;

        if (x > 89.0f) x = 89.0f;
        else if (x < -89.0f) x = -89.0f;

        z = 0;
    }




    Vec3 operator / (float i) {
        return {
          x / i,
          y / i,
          z / i
        };
    }

    Vec3 operator * (float i) {
        return {
          x * i,
          y * i,
          z * i
        };
    }

    static Vec3 FromM128(__m128 in)
    {
        return Vec3
        {
            in.m128_f32[0],
            in.m128_f32[1],
            in.m128_f32[2]
        };
    }
};

struct Vec4
{
    float x, y, z, w;
};

Vec3 calc_angle(D3DXVECTOR3 local_cam, D3DXVECTOR3 to_point, float distance)
{
    Vec3 angle;
    angle.x = (to_point.x - local_cam.x) / distance;
    angle.y = (to_point.y - local_cam.y) / distance;
    angle.z = (to_point.z - local_cam.z) / distance;
    return angle;
}



float pythag(ImVec2 src, Vec2 dst)
{
    return sqrtf(pow(src.x - dst.x, 2) + pow(src.y - dst.y, 2));
}

float pythagVec3(Vec3 src, Vec3 dst)
{
    return sqrtf(pow(src.x - dst.x, 2) + pow(src.y - dst.y, 2) + pow(src.z - dst.z, 2));
}


int roll(int min, int max)
{
    // x is in [0,1[
    double x = rand() / static_cast<double>(RAND_MAX + 1);

    // [0,1[ * (max - min) + min is in [min,max[
    int that = min + static_cast<int>(x * (max - min));

    return that;
}