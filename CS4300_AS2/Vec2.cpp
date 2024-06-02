#include "Vec2.h"
#include <math.h>

Vec2::Vec2()
{

}

Vec2::Vec2(float xin, float yin)
	: x(xin), y(yin)
{

}

Vec2 Vec2::operator + (const Vec2& rhs) const
{
	//TODO
	return Vec2(0, 0);
}

Vec2 Vec2::operator - (const Vec2& rhs) const
{
	//TODO
	return Vec2(0, 0);
}

Vec2 Vec2::operator / (const float val) const
{
	//TODO
	return Vec2(0,0)
}

Vec2 Vec2::operator * (const float val) const
{
	//TODO
	return Vec2(0, 0);
}

bool Vec2::operator == (const Vec2& rhs) const
{
	//TODO
	return false;
}

