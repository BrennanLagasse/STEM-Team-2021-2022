// Libraries
#include <Servo.h>

// Universal constants
const double pi = 3.1415;

// Challenge specific constants
const double range = 25;
const double speed = 5/60; // In ft/s

// Pin Constants
const int trigPin = 2;
const int echoPin = 3;
const int servoPin = 7;

// Servo variables
double pos = 90;
boolean servoDirection = false;

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
    
    double tempDistTotal = 0;
    double startTime = millis();
    
    for(int i = 0; i < 50; i++) {
      // Group of 50
      
      // Move servo
      if(servoDirection) {
          pos += 1.5;
          myservo.write(pos);

          if(pos > 90 + range) {
              servoDirection = false;
          }
      }
      else {
          pos -= 1.5;
          myservo.write(pos);
          
          if(pos < 90 - range) {
              servoDirection = true;
          }
      }

      // Collect data point
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);

      // Convert signal to distance
      double duration = pulseIn(echoPin, HIGH);
      double d = (duration * 0.0343 / 2);
      double a = pos * (pi/180);
      double distance = d * sin(a);

      // Check if distance is valid
      if(d > 0 && d < 500 && pos > 90 - range + 5 && pos < 90 + range - 5) {
        // Add distance to the log
        tempDistTotal += distance/50;

        Serial.println(distance);
      }
      else {
        // Serial.println("Value detected out of sensor range.");
        i--;
      }

      // Wait to limit data input
      delayMicroseconds(30000);
    }
    // Serial.println(50 - tempDistTotal);
    dataLog[numDataPoints][0] = tempDistTotal;
    dataLog[numDataPoints][1] = startTime;
    numDataPoints++;
  }
}

void pickLocation() {
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
  
  // Output values
  Serial.println();
  double t = landTime/1000;
  double position = t * speed;
  
  if(position < 0) {
    position = 0;
  }
  else if(position > 8) {
    position = 8;
  }
  Serial.println("Position: " + position);
}

void resetSearch() {
  pos = 90;
  myservo.write(pos);
  
  // Reset data log (values remain, but are overwritten/ignored
  numDataPoints = 0;
}

void setup() {
  myservo.attach(servoPin);

  Serial.begin(9600);

  Serial.println("Starting");

  myservo.write(pos);
  
  // Setup pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin,INPUT);

  Serial.begin(9600);

  // One time delay to ensure it starts at reset position
  delayMicroseconds(1500);
}

boolean c = true;

void loop() { 
  if(c) { // This needs a button associated with it
    search(searchLength, sensorDelay);
    pickLocation();
    resetSearch();
  }
}
