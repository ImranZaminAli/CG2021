#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <Utils.h>
#include <fstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <glm/glm.hpp>

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

vector<glm::vec3> interpolateThreeElementValues(vector<float> from, vector<float> to, size_t numberOfValues){ // todo run, test and debug this function
	vector<glm::vec3> result;
	for(int i = 0; i < numberOfValues; i++){
		glm::vec3 v = glm::vec3(0,0,0);
		result.push_back(v);
	}
	
	for(int i = 0; i < 3; i++){
		vector<float> temp;
		temp = interpolateSingleFloats(from[i], to[i], numberOfValues);
		for(int j = 0; j < temp.size(); j++){
			result[j][i] = temp[j];
		}
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
	for (size_t x = 0; x < window.width; x++) {
		float value = int(shades[index]);
		uint32_t colour = (255 << 24) + (int(value) << 16) + (int(value) << 8) + int(value); 
		for (size_t y = 0; y < window.height; y++) {
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
	vector<float> from, to;
	from.push_back(1);
	from.push_back(4);
	from.push_back(9.2);
	to.push_back(4);
	to.push_back(1);
	to.push_back(9.8);
	size_t num = 4;
	vector<glm::vec3> result = interpolateThreeElementValues(from, to, num);
	for(int i = 0; i < num; i++){
		for(int j = 0; j < 3; j++) cout << result[i][j] << " ";
		cout << endl;
	}
	while (true) {
		// We MUST poll for events - otherwise the window will freeze !
		if (window.pollForInputEvents(event)) handleEvent(event, window);
		draw(window);
		// Need to render the frame at the end, or nothing actually gets shown on the screen !
		window.renderFrame();
	}
}
