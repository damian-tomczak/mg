#pragma once
#include <cmath>
#include <array>
#include <format>

struct Vec3;

struct Vec4
{
    float x, y, z, w;

    auto operator<=>(const Vec4& other) const = default;

    constexpr Vec4() = default;

    Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
    {}

    Vec4(float value) : x(value), y(value), z(value), w(value)
    {}

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

    auto operator<=>(const Vec3& other) const = default;

    Vec3(float x, float y, float z) : x(x), y(y), z(z)
    {}

    Vec3(float value) : x(value), y(value), z(value)
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

struct Mat4
{
    std::array<Vec4, 4> data;

    constexpr Mat4() = default;
    constexpr Mat4(
        const float x1, const float y1, const float z1, const float w1,
        const float x2, const float y2, const float z2, const float w2,
        const float x3, const float y3, const float z3, const float w3,
        const float x4, const float y4, const float z4, const float w4);

    constexpr Mat4(const float v);

    Vec4& operator[](const size_t index);
    const Vec4& operator[](const size_t index) const;
};

constexpr Mat4 operator*(const Mat4& lhs, const Mat4& rhs);
Mat4 inverse(const Mat4& mat);
constexpr Mat4 transpose(const Mat4& mat);
constexpr Mat4 translate(const Mat4& matrix, const Vec3& translation);
constexpr Mat4 scale(const Mat4& matrix, const Vec3& scaleObjVec);
std::string to_string(const Mat4 mat);


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

inline constexpr Mat4 operator*(const Mat4& lhs, const Mat4& rhs)
{
    return
    {
        rhs.data[0].x * lhs.data[0].x + rhs.data[0].y * lhs.data[1].x + rhs.data[0].z * lhs.data[2].x + rhs.data[0].w * lhs.data[3].x,
        rhs.data[0].x * lhs.data[0].y + rhs.data[0].y * lhs.data[1].y + rhs.data[0].z * lhs.data[2].y + rhs.data[0].w * lhs.data[3].y,
        rhs.data[0].x * lhs.data[0].z + rhs.data[0].y * lhs.data[1].z + rhs.data[0].z * lhs.data[2].z + rhs.data[0].w * lhs.data[3].z,
        rhs.data[0].x * lhs.data[0].w + rhs.data[0].y * lhs.data[1].w + rhs.data[0].z * lhs.data[2].w + rhs.data[0].w * lhs.data[3].w,

        rhs.data[1].x * lhs.data[0].x + rhs.data[1].y * lhs.data[1].x + rhs.data[1].z * lhs.data[2].x + rhs.data[1].w * lhs.data[3].x,
        rhs.data[1].x * lhs.data[0].y + rhs.data[1].y * lhs.data[1].y + rhs.data[1].z * lhs.data[2].y + rhs.data[1].w * lhs.data[3].y,
        rhs.data[1].x * lhs.data[0].z + rhs.data[1].y * lhs.data[1].z + rhs.data[1].z * lhs.data[2].z + rhs.data[1].w * lhs.data[3].z,
        rhs.data[1].x * lhs.data[0].w + rhs.data[1].y * lhs.data[1].w + rhs.data[1].z * lhs.data[2].w + rhs.data[1].w * lhs.data[3].w,

        rhs.data[2].x * lhs.data[0].x + rhs.data[2].y * lhs.data[1].x + rhs.data[2].z * lhs.data[2].x + rhs.data[2].w * lhs.data[3].x,
        rhs.data[2].x * lhs.data[0].y + rhs.data[2].y * lhs.data[1].y + rhs.data[2].z * lhs.data[2].y + rhs.data[2].w * lhs.data[3].y,
        rhs.data[2].x * lhs.data[0].z + rhs.data[2].y * lhs.data[1].z + rhs.data[2].z * lhs.data[2].z + rhs.data[2].w * lhs.data[3].z,
        rhs.data[2].x * lhs.data[0].w + rhs.data[2].y * lhs.data[1].w + rhs.data[2].z * lhs.data[2].w + rhs.data[2].w * lhs.data[3].w,

        rhs.data[3].x * lhs.data[0].x + rhs.data[3].y * lhs.data[1].x + rhs.data[3].z * lhs.data[2].x + rhs.data[3].w * lhs.data[3].x,
        rhs.data[3].x * lhs.data[0].y + rhs.data[3].y * lhs.data[1].y + rhs.data[3].z * lhs.data[2].y + rhs.data[3].w * lhs.data[3].y,
        rhs.data[3].x * lhs.data[0].z + rhs.data[3].y * lhs.data[1].z + rhs.data[3].z * lhs.data[2].z + rhs.data[3].w * lhs.data[3].z,
        rhs.data[3].x * lhs.data[0].w + rhs.data[3].y * lhs.data[1].w + rhs.data[3].z * lhs.data[2].w + rhs.data[3].w * lhs.data[3].w,
    };
}

inline Mat4 inverse(const Mat4& mat)
{
    const auto det23zw = mat[2].z * mat[3].w - mat[3].z * mat[2].w;
    const auto det23yw = mat[2].y * mat[3].w - mat[3].y * mat[2].w;
    const auto det23yz = mat[2].y * mat[3].z - mat[3].y * mat[2].z;
    const auto det23xw = mat[2].x * mat[3].w - mat[3].x * mat[2].w;
    const auto det23xz = mat[2].x * mat[3].z - mat[3].x * mat[2].z;
    const auto det23xy = mat[2].x * mat[3].y - mat[3].x * mat[2].y;
    const auto det13zw = mat[1].z * mat[3].w - mat[3].z * mat[1].w;
    const auto det13yw = mat[1].y * mat[3].w - mat[3].y * mat[1].w;
    const auto det13yz = mat[1].y * mat[3].z - mat[3].y * mat[1].z;
    const auto det13xw = mat[1].x * mat[3].w - mat[3].x * mat[1].w;
    const auto det13xz = mat[1].x * mat[3].z - mat[3].x * mat[1].z;
    const auto det13xy = mat[1].x * mat[3].y - mat[3].x * mat[1].y;
    const auto det12zw = mat[1].z * mat[2].w - mat[2].z * mat[1].w;
    const auto det12yw = mat[1].y * mat[2].w - mat[2].y * mat[1].w;
    const auto det12yz = mat[1].y * mat[2].z - mat[2].y * mat[1].z;
    const auto det12xw = mat[1].x * mat[2].w - mat[2].x * mat[1].w;
    const auto det12xz = mat[1].x * mat[2].z - mat[2].x * mat[1].z;
    const auto det12xy = mat[1].x * mat[2].y - mat[2].x * mat[1].y;

    const auto tempInverse00 = +(mat[1].y * det23zw - mat[1].z * det23yw + mat[1].w * det23yz);
    const auto tempInverse01 = -(mat[1].x * det23zw - mat[1].z * det23xw + mat[1].w * det23xz);
    const auto tempInverse02 = +(mat[1].x * det23yw - mat[1].y * det23xw + mat[1].w * det23xy);
    const auto tempInverse03 = -(mat[1].x * det23yz - mat[1].y * det23xz + mat[1].z * det23xy);

    const auto det =
        mat[0].x * tempInverse00 +
        mat[0].y * tempInverse01 +
        mat[0].z * tempInverse02 +
        mat[0].w * tempInverse03;

    assert(det != 0);

    return
    {
        tempInverse00 / det, (-(mat[0].y * det23zw - mat[0].z * det23yw + mat[0].w * det23yz)) / det, (+(mat[0].y * det13zw - mat[0].z * det13yw + mat[0].w * det13yz)) / det, (-(mat[0].y * det12zw - mat[0].z * det12yw + mat[0].w * det12yz)) / det,
        tempInverse01 / det, (+(mat[0].x * det23zw - mat[0].z * det23xw + mat[0].w * det23xz)) / det, (-(mat[0].x * det13zw - mat[0].z * det13xw + mat[0].w * det13xz)) / det, (+(mat[0].x * det12zw - mat[0].z * det12xw + mat[0].w * det12xz)) / det,
        tempInverse02 / det, (-(mat[0].x * det23yw - mat[0].y * det23xw + mat[0].w * det23xy)) / det, (+(mat[0].x * det13yw - mat[0].y * det13xw + mat[0].w * det13xy)) / det, (-(mat[0].x * det12yw - mat[0].y * det12xw + mat[0].w * det12xy)) / det,
        tempInverse03 / det, (+(mat[0].x * det23yz - mat[0].y * det23xz + mat[0].z * det23xy)) / det, (-(mat[0].x * det13yz - mat[0].y * det13xz + mat[0].z * det13xy)) / det, (+(mat[0].x * det12yz - mat[0].y * det12xz + mat[0].z * det12xy)) / det,
    };
}

inline constexpr Mat4 transpose(const Mat4& mat)
{
    return
    {
        mat.data[0].x, mat.data[1].x, mat.data[2].x, mat.data[3].x,
        mat.data[0].y, mat.data[1].y, mat.data[2].y, mat.data[3].y,
        mat.data[0].z, mat.data[1].z, mat.data[2].z, mat.data[3].z,
        mat.data[0].w, mat.data[1].w, mat.data[2].w, mat.data[3].w,
    };
}

inline constexpr Mat4 translate(const Mat4& matrix, const Vec3& translation)
{
    return
    {
        matrix[0].x                , matrix[0].y                , matrix[0].z                , matrix[0].w,
        matrix[1].x                , matrix[1].y                , matrix[1].z                , matrix[1].w,
        matrix[2].x                , matrix[2].y                , matrix[2].z                , matrix[2].w,
        matrix[3].x + translation.x, matrix[3].y + translation.y, matrix[3].z + translation.z, matrix[3].w,
    };
}

inline constexpr Mat4 scale(const Mat4& matrix, const Vec3& scaleObjVec)
{
    const Mat4 scaleObjMatrix
    {
        scaleObjVec.x, 0         , 0         , 0,
        0         , scaleObjVec.y, 0         , 0,
        0         , 0         , scaleObjVec.z, 0,
        0         , 0         , 0         , 1,
    };

    return matrix * scaleObjMatrix;
}

inline std::string to_string(const Mat4 mat)
{
    return std::format("\n"
        "{:>10.7f}, {:>10.7f}, {:>10.7f}, {:>10.7f},\n"
        "{:>10.7f}, {:>10.7f}, {:>10.7f}, {:>10.7f},\n"
        "{:>10.7f}, {:>10.7f}, {:>10.7f}, {:>10.7f},\n"
        "{:>10.7f}, {:>10.7f}, {:>10.7f}, {:>10.7f}\n",
        mat[0].x, mat[0].y, mat[0].z, mat[0].w,
        mat[1].x, mat[1].y, mat[1].z, mat[1].w,
        mat[2].x, mat[2].y, mat[2].z, mat[2].w,
        mat[3].x, mat[3].y, mat[3].z, mat[3].w);
}