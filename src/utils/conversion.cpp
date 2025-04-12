#include "conversion.h"
#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_stdinc.h"
#include "logger.h"
#include <cmath>
#include <glm/ext/vector_float2.hpp>
#include <ostream>
#include <random>
#include <utility>

template <typename T> Vector2<T>::operator std::pair<T, T>() const {
    return std::pair(x, y);
}

template <typename T> Vector2<T>::operator SDL_Point() const {
    return {(int)x, (int)y};
}

template <typename T> Vector2<T>::operator SDL_FPoint() const {
    return {(float)x, (float)y};
}

template <typename T> Vector2<T>::operator glm::vec<2, T>() const {
    return glm::vec<2, T>(x, y);
}

template <typename T> T Vector2<T>::Length() {
    return std::sqrt(x * x + y * y);
}

template <typename T> Vector2<T> &Vector2<T>::Normalize() {
    double length = this->Length();
    this->x /= length;
    this->y /= length;
    return *this;
}

template <typename T> Vector2<T> &Vector2<T>::operator*=(double n) {
    x *= n;
    y *= n;
    return *this;
}

template <typename T> Vector2<T> &Vector2<T>::operator/=(double n) {
    x /= n;
    y /= n;
    return *this;
}

template <typename T>
Vector2<T> &Vector2<T>::operator+=(const Vector2<T> &other) {
    x += other.x;
    y += other.y;
    return *this;
}

template <typename T>
Vector2<T> &Vector2<T>::operator-=(const Vector2<T> &other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

template <typename T>
Vector2<T> Vector2<T>::operator+(const Vector2<T> &other) {
    Vector2<T> newVec(x + other.x, y + other.y);
    return newVec;
}

template <typename T>
Vector2<T> Vector2<T>::operator-(const Vector2<T> &other) {
    Vector2<T> newVec(x - other.x, y - other.y);
    return newVec;
}

template <typename T> double Vector2<T>::operator*(const Vector2<T> &other) {
    return x * other.x + y * other.y;
}

template <typename T> double Vector2<T>::operator%(const Vector2<T> &other) {
    return this->x * other.y - this->y * other.x;
}

template <typename T> Vector2<T> Vector2<T>::operator/(double n) {
    Vector2<T> newVec(x / n, y / n);
    return newVec;
}

template <typename T> bool Vector2<T>::operator==(const Vector2<T> &other) {
    return this->x == other.x && this->y == other.y;
}

template <typename T>
inline Vector2<T> operator*(const Vector2<T> &a, double b) {
    Vector2<T> newVec(a.x * b, a.y * b);
    return newVec;
}

template <typename T>
inline Vector2<T> operator*(double a, const Vector2<T> &b) {
    Vector2<T> newVec(b.x * a, b.y * a);
    return newVec;
}

template <typename T>
inline std::ostream &operator<<(std::ostream &os, const Vector2<T> &vec) {
    os << vec.x << " " << vec.y;
    return os;
}

// 显式实例化需要的模板类型
template std::ostream &operator<<(std::ostream &, const Vector2<float> &);
template std::ostream &operator<<(std::ostream &, const Vector2<double> &);
template std::ostream &operator<<(std::ostream &, const Vector2<int> &);

template Vector2<float> operator*(const Vector2<float> &, double);
template Vector2<double> operator*(const Vector2<double> &, double);
template Vector2<int> operator*(const Vector2<int> &, double);

template Vector2<float> operator*(double, const Vector2<float> &);
template Vector2<double> operator*(double, const Vector2<double> &);
template Vector2<int> operator*(double, const Vector2<int> &);

template class Vector2<float>;
template class Vector2<double>;
template class Vector2<int>;

int RandomInt(int from, int to) {
    if (to <= from) {
        F_LOG_ERROR("random create error: from:{} is bigger than to{}.", from,
                    to);
    }
    thread_local static std::random_device rd;
    thread_local static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(from, to - 1);
    return dist(gen);
}

bool RandomBool() {
    int randomInt = RandomInt(0, 2);
    return (bool)randomInt;
}

float RandomFloat(float from, float to) {
    if (to <= from) {
        F_LOG_ERROR("random create error: from:{} is bigger than to{}.", from,
                    to);
    }
    thread_local static std::random_device rd;
    thread_local static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(from, to);
    return dist(gen);
}

float RandomFloat01() { return RandomFloat(0, 1); }

SDL_FColor RandomFColor(float r, float g, float b) {
    if (r < 0)
        r = RandomFloat01();
    if (g < 0)
        g = RandomFloat01();
    if (b < 0)
        g = RandomFloat01();
    return SDL_FColor{r, g, b, 1};
}

SDL_Color RandomColor(Uint8 r, Uint8 g, Uint8 b) {
    r = RandomInt(0, 256);
    g = RandomInt(0, 256);
    b = RandomInt(0, 256);
    return SDL_Color{r, g, b, 255};
}

glm::vec2 RandomPos(glm::vec2 leftTop, glm::vec2 rightButtom) {
    glm::vec2 newPos;
    newPos.x = RandomFloat(leftTop.x, rightButtom.x);
    newPos.y = RandomFloat(leftTop.y, rightButtom.y);
    return newPos;
}