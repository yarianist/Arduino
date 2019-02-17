#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// called this way, it uses the default address 0x40 (Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);)

// Variable Block

char inData[20]; // Allocate some space for the string
char inChar=-1; // Where to store the character read
byte index = 0; // Index into array; where to store the character

#define SERVOCEN  355 // this is the center pulse?
#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  625 // this is the 'maximum' pulse length count (out of 4096)


const uint8_t trigPin = 4;
const uint8_t echoPin = 2;

uint8_t servonum = 0;

//Prog Start


void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(60);
  //Reset servos to center on startup
  for (int i=0; i >= 6; i++){
       servonum = 0;
       for (uint16_t pulselen = SERVOMIN; pulselen < SERVOCEN; pulselen++) { pwm.setPWM(servonum, 0, pulselen); } break;
       delay(500);
  }
}

char Comp(char* This) {
    while (Serial.available() > 0) // Don't read unless
                                   // there you know there is data
    {
        if(index < 19) // One less than the size of the array
        {
            inChar = Serial.read(); // Read a character
            inData[index] = inChar; // Store it
            index++; // Increment where to write next
            inData[index] = '\0'; // Null terminate the string
        }
    }

    if (strcmp(inData,This)  == 0) {
        for (int i=0;i<19;i++) {
            inData[i]=0;
        }
        index=0;
        return(0);
    }
    else {
        return(1);
    }
}



void loop() {
        if (Serial.available() > 0) {
                // troubleshooting
                // Serial.print("BYTE: ");
                // Serial.print(incomingByte);
                // Serial.println(" ");
               
		switch (Comp) {
                        case "49": servonum = 0; Serial.println("S1:"); break;
                        case '51': servonum = 2; Serial.println("S3:"); break;                     
                        case "52": servonum = 3; Serial.println("S4:"); break;                      
                        case "53": servonum = 4; Serial.println("S5:"); break;
                        case "54": servonum = 5; Serial.println("S6:"); break;
                        case "98": Serial.print("B [ ");
                                 for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) { pwm.setPWM(servonum, 0, pulselen); } 
                                 break;
                        case "99": Serial.print("C [ ");
                                 for (uint16_t pulselen = SERVOMIN; pulselen != SERVOCEN; pulselen++) {pwm.setPWM(servonum, 0, pulselen); }
                                 break;
 			case "102": Serial.print("F [ ");
                                 for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) { pwm.setPWM(servonum, 0, pulselen); }
                                 break;
			default: delay(50);
		}
                delay(500);
        } else {
               Serial.println("INPUT!");
               delay (1000);
                   if (Comp("m1 on")==0) {
        Serial.write("Motor 1 -> Online\n");
    }
    if (Comp("m1 off")==0) {
        Serial.write("Motor 1 -> Offline\n");
    }
	}
        Serial.println("");
}


