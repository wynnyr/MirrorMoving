#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowTitle("Mirror Remote");

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
		circlePosX[i] = ofMap(ServoPan_min + ((ServoPan_max - ServoPan_min)   * 0.5), ServoPan_min, ServoPan_max, borderLeft, windowWidth - borderRight);
		circlePosY[i] = ofMap(ServoTilt_min + ((ServoTilt_max - ServoTilt_min) * 0.5), ServoTilt_min, ServoTilt_max, borderTop, windowHeight - borderBottom);
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	ofxOscMessage m;

	windowWidth = ofGetWindowWidth();
	windowHeight = ofGetWindowHeight();

	mirrorPosX = ofMap(circlePosX[mirrorIndex], borderLeft, windowWidth - borderRight, ServoPan_min, ServoPan_max) - (ServoPan_min  * 1.0);
	mirrorPosY = ofMap(circlePosY[mirrorIndex], borderTop, windowHeight - borderBottom, ServoTilt_min, ServoTilt_max ) - (ServoTilt_min * 1.0);

	dmx_mirrorPosX_Coarse = ofMap(mirrorPosX, 0, Servo_Pan_Res, 0, 255);
	dmx_mirrorPosY_Coarse = ofMap(mirrorPosY, 0, Servo_Tilt_Res, 0, 255);

	ServoPosX_Coarse = ofMap(dmx_mirrorPosX_Coarse, 0, 255, ServoPan_min, ServoPan_max) - (ServoPan_min  * 1.0);
	ServoPosY_Coarse = ofMap(dmx_mirrorPosY_Coarse, 0, 255, ServoTilt_min, ServoTilt_max) - (ServoTilt_min * 1.0);

	ServoPosX_Error = mirrorPosX - ServoPosX_Coarse;
	ServoPosY_Error = mirrorPosY - ServoPosY_Coarse;

	dmx_mirrorPosX_Fine = ofMap(ServoPosX_Error, 0.0, (Servo_Pan_Res / 255.0), 0, 255);
	dmx_mirrorPosY_Fine = ofMap(ServoPosY_Error, 0.0, (Servo_Tilt_Res / 255.0), 0, 255);

	ServoPosX_Error_Check = ofMap(dmx_mirrorPosX_Fine, 0, 255, 0, (Servo_Pan_Res / 255.0));
	ServoPosY_Error_Check = ofMap(dmx_mirrorPosY_Fine, 0, 255, 0, (Servo_Tilt_Res / 255.0));

	ServoPosX_Total = ServoPosX_Coarse + ServoPosX_Error_Check;
	ServoPosY_Total = ServoPosY_Coarse + ServoPosY_Error_Check;

	osc_mirrorPosX_Coarse = ofMap(dmx_mirrorPosX_Coarse, 0, 255, 0.0, 1.0);
	osc_mirrorPosY_Coarse = ofMap(dmx_mirrorPosY_Coarse, 0, 255, 0.0, 1.0);
	osc_mirrorPosX_Fine = ofMap(dmx_mirrorPosX_Fine, 0, 255, 0.0, 1.0);
	osc_mirrorPosY_Fine = ofMap(dmx_mirrorPosY_Fine, 0, 255, 0.0, 1.0);

	currentMillis_Main = ofGetElapsedTimeMillis();
	
	if ((currentMillis_Main - previousMillis_Main) > 10) {
		previousMillis_Main = currentMillis_Main;

		if (fsend == 1) {
			
			if (osc_mirrorPosX_Coarse != osc_mirrorPosX_Coarse_prev) {
				osc_mirrorPosX_Coarse_prev = osc_mirrorPosX_Coarse;
				m.setAddress("/dmx/fader" + ofToString((mirrorIndex * 4) + 1));
				m.addFloatArg(osc_mirrorPosX_Coarse);
				sender.sendMessage(m, false);
			}
			fsend = 2;
		}
		else if (fsend == 2) {
			
			if (osc_mirrorPosX_Fine != osc_mirrorPosX_Fine_prev) {
				osc_mirrorPosX_Fine_prev = osc_mirrorPosX_Fine;
				m.setAddress("/dmx/fader" + ofToString((mirrorIndex * 4) + 2));
				m.addFloatArg(osc_mirrorPosX_Fine);
				sender.sendMessage(m, false);
			}
			fsend = 3;
		}
		else if (fsend == 3) {
			
			if (osc_mirrorPosY_Coarse != osc_mirrorPosY_Coarse_prev) {
				osc_mirrorPosY_Coarse_prev = osc_mirrorPosY_Coarse;
				m.setAddress("/dmx/fader" + ofToString((mirrorIndex * 4) + 3));
				m.addFloatArg(osc_mirrorPosY_Coarse);
				sender.sendMessage(m, false);
			}
			fsend = 4;
		}
		else if (fsend == 4) {
			if (osc_mirrorPosY_Fine != osc_mirrorPosY_Fine_prev) {
				osc_mirrorPosY_Fine_prev = osc_mirrorPosY_Fine;
				m.setAddress("/dmx/fader" + ofToString((mirrorIndex * 4) + 4));
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
		buf[1] = "mirror " + ofToString(mirrorIndex + 1) + " Pos [" + ofToString(mirrorPosX, 3) + string(",") + ofToString(mirrorPosY, 3) + "]";


		buf[2] = "OSC_X " + ofToString(mirrorIndex + 1) + " Pos [" + ofToString(osc_mirrorPosX_Coarse, 6) + string(",") + ofToString(osc_mirrorPosX_Fine, 6) + "]";
		buf[3] = "OSC_Y " + ofToString(mirrorIndex + 1) + " Pos [" + ofToString(osc_mirrorPosY_Coarse, 6) + string(",") + ofToString(osc_mirrorPosY_Fine, 6) + "]";
		buf[4] = "DMX_X " + ofToString(mirrorIndex + 1) + " Pos [" + ofToString(dmx_mirrorPosX_Coarse, 3) + string(",") + ofToString(dmx_mirrorPosX_Fine, 3) + "]";
		buf[5] = "DMX_Y " + ofToString(mirrorIndex + 1) + " Pos [" + ofToString(dmx_mirrorPosY_Coarse, 3) + string(",") + ofToString(dmx_mirrorPosY_Fine, 3) + "]";

		buf[6] = "Servo Coarse " + ofToString(mirrorIndex + 1) + " Pos [" + ofToString(ServoPosX_Coarse, 3) + string(",") + ofToString(ServoPosY_Coarse, 3) + "]";
		buf[7] = "Error " + ofToString(mirrorIndex + 1) + " Pos [" + ofToString(ServoPosX_Error, 3) + string(",") + ofToString(ServoPosX_Error, 3) + "]";
		buf[8] = "Error Check " + ofToString(mirrorIndex + 1) + " Pos [" + ofToString(ServoPosX_Error_Check, 3) + string(",") + ofToString(ServoPosY_Error_Check, 3) + "]";
		buf[9] = "Servo Target" + ofToString(mirrorIndex + 1) + " Pos [" + ofToString(ServoPosX_Total, 3) + string(",") + ofToString(ServoPosY_Total, 3) + "]";

		buf[19] = "time(ms)" + ofToString(currentMillis_Main);


		

		
		ofDrawBitmapString(buf[1], 10, txtPos); txtPos = txtPos + 20;
		ofDrawBitmapString(buf[2], 10, txtPos); txtPos = txtPos + 20;
		ofDrawBitmapString(buf[3], 10, txtPos); txtPos = txtPos + 20;
		ofDrawBitmapString(buf[4], 10, txtPos); txtPos = txtPos + 20;
		ofDrawBitmapString(buf[5], 10, txtPos); txtPos = txtPos + 20;
		ofDrawBitmapString(buf[6], 10, txtPos); txtPos = txtPos + 20;
		ofDrawBitmapString(buf[7], 10, txtPos); txtPos = txtPos + 20;
		ofDrawBitmapString(buf[8], 10, txtPos); txtPos = txtPos + 20;
		ofDrawBitmapString(buf[9], 10, txtPos); txtPos = txtPos + 20;
		ofDrawBitmapString(buf[19], 10, windowHeight - 15);
	}
	
	

	ofNoFill();
	ofSetHexColor(0xFFFFFF);

	//Draw PanalXY Rectangle
	ofDrawRectangle(borderLeft, borderTop, windowWidth - (borderLeft + borderRight), windowHeight - (borderTop + borderBottom));
	ofDrawLine(circlePosX[mirrorIndex], borderTop, circlePosX[mirrorIndex], windowHeight - borderBottom);
	ofDrawLine(borderLeft, circlePosY[mirrorIndex], windowWidth - borderRight, circlePosY[mirrorIndex]);

	//Draw Slide Rectangle
	ofDrawRectangle(windowWidth - 32, borderTop, 16, windowHeight - (borderTop + borderBottom));
	ofDrawRectangle(borderLeft, windowHeight - 32, windowWidth - (borderLeft + borderRight), 16);
	ofDrawLine(windowWidth - 32, SlideVer[mirrorIndex], windowWidth - 16, SlideVer[mirrorIndex]);
	ofDrawLine(SlideHor[mirrorIndex], windowHeight - 32, SlideHor[mirrorIndex], windowHeight - 16);



	//Draw Circle Position
	if (ObjSelect == 1) {
		drawCircle(circlePosX[mirrorIndex], circlePosY[mirrorIndex], circleRadius, 0xFF00FF, true);
		drawCircle(windowWidth - 24, SlideVer[mirrorIndex], 6, slideColor1, false);
		drawCircle(SlideHor[mirrorIndex], windowHeight - 24, 6, slideColor2, false);
	}
	else if (ObjSelect == 2) {
		drawCircle(circlePosX[mirrorIndex], circlePosY[mirrorIndex], circleRadius, 0xFF00FF, false);
		drawCircle(windowWidth - 24, SlideVer[mirrorIndex], 6, slideColor1, true);
		drawCircle(SlideHor[mirrorIndex], windowHeight - 24, 6, slideColor2, false);
	}
	else if (ObjSelect == 3) {
		drawCircle(circlePosX[mirrorIndex], circlePosY[mirrorIndex], circleRadius, 0xFF00FF, false);
		drawCircle(windowWidth - 24, SlideVer[mirrorIndex], 6, slideColor1, false);
		drawCircle(SlideHor[mirrorIndex], windowHeight - 24, 6, slideColor2, true);
	}
	else {
		drawCircle(circlePosX[mirrorIndex], circlePosY[mirrorIndex], circleRadius, 0xFF00FF, false);
		drawCircle(windowWidth - 24, SlideVer[mirrorIndex], 6, slideColor1, false);
		drawCircle(SlideHor[mirrorIndex], windowHeight - 24, 6, slideColor2, false);

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
			float circlePosX_tmp = ofMap(mirrorPosX + ServoPan_min - 1, ServoPan_min, ServoPan_max, borderLeft, windowWidth - borderRight);

			if (circlePosX_tmp > borderLeft) {
				circlePosX[mirrorIndex] = circlePosX_tmp;
			}
			else {
				circlePosX[mirrorIndex] = borderLeft;
			}
		}
		else if (key == OF_KEY_RIGHT) {
			float circlePosX_tmp = ofMap(mirrorPosX + ServoPan_min + 1, ServoPan_min, ServoPan_max, borderLeft, windowWidth - borderRight);
			if (circlePosX_tmp < (windowWidth - borderRight)) {
				circlePosX[mirrorIndex] = circlePosX_tmp;
			}
			else {
				circlePosX[mirrorIndex] = windowWidth - borderRight;
			}
		}
		if (key == OF_KEY_UP) {
			float circlePosY_tmp = ofMap(mirrorPosY + ServoTilt_min - 1, ServoTilt_min, ServoTilt_max, borderTop, windowHeight - borderBottom);
			if (circlePosY_tmp > borderTop) {
				circlePosY[mirrorIndex] = circlePosY_tmp;
			}
			else {
				circlePosY[mirrorIndex] = borderTop;
			}
		}
		else if (key == OF_KEY_DOWN) {
			float circlePosY_tmp = ofMap(mirrorPosY + ServoTilt_min + 1, ServoTilt_min, ServoTilt_max, borderTop, windowHeight - borderBottom);
			if (circlePosY_tmp < (windowHeight - borderBottom)) {
				circlePosY[mirrorIndex] = circlePosY_tmp;
			}
			else {
				circlePosY[mirrorIndex] = windowHeight - borderBottom;
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
void ofApp::mouseDragged(int x, int y, int button){
	if (PanalXY_MouseOver == true) {
		int i_mirrorPosX = ofMap(x, borderLeft, windowWidth - borderRight, ServoPan_min, ServoPan_max) - (ServoPan_min  * 1.0);
		int i_mirrorPosY = ofMap(y, borderTop, windowHeight - borderBottom, ServoTilt_min, ServoTilt_max) - (ServoTilt_min * 1.0);

		float xTmp = ofMap(i_mirrorPosX + ServoPan_min, ServoPan_min, ServoPan_max, borderLeft, windowWidth - borderRight);
		float yTmp = ofMap(i_mirrorPosY + ServoTilt_min, ServoTilt_min, ServoTilt_max, borderTop, windowHeight - borderBottom);

		if ((xTmp < borderLeft)) {
			circlePosX[mirrorIndex] = borderLeft;
		}
		else if (xTmp >(windowWidth - borderRight)) {
			circlePosX[mirrorIndex] = windowWidth - borderRight;
		}
		else {
			circlePosX[mirrorIndex] = xTmp;
		}


		if (yTmp < borderTop) {
			circlePosY[mirrorIndex] = borderTop;
		}
		else if (yTmp >(windowHeight - borderBottom)) {
			circlePosY[mirrorIndex] = windowHeight - borderBottom;
		}
		else {
			circlePosY[mirrorIndex] = yTmp;
		}
	}
	else if (SlideVer_MouseOver == true) {
		if ((y >= borderTop) && y <= (windowHeight - borderBottom)) {
			SlideVer[mirrorIndex] = y;
		}
	}
	else if (SlideHor_MouseOver == true) {
		if ((x >= borderLeft) && x <= (windowWidth - borderRight)) {
			SlideHor[mirrorIndex] = x;
		}
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	PanalXY_MouseOver = checkMouseOver(x, y, circlePosX[mirrorIndex], circlePosY[mirrorIndex], circleRadius);

	PanalXY_MouseOver = checkMouseOver(x, y, circlePosX[mirrorIndex], circlePosY[mirrorIndex], circleRadius);
	SlideVer_MouseOver = checkMouseOver(x, y, windowWidth - 24, SlideVer[mirrorIndex], circleRadius);
	SlideHor_MouseOver = checkMouseOver(x, y, SlideHor[mirrorIndex], windowHeight - 24, circleRadius);

	if (PanalXY_MouseOver) {
		ObjSelect = 1;
	}

	else if (SlideVer_MouseOver) {
		ObjSelect = 2;
	}
	else if (SlideHor_MouseOver) {
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
