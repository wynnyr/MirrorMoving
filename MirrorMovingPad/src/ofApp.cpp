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

	//mirrorPosX = ofMap(circlePosX[mirrorIndex], borderLeft, windowWidth - borderRight, ServoPan_min, ServoPan_max) - (ServoPan_min  * 1.0);
	//mirrorPosY = ofMap(circlePosY[mirrorIndex], borderTop, windowHeight - borderBottom, ServoTilt_min, ServoTilt_max ) - (ServoTilt_min * 1.0);
/*
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
	*/
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
		buf[2] = "DMX_X " + ofToString(mirrorIndex + 1)  + " Pos [" + ofToString(DMX_Pan_Coarse[mirrorIndex], 3)  + string(",") + ofToString(DMX_Pan_Fine[mirrorIndex], 3)  + "]";
		buf[3] = "DMX_Y " + ofToString(mirrorIndex + 1)  + " Pos [" + ofToString(DMX_Tilt_Coarse[mirrorIndex], 3) + string(",") + ofToString(DMX_Tilt_Fine[mirrorIndex], 3) + "]";

		buf[4] = "OSC_X " + ofToString(mirrorIndex + 1) + " Pos [" + ofToString(osc_mirrorPosX_Coarse, 6) + string(",") + ofToString(osc_mirrorPosX_Fine, 6) + "]";
		buf[5] = "OSC_Y " + ofToString(mirrorIndex + 1) + " Pos [" + ofToString(osc_mirrorPosY_Coarse, 6) + string(",") + ofToString(osc_mirrorPosY_Fine, 6) + "]";


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
		ofDrawBitmapString(buf[10], 10, txtPos); txtPos = txtPos + 20;
		ofDrawBitmapString(buf[19], 10, windowHeight - 15);
	}
	
	

	ofNoFill();
	ofSetHexColor(0xFFFFFF);

	//Draw PanalXY Rectangle
	ofDrawRectangle(borderLeft, borderTop, windowWidth - (borderLeft + borderRight), windowHeight - (borderTop + borderBottom));
	ofDrawLine(circlePosX, borderTop, circlePosX, windowHeight - borderBottom);
	ofDrawLine(borderLeft, circlePosY, windowWidth - borderRight, circlePosY);

	//Draw Slide Rectangle
	ofDrawRectangle(windowWidth - 32, borderTop, 16, windowHeight - (borderTop + borderBottom));
	ofDrawRectangle(borderLeft, windowHeight - 32, windowWidth - (borderLeft + borderRight), 16);
	ofDrawLine(SlideX, windowHeight - 32, SlideX, windowHeight - 16);
	ofDrawLine(windowWidth - 32, SlideY, windowWidth - 16, SlideY);


	//Draw Circle Position
	if (ObjSelect == 1) {
		drawCircle(circlePosX, circlePosY, circleRadius, 0xFF00FF, true);
		drawCircle(windowWidth - 24, SlideY, 6, slideColor1, false);
		drawCircle(SlideX, windowHeight - 24, 6, slideColor2, false);
	}
	else if (ObjSelect == 2) {
		drawCircle(circlePosX, circlePosY, circleRadius, 0xFF00FF, false);
		drawCircle(SlideX, windowHeight - 24, 6, slideColor2, true);
		drawCircle(windowWidth - 24, SlideY, 6, slideColor1, false);
	}
	else if (ObjSelect == 3) {
		drawCircle(circlePosX, circlePosY, circleRadius, 0xFF00FF, false);
		drawCircle(SlideX, windowHeight - 24, 6, slideColor2, false);
		drawCircle(windowWidth - 24, SlideY, 6, slideColor1, true);
	}
	else {
		drawCircle(circlePosX, circlePosY, circleRadius, 0xFF00FF, false);
		drawCircle(SlideX, windowHeight - 24, 6, slideColor2, false);
		drawCircle(windowWidth - 24, SlideY, 6, slideColor1, false);
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
			else if (ObjSelect == 2) {
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
			else if (ObjSelect == 2) {
				if (DMX_Pan_Fine[mirrorIndex] > 0) {
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
			else if (ObjSelect == 3) {
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
			else if (ObjSelect == 3) {
				if (DMX_Tilt_Fine[mirrorIndex] > 0) {
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
void ofApp::mouseDragged(int x, int y, int button){
	
	if (PanalXY_MouseOver == true) {

		int DMX_Pan_Tmp  = ofMap(x, borderLeft, windowWidth  - borderRight,   0, 255);
		int DMX_Tilt_Tmp = ofMap(y, borderTop,  windowHeight - borderBottom,  0, 255);

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

		circlePosX = ofMap(DMX_Pan_Coarse[mirrorIndex],  0, 255, borderLeft, windowWidth  - borderRight);
		circlePosY = ofMap(DMX_Tilt_Coarse[mirrorIndex], 0, 255, borderTop,  windowHeight - borderBottom);
	}

/*
	else if (SlideHor_MouseOver == true) {
		int i_mirrorPosX = ofMap(x, borderLeft, windowWidth - borderRight, ServoPan_min, ServoPan_max) - (ServoPan_min  * 1.0);
		float xTmp = ofMap(i_mirrorPosX + ServoPan_min, ServoPan_min, ServoPan_max, borderLeft, windowWidth - borderRight);

		if ((xTmp >= borderLeft) && xTmp <= (windowWidth - borderRight)) {
			SlideX = xTmp;
		}
	}
	else if (SlideVer_MouseOver == true) {
		
		int i_mirrorPosY = ofMap(y, borderTop, windowHeight - borderBottom, ServoTilt_min, ServoTilt_max) - (ServoTilt_min * 1.0);
		float yTmp = ofMap(i_mirrorPosY + ServoTilt_min, ServoTilt_min, ServoTilt_max, borderTop, windowHeight - borderBottom);

		if ((yTmp >= borderTop) && yTmp <= (windowHeight - borderBottom)) {
			SlideX = yTmp;
		}
	}
*/
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	PanalXY_MouseOver  = checkMouseOver(x, y, circlePosX, circlePosY, circleRadius);
	SlideX_MouseOver = checkMouseOver(x, y, SlideX, windowHeight - 24, circleRadius);
	SlideY_MouseOver = checkMouseOver(x, y, windowWidth - 24, SlideY, circleRadius);
	

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
