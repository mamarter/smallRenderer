#pragma once

#include <vector>
#include <string>

class Vec3f
{
public:
	Vec3f() : 
	x(0.f),
	y(0.f),
	z(0.f) {}

	Vec3f(float x, float y, float z) : 
	x(x),
	y(y),
	z(z) {}

	float x, y, z;
};

class Vec2i
{
public:
	Vec2i() :
	x(0),
	y(0) {}

	Vec2i(int x, int y) :
	x(x),
	y(y){}
	
	friend bool operator==(const Vec2i& curr,const Vec2i& other) { return (curr.x==other.x) && (curr.y==other.y); }
	friend bool operator!=(const Vec2i& curr,const Vec2i& other) { return !(curr==other); }

	friend Vec2i operator-(Vec2i lhs, const Vec2i& rhs) { lhs.x-= rhs.x; lhs.y-=rhs.y; return lhs; }
	friend Vec2i operator+(Vec2i lhs, const Vec2i& rhs) { lhs.x+= rhs.x; lhs.y+=rhs.y; return lhs; }
	friend Vec2i operator*(Vec2i lhs, float rhs) { lhs.x*=rhs; lhs.y*=rhs; return lhs; }

	int x, y;
};


std::ostream& operator<<(std::ostream& out, const Vec3f& f);


class Model
{

public:

	enum ERowType 
	{
		VERTEX = 0,
		NORMAL = 1,
		TEXTURE = 2,
		FACE = 3 ,
		UNKNOWN
	};

	Model();
	void LoadModel(const std::string& modelPath);

	int GetVerticesSize() const;
	int GetFacesSize() const;

	const Vec3f& GetVertex(int index) const;
	const std::vector<Vec3f>& GetFace(int index) const;

private:
	void CreateFace(std::stringstream&);

	std::vector<Vec3f> mVertices;
	std::vector<Vec3f> mTextureCoordinates;
	std::vector<Vec3f> mNormals;
	std::vector<std::vector<Vec3f>> mFaces;

	static std::vector<std::string> mRowTypes;


};