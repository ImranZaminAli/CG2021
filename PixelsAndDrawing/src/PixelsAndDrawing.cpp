#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <Utils.h>
#include <fstream>
#include <vector>
#include <iomanip>
#include <algorithm>

#define WIDTH 320
#define HEIGHT 240

using namespace std;

vector<float> interpolateSingleFloats(float from, float to, size_t numberOfValues){
	vector<float> result;
	float increment = (to - from)/(numberOfValues-1);
	
	for(size_t i = 0; i < numberOfValues; i++){
		result.push_back(from);
		from += increment;
	}

	return result;
}

void draw(DrawingWindow &window) {
	window.clearPixels();
	vector<float> shades;
	const int gradient = 100;
	shades = interpolateSingleFloats(0, 255, gradient);
	reverse(shades.begin(), shades.end());
	vector<float> xCoords;
	xCoords = interpolateSingleFloats(0, WIDTH, gradient);
	int index = 1;
	//for(int i = 0; i < shades.size(); i++) cout << shades[i] << " " << xCoords[i] << endl;
	//cout << xCoords[index];
	for (size_t x = 0; x < window.width; x++) {
		float value = int(shades[index]);
		uint32_t colour = (255 << 24) + (int(value) << 16) + (int(value) << 8) + int(value); 
		//red = green = blue = shades[index];
		for (size_t y = 0; y < window.height; y++) {
			/*float red = rand() % 256;
			float green = 0.0;
			float blue = 0.0;
			uint32_t colour = (255 << 24) + (int(red) << 16) + (int(green) << 8) + int(blue);*/
			window.setPixelColour(x, y, colour);
		}
		
		if(x > xCoords[index]) index++;
			
		
	}
}

void handleEvent(SDL_Event event, DrawingWindow &window) {
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_LEFT) std::cout << "LEFT" << std::endl;
		else if (event.key.keysym.sym == SDLK_RIGHT) std::cout << "RIGHT" << std::endl;
		else if (event.key.keysym.sym == SDLK_UP) std::cout << "UP" << std::endl;
		else if (event.key.keysym.sym == SDLK_DOWN) std::cout << "DOWN" << std::endl;
	} else if (event.type == SDL_MOUSEBUTTONDOWN) {
		window.savePPM("output.ppm");
		window.saveBMP("output.bmp");
	}
}


int main(int argc, char *argv[]) {
	DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
	SDL_Event event;
	/*vector<float> result;
	result = interpolateSingleFloats(2.2, 8.5, 7);
	
	for(size_t i = 0; i < result.size(); i++)
		cout << setprecision(3) << result[i] << " ";
	cout << endl;
	//cout << "\n\nTHIS IS A TEST\n\n";*/
	//draw(window);
	//window.renderFrame();
	while (true) {
		// We MUST poll for events - otherwise the window will freeze !
		if (window.pollForInputEvents(event)) handleEvent(event, window);
		draw(window);
		// Need to render the frame at the end, or nothing actually gets shown on the screen !
		window.renderFrame();
	}
}
