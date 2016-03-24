
#include "Model.h"
#include <fstream>
#include <iostream>
#include <cassert>

#include <sstream>

std::vector<std::string> Model::mRowTypes = {"v","vn","vt","f"};

Model::Model()
{

}

Model::Model(const std::string& modelPath)
{
	LoadModel(modelPath);
}

void Model::LoadModel(const std::string& modelPath)
{
	std::ifstream modelFile;
	modelFile.open(modelPath);
	std::string line, type;
	while(getline(modelFile, line))
	{
		std::stringstream sstream(line);
//		std::cout<<line<<std::endl;
		sstream >> type;

		if(type == mRowTypes[ERowType::VERTEX])
		{
			float x, y, z;
			sstream >> x >> y >> z;
			mVertices.push_back(Vec3f(x, y, z));
		}
		else if(type == mRowTypes[ERowType::NORMAL])
		{
			float x, y, z;
			sstream >> x >> y >> z;
			mNormals.push_back(Vec3f(x, y, z));	
		}
		else if(type == mRowTypes[ERowType::TEXTURE])
		{
			float x, y;
			sstream >> x >> y;
			mTextureCoordinates.push_back(Vec2i(x, y));
		}
		else if(type == mRowTypes[ERowType::FACE])
		{
			CreateFace(sstream);
		}
		else
		{
			assert("Type not recognised");
		}
	}
}

void Model::CreateFace(std::stringstream& sstream)
{
	std::string line;
	std::vector<Vertex> verticesVector;
	
	for (int i=0; i<3; i++) {
		Vertex newVert;
		sstream >> line;
		std::string::size_type prev = 0, current;
		while((current = line.find("/", prev)) != std::string::npos ||
				((current - prev) > 0) )
		{
			int vIndex = stoi(line.substr(prev, current - prev));
			if(prev == 0)
			{
				newVert.xyz = mVertices[vIndex-1];
			}
			else if(current == std::string::npos)
			{
				//normals
			}
			else
			{
				//text coord
				newVert.uv = mTextureCoordinates[vIndex-1];
			}
			
			prev = current == std::string::npos ? current : current + 1;
		}

		verticesVector.push_back(newVert);
	}

	mFaces.push_back(verticesVector);
}
	
int Model::GetVerticesSize() const
{
	return static_cast<int>(mVertices.size());
}

int Model::GetFacesSize() const
{
	return static_cast<int>(mFaces.size());
}

const Vec3f& Model::GetVertex(int index) const
{
	return mVertices[index];
}

const std::vector<Vertex>& Model::GetFace(int index) const
{
	return mFaces[index];
}

std::ostream& operator<<(std::ostream& out, const Vec3f& f)
{
   return out << "(" << f.x << ',' << f.y << ',' << f.z << ')';
}

