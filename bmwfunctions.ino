uint8_t sendMsg(uint8_t msg[]) {
  //sc = Serial.available();
  //for(int i=0; i<10;i++) {
  // ****************     
    
  //Serial.available()
  Serial.write(msg, msg[1]+2);

}



uint8_t message(String str, int field) {
  
  uint8_t fullmsg[] = {0x68, 0x10, 0x3B, 0x23, 0x62, 0x30, 0x08};
 
  Serial.println(fullmsg[0]);
  if (fullmsg[0] != 0x00) {
  
      int sll = sizeof(fullmsg);
      int maxilgis = fullmsg[1]-sizeof(fullmsg)+1;
      int ilgis = str.length();
    
      String str2;
      if (ilgis > maxilgis) str2 = str.substring(0,maxilgis); else str2 = str;
      if (ilgis < maxilgis) { 
        for(int i=0; i<maxilgis-ilgis;i++) { 
          //str2.append(" "); 
          str2  += " ";
        }    
      }
    
    
      for(int i=0; i<maxilgis;i++) {
        fullmsg[sll+i] = str2[i];
      }
      fullmsg[fullmsg[1]+1] = checksum(fullmsg);
      
      for(int i=0; i<fullmsg[1]+2; i++) {
        Serial.print(fullmsg[i], HEX);
        Serial.println(' ');
      }
      //Serial.write(fullmsg, fullmsg[1]+2);
  } else Serial.println("Nera ka daryt");
}





byte checksum(uint8_t tmp[]) {
  byte csum = tmp[0];
  for (int i=1; i<=tmp[1]; i++) {
    csum = csum ^ tmp[i];
  }
  return csum;
}


void rrdown(int x) { for(int i=0;i<x;i++) { irsend.sendNEC(0xFF28D7, 32); }}
void rrup(int x)   { for(int i=0;i<x;i++) { irsend.sendNEC(0xFF08F7, 32); }}
void rrleft(int x)   { for(int i=0;i<x;i++) { irsend.sendNEC(0xFF8877, 32); }}
void rrright(int x)   { for(int i=0;i<x;i++) { irsend.sendNEC(0xFFC837, 32); }}
void rrok(int x)   { 
   for(int i=0;i<x;i++) { irsend.sendNEC(0xFF6897, 32); }
   //68 04 FF 36 A0 05 --- CD input
   //const uint8_t message2[] = {0x68, 0x04, 0xFF, 0x36, 0xA0, 0x05};
   //Serial.write(message2, message2[1]+2);  
}
void rrplaypause(int x)   { for(int i=0;i<x;i++) { irsend.sendNEC(0xFFD02F, 32); }}
void rrstop(int x)   { for(int i=0;i<x;i++) { irsend.sendNEC(0xFFB04F, 32); }}
void rrmusic(int x)   { for(int i=0;i<x;i++) { irsend.sendNEC(0xFF20DF, 32); }}
void rrmovie(int x)   { for(int i=0;i<x;i++) { irsend.sendNEC(0xFF40BF, 32); }}
void rr16x9(int x)   { for(int i=0;i<x;i++) { irsend.sendNEC(0xFFA05F, 32); }}
void rrreturn(int x)   { for(int i=0;i<x;i++) { irsend.sendNEC(0xFF708F, 32); }}
void rrnext(int x)   { for(int i=0;i<x;i++) { irsend.sendNEC(0xFFB24D, 32); }
  //message("BLABLA");  
  //const uint8_t bbd[] = {0x68, 0x18, 0x3B, 0x23, 0x62, 0x30, 0x07, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x08, 0x2E, 0x4A, 0x55, 0x5A, 0x49, 0x4B, 0x41, 0x2E, 0x3D, 0x20, 0x2E, 0x00, 0x2E};
  //Serial.write(bbd, bbd[1]+2);
}
void rrprev(int x)   { for(int i=0;i<x;i++) { irsend.sendNEC(0xFFA857, 32); }
  //const uint8_t bbd[] = {0x68, 0x19, 0x3B, 0x23, 0x62, 0x30, 0x01, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x08, 0x2E, 0x4A, 0x55, 0x5A, 0x49, 0x4B, 0x41, 0x2E, 0x3D, 0x20, 0x2E, 0x00, 0x2E, 0x01};
  //Serial.write(bbd, bbd[1]+2);
}
void rrpower(int x)   { for(int i=0;i<x;i++) { irsend.sendNEC(0xFF00FF, 32); }}

void afterpress() {
  delay(500);
}

