#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	if (arguments.size() > 0) {

		if (arguments[1] == "-f") {
			DMX16bit_Mode = true;
		}

	}


	if (DMX16bit_Mode) {
		ofSetWindowTitle("Mirror Remote - Mode 16bit");
		borderLeft   = 30;
		borderRight  = 50;
		borderTop    = 40;
		borderBottom = 50;
	}
	else {
		ofSetWindowTitle("Mirror Remote - Mode 8bit");
		borderLeft   = 30;
		borderRight  = 30;
		borderTop    = 40;
		borderBottom = 30;
	}
	

	ofBackground(40, 100, 40);
	ofSetCircleResolution(50);
	sender.setup(HOST, PORT);


	ServoPan_min  = 0x02CF;   //764
	ServoPan_max  = 0x0ACF;  //2764
	ServoTilt_min = 0x0320;  //800
	ServoTilt_max = 0x041F;  //1055

	windowWidth = ofGetWindowWidth();
	windowHeight = ofGetWindowHeight();



	Servo_Pan_Res = ServoPan_max - ServoPan_min;
	Servo_Tilt_Res = ServoTilt_max - ServoTilt_min;

	for (int i = 0; i < 9; i++) {
		DMX_Pan_Coarse[i]  = 127;
		DMX_Tilt_Coarse[i] = 127;

		DMX_Pan_Fine[i]  = 0;
		DMX_Tilt_Fine[i] = 0;


	}

	circlePosX = ofMap(DMX_Pan_Coarse[mirrorIndex],  0, 255, borderLeft, windowWidth - borderRight);
	circlePosY = ofMap(DMX_Tilt_Coarse[mirrorIndex], 0, 255, borderTop, windowHeight - borderBottom);

	SlideX   = ofMap(DMX_Pan_Fine[mirrorIndex],  0, 255, borderLeft, windowWidth - borderRight);
	SlideY   = ofMap(DMX_Tilt_Fine[mirrorIndex], 0, 255, borderTop, windowHeight - borderBottom);

}

//--------------------------------------------------------------
void ofApp::update(){
	ofxOscMessage m;

	windowWidth = ofGetWindowWidth();
	windowHeight = ofGetWindowHeight();

	ServoPosX_Coarse = ofMap(DMX_Pan_Coarse[mirrorIndex],  0, 255, ServoPan_min, ServoPan_max)    - (ServoPan_min  * 1.0);
	ServoPosY_Coarse = ofMap(DMX_Tilt_Coarse[mirrorIndex], 0, 255, ServoTilt_min, ServoTilt_max ) - (ServoTilt_min * 1.0);

	ServoPosX_Fine = ofMap(DMX_Pan_Fine[mirrorIndex], 0,  255, 0, Servo_Pan_Res  / 255.0);
	ServoPosY_Fine = ofMap(DMX_Tilt_Fine[mirrorIndex], 0, 255, 0, Servo_Tilt_Res / 255.0);

	ServoPosX_Total = ServoPosX_Coarse + ServoPosX_Fine;
	ServoPosY_Total = ServoPosY_Coarse + ServoPosY_Fine;

	osc_mirrorPosX_Coarse = ofMap(DMX_Pan_Coarse[mirrorIndex],  0, 255, 0.0, 1.0);
	osc_mirrorPosY_Coarse = ofMap(DMX_Tilt_Coarse[mirrorIndex], 0, 255, 0.0, 1.0);
	osc_mirrorPosX_Fine = ofMap(DMX_Pan_Fine[mirrorIndex],  0, 255, 0.0, 1.0);
	osc_mirrorPosY_Fine = ofMap(DMX_Tilt_Fine[mirrorIndex], 0, 255, 0.0, 1.0);

	currentMillis_Main = ofGetElapsedTimeMillis();
	
	if ((currentMillis_Main - previousMillis_Main) > 10) {
		previousMillis_Main = currentMillis_Main;

		if (fsend == 1) {
			
			if (osc_mirrorPosX_Coarse != osc_mirrorPosX_Coarse_prev) {
				osc_mirrorPosX_Coarse_prev = osc_mirrorPosX_Coarse;
				m.setAddress("/dmx/mp");
				m.addFloatArg(osc_mirrorPosX_Coarse);
				sender.sendMessage(m, false);
			}
			fsend = 2;
		}
		else if (fsend == 2) {

			if (osc_mirrorPosY_Coarse != osc_mirrorPosY_Coarse_prev) {
				osc_mirrorPosY_Coarse_prev = osc_mirrorPosY_Coarse;
				m.setAddress("/dmx/mt");
				m.addFloatArg(osc_mirrorPosY_Coarse);
				sender.sendMessage(m, false);
			}

			if (DMX16bit_Mode) {
				fsend = 3;
			}
			else {
				fsend = 1;
			}
		}
		else if (fsend == 3) {
			
			if (osc_mirrorPosX_Fine != osc_mirrorPosX_Fine_prev) {
				osc_mirrorPosX_Fine_prev = osc_mirrorPosX_Fine;
				m.setAddress("/dmx/mp16b");
				m.addFloatArg(osc_mirrorPosX_Fine);
				sender.sendMessage(m, false);
			}
			fsend = 4;
		}

		else if (fsend == 4) {
			if (osc_mirrorPosY_Fine != osc_mirrorPosY_Fine_prev) {
				osc_mirrorPosY_Fine_prev = osc_mirrorPosY_Fine;
				m.setAddress("/dmx/mt16b");
				m.addFloatArg(osc_mirrorPosY_Fine);
				sender.sendMessage(m, false);
			}
			fsend = 1;
		}
		
	}
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	string buf[20];
	int txtPos = 20;

	ofSetHexColor(0xFFFFFF);
	buf[0] = "osc to " + string(HOST) + string(":") + ofToString(PORT);
	ofDrawBitmapString(buf[0], 10, txtPos); txtPos = txtPos + 20;


	if (bDebugMode) {

		if (DMX16bit_Mode){
			buf[1] = "DMX_X " + ofToString(mirrorIndex + 1) + " [" + ofToString(DMX_Pan_Coarse[mirrorIndex], 3) + string(",") + ofToString(DMX_Pan_Fine[mirrorIndex], 3) + "]";
			buf[2] = "DMX_Y " + ofToString(mirrorIndex + 1) + " [" + ofToString(DMX_Tilt_Coarse[mirrorIndex], 3) + string(",") + ofToString(DMX_Tilt_Fine[mirrorIndex], 3) + "]";
			buf[3] = "OSC_X " + ofToString(mirrorIndex + 1) + " [" + ofToString(osc_mirrorPosX_Coarse, 6) + string(",") + ofToString(osc_mirrorPosX_Fine, 6) + "]";
			buf[4] = "OSC_Y " + ofToString(mirrorIndex + 1) + " [" + ofToString(osc_mirrorPosY_Coarse, 6) + string(",") + ofToString(osc_mirrorPosY_Fine, 6) + "]";
			buf[5] = "Servo Coarse " + ofToString(mirrorIndex + 1) + " [" + ofToString(ServoPosX_Coarse, 3) + string(",") + ofToString(ServoPosY_Coarse, 3) + "]";
			buf[6] = "Servo Fine " + ofToString(mirrorIndex + 1) + " [" + ofToString(ServoPosX_Fine, 3) + string(",") + ofToString(ServoPosY_Fine, 3) + "]";
			buf[7] = "Target" + ofToString(mirrorIndex + 1) + " [" + ofToString(ServoPosX_Total, 3) + string(",") + ofToString(ServoPosY_Total, 3) + "]";
			buf[19] = "time(ms)" + ofToString(currentMillis_Main);
			ofDrawBitmapString(buf[1], 10, txtPos); txtPos = txtPos + 20;
			ofDrawBitmapString(buf[2], 10, txtPos); txtPos = txtPos + 20;
			ofDrawBitmapString(buf[3], 10, txtPos); txtPos = txtPos + 20;
			ofDrawBitmapString(buf[4], 10, txtPos); txtPos = txtPos + 20;
			ofDrawBitmapString(buf[5], 10, txtPos); txtPos = txtPos + 20;
			ofDrawBitmapString(buf[6], 10, txtPos); txtPos = txtPos + 20;
			ofDrawBitmapString(buf[7], 10, txtPos); txtPos = txtPos + 20;
			ofDrawBitmapString(buf[19], 10, windowHeight - 5);
		}
		else {
			buf[1] = "DMX" + ofToString(mirrorIndex + 1) + " [" + ofToString(DMX_Pan_Coarse[mirrorIndex], 3) + string(",") + ofToString(DMX_Tilt_Coarse[mirrorIndex], 3) + "]";
			buf[2] = "OSC" + ofToString(mirrorIndex + 1) + " [" + ofToString(osc_mirrorPosX_Coarse, 6) + string(",") + ofToString(osc_mirrorPosY_Coarse, 6) + "]";
			buf[3] = "Target" + ofToString(mirrorIndex + 1) + " [" + ofToString(ServoPosX_Total, 3) + string(",") + ofToString(ServoPosY_Total, 3) + "]";
			buf[19] = "time(ms)" + ofToString(currentMillis_Main);
			ofDrawBitmapString(buf[1], 10, txtPos); txtPos = txtPos + 20;
			ofDrawBitmapString(buf[2], 10, txtPos); txtPos = txtPos + 20;
			ofDrawBitmapString(buf[3], 10, txtPos); txtPos = txtPos + 20;
			ofDrawBitmapString(buf[19], 10, windowHeight - 5);
		}
	}
	
    ofNoFill();
	ofSetHexColor(0xFFFFFF);

	//Draw PanalXY Rectangle
	ofDrawRectangle(borderLeft, borderTop, windowWidth - (borderLeft + borderRight), windowHeight - (borderTop + borderBottom));
	ofDrawLine(circlePosX, borderTop, circlePosX, windowHeight - borderBottom);
	ofDrawLine(borderLeft, circlePosY, windowWidth - borderRight, circlePosY);
	drawCircle(circlePosX, circlePosY, circleRadius, 0xFF00FF, PanalXY_MouseOver);

	
	if (DMX16bit_Mode) {
		//Draw Slide Rectangle
		ofNoFill();
		ofSetHexColor(0xFFFFFF);

		ofDrawRectangle(windowWidth - 32, borderTop, 16, windowHeight - (borderTop + borderBottom));
		ofDrawRectangle(borderLeft, windowHeight - 32, windowWidth - (borderLeft + borderRight), 16);
		ofDrawLine(SlideX, windowHeight - 32, SlideX, windowHeight - 16);
		ofDrawLine(windowWidth - 32, SlideY, windowWidth - 16, SlideY);

		//Draw Circle Position
		drawCircle(windowWidth - 24, SlideY, 6, slideColor1, SlideY_MouseOver);
		drawCircle(SlideX, windowHeight - 24, 6, slideColor2, SlideX_MouseOver);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key >= '1' && key <= '9') {
		mirrorIndex = key - '1';
	}
	
	if (key == 'd' || key == 'D') {
		bDebugMode = !bDebugMode;
	}

	if ((key == OF_KEY_LEFT) || (key == OF_KEY_RIGHT) || (key == OF_KEY_UP) || (key == OF_KEY_DOWN))
	{
		if (key == OF_KEY_LEFT) {
			if (ObjSelect == 1) {
				if (DMX_Pan_Coarse[mirrorIndex] > 0) {
					DMX_Pan_Coarse[mirrorIndex] = DMX_Pan_Coarse[mirrorIndex] - 1;
					circlePosX = ofMap(DMX_Pan_Coarse[mirrorIndex], 0, 255, borderLeft, windowWidth - borderRight);
				}
			}
			else if ((ObjSelect == 2) && (DMX16bit_Mode == true)) {
				if (DMX_Pan_Fine[mirrorIndex] > 0) {
					DMX_Pan_Fine[mirrorIndex] = DMX_Pan_Fine[mirrorIndex] - 1;
					SlideX = ofMap(DMX_Pan_Fine[mirrorIndex], 0, 255, borderLeft, windowWidth - borderRight);
				}
			}
		}
		else if (key == OF_KEY_RIGHT) {
			if (ObjSelect == 1) {
				if (DMX_Pan_Coarse[mirrorIndex] < 255) {
					DMX_Pan_Coarse[mirrorIndex] = DMX_Pan_Coarse[mirrorIndex] + 1;
					circlePosX = ofMap(DMX_Pan_Coarse[mirrorIndex], 0, 255, borderLeft, windowWidth - borderRight);
				}

			}
			else if ((ObjSelect == 2) && (DMX16bit_Mode == true)) {
				if (DMX_Pan_Fine[mirrorIndex] < 255) {
					DMX_Pan_Fine[mirrorIndex] = DMX_Pan_Fine[mirrorIndex] + 1;
					SlideX = ofMap(DMX_Pan_Fine[mirrorIndex], 0, 255, borderLeft, windowWidth - borderRight);

				}
			}
		}
		
		if (key == OF_KEY_UP) {
			if (ObjSelect == 1) {
				if (DMX_Tilt_Coarse[mirrorIndex] > 0) {
					DMX_Tilt_Coarse[mirrorIndex] = DMX_Tilt_Coarse[mirrorIndex] - 1;
					circlePosY = ofMap(DMX_Tilt_Coarse[mirrorIndex], 0, 255, borderTop, windowHeight - borderBottom);
				}
			}
			else if ((ObjSelect == 3) && (DMX16bit_Mode == true)) {
				if (DMX_Tilt_Fine[mirrorIndex] > 0) {
					DMX_Tilt_Fine[mirrorIndex] = DMX_Tilt_Fine[mirrorIndex] - 1;
					SlideY = ofMap(DMX_Tilt_Fine[mirrorIndex], 0, 255, borderTop, windowHeight - borderBottom);
				}
			}
		}
		else if (key == OF_KEY_DOWN) {
			if (ObjSelect == 1) {
				if (DMX_Tilt_Coarse[mirrorIndex] < 255) {
					DMX_Tilt_Coarse[mirrorIndex] = DMX_Tilt_Coarse[mirrorIndex] + 1;
					circlePosY = ofMap(DMX_Tilt_Coarse[mirrorIndex], 0, 255, borderTop, windowHeight - borderBottom);
				}
			}
			else if ((ObjSelect == 3) && (DMX16bit_Mode == true)) {
				if (DMX_Tilt_Fine[mirrorIndex] < 255) {
					DMX_Tilt_Fine[mirrorIndex] = DMX_Tilt_Fine[mirrorIndex] + 1;
					SlideY = ofMap(DMX_Tilt_Fine[mirrorIndex], 0, 255, borderTop, windowHeight - borderBottom);
				}
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

	if (PanalXY_MouseOver == true) {

		int DMX_Pan_Tmp = ofMap(x, borderLeft, windowWidth - borderRight, 0, 255);
		int DMX_Tilt_Tmp = ofMap(y, borderTop, windowHeight - borderBottom, 0, 255);

		if (DMX_Pan_Tmp < 0)
			DMX_Pan_Coarse[mirrorIndex] = 0;
		else if (DMX_Pan_Tmp > 255)
			DMX_Pan_Coarse[mirrorIndex] = 255;
		else
			DMX_Pan_Coarse[mirrorIndex] = DMX_Pan_Tmp;

		if (DMX_Tilt_Tmp < 0)
			DMX_Tilt_Coarse[mirrorIndex] = 0;
		else if (DMX_Tilt_Tmp > 255)
			DMX_Tilt_Coarse[mirrorIndex] = 255;
		else
			DMX_Tilt_Coarse[mirrorIndex] = DMX_Tilt_Tmp;

		circlePosX = ofMap(DMX_Pan_Coarse[mirrorIndex], 0, 255, borderLeft, windowWidth - borderRight);
		circlePosY = ofMap(DMX_Tilt_Coarse[mirrorIndex], 0, 255, borderTop, windowHeight - borderBottom);
	}
	else if ((SlideX_MouseOver == true) && (DMX16bit_Mode == true)) {
		int DMX_Pan_Tmp  = ofMap(x, borderLeft, windowWidth - borderRight, 0, 255);

		if (DMX_Pan_Tmp < 0)
			DMX_Pan_Fine[mirrorIndex] = 0;
		else if (DMX_Pan_Tmp > 255)
			DMX_Pan_Fine[mirrorIndex] = 255;
		else
			DMX_Pan_Fine[mirrorIndex] = DMX_Pan_Tmp;

		SlideX = ofMap(DMX_Pan_Fine[mirrorIndex], 0, 255, borderLeft, windowWidth - borderRight);

	}
	else if ((SlideY_MouseOver == true) && (DMX16bit_Mode == true)) {
		int DMX_Tilt_Tmp = ofMap(y, borderTop, windowHeight - borderBottom, 0, 255);

		if (DMX_Tilt_Tmp < 0)
			DMX_Tilt_Fine[mirrorIndex] = 0;
		else if (DMX_Tilt_Tmp > 255)
			DMX_Tilt_Fine[mirrorIndex] = 255;
		else
			DMX_Tilt_Fine[mirrorIndex] = DMX_Tilt_Tmp;

		SlideY = ofMap(DMX_Tilt_Fine[mirrorIndex], 0, 255, borderTop, windowHeight - borderBottom);
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	PanalXY_MouseOver  = checkMouseOver(x, y, circlePosX, circlePosY, circleRadius);
	SlideX_MouseOver   = checkMouseOver(x, y, SlideX, windowHeight - 24, circleRadius);
	SlideY_MouseOver   = checkMouseOver(x, y, windowWidth - 24, SlideY, circleRadius);
	

	if (PanalXY_MouseOver) {
		ObjSelect = 1;
	}
	else if (SlideX_MouseOver) {
		ObjSelect = 2;
	}
	else if (SlideY_MouseOver) {
		ObjSelect = 3;
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void  ofApp::drawCircle(float x, float y, float size, int HexColor, bool Active)
{
	ofSetHexColor(HexColor);
	if (Active)
	{
		ofFill();
		ofDrawCircle(x, y, size - 2);
		ofNoFill();
		ofDrawCircle(x, y, size);
	}
	else
	{
		ofFill();
		ofDrawCircle(x, y, size);
	}
}

bool  ofApp::checkMouseOver(float mousePosX, float mousePosY, float objPosX, float objPosY, float distRadius)
{
	float diffx = mousePosX - objPosX;
	float diffy = mousePosY - objPosY;
	float dist = sqrt(diffx*diffx + diffy*diffy);

	if (dist < distRadius) {
		return(true);
	}
	else {
		return(false);
	}
}
