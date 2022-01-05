int trigPin = 2;
int echoPin = 3;
double duration;
double distance;


int numDataPoints = 0;
double dataLog[50][2];


void search(int duration) {
  // @ Param duration - duration of the search in seconds

  // Set the ending time to the current time plus the duration in milliseconds
  int endTime = duration*1000 + millis();

  while(millis() < endTime) {
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
      double distance = duration * 0.0343 /2;

      // Check if distance is valid
      if(distance > 0 && distance < 500) {
        // Add distance to the log
        tempDistTotal += distance/50;
      }
      else {
        Serial.println("Value detected out of sensor range.");
        i--;
      }
    }
    dataLog[numDataPoints][0] = tempDistTotal;
    dataLog[numDataPoints][1] = startTime;
    numDataPoints++;
  }
  Serial.println("Done searching");
}

void setup() {
  // Setup pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin,INPUT);

  Serial.begin(9600);

  // Run search - only runs once, so it is in setup
  search(10);

  // The optimal landing location is flat and low

  // Land time is set to zero by default
  int landTime = 0;

  // Choose landing range (should be around the length of the landing module
  int landRange = 5;

  // Now that data is pooled, search for a section with minimum deviance
  int minDeviance = 500;

  for(int i = 0; i <= dataLog.size() - landRange; i++) {
    int max = 0;
    int min = 500;

    for(int j = i; j < i + landRange; j++) {
      if(dataLog[j][0] > max) {
        max = dataLog[j][0];
      }
      if(dataLog[j][0] < min) {
        min = dataLog[j][0];
      }
    }

    double deviance = max - min;

    if(deviance < minDeviance) {
      minDeviance = deviance;
      landTime = (dataLog[i][1] + dataLog[i + landRange - 1][1])*0.5;
    }
  }

  System.out.println("The optimal landing location is at " + landTime + " seconds");
}
