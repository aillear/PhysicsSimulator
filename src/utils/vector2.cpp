#include "vector2.h"
#include <cmath>
#include <ostream>
#include <valarray>

template<typename T>
T Vector2<T>::getNorm() {
    return std::sqrt(x*x + y*y);
}


template<typename T>
Vector2<T>& Vector2<T>::operator*=(double n) {
    x *= n;
    y *= n;
    return *this;
}

template<typename T>
Vector2<T>& Vector2<T>::operator/=(double n) {
    x /= n;
    y /= n;
    return *this;
}

template<typename T>
Vector2<T> Vector2<T>::operator+(const Vector2<T>& other) {
    Vector2<T> newVec(x+other.x, y+other.y);
    return newVec;
}

template<typename T>
Vector2<T> Vector2<T>::operator-(const Vector2<T>& other) {
    Vector2<T> newVec(x-other.x, y-other.y);
    return newVec;
}

template<typename T>
double Vector2<T>::operator*(const Vector2<T>& other) {
    return x*other.x + y*other.y;
}

template<typename T>
Vector2<T> Vector2<T>::operator/(double n) {
    Vector2<T> newVec(x/n, y/n);
    return newVec;
}

template<typename T>
bool Vector2<T>::operator==(const Vector2<T>& other) {
    return this->x == other.x && this->y == other.y;
} 

template<typename T>
Vector2<T> operator*(const Vector2<T>& a, double b) {
    Vector2<T> newVec(a.x*b, a.y*b);
    return newVec;
}

template<typename T>
Vector2<T> operator*(double a, const Vector2<T>& b) {
    Vector2<T> newVec(b.x*a, b.y*a);
    return newVec;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const Vector2<T>& vec) {
    os << vec.x << " " << vec.y;
    return os;
}


// 显式实例化需要的模板类型
template class Vector2<float>;
template class Vector2<double>;
template class Vector2<int>;

template std::ostream& operator<<(std::ostream&, const Vector2<float>&);
template std::ostream& operator<<(std::ostream&, const Vector2<double>&);
template std::ostream& operator<<(std::ostream&, const Vector2<int>&);

template Vector2<float> operator*(const Vector2<float>&, double);
template Vector2<double> operator*(const Vector2<double>&, double);
template Vector2<int> operator*(const Vector2<int>&, double);

template Vector2<float> operator*(double, const Vector2<float>&);
template Vector2<double> operator*(double, const Vector2<double>&);
template Vector2<int> operator*(double, const Vector2<int>&);
