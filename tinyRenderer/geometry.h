//
//  Geometry.hpp
//  tinyRenderer
//
//  Created by Maria Marco Terraes on 19/03/2016.
//  Copyright © 2016 Maria Marco Terraes. All rights reserved.
//

#ifndef Geometry_h
#define Geometry_h

#include <cmath>

template <class T>
class Vec3
{
public:
	Vec3() :
	x(0),
	y(0),
	z(0) {}
	
	Vec3(T _x, T _y, T _z) :
	x(_x),
	y(_y),
	z(_z) {}

	friend Vec3<T> operator-(Vec3<T> lhs, const Vec3<T>& rhs) { lhs.x-= rhs.x; lhs.y-=rhs.y; lhs.z-=rhs.z; return lhs; }
	friend Vec3<T> operator^(const Vec3<T>& v1, const Vec3<T>& v2)
	{
		return Vec3<T>(v1.y*v2.z-v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
	}
	friend std::ostream& operator<<(std::ostream& out, const Vec3<T>& f);
	friend T operator*(const Vec3<T>& lhs, const Vec3<T>& rhs) { return lhs.x*rhs.x+lhs.y*rhs.y+lhs.z*rhs.z; }
	void normalize()
	{
		T magnitude = std::sqrt(x*x+y*y+z*z);
		x /= magnitude;
		y /= magnitude;
		z /= magnitude;
	}
	
	T& operator[] (const int nIndex)
	{
		return raw[nIndex];
	}
	
	union {
		struct {T u, v, w;};
		struct {T x, y, z;};
		T raw[3];
	};
};

template <class T>
class Vec2
{
public:
	Vec2() :
	x(0),
	y(0) {}
	
	Vec2(T _x, T _y) :
	x(_x),
	y(_y){}
	
	friend bool operator==(const Vec2<T>& curr,const Vec2<T>& other) { return (curr.x==other.x) && (curr.y==other.y); }
	friend bool operator!=(const Vec2<T>& curr,const Vec2<T>& other) { return !(curr==other); }
	
	friend Vec2<T> operator-(Vec2<T> lhs, const Vec2<T>& rhs) { lhs.x-= rhs.x; lhs.y-=rhs.y; return lhs; }
	friend Vec2<T> operator+(Vec2<T> lhs, const Vec2<T>& rhs) { lhs.x+= rhs.x; lhs.y+=rhs.y; return lhs; }
	friend Vec2<T> operator*(Vec2<T> lhs, float rhs) { lhs.x*=rhs; lhs.y*=rhs; return lhs; }
	
	T& operator[] (const int nIndex)
	{
		return raw[nIndex];
	}
	
	union {
		struct {T u, v;};
		struct {T x, y;};
		T raw[2];
	};
};

using Vec3f = Vec3<float>;
using Vec2i = Vec2<int>;

#endif /* Geometry_h */
