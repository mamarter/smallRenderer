#include "tgaimage.h"
#include "Model.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) { 
    bool steep = false; 
    if (std::abs(x0-x1)<std::abs(y0-y1)) { 
        std::swap(x0, y0); 
        std::swap(x1, y1); 
        steep = true; 
    } 
    if (x0>x1) { 
        std::swap(x0, x1); 
        std::swap(y0, y1); 
    } 
    int dx = std::abs(x1-x0); 
    int dy = std::abs(y1-y0); 
    float derror = dy/float(dx);
    float error = 0; 
    int y = y0; 
    for (int x=x0; x<=x1; x++) { 
        if (steep) { 
            image.set(y, x, color); 
        } else { 
            image.set(x, y, color); 
        } 
        error += derror; 
        if (error>.5) { 
            y += (y1>y0?1:-1); 
            error -= 1.; 
        } 
    } 
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
		    line(x0, y0, x1, y1, image, white); 
    	} 
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

