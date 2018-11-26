#include <SoftwareSerial.h>
#include "myfifo.h"
#include "motor.h"

// G1000 1000 500
#define TOP 1000L

SoftwareSerial blt(2, 3);
motor M1(12,11,10,9);
motor M2(7,6,5,4);

unsigned long t0;
long pos = 0;
void setup() {
    blt.begin(9600);
    Serial.begin(115200);
    Serial.println("CMD :");
  
    blt.begin(9600);
    digitalWrite(13, HIGH); 
    delay(2000);
    digitalWrite(13, LOW); 
    t0 = micros();
    pos = 0;
}

void loop() {
  if(0)
    read_serial();
  else 
    read_bt();
  M1.go(pos);
  M2.go(pos);
  pos +=1;
  t0+= TOP;
  long de = (t0-micros())/1000L;
  Serial.println(de);
  if(de<TOP)
    delay(de); 
}

void read_serial(){
  String msg;
  while (Serial.available()) {
    delay(10);
    if (Serial.available() > 0) {
      char c = Serial.read();
      msg += c;

    }
  }
  Serial.flush();
/*  if (msg != "") {
    blt.print(msg);
  }*/
  if(msg!="")
    cmd(msg);
}

void read_bt(){
  String ans="";
    while (blt.available()) {
      delay(10);
      if (blt.available() > 0) {
        char c = blt.read();
        ans += c;
  
      }
    }
    blt.flush();
    cmd(ans);

}

void cmd(String ans){
    if(ans !=""){
      long pas[]={0,0,0};
      int j=0;

      bool neg = false;
      for (int i=1; i < ans.length(); i++){
        if(ans[i] == '-') {
          neg = true;
        }
        else if(ans[i]<='9' && ans[i]>='0'){
         pas[j]=pas[j]*10 + long(ans[i]-'0');
        }
        else if(ans[i] == ' '){
           if(neg) pas[j] = - pas[j];
           j++;
           if(j>=3) break;
           neg = false;
        }
        else 
          break;
      }

      switch(ans[0]){
        case 'c':
        case 'C': M1.reset_o(); M2.reset_o(); break;
        case 'd':
        case 'D': M1.go_to_o(); M2.go_to_o(); break;
        case 'g':
        case 'G': 
                  pos=0;
                  M1.go_pas(pas[0], pas[2]);
                  M2.go_pas(pas[1], pas[2]);
                  break;
        case 't':
            Serial.print("pas : ");
            Serial.print(M1.get_pas_o());
            Serial.print(' ');
            Serial.print(M2.get_pas_o());
            Serial.println(' ');
      }
    }  
}
