
#include <Servo.h>
Servo servo;

int state;
int data1;
int data2;//pin 0,1

int angle = 90;
int x = 0;
int speedl = 0;
int speedh = 255;
int speedr = 255;

const int led1 = 11;
const int led2 = 12;
const int ldrPin = A2;

const int rf = 7;
const int rb = 8;

const int lb = 6;
const int lf = 4;

const int er = 9;
const int el = 3;

const int trig = A5;
const int echo = A4;
const int serv = 2;

int leftsensor;//A0
int rightsensor;//A1
int ldrStatus;

int right();
int left();
int frwd();
int bck();
int stp();
int rright();
int rleft();

int caller();
int linetracking();
int acc();
int ldr();
class ultrasonic{
  public:

    int read() {

      digitalWrite(trig, LOW);
      delayMicroseconds(10);
      digitalWrite(trig, HIGH);
      delayMicroseconds(10);
      digitalWrite(trig, LOW);
      distance = pulseIn(echo, HIGH);
      distance = distance * 0.034 / 2;
      if (distance > 0 && distance < 300) {
        //Serial.println(distance);

        return distance;
      }
      else if (distance == 0   ) {
        read();
      }
      else {
        //frwd();
         return 300;
      }
    }

    int servoscan() {
      for (angle = 90; angle > 0; angle--) {
        servo.write(angle);
        delay(5);
      }
      d1 = read();
      delay(50);
      int d12 = read(); 
      delay(50);
      int d13 = read();
      delay(30);


      for (angle; angle < 180; angle++) {
        servo.write(angle);
        delay(5);
      }
      d2 = read();
      delay(50);
      int d22 = read();
      delay(30);
      int d23= read();
      delay(30);


      for (angle; angle != 90; angle--) {
        servo.write(angle);
        delay(5);
      }


      if (d1 >= d2 && d12>=d22 || d1 >= d2 && d13>=d23 || d12 >= d22 && d13>=d23 ) {
        return (1);
      }
      else if(d1<d2  && d12<d22 || d1 < d2 && d13<d23 || d12 < d22 && d13<d23 ){
        return (0);
        }
      else {
        //servoscan(); 
      }


    }
    void turn() {
      frwd();
      r = read();
      if (r <= 30 && r != 0) { 
        stp();
        r = read();
        if(r<=30 && r!=0){
        

        y = servoscan();
        delay(2000);
        if (y == 1) {

          rright();
          //rleft();
          //Serial.println("right");

        }

        else if (y == 0) {


          rleft();
          //rright();
          //Serial.println("left");

        }
        delay(450);                                                           //test!
      }}

    }
  private :
    int r , y;
    int d1, d2;
    long distance;

};
void setup(){

  Serial.begin(38400);
  pinMode(rf, OUTPUT);
  pinMode(lf, OUTPUT);
  pinMode(rb, OUTPUT);
  pinMode(lb, OUTPUT);
  pinMode(er, OUTPUT);
  pinMode(el, OUTPUT);
  pinMode(trig, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(ldrPin, INPUT);
  pinMode(echo, INPUT_PULLUP);
  pinMode(leftsensor, INPUT_PULLUP);
  pinMode(rightsensor, INPUT_PULLUP);
  servo.attach(serv);



}
void loop(){
  caller();
}
int right(){
  digitalWrite(rf, HIGH);
  digitalWrite(rb, LOW);
  digitalWrite(lf, HIGH);
  digitalWrite(lb, LOW);
  analogWrite(er, speedh);
  analogWrite(el, speedl);
}
int left(){
  digitalWrite(rf, HIGH);
  digitalWrite(rb, LOW);
  digitalWrite(lf, HIGH);
  digitalWrite(lb, LOW);
  analogWrite(er, speedl);
  analogWrite(el, speedh);
}
int frwd(){
  digitalWrite(rf, HIGH);
  digitalWrite(rb, LOW);
  digitalWrite(lf, HIGH);
  digitalWrite(lb, LOW);
  analogWrite(er, speedh);
  analogWrite(el, speedh);
}
int bck(){
  digitalWrite(rf, LOW);
  digitalWrite(rb, HIGH);
  digitalWrite(lf, LOW);
  digitalWrite(lb, HIGH);
  analogWrite(er, speedh);
  analogWrite(el, speedh);
}
int stp(){
  digitalWrite(rf, LOW);
  digitalWrite(rb, LOW);
  digitalWrite(lf, LOW);
  digitalWrite(lb, LOW);
  digitalWrite(er, LOW);
  digitalWrite(el, LOW);
}
int rleft(){
  digitalWrite(rf, HIGH);
  digitalWrite(rb, LOW);
  digitalWrite(lf, LOW);
  digitalWrite(lb, HIGH);
  analogWrite(er, speedh);
  analogWrite(el, speedr);
}
int rright(){
  digitalWrite(rf, LOW);
  digitalWrite(rb, HIGH);
  digitalWrite(lf, HIGH);
  digitalWrite(lb, LOW);
  analogWrite(er, speedr);
  analogWrite(el, speedh);
}
int linetracking(){                                        
  leftsensor = digitalRead(A0);
  rightsensor = digitalRead(A1);
  if (leftsensor == 1 && rightsensor == 1) {
    frwd();
  }
  else  if (leftsensor == 1 && rightsensor == 0) { 
    //rright();
    right();
    /*
    while(leftsensor == 1 && rightsensor == 0){
   
    leftsensor = digitalRead(A0);
    rightsensor = digitalRead(A1);
    } */
    }
  else if (rightsensor == 1 && leftsensor == 0) {
    //rleft();
    left();
   /* while(rightsensor == 1 && leftsensor == 0){
    
    leftsensor = digitalRead(A0);
    rightsensor = digitalRead(A1);
    }*/
  }
}
int acc(){                                                //test!

  if (Serial.available() > 0) {
    state = Serial.read();
  }
  if (state == data1) {
    if (data1 > 380) {

      frwd();
    }
    if (data1 < 310) {
      bck();

    }
  }

  if (state == data2) {
    if (data2 > 180) {
      left();
    }
    if (data2 < 110) {
      right();
    }
  }
  if (data1 > 330 && data1 < 360 && data2 > 130 && data2 < 160) {
    //stop car
    stp();
  }
}
int caller(){                                            //test!
  ldr();
  leftsensor = digitalRead(A0);
  rightsensor = digitalRead(A1);
  if (leftsensor == 0 && rightsensor == 0) {
    frwd();
    x++;
    while(leftsensor == 0 && rightsensor == 0){
      leftsensor = digitalRead(A0);
      rightsensor = digitalRead(A1);
      }
  }
  if (x == 0) {
    linetracking();
  }
  else if (x == 1) {
    ultrasonic ult;
    ult.turn();
  }
  else if (x == 2) {
  stp();
  //acc();
}
}
int ldr(){
  ldrStatus = analogRead(ldrPin);
  if (ldrStatus <= 300) {

    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);


  }
  else {

    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);

  }
}
