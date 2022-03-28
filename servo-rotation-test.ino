#include <Servo.h>

double range = 30;

double target = 90 - range;

Servo myservo;

int pos = 90;
bool servoDirection = true;

void setup() {

  myservo.attach(7);

  Serial.begin(9600);

  Serial.println("Starting");

  myservo.write(target);
}

void loop() {
  
//put your main code here, to run repeatedly:

    Serial.println(myservo.read());
    

    if(servoDirection) {
      pos += 3;
      myservo.write(pos);
      delay(15);

      if(pos > 90 + range) {
        servoDirection = false;
      }
    }
    else {
      pos -= 3;
      myservo.write(pos);
      delay(15);

      if(pos < 90 - range) {
        servoDirection = true;
      }
    }
  
 
}
