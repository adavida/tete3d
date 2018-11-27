#include "myfifo.h"

myfifo::myfifo(){
  fisrt = NULL;
  last = NULL;
}

myfifo::~myfifo(){
  while(fisrt != NULL){
    del_el(fisrt);
  }
}

void myfifo::push(char c, long p1, long p2, long p3){
  fifo_el * el= new_el();
  el->data.c = c;
  el->data.p1 = p1;
  el->data.p2 = p2;
  el->data.p3 = p3;
}

fifo_data myfifo::pop(){
  fifo_data data = fisrt->data;
  del_el(fisrt);
  return data;
}

fifo_el * myfifo::new_el(){
  fifo_el * el = new fifo_el;
  if(fisrt == NULL) fisrt = el;
  el->prev = last;
  el->next = NULL;
  last->next = el;
  last = el;
  delay(1); // si non sa ne fonctionne pas, mais pk?
  return el;
}

void myfifo::del_el(fifo_el* el){
  if(last  == el) last  = el->prev;
  if(fisrt == el) fisrt = el->next;
  if(el->prev != NULL) el->prev->next = el->next;
  if(el->next != NULL) el->next->prev = el->prev;
  delete el;
}

bool myfifo::as_element(){
  return fisrt!=NULL;
}

void myfifo::empty(){
  while(fisrt != NULL){
    del_el(fisrt);
  }
}
