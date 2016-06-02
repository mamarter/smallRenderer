#include "tgaimage.h"
#include "Model.h"

#include <vector>
#include <utility>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green   = TGAColor(0, 255, 0,   255);

int height = 800;
int width = 800;

void line(Vec2i t0, Vec2i t1, TGAImage &image, TGAColor color) {
	
	bool steep = false; 
	if (std::abs(t0.x-t1.x)<std::abs(t0.y-t1.y)) {
		std::swap(t0.x, t0.y);
		std::swap(t1.x, t1.y);
		steep = true; 
	} 
	if (t0.x>t1.x) {
		std::swap(t0.x, t1.x);
		std::swap(t0.y, t1.y);
	} 
	int dx = std::abs(t1.x-t0.x);
	int dy = std::abs(t1.y-t0.y);
	float derror = dy/float(dx);
	float error = 0; 
	int y = t0.y;
	for (int x=t0.x; x<=t1.x; x++) {
		if (steep) { 
			image.set(y, x, color); 
		} else { 
			image.set(x, y, color); 
		} 
		error += derror; 
		if (error>.5) { 
			y += (t1.y>t0.y?1:-1);
			error -= 1.; 
		} 
	} 
}

void drawHorizontalLines(Vec2i initial, Vec2i final, Vec2i initialLong, Vec2i finalLong, TGAImage &image, TGAColor color) {
	
	for (int h = initial.y; h <= final.y; h++) {
		const int shorter = final.y - initial.y;
		const int longer = finalLong.y - initialLong.y;
		if(shorter == 0 || longer == 0)
			continue;
		const float perc0 = std::abs(h-initialLong.y)/(float)longer;
		const float perc1 = std::abs(h-initial.y)/(float)shorter;
		
		Vec2i p0 = initialLong + (finalLong-initialLong)*perc0;
		Vec2i p1 = initial + (final-initial)*perc1;
		if((p1.x - p0.x)<0) std::swap(p0, p1);
		for (int x=p0.x; x<=p1.x; x++)
		{
			image.set(x, h, color);
		}
	}
}

void fillTriangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color)
{
	//bubble sort
	if(t0.y > t1.y) std::swap(t0, t1);
	if(t1.y > t2.y) std::swap(t2, t1);
	if(t0.y > t1.y) std::swap(t0, t1);
	
	drawHorizontalLines(t0, t1, t0, t2, image, color);
	drawHorizontalLines(t1, t2, t0, t2, image, color);
}

std::pair<Vec2i, Vec2i> CreateAABB(const std::vector<Vec3f>& triangle, const TGAImage &image)
{
	std::pair<Vec2i, Vec2i> aabb(Vec2i(image.get_width()-1, image.get_height()-1), Vec2i(0, 0));
	for(auto tri: triangle)
	{
		aabb.first.x = std::min(aabb.first.x, std::max(0, (int)tri.x));
		aabb.first.y = std::min(aabb.first.y, std::max(0, (int)tri.y));
		aabb.second.x = std::max(aabb.second.x, std::min(image.get_width()-1, (int)tri.x));
		aabb.second.y = std::max(aabb.second.y, std::min(image.get_height()-1, (int)tri.y));
	}
	return aabb;
}

Vec3f GetBarycentricCoordinates(const Vec2i& point, const std::vector<Vec3f>& tri)
{
	const auto AB = tri[2] - tri[0];
	const auto AC = tri[1] - tri[0];
	const auto PA = tri[0] - Vec3f(point.x,point.y, 0.f);
	Vec3f v1 (AB.x, AC.x, PA.x);
	Vec3f v2 (AB.y, AC.y, PA.y);

	Vec3f res = v1 ^ v2;
	if(std::abs(res[2]) < 1)
	{
		return Vec3f(-1,1,1);
	}
	return Vec3f(1.f-(res.x+res.y)/res.z, res.y/res.z, res.x/res.z);
}

void drawTriangle(std::vector<Vec3f> triangle, std::vector<float> &zbuffer, TGAImage &image, const std::vector<Vec2f>& texCoord, float intensity)
{
	TGAImage diffuse;
	diffuse.read_tga_file("african_head_diffuse.tga");
	diffuse.flip_vertically();
	auto aabb = CreateAABB(triangle, image);
	for(int i = aabb.first.x; i <= aabb.second.x; i++)
	{
		for(int j = aabb.first.y; j <= aabb.second.y; j++)
		{
			Vec3f p(GetBarycentricCoordinates(Vec2i(i,j), triangle));
			if(p.x >= 0.f && p.y >= 0.f && p.z >= 0.f)
			{
				// lerp for finding out z and tex coordinate
				float z = 0.f;
				Vec2f tex;
				for (int k=0; k<3; k++)
				{
					z += triangle[k].z * p[k];
					tex.u += texCoord[k].u * p[k];
					tex.v += texCoord[k].v * p[k];
				}
				const auto pixelIndex = i+j*image.get_width();
				if (zbuffer[pixelIndex] < z)
				{
					zbuffer[pixelIndex] = z;
					TGAColor vertexColor = diffuse.get(tex.u*diffuse.get_width(), tex.v*diffuse.get_height());
					vertexColor.r *= intensity;
					vertexColor.g *= intensity;
					vertexColor.b *= intensity;
					image.set(i, j, vertexColor);
				}
			}
		}
	}
}

Vec3f world2screen(Vec3f v)
{
	return Vec3f(int((v.x+1.)*width/2.+.5), int((v.y+1.)*height/2.+.5), v.z);
}

Vec2i scaleTexCoord(Vec2i v)
{
	return Vec2i(v.u*width, v.v*height);
}

void drawMesh(TGAImage& image, const Model& newModel)
{
	const Vec3f lightDir(0.f, 0.f, -1.f);
	
	std::vector<float> zbuffer(width*height, -std::numeric_limits<float>::max());

	for(int i = 0; i < newModel.GetFacesSize(); i++)
	{
		// face in world coordinates
		const std::vector<Vertex>& face = newModel.GetFace(i);
		std::vector<Vec3f> screenCoord;
		std::vector<Vec2f> texCoord;

		for (int j=0; j<3; j++)
		{
			//from world to screen coordinates
			screenCoord.push_back(world2screen(face[j].xyz));
			texCoord.push_back(face[j].uv);
		}
		Vec3f normal = (face[2].xyz-face[0].xyz) ^ (face[1].xyz-face[0].xyz);
		normal.normalize();

		float intensity = normal * lightDir;

		if(intensity > 0.f)
		{
			drawTriangle(screenCoord, zbuffer, image, texCoord, intensity);
		}
	}
}

int main(int argc, char** argv) {

	if(argc == 3)
	{
		width = atoi(argv[1]);
		height = atoi(argv[2]);
	}

	TGAImage image(width, height, TGAImage::RGB);
	Model newModel("african_head.obj");
	drawMesh(image, newModel);
	
	image.flip_vertically(); // origin at the left bottom corner of the image
	image.write_tga_file("output.tga");

	return 0;
}

