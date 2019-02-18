#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <avr/pgmspace.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// called this way, it uses the default address 0x40
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);

#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOCEN  375 // this is the center pulse?
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)

const uint8_t trigPin = 4;
const uint8_t echoPin = 2;
const char s_0[] PROGMEM = "C";   // "String 0" etc are strings to store - change to suit.
const char s_1[] PROGMEM = "C";
const char s_2[] PROGMEM = "C";
const char s_3[] PROGMEM = "C";
const char s_4[] PROGMEM = "C";
const char s_5[] PROGMEM = "C";

const char* const s_table[] PROGMEM = {s_0, s_1, s_2, s_3, s_4, s_5};

int incomingByte = 0;

// our servo # counter
uint8_t servonum = 0;

char buffer[30];





void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
}


void loop() {
        if (Serial.available() > 0) 
	{
                incomingByte = Serial.read();
                //Serial.print("BYTE: ");
                //Serial.print(incomingByte);
                //Serial.println(" ");
		switch (incomingByte)
		{       case 49: servonum = 0; Serial.println("S1:"); break;
                        case 50: servonum = 1; Serial.println("S2:"); break;
                        case 51: servonum = 2; Serial.println("S3:"); break;                     
                        case 52: servonum = 3; Serial.println("S4:"); break;                      
                        case 53: servonum = 4; Serial.println("S5:"); break;
                        case 54: servonum = 5; Serial.println("S6:"); break;
                        case 98: Serial.print("B [ ");
                          for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) { pwm.setPWM(servonum, 0, pulselen); } break;
                        case 99: Serial.print("C [ ");
                          for (uint16_t pulselen = SERVOMIN; pulselen != SERVOCEN; pulselen++) {pwm.setPWM(servonum, 0, pulselen); } break;
 			case 102: Serial.print("F [ ");
                          for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) { pwm.setPWM(servonum, 0, pulselen); } break;
			default: delay(50);
		}
                delay(500);
        } else {

  uint32_t duration; // duration of the round trip
  uint32_t cm;        // distance of the obstacle
 
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(3);


  // Start trigger signal


  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.

  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
 
  // convert the time into a distance
  cm = (uint32_t)((duration<<4)+duration)/1000.0; // cm = 17 * duration/1000

  Serial.print(" RANGE1: ");                          
  Serial.print(cm);
  Serial.print(" cm");
  delay(100);

	}

  for (int i = 0; i < 6; i++)
  {
    strcpy_P(buffer, (char*)pgm_read_word(&(s_table[i]))); // Necessary casts and dereferencing, just copy.
    Serial.print(" Servo[");
    Serial.print(i);
    Serial.print("]:");
    Serial.print(buffer);
    Serial.print(" ");
    delay( 100 );
  }
  Serial.println("");
}
