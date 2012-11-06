#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	light.enable();

	//load models.. this is the only place where i am making an assumption that the models are sorted in back to front order
	model[0].loadModel("teapot.obj", true);
	model[0].setPosition(ofGetWidth() / 3, ofGetHeight() / 2, -400);
	model[0].setRotation(0, -90, 1, 0, 0);
	model[1].loadModel("bunny.obj", true);
	model[1].setPosition(2.0f * ofGetWidth() / 3, ofGetHeight(), -200);
	model[1].setRotation(0, 0, 1, 0, 0);
	
	axisOfRotation[0] = ofVec3f(0, 0, 1);
	axisOfRotation[1] = ofVec3f(0, 1, 0);

	glShadeModel(GL_SMOOTH);
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	
	//load shaders
	depthShader.load("Shaders/depth.vert", "Shaders/depth.frag");
	edgeDetectShader.load("Shaders/edgeDetect.vert", "Shaders/edgeDetect.frag");
	
	//modelFBO needs some extra settings like depthtexture.. hence use Settings struct
	ofFbo::Settings s;
	s.width = ofGetWidth();
	s.height = ofGetHeight();
	s.internalformat = GL_RGBA;
	s.useDepth = true;
	s.depthStencilAsTexture = true;

	modelFBO.allocate(s);
	modelFBO.begin();
	ofClear(0, 0, 0, 255);
	modelFBO.end();

	depthFBO.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	depthFBO.begin();
	ofClear(0, 0, 0, 255);
	depthFBO.end();

	edgeFBO.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	edgeFBO.begin();
	ofClear(0, 0, 0, 255);
	edgeFBO.end();

	finalFBO.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	finalFBO.begin();
	ofClear(0, 0, 0, 255);
	finalFBO.end();

}

//--------------------------------------------------------------
void testApp::update(){

}

void testApp::drawRect() {
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
	glTexCoord2f(ofGetWidth(), 0); glVertex3f(ofGetWidth(), 0, 0);
	glTexCoord2f(ofGetWidth(), ofGetHeight()); glVertex3f(ofGetWidth(), ofGetHeight(), 0);
	glTexCoord2f(0, ofGetHeight());  glVertex3f(0, ofGetHeight(), 0);
	glEnd();
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0, 0, 0, 255);
	
	finalFBO.begin();
	ofClear(0, 0, 0, 0);
	finalFBO.end();
	
	//do this for each model
	for (int i=0; i<2; i++) {
		glEnable(GL_DEPTH_TEST);
		modelFBO.begin();
		ofClear(0, 0, 0, 0);
		model[i].setRotation(1, ofGetElapsedTimef() * 100, axisOfRotation[i].x, axisOfRotation[i].y, axisOfRotation[i].z);
		model[i].draw(ofPolyRenderMode::OF_MESH_FILL);
		modelFBO.end();
		glDisable(GL_DEPTH_TEST);

		//threshold the depth texture
		depthShader.begin();
		depthShader.setUniformTexture("depthTex", modelFBO.getDepthTexture(), 1);
		depthFBO.begin();
		ofClear(0, 0, 0, 0);
		drawRect();
		depthFBO.end();
		depthShader.end();
	
		//create the silhouette for this model
		edgeDetectShader.begin();
		edgeDetectShader.setUniformTexture("depthTex", depthFBO.getTextureReference(), 1);
		edgeDetectShader.setUniformTexture("tex", modelFBO.getTextureReference(), 2);
		edgeDetectShader.setUniform1f("width", ofGetWidth() * 1.0f);
		edgeDetectShader.setUniform1f("height", ofGetHeight() * 1.0f);
		edgeFBO.begin();
		ofClear(0, 0, 0, 0);
		drawRect();
		edgeFBO.end();
		edgeDetectShader.end();
	
		//render the silhouetted model into the finalFBO
		finalFBO.begin();
		edgeFBO.draw(0, 0);
		finalFBO.end();
	}

	finalFBO.draw(0, 0);
	
}

