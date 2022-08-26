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

vector<glm::vec3> interpolateThreeElementValues(glm::vec3 from, glm::vec3 to, size_t numberOfValues){ // todo run, test and debug this function
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


// write a line of pixels to the window
void writeLine(DrawingWindow &window, vector<glm::vec3>* shades, int x, int y, size_t numberOfValues, bool vertical){
	vector<float> coords = interpolateSingleFloats(0, (vertical? HEIGHT : WIDTH), numberOfValues);
	int index = 1;
	
	int end = (vertical ? HEIGHT : WIDTH);
	for(size_t i = 0; i < end; i++){
		glm::vec3 value;
		value = (*shades)[index-1];
		uint32_t colour = (255 << 24) + (int(value[0]) << 16) + (int(value[1]) << 8) + int(value[2]);
		if (vertical) window.setPixelColour(x, i, colour);
		else window.setPixelColour(i, y, colour);
			
		
		if(i > coords[index]) index++;
	}
}

void makeBorders(DrawingWindow &window, glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 botLeft, glm::vec3 botRight, vector<glm::vec3>* rightBorder, vector<glm::vec3>* leftBorder, size_t numberOfValues){
	*rightBorder = interpolateThreeElementValues(topRight, botRight, numberOfValues);
	writeLine(window, rightBorder, window.width -1, 0, numberOfValues, true);
	*leftBorder = interpolateThreeElementValues(topLeft, botLeft, numberOfValues);
	writeLine(window, leftBorder, 0,0, numberOfValues, true);
	
	
}

void draw(DrawingWindow &window){
	window.clearPixels();
	glm::vec3 topLeft, topRight, botLeft, botRight;
	topLeft = glm::vec3(255,0,0);
	topRight = glm::vec3(0,0,255);
	botLeft = glm::vec3(255,255,0);
	botRight = glm::vec3(0,255,0);
	const size_t numberOfValues = 100;
	
	vector<glm::vec3> rightBorder, leftBorder;
	makeBorders(window, topLeft, topRight, botLeft, botRight, &rightBorder, &leftBorder, numberOfValues);
	vector<float> coords = interpolateSingleFloats(0, window.height, numberOfValues);
	int index = 1;
	for(int i = 0; i < window.height; i++){
		vector<glm::vec3> shades;
		shades = interpolateThreeElementValues(leftBorder[index-1], rightBorder[index-1], numberOfValues);
		writeLine(window, &shades, 0, i, numberOfValues, false);
		if(i > coords[index]) index++;
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
	draw(window);
	window.renderFrame();
	while (true) {
		// We MUST poll for events - otherwise the window will freeze !
		if (window.pollForInputEvents(event)) handleEvent(event, window);
		//draw(window);
		// Need to render the frame at the end, or nothing actually gets shown on the screen !
		//window.renderFrame();
	}
}
