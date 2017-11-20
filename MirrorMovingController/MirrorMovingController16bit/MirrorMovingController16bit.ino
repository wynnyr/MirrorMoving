#include <SPI.h>
#include <Artnet.h>
#include <Ethernet2.h>
#include <EthernetUdp2.h>
#include <DynamixelSerial1.h>

Artnet artnet;
EthernetClient client;

const int ledPin1 = 3;
const int ledPin2 = 5;
const int btnPin1 = 6;
const int btnPin2 = 7;

int flagbtn1 = 0;


// Servo Address
const int servoPan = 1;
const int servoTilt= 2;


// Servo Position Limit
int servoTiltMin = 622;
int servoTiltMax = 704;
int servoPanMin  = 204;
int servoPanMax  = 820;

int servoTilt_current = 672;
int servoPan_current  = 512;

int servoTilt_target  = servoTilt_current;
int servoPan_target   = servoPan_current;
int servoTilt_target_prev = servoTilt_target;
int servoPan_target_prev  = servoPan_target;

float servoPan_Coarse_Target  = servoPan_current;
float servoTilt_Coarse_Target = servoTilt_current;

float servoPan_Fine_Target  = 0.0;
float servoTilt_Fine_Target = 0.0;

byte ip[]  = {192, 168, 1, 23};
byte mac[] = {0x04, 0xE9, 0xE5, 0x00, 0x69, 0xEC};
int DMX_universe = 0;
int DMX_Addr     = 0;

int  debugMode  = 0;
int  servoError = -1;

int  ledState = LOW; 
int  ledBlinkCount = 0;
unsigned long previousMillis = 0;

unsigned long previousMillis_Tilt = 0;
unsigned long previousMillis_Pan = 0;
unsigned long previousMillis_Main = 0;

int ff=0;

byte DmxdataRecv[] = {0,0,0,0};
byte Dmxsequence = 0;
byte DmxsequencePrev = 0;

void setup() {
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);  
  pinMode(btnPin1, INPUT_PULLUP);
  pinMode(btnPin2, INPUT_PULLUP); 

  digitalWrite(ledPin1,HIGH);
  digitalWrite(ledPin2,HIGH);

  Serial.begin(9600);
  Dynamixel.begin(1000000, 2);
  Dynamixel.moveSpeed (servoPan  ,servoPan_current,300);
  Dynamixel.moveSpeed (servoTilt ,servoTilt_current,300); 

  Dynamixel.setCMargin(servoPan,0,0);
  Dynamixel.setCMargin(servoTilt,0,0);
  Dynamixel.setCSlope(servoPan,254,254);
  Dynamixel.setCSlope(servoTilt,254,254);
  
  artnet.begin(mac, ip);
  artnet.setArtDmxCallback(onDmxFrame);

  
}

void loop() {
  unsigned long currentMillis_Main = millis();
  
  artnet.read();
  ledBlink();

  if (currentMillis_Main - previousMillis_Main >= 1){
    previousMillis_Main = currentMillis_Main;

    //servoPan_target  = ceil(servoPan_Coarse_Target  + servoPan_Fine_Target);
    //servoTilt_target = ceil(servoTilt_Coarse_Target + servoTilt_Fine_Target);

    //if(servoPan_target != servoPan_target_prev){
    //  servoPan_target_prev = servoPan_target;
    //  ff = 1;
    //}

    //if(servoTilt_target != servoTilt_target_prev){
    //  servoTilt_target_prev = servoTilt_target; 
    //  ff = 1;
    //}

    if(Dmxsequence != DmxsequencePrev){
      DmxsequencePrev = Dmxsequence;
      //Serial.print("[Pan,Tilt] - [");
      //Serial.print(servoPan_target);
      //Serial.print(" , ");
      //Serial.print(servoTilt_target);
      //Serial.println("]");
      Serial.print("[DMX Data] - ");
      Serial.print(Dmxsequence);
      Serial.print(" [");
      Serial.print(DmxdataRecv[0]);
      Serial.print(" ");
      Serial.print(DmxdataRecv[1]);
      Serial.print(" ");
      Serial.print(DmxdataRecv[2]);
      Serial.print(" ");
      Serial.print(DmxdataRecv[3]);
      Serial.println("]");
    }

    //Dynamixel.moveSpeed(servoPan ,  servoPan_target,1023) ;
    //Dynamixel.moveSpeed(servoTilt , servoTilt_target,1023) ;


  }

  if (digitalRead(btnPin1) == LOW && flagbtn1 == 0){
      flagbtn1 = 1;
      //int Pan_Position  = Dynamixel.readPosition(servoPan);
      //int Tilt_Position = Dynamixel.readPosition(servoTilt);
      //Serial.print("Pan_Pos:");
      //Serial.print(Pan_Position);
      //Serial.print(" Tilt_Pos:");
      //Serial.println(Tilt_Position);
      Serial.print("[DMX Data] - [");
      Serial.print(DmxdataRecv[0]);
      Serial.print(" ");
      Serial.print(DmxdataRecv[1]);
      Serial.print(" ");
      Serial.print(DmxdataRecv[2]);
      Serial.print(" ");
      Serial.print(DmxdataRecv[3]);
      Serial.println("]");
    }

    if (digitalRead(btnPin2) == LOW && flagbtn1 == 1){
      flagbtn1 = 0;
    }
}

void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data)
{
  if(universe == DMX_universe){
    Dmxsequence = sequence;
    DmxdataRecv[0]= data[DMX_Addr];
    DmxdataRecv[1]= data[DMX_Addr+1];
    DmxdataRecv[2]= data[DMX_Addr+2];
    DmxdataRecv[3]= data[DMX_Addr+3];

    //servoPan_Coarse_Target  = map(data[DMX_Addr],   0, 255, servoPanMin, servoPanMax);
    //servoPan_Fine_Target    = map(data[DMX_Addr+1], 0, 255, 0, (servoPanMax - servoPanMin)/255.0);
    //servoTilt_Coarse_Target = map(data[DMX_Addr+2], 0, 255, servoTiltMin, servoTiltMax);
    //servoTilt_Fine_Target   = map(data[DMX_Addr+3], 0, 255, 0, (servoTiltMax - servoTiltMin)/255.0);
    
    digitalWrite(ledPin2,LOW);
  }
  else
  {
    digitalWrite(ledPin2,HIGH);
  }
  
  ledBlinkCount = 50;
}

void ledBlink(void){
  static long interval;
  unsigned long currentMillis = millis();
  
  if (ledBlinkCount >= 1){
    ledBlinkCount = ledBlinkCount-1;
    interval=50;
  }
  else{
    digitalWrite(ledPin2,HIGH); 
    interval=500;
  }
    
  if (currentMillis - previousMillis >= interval){
    previousMillis = currentMillis;
    
    if (ledState == LOW){
      ledState = HIGH;
    } else{
      ledState = LOW;
    }

    digitalWrite(ledPin1,ledState);
  }
}

