#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void drawRect();

		ofxAssimpModelLoader model[2];
		ofVec3f axisOfRotation[2];
		ofLight light;
		ofShader depthShader, edgeDetectShader;
		ofFbo modelFBO, depthFBO, edgeFBO, finalFBO;
};
