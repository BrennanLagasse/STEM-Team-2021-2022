
int trigPin = 2;
int echoPin = 3;
double duration;
double distance;





void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin,INPUT);

  Serial.begin(9600);

  // Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
  //Serial.println("with Arduino UNO R3");
  
}



void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Distance in centimeters - between 30 and 450
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.0343 /2;
  Serial.println(distance);
  
  //
   

}
