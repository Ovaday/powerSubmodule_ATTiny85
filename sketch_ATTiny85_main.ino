#include <avr/power.h>
#define voltage A1 // no PWM
#define usb A2 // P3
#define REL 0 //PWM
unsigned long timing;
int relState = LOW;             // этой переменной устанавливаем состояние светодиода
int val;
int x = 0;
int y = 0;
int xold = 0;
int yold = 0;
boolean relay;
void setup() {
  pinMode(voltage, INPUT);
  pinMode(usb, INPUT);
  pinMode(REL, OUTPUT);
  val = 0;
  relay =  false;
digitalWrite(REL, LOW);
timing = millis();
}
void (* resetFunc) (void) = 0;

void loop() {
  if ( millis() >= 3000000000){
    resetFunc();
  }//сброс контроллера при переполнении
  
 x = analogRead(voltage);
 y = analogRead(usb);
 if ( xold > (x + 23) || xold < (x - 23) || yold > (y + 30) || yold < (y  - 30)){
 xold = x;
 yold = y;
 if ( x >= 620){ //напряжение выше 16,5В
  digitalWrite(REL, LOW); 
  relay =  false;
  }
  else if ( x < 610 && x >= 407 && y < 250){ //работать, если напряжение между 10,5 и 16,5В
  digitalWrite(REL, HIGH); 
  relay =  true;
  }
  else{
  digitalWrite(REL, LOW); 
  relay =  false;
  }
 }
  
  
  delay(50);
  
}
