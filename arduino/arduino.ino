#define SOFTSERIAL

#ifdef SOFTSERIAL
#include <SoftwareSerial.h>
#endif

#include "myfifo.h"
#include "motor.h"

// G1000 1000 500
#define TOP 1000L

#ifdef SOFTSERIAL
  SoftwareSerial blt(2, 3);
#endif

motor M_pan(12,11,10,9);
motor M_tilt(7,6,5,4);
myfifo fifo;

unsigned long t0;
long pos = 0;
bool pause = false;

void setup() {
#ifdef SOFTSERIAL
    Serial.begin(115200);
    Serial.println("CMD :");
    blt.begin(9600);
    delay(10);
    blt.flush();
#else 
    Serial.begin(9600);
#endif
    digitalWrite(13, HIGH); 
    delay(2000);
    digitalWrite(13, LOW); 
    t0 = micros();
    pos = 0;
}

void loop() {
#ifdef SOFTSERIAL
  read_bt();
#endif  
  if(pause==false){
    bool m1t = M_pan.go()==false;
    bool m2t = M_tilt.go()==false;
    if( m1t or m2t ) {
      if(fifo.as_element()){
        fifo_data data = fifo.pop();
        switch(data.c){
          case 'd':
          case 'D': M_pan.go_to_o(); M_tilt.go_to_o(); break;
          case 'g':
          case 'G': 
                    pos=0;
                    M_pan.go_pas(data.p1, data.p3);
                    M_tilt.go_pas(data.p2, data.p3);
                    break;

        }
      }
    }
  }
  pos +=1;
  t0+= TOP;
  long de = (t0-micros())/1000L;
  if(de<TOP) delay(de);
}
#ifndef SOFTSERIAL
void serialEvent() {
  String msg;
  while (Serial.available()) {
    delay(1);
    if (Serial.available() > 0) {
      char c = Serial.read();
      msg += c;

    }
  }
  Serial.flush();
  if(msg!="")
    cmd(msg);
}
#else
void read_bt(){
  String ans="";
    while (blt.available()) {
      delayMicroseconds(100);
      if (blt.available() > 0) {
        char c = blt.read();
        ans += c;
      }
    }
    blt.flush();
    cmd(ans);

}
#endif
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
        case 's':
        case 'S':
              M_pan .go_continue(pas[0]);
              M_tilt.go_continue(pas[1]);
              break;
        case 'p': 
        case 'P': 
              pause = !pause;
              break;
        case 'r':
        case 'R': 
              M_pan.reset_o();
              M_tilt.reset_o();
              fifo.empty();
              pause = false; 
              break;
        case 't':
        case 'T':
              Serial.print("pas : ");
              Serial.print(M_pan.get_pas_o());
              Serial.print(' ');
              Serial.print(M_tilt.get_pas_o());
              Serial.println(' ');
              break;
        default : fifo.push(ans[0],pas[0], pas[1], pas[2]);
      }
    }
}
