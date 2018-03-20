
#include <WString.h>
#include <Metro.h>

#include <IRremote.h>
#include <IRremoteInt.h>
// # include <Summer.h>
#include <EEPROM.h>

//Summer summer = Summer(12);

#define selfPowerPin A0
#define batteryPin A2
#define playerPin 8
#define cameraPin 4
#define lcdPin 7
#define minVoltage 500

IRsend irsend;
Metro loopTimer = Metro(3000);

unsigned char cSender;
int cLength = 0;
int voltage = 0;
int sc = 0;
unsigned char cReceiver;
unsigned char cBytes[30];


int lcdPinState = 0;
#define eepromLcdPinState 0

const uint8_t message3[] = {0x68, 0x04, 0xFF, 0x36, 0xA0, 05}; // CD input

void setup() { 
  lcdPinState = EEPROM.read(eepromLcdPinState);
  pinMode(13, OUTPUT);
  pinMode(selfPowerPin, OUTPUT);
  pinMode(playerPin, OUTPUT);
  pinMode(lcdPin, OUTPUT);
  pinMode(cameraPin, OUTPUT);
  
  digitalWrite(playerPin, LOW); 
  digitalWrite(lcdPin, lcdPinState); 
  digitalWrite(selfPowerPin, HIGH);
  digitalWrite(cameraPin, LOW);
  
  UCSR0C = UCSR0C | B00100000;
  Serial.begin (9600);
  //summer.setTempo(300);
  
  // set input to CD 
  // TODO save/recall to/from eeprom
  const uint8_t message2[] = {0x68, 0x04, 0xFF, 0x36, 0xA0, 0x05};
  Serial.write(message2, message2[1]+2);
} 




void loop(){
  
///////////////////////////////////////////////////////////////////////////////////////////
  if (Serial.available() > 0) {
    cSender = Serial.read();
    //Serial.println("gavau kazka per seriala");
  
    if ( (cSender == 0x50) || (cSender == 0xF0) || (cSender == 0x68) || (cSender == 0x80) ) {
      //Serial.println("gavau senderio bita");
      delay(10);    
      if (Serial.available() > 0) {
        cLength = Serial.read();
        if (Serial.available() > 0) cReceiver = Serial.read();
	

      unsigned char tmp = cSender;
      for(int i=0; i<cLength-1 && i<= 30; i++) {
	  if (Serial.available() > 0) { 
            tmp = Serial.read();
            cBytes[i] = tmp;
          }
	}




      //************************************************************
      // CD CHANGER
      //************************************************************
      // 68 03 18 01 72  RAD  --> CDC : Device status request
      if (cSender == 0x68 && cLength == 3 && cBytes[0] == 0x01 && cBytes[1] == 0x72) {
        const uint8_t message[] = {0x18, 0x04, 0xFF, 0x02, 0x00, 0xE1};
        Serial.write(message, message[1]+2);

        uint8_t message2[] = {0x68, 0x10, 0x3B, 0x23, 0x62, 0x30, 0x42, 0x4D, 0x57, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x6A};
        sendMsg(message2);

        uint8_t message3[] = {0x68, 0x1A, 0x3B, 0xA5, 0x62, 0x01, 0x06, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x6F, 0x77, 0x6E, 0x33, 0x65, 0x64, 0x20, 0x62, 0x79, 0x20, 0x20, 0x20, 0x20, 0x20, 0xD6};
        sendMsg(message3);
        
        uint8_t message4[] = {0x68, 0x1A, 0x3B, 0xA5, 0x62, 0x01, 0x07, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x75, 0x6E, 0x74, 0x69, 0x74, 0x6C, 0x65, 0x64, 0x20, 0x20, 0x20, 0x20, 0x20, 0x97};
        sendMsg(message4);
     
     //          0  1  2  3   
     // 80 0A BF 13 03 10 00 00 00 00 1D 28 --- R
     // 80 0A BF 13 00 11 00 00 00 00 21 16
     // 80 0A BF 13 02 10 00 00 00 00 21 15
     
     // 80 0A BF 13 03 B0 00 00 00 00 1D 88 --- P
     // 80 0A BF 13 00 B1 00 00 00 00 21 B6
     // 80 0A BF 13 02 B0 00 00 00 00 21 B5



     
     // 80 0A BF 13 03 80 00 00 00 00 1D B8 --- D
     // 80 0A BF 13 00 81 00 00 00 00 21 86
     // 80 0A BF 13 02 80 00 00 00 00 21 85

     // REVERSE ON   
     } else if (cSender == 0x80 && cLength == 10 && cBytes[0] == 0x13 && (cBytes[1] == 0x03 || cBytes[1] == 0x00 || cBytes[1] == 0x02) && (cBytes[2] == 0x10 || cBytes[2] == 0x11) ) {   
        digitalWrite(cameraPin, HIGH);
        digitalWrite(lcdPin, HIGH);
        //const uint8_t vol_down_msg[] = {0xF0, 0x04, 0x68, 0x32, 0x30, 0x9E};
        //Serial.write(vol_down_msg, vol_down_msg[1]+2);
     
     // P or D ON
     } else if (cSender == 0x80 && cLength == 10 && cBytes[0] == 0x13 && (cBytes[1] == 0x03 || cBytes[1] == 0x02 || cBytes[1] == 0x00) && (cBytes[2] == 0xB0 || cBytes[2] == 0xB1 || cBytes[2] == 0x80 || cBytes[2] == 0x81) ) {   
        digitalWrite(cameraPin, LOW);  
        digitalWrite(lcdPin, lcdPinState);
     
      //************************************************************
      // 68 05 18 38 00 00 4D   RAD  --> CDC : CD status request
      // 68 05 18 38 03 00 4E
      } else if (cSender == 0x68 && cLength == 5 && cBytes[0] == 0x38 && (cBytes[3] == 0x4D || cBytes[3] == 0x4E)) {
        delay(2);
        uint8_t message2[] = {0x68, 0x10, 0x3B, 0x23, 0x62, 0x30, 0x42, 0x4D, 0x57, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x6A};
        //Serial.write(message2, message2[1]+2);          
        sendMsg(message2);
        const uint8_t message3[] = {0x68, 0x1A, 0x3B, 0xA5, 0x62, 0x01, 0x06, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x6F, 0x77, 0x6E, 0x33, 0x65, 0x64, 0x20, 0x62, 0x79, 0x20, 0x20, 0x20, 0x20, 0x20, 0xD6};
        Serial.write(message3, message3[1]+2);  
        const uint8_t message4[] = {0x68, 0x1A, 0x3B, 0xA5, 0x62, 0x01, 0x07, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x75, 0x6E, 0x74, 0x69, 0x74, 0x6C, 0x65, 0x64, 0x20, 0x20, 0x20, 0x20, 0x20, 0x97};
        Serial.write(message4, message4[1]+2);  

        //summer.playTone(500,3);

        /*       
        const uint8_t message[] = {0x68, 0x04, 0xFF, 0x36, 0xA0, 05}; // CD input
        Serial.write(message, message[1]+2);
        */
      
      //************************************************************
      // VAIRAS
      //************************************************************
      } else if (cLength == 4 && cBytes[0] == 0x3B) { // VAIRAS
         //unsigned char tmp3 = cBytes[1];
         //Serial.println(tmp3, HEX);
         switch(cBytes[1]) {
           case 0xA0: // kalbanti boba
             //rrok(1);
             
             if ( digitalRead(cameraPin) == LOW ) { // cammera is off
               digitalWrite(cameraPin, HIGH);
               digitalWrite(lcdPin, HIGH);
               
             } else {                              // cammera is on
                digitalWrite(cameraPin, LOW);  
                digitalWrite(lcdPin, lcdPinState);
             }
             break;
           case 0x00:  // R/T
           case 0x40: // R/T
             //rrmusic(1);
             lcdPinState = (lcdPinState == 1) ? 0 : 1;
             digitalWrite(lcdPin, lcdPinState);
             digitalWrite(cameraPin, LOW);
             break;
           case 0x01: // next
             rrnext(1);
             break;
           case 0x28: // prev
             rrprev(1);
             break;
         }
      }
      
      //************************************************************
      if (cLength == 4 && cBytes[0] == 0x48) { // TELIKO KNOPKES
        switch (cBytes[1]) {
          case 0x14: // < >
              lcdPinState = (lcdPinState == 1) ? 0 : 1;
              digitalWrite(lcdPin, lcdPinState);
              break;
          case 0x08: // tel
              rrreturn(1);
              break;          
          case 0x85: // knob release
              if (lcdPinState) rrok(1);
              break;

          case 0: // >>>
              if (lcdPinState) rrright(1);
              break;
          case 0x10: // <<<
              if (lcdPinState) rrleft(1);
              break;
          case 0x7: // clock
              if (lcdPinState) rr16x9(1);
              break;
              
        }

        } 
      }
      //************************************************************  
      if (cLength == 4 && cBytes[0] == 0x49) { // ROTARY
        switch (cBytes[1]) {
          case 0x81:  // right 1
             if (lcdPinState) rrdown(1);
             break;
           case 0x82:  // right 2
             if (lcdPinState) rrdown(2);
             break;
           case 0x83:  // right 3
             if (lcdPinState) rrdown(3);
             break;
           case 0x84:
             if (lcdPinState) rrdown(4);
             break;
           case 0x85:
             if (lcdPinState) rrdown(5);
             break;
           
           case 0x01:    // left 1
             if (lcdPinState) rrup(1);
             break;
           case 0x02:    // left 2
             if (lcdPinState) rrup(2);
             break;
           case 0x03:    // left 3
             if (lcdPinState) rrup(3);
             break;
           case 0x04:
             if (lcdPinState) rrup(4);
             break;
           case 0x05:
             if (lcdPinState) rrup(5);
             break;
        }
      }
      //************************************************************  
  
  
      }
    }
  ///////////////////////////////////////////////////////////////////////////////////////////
  
  voltage = analogRead(batteryPin) * 2; //1.9965;
  
  if (loopTimer.check()) { // atejo laikas tikrint poweri
    loopTimer.reset(); // resetinam timeri

    //Serial.print("voltage: ");
    //Serial.println(voltage);
    
    if (voltage < minVoltage) {   // <------ NERA +12 v
      //Serial.println("nera +12v");
      if (digitalRead(playerPin) == HIGH) { // <------ NEATJUNGTA RELE
        //Serial.println("neatjungtas playeris. isjungiam.");
        //summer.playTone(500,1);
        rrpower(1);
        delay(1000); 
        digitalWrite(playerPin, LOW); 
        digitalWrite(selfPowerPin, LOW);
        digitalWrite(lcdPin, LOW);
        digitalWrite(13, LOW); 
      } else {                       // <------ ATJUNGTA RELE (viskas OK)
        //summer.playTone(3000, 1);   
       digitalWrite(selfPowerPin, LOW); 
      }
      EEPROM.write(eepromLcdPinState, lcdPinState);

    } else {                        // <------- YRA +12 v
      //Serial.println("yra +12v");
      if (digitalRead(playerPin) == LOW) { // <------- JEI RELE ATJUNGTA
        //Serial.println("playeris isjungtas. ijungiam.");
        digitalWrite(playerPin, HIGH);
        digitalWrite(lcdPin, lcdPinState);
        digitalWrite(13, HIGH); 
        //summer.playTone(10000,5); 
      }
    }
  }
    

  


    
}

