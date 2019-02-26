#include <Adafruit_PWMServoDriver.h>
#include <SPI.h>
#include <SD.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

File myFile;
  Sd2Card card;
  SdVolume volume;
  SdFile root;

#define NUM_SAMPLES 10
#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOCEN  375 // this is the center pulse?
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)

const uint8_t trigPin = 4;
const uint8_t echoPin = 2;
const char s_0[] PROGMEM = "C";
const char s_1[] PROGMEM = "C";
const char s_2[] PROGMEM = "C";
const char s_3[] PROGMEM = "C";
const char s_4[] PROGMEM = "C";
const char s_5[] PROGMEM = "C";
const char s_6[] PROGMEM = "C";
const char s_7[] PROGMEM = "C";

const char* const s_table[] PROGMEM = {s_0, s_1, s_2, s_3, s_4, s_5, s_6, s_7};

int sum0 = 0;

const int xpin = 0;
const int ypin = 1;
const int zpin = 2;

const int chipSelect = 4;
float voltage0 = 0.0;
unsigned char sample_count = 0;

int incomingByte = 0;

uint8_t servonum = 0;

char buffer[15];

void setup() { 

    Serial.begin(9600);

  if (SD.exists("boot.txt")) {
    Serial.println("boot.log exists.");
  } else {
    Serial.println("boot.log doesn't exist.");
  }

myFile = SD.open("boot.txt", FILE_WRITE);
  if (myFile) {
    myFile.println("testing 1, 2, 3.");



  

  pwm.begin();
  pwm.setPWMFreq(60);
  
  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Serial.println(F("SD initialization failed."));
    while (1);
  }
  
  myFile.println("Card type: ");
  switch (card.type()) {
    case SD_CARD_TYPE_SD1:  myFile.println("SD1");  break;
    case SD_CARD_TYPE_SD2:  myFile.println("SD2");  break;
    case SD_CARD_TYPE_SDHC: myFile.println("SDHC"); break;
    default: myFile.println("Unknown");
  }

  if (!volume.init(card)) { myFile.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card"); while (1); }

  Serial.print(F("Clusters:          "));
  Serial.println(volume.clusterCount());
  Serial.print(F("Blocks x Cluster:  "));
  Serial.println(volume.blocksPerCluster());
  Serial.print(F("Total Blocks:      "));
  Serial.println(volume.blocksPerCluster() * volume.clusterCount());
  uint32_t volumesize;
  Serial.print(F("Volume type is:    FAT"));
  Serial.println(volume.fatType(), DEC);
  volumesize = volume.blocksPerCluster();
  volumesize *= volume.clusterCount();
  volumesize /= 2;
  Serial.print(F("Volume size (Kb):  "));
  Serial.println(volumesize);
  Serial.print(F("Volume size (Mb):  "));
  volumesize /= 1024;
  Serial.println(volumesize);
  Serial.print(F("Volume size (Gb):  "));
  Serial.println((float)volumesize / 1024.0);

  Serial.println(F("\nFiles found on the card (name, date and size in bytes): "));
  root.openRoot(volume);

  root.ls(LS_R | LS_DATE | LS_SIZE);

      myFile.close();
  
  } else {
    Serial.println(F("error opening boot.log"));
  }


  
}

void loop(void) {
    while (sample_count < NUM_SAMPLES) {
        sum0 += analogRead(A3);
        sample_count++;
        delay(10);
    }

    int x = analogRead(xpin);
    int y = analogRead(ypin);
    int z = analogRead(zpin);
 
    float zero_G = 512.0;
    float scale = 102.3; 
    
    voltage0 = ((float)sum0 / (float)NUM_SAMPLES * 5.015) / 1024.0;
    
    Serial.print(voltage0 * 11.132); Serial.print (" V IOREF   ");

    Serial.print(((float)x - 331.5)/65*9.8); Serial.print("\t");
    Serial.print(((float)y - 329.5)/68.5*9.8); Serial.print("\t");
    Serial.print(((float)z - 340)/68*9.8); Serial.print("\n");
    
    sample_count = 0;
    sum0 = 0;

  if (Serial.available() > 0) 
  {
    incomingByte = Serial.read();
    Serial.print(F("BYTE: "));
    Serial.println(incomingByte);
    switch (incomingByte)
    {       case 49: servonum = 0; Serial.println(F("S1:")); break;
            case 50: servonum = 1; Serial.println(F("S2:")); break;
            case 51: servonum = 2; Serial.println(F("S3:")); break;                     
            case 52: servonum = 3; Serial.println(F("S4:")); break;                      
            case 53: servonum = 4; Serial.println(F("S5:")); break;
            case 54: servonum = 5; Serial.println(F("S6:")); break;
            case 98: Serial.print(F("B [ "));
            for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) { pwm.setPWM(servonum, 0, pulselen); } break;
            case 99: Serial.print(F("C [ "));
              for (uint16_t pulselen = SERVOMIN; pulselen != SERVOCEN; pulselen++) {pwm.setPWM(servonum, 0, pulselen); } break;
            case 102: Serial.print(F("F [ "));
              for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) { pwm.setPWM(servonum, 0, pulselen); } break;
            default: delay(50);
  }
    delay(500);
  } else {

  uint32_t duration; // duration of the round trip
  uint32_t cm;        // distance of the obstacle

  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(3);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
 
  cm = (uint32_t)((duration<<4)+duration)/1000.0; // cm = 17 * duration/1000

  Serial.print(F(" RANGE1: "));                          
  Serial.print(cm);
  Serial.print(F(" cm"));
  delay(100);

  }

  for (int i = 0; i < 6; i++)
  {
    strcpy_P(buffer, (char*)pgm_read_word(&(s_table[i])));
    Serial.print(F(" Servo["));
    Serial.print(i);
    Serial.print(F("]:"));
    Serial.print(buffer);
    Serial.print(F(" "));
    delay( 100 );
  }
  Serial.println(F(""));

}
