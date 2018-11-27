#ifndef motor_h
#define motor_h

#include <Arduino.h>
#define MOTOR_PAS 8
#define MOTOR_PIN 4
class motor
{
  public:
    motor(int, int, int, int);
    bool go( );             // donne l'autre de passé un pas
    void go_pas(long , long); // donne un nombres de pas
    long get_pas_o();           // retourne le nombres de pas vers l'origine
    void reset_o();             // resinisalise la possition d'origine
    void go_to_o();             // retourne à la possition origine
  private:
    int _l[4];                  // pin utiliser par le moteur
    int _pos;                   // possition dans le cycle
    long _pas_t=100;            // nombres de pas justqu'a la fin du mouvement
    long _pos_p=0;              // nombre de mouvement effectué
    long _pos_t=0;              // compte dans cycle
    long _pas_from_o=0;         // position depuis l'origne
    long _go_pas = 0;           // nombre de mouvement total
    bool _data[MOTOR_PAS][MOTOR_PIN]={
      {true,false,false,false},
      {true,true,false,false},
      {false,true,false,false},
      {false,true,true,false},
      {false,false,true,false},
      {false,false,true,true},
      {false,false,false,true},
      {true,false,false,true}
    };
};

#endif
