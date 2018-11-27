#include <SoftwareSerial.h>
#include "myfifo.h"
#include "motor.h"

// G1000 1000 500
#define TOP 1000L

SoftwareSerial blt(2, 3);
motor M1(12,11,10,9);
motor M2(7,6,5,4);
myfifo fifo;

unsigned long t0;
long pos = 0;
bool pause = false;

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
  if(pause==false){
    bool m1t = M1.go()==false;
    bool m2t = M2.go()==false;
    if( m1t or m2t ) {
      if(fifo.as_element()){
        fifo_data data = fifo.pop();
        switch(data.c){
          case 'd':
          case 'D': M1.go_to_o(); M2.go_to_o(); break;
          case 'g':
          case 'G': 
                    pos=0;
                    M1.go_pas(data.p1, data.p3);
                    M2.go_pas(data.p2, data.p3);
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
        case 'p': 
        case 'P': 
              pause = !pause;
              break;
        case 'r':
        case 'R': 
              M1.reset_o();
              M2.reset_o();
              fifo.empty();
              pause = false; 
              break;
        case 't':
        case 'T':
              Serial.print("pas : ");
              Serial.print(M1.get_pas_o());
              Serial.print(' ');
              Serial.print(M2.get_pas_o());
              Serial.println(' ');
              break;
        default : fifo.push(ans[0],pas[0], pas[1], pas[2]);
      }
    }
}
