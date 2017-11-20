#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

#define HOST "192.168.1.115"
#define PORT 8000

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void drawCircle(float x, float y, float size, int HexColor, bool Active);
		bool checkMouseOver(float mousePosX, float mousePosY, float objPosX, float objPosY, float distRadius);


		ofTrueTypeFont font;
		ofxOscSender sender;

		int mirrorPosX = 0;
		int mirrorPosY = 0;

		int   mirrorIndex = 0;
		int	  circleRadius = 12;
		float circlePosX[9] = { 0,0,0,0,0,0,0,0,0 };
		float circlePosY[9] = { 0,0,0,0,0,0,0,0,0 };

		bool PanalXY_MouseOver = false;

		int borderLeft = 30;
		int borderRight = 30;
		int borderTop = 80;
		int borderBottom = 30;

		int windowWidth;
		int windowHeight;
		int window_W_Res;
		int window_H_Res;

		int drawFirst = 0;

		int loop_send_count = 0;

		int ServoPan_min;
		int ServoPan_max;
		int ServoTilt_min;
		int ServoTilt_max;
		int Servo_Pan_Res;
		int	Servo_Tilt_Res;

		float osc_mirrorPosX_Coarse = 0;
		float osc_mirrorPosX_Fine = 0;
		float osc_mirrorPosY_Coarse = 0;
		float osc_mirrorPosY_Fine = 0;

		float osc_mirrorPosX_Coarse_prev = 0;
		float osc_mirrorPosX_Fine_prev = 0;
		float osc_mirrorPosY_Coarse_prev = 0;
		float osc_mirrorPosY_Fine_prev = 0;

		float f_mirrorPosX_Coarse;
		float f_mirrorPosY_Coarse;

		int   dmx_mirrorPosX_Coarse;
		int   dmx_mirrorPosY_Coarse;
		int   dmx_mirrorPosX_Fine;
		int   dmx_mirrorPosY_Fine;

		float ServoPosX_Total;
		float ServoPosY_Total;
		float ServoPosX_Coarse;
		float ServoPosY_Coarse;
		float ServoPosX_Fine;
		float ServoPosY_Fine;
		float ServoPosX_Error;
		float ServoPosY_Error;

		float ServoPosX_Error_Check;
		float ServoPosY_Error_Check;
};
