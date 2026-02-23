#pragma once

class Vec2 {
public:
	float x;
	float y;
	
public:
	Vec2();
	Vec2(float x, float y);

	float length() const; // magnitude

	Vec2 normalized() const; // normalization

	// operators
	Vec2 operator+(const Vec2& rhs) const;
	Vec2 operator-(const Vec2& rhs) const;
	Vec2 operator*(float scalar) const;
	Vec2 operator/(float scalar) const;

	Vec2& operator+=(const Vec2& rhs);
	Vec2& operator-=(const Vec2& rhs);
};