#pragma once
#include <format>
#include <ostream>


template <typename T> struct Vector2 {
  public:
    T x;
    T y;

    Vector2(T _x, T _y) : x(_x), y(_y) { ; }
    Vector2(std::pair<T, T> &p) : x(p.first), y(p.second) { ; }
    operator std::pair<T, T>() const;

    inline T Length();
    // Normalize only work correctly in float or double.
    inline Vector2<T> &Normalize();

    inline Vector2<T> &operator*=(double);
    inline Vector2<T> &operator/=(double);
    inline Vector2<T> &operator+=(const Vector2<T> &);
    inline Vector2<T> &operator-=(const Vector2<T> &);

    inline double operator*(const Vector2<T> &);
    inline double operator%(const Vector2<T> &);
    inline Vector2<T> operator/(double);
    inline Vector2<T> operator+(const Vector2<T> &);
    inline Vector2<T> operator-(const Vector2<T> &);

    inline bool operator==(const Vector2<T> &);

    template <typename U>
    inline friend Vector2<U> operator*(const Vector2<U> &, double);
    template <typename U>
    inline friend Vector2<U> operator*(double, const Vector2<U> &);
    template <typename U>
    inline friend std::ostream &operator<<(std::ostream &, const Vector2<U> &);
};

using Vector2i = Vector2<int>;
using Vector2f = Vector2<float>;
using Vector2d = Vector2<double>;

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