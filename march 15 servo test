# STEM-Team-2021-2022
Mark T Sheehan STEM Team code for Sikorsky STEM Challenge 2021-2022

// Libraries
#include <Servo.h>

double target = 75;
double range = 15;

// Pin Constants
int trigPin = 2;
int echoPin = 3;
int servoPin = 7;

//Tuning constants 
double searchLength = 15;
double sensorDelay = 0.2;
int landRange = 15;
int maxVals = 100;

// Global variables 
int numDataPoints = 0;
double dataLog[100][2];

// Temporary angle constant, handle 
Servo myservo;
double angle = 0;

void search(int t, int delay) {
  // @ Param t - duration of the search in seconds
  // @ Param delay - delay between sensor readings (in seconds)

  // Set the ending time to the current time plus the duration in milliseconds
  int endTime = t*1000 + millis();

  while(millis() < endTime && numDataPoints < maxVals) {

     //servo code meant to check and change the position of the servo-swinging it back and forth
     if(abs(target - myservo.read()) <= 1){
        if(target == 90 - range)
        {
          target = 90 + range;
          myservo.write(target);
        }
        
        else
        {
          target = 90 - range;
          myservo.write(target);
        }
     }
    double tempDistTotal = 0;
    double startTime = millis();
    
    for(int i = 0; i < 50; i++) {
      // Collect data point
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);

      // Convert signal to distance
      double duration = pulseIn(echoPin, HIGH);
      double distance = (duration * 0.0343 /2) * cos(myservo.read());

      // Check if distance is valid
      if(distance > 0 && distance < 500) {
        // Add distance to the log
        tempDistTotal += distance/50;
      }
      else {
        Serial.println("Value detected out of sensor range.");
        i--;
      }

      // Wait to limit data input
      delayMicroseconds(delay * 10000);
    }
    Serial.println(50 - tempDistTotal);
    dataLog[numDataPoints][0] = tempDistTotal;
    dataLog[numDataPoints][1] = startTime;
    numDataPoints++;
  }
}

void setup() {

  myservo.attach(servoPin);

  Serial.begin(9600);

  Serial.println("Starting");

  myservo.write(75);
  
  // Setup pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin,INPUT);

  Serial.begin(9600);

  // Run search - only runs once, so it is in setup
  search(searchLength, sensorDelay);

  // The optimal landing location is flat and low

  // Land time is set to zero by default
  double landTime = 0;
  int landIndex = 0;

  // Now that data is pooled, search for a section with minimum deviance
  double minDeviance = 500;
  

  for(int i = 0; i <= numDataPoints - landRange; i++) {
    double maxVal = 0;
    double minVal = 500;

    for(int j = i; j < i + landRange; j++) {
      if(dataLog[j][0] > maxVal) {
        maxVal = dataLog[j][0];
      }
      if(dataLog[j][0] < minVal) {
        minVal = dataLog[j][0];
      }
    }

    double deviance = maxVal - minVal;

    if(deviance < minDeviance) {
      minDeviance = deviance;
      landTime = (dataLog[i][1] + dataLog[i + landRange - 1][1])*0.5;
      landIndex = i + landRange / 2;
    }
  }
  Serial.println();
  Serial.print(landTime/1000);
  Serial.println("s");
  Serial.print(landIndex);
  Serial.println("i");
}

void loop() { }
