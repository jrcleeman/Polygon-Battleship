#pragma once

class Vec2
{
public:
	float x = 0;
	float y = 0;
	
	
	Vec2();
	Vec2(float xin, float yin);

	bool operator == (const Vec2& rhs) const;
	bool operator != (const Vec2& rhs) const;

	Vec2 operator + (const Vec2& rhs) const;
	Vec2 operator - (const Vec2& rhs) const;
	Vec2 operator / (const Vec2& rhs) const;
	Vec2 operator * (const Vec2& rhs) const;

	void operator += (const Vec2& rhs) const;
	void operator -= (const Vec2& rhs) const;
	void operator *= (const Vec2& rhs) const;
	void operator /= (const Vec2& rhs) const;

	float dist(const Vec2& rhs) const;
};