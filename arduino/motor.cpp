#include "motor.h"

motor::motor(int p1, int p2, int p3, int p4){
  _l[0] = p1;
  _l[1] = p2;
  _l[2] = p3;
  _l[3] = p4;
  _pos  = 0;
  for(int i=0; i<4; i++) {
    pinMode(_l[i], OUTPUT);
  }
  go_pas(0,1);
}

void motor::go_pas(long count, long pas_t){
  chmode(0);
  _go_pas = count;
  _pas_t  = pas_t < abs(count) ? abs(count) : pas_t;
  _pos_p  = 0;
  _pos_t  = 0;
}

void motor::go_continue(int x){
  chmode(x);
  _go_pas=x>0?x:-x;
}
void motor::chmode(int x){
  if(x == 0)    _mode = 0;
  else if (x>0) _mode = 1;
  else if (x<0) _mode = 2;
}

bool motor::go(){
  int change = 0;
  switch(_mode){
    case 1:
    case 2:
        if(_go_pas<=_pos_p){
          change = _mode == 1? 1:-1;
          _pos_p=0;
        }
        else 
          _pos_p+=1;
        break;
    default:
        if (_pos_t == _pas_t) return false;
        long p = (_pos_t * _go_pas)/_pas_t ;
        _pos_t++;
        if(_pos_p == p){
          if(_go_pas > 0){
            if(_go_pas > p){
              change  = 1;
              _pos_p ++;
            }
          }
          else{
           if(_go_pas < p){
             change  = -1;
              _pos_p --;
           }
          }
        }
  }
  if(change !=0){
    _pos += change;
    _pas_from_o += change;
    if(_pos >= MOTOR_PAS) _pos = 0;
    if(_pos <  0)         _pos = MOTOR_PAS - 1;  
    for(int i=0; i<MOTOR_PIN; i++){
      digitalWrite(_l[i], _data[_pos][i]);
    }
  }
  return true;
}

long motor::get_pas_o(){
  return _pas_from_o;
}

void motor::go_to_o(){
  go_pas(-_pas_from_o, _pas_from_o*5);
}

void motor::reset_o(){
  go_pas(0, 100);
  _pas_from_o = 1;
}
