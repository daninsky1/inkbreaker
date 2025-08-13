//
// Created by daniel on 07/08/2025.
//
#pragma once
#include <concepts>
#include <cinttypes>

#include <boost/operators.hpp>

namespace gfx
{

template<typename T>
concept Numeric = std::is_signed_v<T>;

template<Numeric T>
struct TPoint
{
    T x{}, y{};
    TPoint operator+(const TPoint &other);
    TPoint operator-(const TPoint &other);
    TPoint operator*(const TPoint &other);
    TPoint operator/(const TPoint &other);
};
using IPoint = TPoint<int32_t>;
using FPoint = TPoint<float>;
using DPoint = TPoint<double>;
using Point = IPoint;

template<Numeric T>
struct TVector2d { T x{}, y{}; };
using IVector2d = TVector2d<int32_t>;
using FVector2d = TVector2d<float>;
using DVector2d = TVector2d<double>;
using Vector2d = FVector2d;

template<Numeric T>
struct TVector3d { T x{}, y{}, z{}; };
using IVector3d = TVector3d<int32_t>;
using FVector3d = TVector3d<float>;
using DVector3d = TVector3d<double>;
using Vector3d = FVector3d;

template<Numeric T>
struct TVector4d { T x{}, y{}, z{}, w{}; };
using IVector4d = TVector4d<int32_t>;
using FVector4d = TVector4d<float>;
using DVector4d = TVector4d<double>;
using Vector4d = FVector4d;

template<Numeric T>
struct TSize
{
    T width{}, height{};
    [[nodiscard]] bool isEmpty() const { return width == 0 || height == 0; }
};
using ISize = TSize<int32_t>;
using FSize = TSize<float>;
using DSize = TSize<double>;
using Size = ISize;

template<Numeric T>
struct TLine { T x1{}, y1{}, x2{}, y2{}; };
using ILine = TLine<int32_t>;
using FLine = TLine<float>;
using DLine = TLine<double>;
using Line = ILine;

template<Numeric T>
struct TRect { T x{}, y{}, width{}, height{}; };
using IRect = TRect<int32_t>;   // ;)
using FRect = TRect<float>;
using DRect = TRect<double>;
using Rect = IRect;

template<Numeric T>
struct TCircle { T x{}, y{}, radius{0}; };
using ICircle = TCircle<int32_t>;
using FCircle =  TCircle<float>;
using DCircle = TCircle<double>;
using Circle = ICircle;

}
