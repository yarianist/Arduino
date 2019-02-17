#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>


Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// called this way, it uses the default address 0x40
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);

#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)

const uint8_t trigPin = 4;
const uint8_t echoPin = 2;

int incomingByte = 0;

// our servo # counter
uint8_t servonum = 0;

void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
}

void setServoPulse(uint8_t n, double pulse) {
  double pulselength;
  
  pulselength = 1000000;   // 1,000,000 us per second
  pulselength /= 60;   // 60 Hz
  Serial.print(pulselength); Serial.println(" us per period"); 
  pulselength /= 4096;  // 12 bits of resolution
  Serial.print(pulselength); Serial.println(" us per bit"); 
  pulse *= 1000;
  pulse /= pulselength;
  Serial.println(pulse);
  pwm.setPWM(n, 0, pulse);
}

void loop() {

        if (Serial.available() > 0) 
	{

                incomingByte = Serial.read();
                Serial.println(incomingByte);
		switch (incomingByte)
		{           
			case 50: for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) { pwm.setPWM(servonum, 0, pulselen); } break;
			case 56: for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) { pwm.setPWM(servonum, 0, pulselen); } break;
			default: delay(500);
		}
                delay(1000);
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
                                
  Serial.print(cm);
  Serial.print(" centimeters");
  Serial.println();
  delay(100);

	}
}
