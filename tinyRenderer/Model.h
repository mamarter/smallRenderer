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

class Vec2f
{
public:
	Vec2f() : 
	x(0.f),
	y(0.f) {}

	Vec2f(float x, float y) : 
	x(x),
	y(y){}

	float x, y;
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