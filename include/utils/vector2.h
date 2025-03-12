#pragma once
#include <ostream>

template <typename T> class Vector2 {
  public:
    T x;
    T y;

    Vector2(T _x, T _y) : x(_x), y(_y) { ; }

    T getNorm();

    Vector2<T> &operator*=(double);
    Vector2<T> &operator/=(double);
    Vector2<T> operator+(const Vector2<T> &);
    Vector2<T> operator-(const Vector2<T> &);
    double operator*(const Vector2<T> &);
    Vector2<T> operator/(double);

    bool operator==(const Vector2<T>&);

    template<typename U>
    friend Vector2<U> operator*(const Vector2<U> &, double);
    template<typename U>
    friend Vector2<U> operator*(double, const Vector2<U> &);
    template<typename U>
    friend std::ostream& operator<<(std::ostream &, const Vector2<U> &);
};

using Vector2i = Vector2<int>;
using Vector2f = Vector2<float>;
using Vector2d = Vector2<double>;
