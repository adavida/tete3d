#ifndef MY_FIFO_H
#define MY_FIFO_H
#include <Arduino.h>

struct fifo_data
{
  char c;
  long p1;
  long p2;
  long p3;  
};

struct fifo_el
{
  fifo_data data;
  fifo_el * prev;
  fifo_el * next;
};

class myfifo
{
  public:
    myfifo();
    ~myfifo();
    void push(char c, long p1, long p2, long);  // ajouté un élément en fin de chaine
    fifo_data pop();                            // recupré le 1 er élement et le suprimmer
    bool as_element();                          // test si la chaine n'est pas vide
    void empty();                               // vide la chaine

  private:
    fifo_el * new_el();                         // cree un nouvelle élement en fin de chaine
    void del_el(fifo_el*);                      // supprime un élement
    fifo_el * fisrt;                            // 1er element de la chaine
    fifo_el * last;                             // derniere élement de la chaine
};

#endif
