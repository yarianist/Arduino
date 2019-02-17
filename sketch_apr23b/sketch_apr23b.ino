/*
 *  Nrf2401 library example 2
 *  April 2009 | jesse dot tane at gmail dot com
 *
 *  If two Arduinos are running this program, they will
 *  automatically detect each other and synchronize their
 *  transmission and reception in a very simple "blocking" 
 *  scheme - alternately blinking an LED for 1 second at a 
 *  time. In this example, a receiver will "block" (not 
 *  send any data) until it has successfully received data. 
 *  The synchronization takes place at the beginning of
 *  the program only however, so if the connection is lost 
 *  mid-program, it could hang.
 *
 */

#include "Nrf2401.h"

Nrf2401 Radio;
volatile bool linkEstablished = false;
unsigned char hello = 128;

// note that the "linkEstablished" variable is declared volatile
// this prevents the compiler from optimizing it out as it is only 
// modified by the interrupt handler - a function that we never directly call

void setup(void)
{
  pinMode(13, OUTPUT);
  randomSeed(analogRead(7));                    // start the random number generator
  attachInterrupt(0, messageReceived, RISING);  // look for rising edges on digital pin 2
  Radio.txMode(1);
  synchronize();
}

void synchronize(void)
{
  while(!linkEstablished)
  {
    Radio.txMode();
    Radio.write(hello);
    Radio.rxMode();
    delay(random(100));  // wait random times in between transmissions
  }                      // in case the two Arduinos started running
}                        // code at the same instant

void loop(void)
{
  while(Radio.mode)
  {
    // block (or do something else) here while the radio is
    // in rxMode (Radio.mode is 1 in rxMode, 0 in txMode)
  }
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  Radio.write(hello);
  Radio.rxMode();
}

void messageReceived(void)
{
  Radio.read();
  Radio.txMode();
  if(!linkEstablished && Radio.data[0] == hello) linkEstablished = true;
}

