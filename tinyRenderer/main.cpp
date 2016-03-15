#include "tgaimage.h"
#include "Model.h"

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

void fillTriangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color)
{
    line(t0, t1, image, color);
    line(t1, t2, image, color);
    line(t2, t0, image, color);
    
    
}

void drawMesh(TGAImage& image, int width, int height)
{
	Model newModel;
	newModel.LoadModel("african_head.obj");

	for(int i = 0; i < newModel.GetFacesSize(); i++)
	{
		const std::vector<Vec3f>& face = newModel.GetFace(i);
		for (int j=0; j<3; j++) { 
		    Vec3f v0 = face[j]; 
		    Vec3f v1 =face[(j+1)%3]; 
		    int x0 = (v0.x+1.)*width/2.; 
		    int y0 = (v0.y+1.)*height/2.; 
		    int x1 = (v1.x+1.)*width/2.; 
		    int y1 = (v1.y+1.)*height/2.; 
		    line(Vec2i(x0, y0), Vec2i(x1, y1), image, white);
    	} 
	}
}

int main(int argc, char** argv) {
	int height = 200;
	int width = 200;
	if(argc == 3)
	{
		width = atoi(argv[1]);
		height = atoi(argv[2]);
	}

	TGAImage image(width, height, TGAImage::RGB);
//	drawMesh(image, width, height);
    
    
    Vec2i t0[3] = {Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80)};
    Vec2i t1[3] = {Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180)};
    Vec2i t2[3] = {Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)};
    fillTriangle(t0[0], t0[1], t0[2], image, red);
    fillTriangle(t1[0], t1[1], t1[2], image, white);
    fillTriangle(t2[0], t2[1], t2[2], image, green);
	
	image.flip_vertically(); // origin at the left bottom corner of the image
	image.write_tga_file("output.tga");

	return 0;
}

