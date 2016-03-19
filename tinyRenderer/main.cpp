#include "tgaimage.h"
#include "Model.h"

#include <vector>
#include <utility>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green   = TGAColor(0, 255, 0,   255);


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

std::pair<Vec2i, Vec2i> CreateAABB(const std::vector<Vec2i>& triangle, const TGAImage &image)
{
	std::pair<Vec2i, Vec2i> aabb(Vec2i(image.get_width()-1, image.get_height()-1), Vec2i(0, 0));
	for(auto tri: triangle)
	{
		aabb.first.x = std::min(aabb.first.x, std::max(0, tri.x));
		aabb.first.y = std::min(aabb.first.y, std::max(0, tri.y));
		aabb.second.x = std::max(aabb.second.x, std::min(image.get_width()-1, tri.x));
		aabb.second.y = std::max(aabb.second.y, std::min(image.get_height()-1, tri.y));
	}
	return aabb;
}

Vec3f GetBarycentricCoordinates(const Vec2i& point, const std::vector<Vec2i>& tri)
{
	const auto AB = tri[2] - tri[0];
	const auto AC = tri[1] - tri[0];
	const auto PA = tri[0] - point;
	Vec3f v1 (AB.x, AC.x, PA.x);
	Vec3f v2 (AB.y, AC.y, PA.y);

	Vec3f res = v1 ^ v2;
	if(std::abs(res[2]) < 1)
	{
		return Vec3f(-1,1,1);
	}
	return Vec3f(1.f-(res.x+res.y)/res.z, res.y/res.z, res.x/res.z);
}

void drawTriangle(const std::vector<Vec2i>& triangle, TGAImage &image, TGAColor color)
{
	auto aabb = CreateAABB(triangle, image);
	for(int i = aabb.first.x; i <= aabb.second.x; i++)
	{
		for(int j = aabb.first.y; j <= aabb.second.y; j++)
		{
			Vec3f p(GetBarycentricCoordinates(Vec2i(i,j), triangle));
			if(p.x >= 0.f && p.y >= 0.f && p.z >= 0.f)
			{
				image.set(i, j, color);
			}
		}
	}
}

void drawMesh(TGAImage& image, int width, int height)
{
	Model newModel;
	newModel.LoadModel("african_head.obj");
	const Vec3f lightDir(0.f, 0.f, -1.f);
	for(int i = 0; i < newModel.GetFacesSize(); i++)
	{
		// face in world coordinates
		const std::vector<Vec3f>& face = newModel.GetFace(i);
		std::vector<Vec2i> screenCoord;
		for (int j=0; j<3; j++)
		{
			screenCoord.push_back(Vec2i(width*(face[j].x+1.)/2.f,height*(face[j].y+1.)/2.f));
		}
		Vec3f normal = (face[2]-face[0]) ^ (face[1]-face[0]);
		normal.normalize();

		float intensity = normal * lightDir;
		if(intensity > 0.f)
			drawTriangle(screenCoord, image, TGAColor(intensity*255, intensity*255, intensity*255, 255));
	}
}

int main(int argc, char** argv) {
	int height = 800;
	int width = 800;
	if(argc == 3)
	{
		width = atoi(argv[1]);
		height = atoi(argv[2]);
	}

	TGAImage image(width, height, TGAImage::RGB);
	drawMesh(image, width, height);
	
	image.flip_vertically(); // origin at the left bottom corner of the image
	image.write_tga_file("output.tga");

	return 0;
}

