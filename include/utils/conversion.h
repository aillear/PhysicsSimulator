#pragma once
#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_stdinc.h"
#include <SDL3/SDL_rect.h>
#include <format>
#include <glm/detail/qualifier.hpp>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/ext/vector_float2.hpp>
#include <ostream>


template <typename T> struct Vector2 {
  public:
    T x;
    T y;

    Vector2(T _x, T _y) : x(_x), y(_y) { ; }
    Vector2(const SDL_Point &p) : x(p.x), y(p.y) { ; }
    Vector2(const SDL_FPoint &p) : x(p.x), y(p.y) { ; }
    Vector2(const std::pair<T, T> &p) : x(p.first), y(p.second) { ; }
    Vector2(const glm::vec<2, T, glm::defaultp> &p) : x(p.x), y(p.y) { ; }
    operator std::pair<T, T>() const;
    operator glm::vec<2, T, glm::defaultp>() const;
    operator SDL_Point() const;
    operator SDL_FPoint() const;

    T Length();
    // Normalize only work correctly in float or double.
    Vector2<T> &Normalize();

    Vector2<T> &operator*=(double);
    Vector2<T> &operator/=(double);
    Vector2<T> &operator+=(const Vector2<T> &);
    Vector2<T> &operator-=(const Vector2<T> &);

    double operator*(const Vector2<T> &);
    double operator%(const Vector2<T> &);
    Vector2<T> operator/(double);
    Vector2<T> operator+(const Vector2<T> &);
    Vector2<T> operator-(const Vector2<T> &);

    bool operator==(const Vector2<T> &);

    template <typename U>
    friend Vector2<U> operator*(const Vector2<U> &, double);
    template <typename U>
    friend Vector2<U> operator*(double, const Vector2<U> &);
    template <typename U>
    friend std::ostream &operator<<(std::ostream &, const Vector2<U> &);
};

using Vector2i = Vector2<int>;
using Vector2f = Vector2<float>;
using Vector2d = Vector2<double>;

inline SDL_Vertex MakeVertex(const SDL_FPoint &&p,
                             const SDL_FColor &&c =
                                 {
                                     0.0f,
                                     0.0f,
                                     0.0f,
                                     0.0f,
                                 },
                             const SDL_FPoint &&uv = {0.0f, 0.0f}) {
    ;
    return SDL_Vertex{p, c, uv};
}

inline SDL_Vertex MakeVertex(const SDL_FPoint &p,
                             const SDL_FColor &c =
                                 {
                                     0.0f,
                                     0.0f,
                                     0.0f,
                                     0.0f,
                                 },
                             const SDL_FPoint &uv = {0.0f, 0.0f}) {
    return SDL_Vertex{p, c, uv};
}

// conversion functions
inline glm::vec2 ToGlmVec2(const SDL_FPoint &v) { return glm::vec2(v.x, v.y); }
inline SDL_FPoint ToFPoint(const glm::vec2 &v) { return SDL_FPoint{v.x, v.y}; }

constexpr float FColorToColorFactor = 255.0f;
constexpr float ColorToFColorFactor = 1.0f / FColorToColorFactor;

inline SDL_FColor ToFColor(const SDL_Color &c) {
    return SDL_FColor{c.r * ColorToFColorFactor, c.g * ColorToFColorFactor,
                      c.b * ColorToFColorFactor, c.a * ColorToFColorFactor};
}

inline SDL_Color ToColor(const SDL_FColor &c) {
    return SDL_Color{static_cast<Uint8>(c.r * FColorToColorFactor),
                     static_cast<Uint8>(c.g * FColorToColorFactor),
                     static_cast<Uint8>(c.b * FColorToColorFactor),
                     static_cast<Uint8>(c.a * FColorToColorFactor)};
}

// reload operators of SDL_FPoint
inline SDL_FPoint operator+(const SDL_FPoint &a, const SDL_FPoint &b) {
    return {a.x + b.x, a.y + b.y};
}

inline SDL_FPoint operator-(const SDL_FPoint &a, const SDL_FPoint &b) {
    return {a.x - b.x, a.y - b.y};
}

inline int operator*(const SDL_FPoint &a, const SDL_FPoint &b) {
    return a.x * b.x + a.y * b.y;
}

inline SDL_FPoint &operator+=(SDL_FPoint &a, const SDL_FPoint &b) {
    a.x += b.x;
    a.y += b.y;
    return a;
}

inline SDL_FPoint &operator-=(SDL_FPoint &a, const SDL_FPoint &b) {
    a.x -= b.x;
    a.y -= b.y;
    return a;
}

// format support for glm::vec2 SDL_FPoint, SDL_Point
template <typename T, typename _CharT>
struct std::formatter<Vector2<T>, _CharT> {
    constexpr auto parse(format_parse_context &ctx) {
        auto pos = ctx.begin();
        return elem_formatter_.parse(ctx);
    }

    template <typename _FormatContext>
    auto format(const Vector2<T> &v, _FormatContext &format_context) const {
        auto out = format_context.out();
        *out++ = '(';
        out = elem_formatter_.format(v.x, format_context);
        out = std::format_to(out, "{}", separator_);
        out = elem_formatter_.format(v.y, format_context);
        *out++ = ')';
        return out;
    }

  private:
    std::formatter<T, _CharT> elem_formatter_;
    std::string_view separator_ = ", ";
};

template <typename T, typename _CharT>
struct std::formatter<glm::vec<2, T, glm::defaultp>, _CharT> {
    constexpr auto parse(format_parse_context &ctx) {
        auto pos = ctx.begin();
        return elem_formatter_.parse(ctx);
    }

    template <typename _FormatContext>
    auto format(const glm::vec<2, T, glm::defaultp> &v,
                _FormatContext &format_context) const {
        auto out = format_context.out();
        *out++ = '(';
        out = elem_formatter_.format(v.x, format_context);
        out = std::format_to(out, "{}", separator_);
        out = elem_formatter_.format(v.y, format_context);
        *out++ = ')';
        return out;
    }

  private:
    std::formatter<T, _CharT> elem_formatter_;
    std::string_view separator_ = ", ";
};

template <typename _CharT> struct std::formatter<SDL_FPoint, _CharT> {
    constexpr auto parse(format_parse_context &ctx) {
        auto pos = ctx.begin();
        return elem_formatter_.parse(ctx);
    }

    template <typename _FormatContext>
    auto format(SDL_FPoint &v, _FormatContext &format_context) const {
        auto out = format_context.out();
        *out++ = '(';
        out = elem_formatter_.format(v.x, format_context);
        out = std::format_to(out, "{}", separator_);
        out = elem_formatter_.format(v.y, format_context);
        *out++ = ')';
        return out;
    }

  private:
    std::formatter<float, _CharT> elem_formatter_;
    std::string_view separator_ = ", ";
};

template <typename _CharT> struct std::formatter<SDL_Point, _CharT> {
    constexpr auto parse(format_parse_context &ctx) {
        auto pos = ctx.begin();
        return elem_formatter_.parse(ctx);
    }

    template <typename _FormatContext>
    auto format(SDL_Point &v, _FormatContext &format_context) const {
        auto out = format_context.out();
        *out++ = '(';
        out = elem_formatter_.format(v.x, format_context);
        out = std::format_to(out, "{}", separator_);
        out = elem_formatter_.format(v.y, format_context);
        *out++ = ')';
        return out;
    }

  private:
    std::formatter<int, _CharT> elem_formatter_;
    std::string_view separator_ = ", ";
};


template <typename _CharT> struct std::formatter<SDL_FRect, _CharT> {
    constexpr auto parse(format_parse_context &ctx) {
        auto pos = ctx.begin();
        return elem_formatter_.parse(ctx);
    }

    template <typename _FormatContext>
    auto format(SDL_FRect &v, _FormatContext &format_context) const {
        auto out = format_context.out();
        *out++ = '(';
        out = elem_formatter_.format(v.x, format_context);
        out = std::format_to(out, "{}", separator_);
        out = elem_formatter_.format(v.y, format_context);
        out = std::format_to(out, "{}", separator_);
        out = elem_formatter_.format(v.w, format_context);
        out = std::format_to(out, "{}", separator_);
        out = elem_formatter_.format(v.h, format_context);
        *out++ = ')';
        return out;
    }

  private:
    std::formatter<float, _CharT> elem_formatter_;
    std::string_view separator_ = ", ";
};

template <typename _CharT> struct std::formatter<SDL_FColor, _CharT> {
    constexpr auto parse(format_parse_context &ctx) {
        auto pos = ctx.begin();
        return elem_formatter_.parse(ctx);
    }

    template <typename _FormatContext>
    auto format(SDL_FColor &v, _FormatContext &format_context) const {
        auto out = format_context.out();
        *out++ = '(';
        out = elem_formatter_.format(v.r, format_context);
        out = std::format_to(out, "{}", separator_);
        out = elem_formatter_.format(v.g, format_context);
        out = std::format_to(out, "{}", separator_);
        out = elem_formatter_.format(v.b, format_context);
        out = std::format_to(out, "{}", separator_);
        out = elem_formatter_.format(v.a, format_context);
        *out++ = ')';
        return out;
    }

  private:
    std::formatter<float, _CharT> elem_formatter_;
    std::string_view separator_ = ", ";
};


int RandomInt(int from, int to);
bool RandomBool();

float RandomFloat(float from, float to);
float RandomFloat01();

SDL_FColor RandomFColor(float r=-1.0f, float g=-1.0f, float b=-1.0f);
SDL_Color RandomColor(Uint8 r, Uint8 g, Uint8 b);

glm::vec2 RandomPos(glm::vec2 leftTop, glm::vec2 rightButtom);

glm::vec2 RandomSize(int width, int height);

inline float Cross(const glm::vec2 &v1, const glm::vec2 &v2) {
    return v1.x * v2.y - v1.y * v2.x;
}