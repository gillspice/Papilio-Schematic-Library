/*
  Gadget Factory
  Papilio One VGA Hello World Example
  
 VGA Wing Introduction:
 http://papilio.cc/index.php?n=Papilio.VGAWingIntroduction
 
 Hardware:
 If using an Arcade MegaWing or a LogicStart MegaWing then select the appropriate "Hyperion" board type in the ZAP IDE.
 If using a VGA Wing or some other device then use the "Hyperion" pinselect board type in the ZAP IDE and be sure to uncomment "setup_pin_select" in setup.
 
 ZPUino Variant:
 Hyperion "The Lord of Light" with HQVGA adapter is needed for this example sketch

 created 2012
 by Jack Gassett
 http://www.gadgetfactory.net
 
 This example code is in the public domain.
 */
#define FREQ 17000          //Freq for all players 

#include <SD.h>
#include "SmallFS.h"
#include "modplayer.h"
#include "ramFS.h"
#include "cbuffer.h"

MODPLAYER modplayer;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
   //Start SmallFS
  if (SmallFS.begin()<0) {
	Serial.println("No SmalLFS found.");
  }
  else{
     Serial.println("SmallFS Started.");
  }  
  
 //Setup timer for YM and mod players, this generates an interrupt at 1700hz
  TMR0CTL = 0;
  TMR0CNT = 0;
  TMR0CMP = ((CLK_FREQ/2) / FREQ )- 1;
  TMR0CTL = _BV(TCTLENA)|_BV(TCTLCCM)|_BV(TCTLDIR)|
  	_BV(TCTLCP0) | _BV(TCTLIEN);
  INTRMASK = BIT(INTRLINE_TIMER0); // Enable Timer0 interrupt
  INTRCTL=1;     
   
  
  modplayer.setup(5);

  modplayer.loadFile("music.mod");
  modplayer.play(true);
  
}

void _zpu_interrupt()
{
  //Interrupt runs at 17KHz
  modplayer.zpu_interrupt();
}

void loop() {
  // put your main code here, to run repeatedly: 
  if (modplayer.getPlaying() == 1)
    modplayer.audiofill();
}
