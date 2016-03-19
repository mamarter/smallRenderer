#pragma once

#include <vector>
#include "geometry.h"

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