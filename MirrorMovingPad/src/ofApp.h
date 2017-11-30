#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

#define HOST "192.168.1.117"
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

		vector<string> arguments;

		ofTrueTypeFont font;
		ofxOscSender sender;

		uint64_t previousMillis_Main;
		uint64_t currentMillis_Main;

		bool DMX16bit_Mode = false;
		bool bDebugMode = 0;

		int fsend=1;

		int borderLeft ;
		int borderRight;
		int borderTop;
		int borderBottom;

		int windowWidth;
		int windowHeight;
		int window_W_Res;
		int window_H_Res;

		
		int DMX_Pan_Coarse[9]  = { 0,0,0,0,0,0,0,0,0 };
		int DMX_Tilt_Coarse[9] = { 0,0,0,0,0,0,0,0,0 };
		int DMX_Pan_Fine[9]    = { 0,0,0,0,0,0,0,0,0 };
		int DMX_Tilt_Fine[9]   = { 0,0,0,0,0,0,0,0,0 };

		int   mirrorIndex  = 0;
		int	  circleRadius = 10;
		float circlePosX;
		float circlePosY;
		float SlideX;
		float SlideY;

		bool SlideX_MouseOver  = false;
		bool SlideY_MouseOver  = false;
		bool PanalXY_MouseOver = false;

		int ObjSelect = 0;
		int slideColor1 = 0x2200FF;
		int slideColor2 = 0x2200FF;

		int drawFirst = 0;

		int loop_send_count = 0;

		int ServoPan_min;
		int ServoPan_max;
		int ServoTilt_min;
		int ServoTilt_max;
		int Servo_Pan_Res;
		int	Servo_Tilt_Res;

		float osc_mirrorPosX_Coarse = 0;
		float osc_mirrorPosX_Fine   = 0;
		float osc_mirrorPosY_Coarse = 0;
		float osc_mirrorPosY_Fine   = 0;

		float osc_mirrorPosX_Coarse_prev = 0;
		float osc_mirrorPosX_Fine_prev   = 0;
		float osc_mirrorPosY_Coarse_prev = 0;
		float osc_mirrorPosY_Fine_prev   = 0;

		float ServoPosX_Total;
		float ServoPosY_Total;
		float ServoPosX_Coarse;
		float ServoPosY_Coarse;
		float ServoPosX_Fine;
		float ServoPosY_Fine;

};
