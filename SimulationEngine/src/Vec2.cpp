#include "Vec2.h"
#include <cmath>

// constructors
Vec2::Vec2()
:	x(0.f), y(0.f)
{
}
Vec2::Vec2(float xVal, float yVal)
	: x(xVal), y(yVal)
{
}

// magnitude 
float Vec2::length() const {
	return std::sqrt(x * x + y * y);
}

// normalization
Vec2 Vec2::normalized() const {
	float len = length();

	if (len == 0.f)
		return Vec2(0.f, 0.f);

	return Vec2(x / len, y / len);
}

//operators
Vec2 Vec2::operator+(const Vec2& rhs) const {
	return Vec2(x + rhs.x, y + rhs.y);
}
Vec2 Vec2::operator-(const Vec2& rhs) const {
	return Vec2(x - rhs.x, y - rhs.y);
}
Vec2 Vec2::operator*(float scalar) const {
	return Vec2(x * scalar, y * scalar);
}
Vec2 Vec2::operator/(float scalar) const {
	return Vec2(x / scalar, y / scalar);
}

Vec2& Vec2::operator+=(const Vec2& rhs) {
	x += rhs.x;
	y += rhs.y;
	return *this;
}
Vec2& Vec2::operator-=(const Vec2& rhs) {
	x -= rhs.x;
	y -= rhs.y;
	return *this;
}