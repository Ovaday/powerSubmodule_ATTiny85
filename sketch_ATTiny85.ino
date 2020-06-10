#include <avr/power.h>
#define voltage A1 // no PWM
#define greLED 0 //PWM
#define yelLED 1 //PWM
#define redLED 4 //PWM
#define buzzer 3 //no PWM ? maybe
unsigned long timing;
unsigned long checkShort;
unsigned long checkLong;
int ledState = LOW;             // этой переменной устанавливаем состояние светодиода
long interval = 100; 
int val;
int brs = 0;
int brsn = 0;
int x = 0;
int xold = 0;
int beepshortcounter, beeplongcounter;
boolean beeper, beepshort, beeplong, blinking;
void setup() {
  clock_prescale_set(clock_div_4);
  pinMode(voltage, INPUT);
  pinMode(greLED, OUTPUT);
  pinMode(yelLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(buzzer, OUTPUT);
  val = 0;
  beeper =  false;
  beepshort = false;
  beeplong = false;
  beepshortcounter = 0;
  beeplongcounter = 0;
digitalWrite(greLED, LOW);
digitalWrite(yelLED, LOW);
digitalWrite(redLED, LOW);
digitalWrite(buzzer, HIGH);
timing = millis();
}
void (* resetFunc) (void) = 0;

void loop() {
  if ( millis() >= 3000000000){
    resetFunc();
  }//сброс контроллера при переполнении
  

 x = analogRead(voltage);
 if ( xold > (x + 7) || xold < (x - 7)){
 xold = x;
 if ( x >= 809){ //напряжение выше 3,9*4, светить зеленым светодидом
  digitalWrite(greLED, HIGH); 
  digitalWrite(yelLED, LOW);
  digitalWrite(redLED, LOW);
  digitalWrite(buzzer, HIGH);
  beeper =  false;
  beepshort = false;
  beeplong = false;
  blinking = false;
  }
  else if ( x < 809 && x >= 799 ){ //гестерезис - Зеленый свет 100%, желтый 40%
  digitalWrite(greLED, HIGH); 
  analogWrite(yelLED, 30); 
  digitalWrite(redLED, LOW);
  digitalWrite(buzzer, HIGH);
  beeper =  false;
  beepshort = false;
  beeplong = false;
  blinking = false;
  }
 else if ( x < 799 && x >= 768 ){ //напряжение ниже 3,9*4 и выше 3,75*4, светить зеленым и желтым светодиодом
  digitalWrite(greLED, HIGH); 
  digitalWrite(yelLED, HIGH); 
  digitalWrite(redLED, LOW);
  digitalWrite(buzzer, HIGH);
  beeper =  false;
  beepshort = false;
  beeplong = false;
  blinking = false;
  }
    else if ( x < 768 && x >= 758 ){ //гестерезис - Зеленый свет 40%, желтый 100%
  analogWrite(greLED, 50); 
  digitalWrite(yelLED, HIGH); 
  digitalWrite(redLED, LOW);
  digitalWrite(buzzer, HIGH);
  beeper =  false;
  beepshort = false;
  beeplong = false;
  blinking = false;
  }
   else if ( x < 758 && x >= 720 ){ //напряжение ниже 3,75*4 и выше 3,55*4, светить желтым светодиодом
  digitalWrite(greLED, LOW); 
  digitalWrite(yelLED, HIGH); 
  digitalWrite(redLED, LOW);
  digitalWrite(buzzer, HIGH);
  beeper =  false;
  beepshort = false;
  beeplong = false;
  blinking = false;
   }
  else if ( x < 720 && x >= 700 ){ //гестерезис - Желтый светит 40%, красный 70%
  digitalWrite(greLED, LOW); 
  brs = (x - 726) * 3;
  brsn = 70 - brs;
  analogWrite(yelLED, brs); 
  analogWrite(redLED, brsn);
  digitalWrite(buzzer, HIGH);
  beeper =  false;
  beepshort = false;
  beeplong = false;
  blinking = false;
  }
   else if ( x < 700 && x >= 675 ){ //напряжение ниже 3,75*4 и выше 3,55*4, светить красным светодиодом и раз в 10сек бибикать
  digitalWrite(greLED, LOW); 
  digitalWrite(yelLED, LOW); 
  digitalWrite(redLED, HIGH);
  beepshort = true;
  beeplong = false;
  blinking = false;
  
   }
  else { // напряжение меньше нижнего предела, бибикать и мигать светодиодом
  digitalWrite(greLED, LOW); 
  digitalWrite(yelLED, LOW); 
  beepshort = false;
  beeplong = true;
  blinking = true;
  if(millis() - timing >= interval) {
    timing = millis();
    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;
    digitalWrite(redLED, ledState);
  }
  }//конец цикла считывания и отображения
 }
  if ( beeplong == true && beeper == false ){
    if ( millis() - checkShort >= 10000){
      digitalWrite(buzzer, LOW);
      checkShort = millis();
      beeper = true;
    }
  }
  if ( beepshort == true && beeper == false ){
    if ( millis() - checkLong >= 5000){
      digitalWrite(buzzer, LOW);
      checkLong = millis();
      beeper = true;
    }
  }

  if(millis() - timing >= interval && blinking == true) {
    timing = millis();
    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;
    digitalWrite(redLED, ledState);
  }
  
  if ( beeper == true){ // если пищим
    if ( beepshort == true ){
     if (beepshortcounter >= 2){
      digitalWrite(buzzer, HIGH);
      beeper = false;
      beepshortcounter = 0;
     }
     else if (x >= 550){
      beepshortcounter = beepshortcounter + 1;
     }
    }
     if ( beeplong == true ){
      if (beeplongcounter >= 5){
      digitalWrite(buzzer, HIGH);
      beeper = false;
      beeplongcounter = 0;
     }
     else if (x >= 550){
      beeplongcounter = beeplongcounter + 1;
    }
     }
     
  }
  delay(100);

  
}
