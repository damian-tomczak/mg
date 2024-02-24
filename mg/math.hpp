#pragma once
#include <cmath>

struct Vec4
{
    float x, y, z, w;

    Vec4(float x = 0.f, float y = 0.f, float z = 0.f, float w = 0.f) : x(x), y(y), z(z), w(w) {}

    Vec4 operator+(const Vec4& rhs) const
    {
        return Vec4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
    }

    Vec4 operator-(const Vec4& rhs) const
    {
        return Vec4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
    }

    Vec4 operator*(float scalar) const
    {
        return Vec4(x * scalar, y * scalar, z * scalar, w * scalar);
    }

    Vec4 operator*(const Vec4& other) const
    {
        return Vec4{x * other.x, y * other.y, z * other.z, w * other.w};
    }

    Vec4 operator-() const
    {
        return Vec4{-x, -y, -z, -w};
    }

    friend Vec4 operator*(float scalar, const Vec4& vec)
    {
        return Vec4{vec.x * scalar, vec.y * scalar, vec.z * scalar, vec.w * scalar};
    }
};


struct Vec3
{
    float x, y, z;

    Vec3(float x = 0.f, float y = 0.f, float z = 0.f) : x(x), y(y), z(z)
    {}

    Vec3 operator+(const Vec3& rhs) const
    {
        return Vec3{x + rhs.x, y + rhs.y, z + rhs.z};
    }

    Vec3 operator-(const Vec3& rhs) const
    {
        return Vec3{x - rhs.x, y - rhs.y, z - rhs.z};
    }

    Vec3 operator*(float scalar) const
    {
        return Vec3{x * scalar, y * scalar, z * scalar};
    }

    Vec3 operator*(const Vec3& other) const
    {
        return Vec3{x * other.x, y * other.y, z * other.z};
    }

    Vec3 operator-() const
    {
        return Vec3{-x, -y, -z};
    }

    friend Vec3 operator*(float scalar, const Vec3& vec)
    {
        return Vec3{vec.x * scalar, vec.y * scalar, vec.z * scalar};
    }
};

Vec3 normalize(Vec3 v)
{
    float mag = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    return Vec3{v.x / mag, v.y / mag, v.z / mag};
}

float dot(const Vec3& v1, const Vec3& v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vec3 reflect(const Vec3& v, const Vec3& n)
{
    return v - n * (2 * dot(v, n));
}