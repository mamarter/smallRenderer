
#include "Model.h"
#include <fstream>
#include <iostream>
#include <cassert>

#include <sstream>

std::vector<std::string> Model::mRowTypes = {"v","vn","vt","f"};

Model::Model()
{

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
			float x, y, z;
			sstream >> x >> y >> z;
			mTextureCoordinates.push_back(Vec3f(x, y, z));
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
	std::string vertex;
	std::vector<Vec3f> verticesVector;
	
	for (int i=0; i<3; i++) {
		sstream >> vertex;
		std::string::size_type prev = 0, current;
		while((current = vertex.find("/", prev)) != std::string::npos ||
				((current - prev) > 0) )
		{
			int vIndex = stoi(vertex.substr(prev, current - prev));
			if(prev == 0)
			{
				verticesVector.push_back(mVertices[vIndex-1]);
			}
			else if(current == std::string::npos)
			{
				//text coord
			}
			else
			{
				//normals
			}
			
			prev = current == std::string::npos ? current : current + 1;
		}
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

const std::vector<Vec3f>& Model::GetFace(int index) const
{
	return mFaces[index];
}

std::ostream& operator<<(std::ostream& out, const Vec3f& f)
{
   return out << "(" << f.x << ',' << f.y << ',' << f.z << ')';
}

