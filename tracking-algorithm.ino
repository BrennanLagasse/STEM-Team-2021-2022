int trigPin = 2;
int echoPin = 3;
double duration;
double distance;

// Store data as [height, time] for every data point
int numDataPoints = 0;
double dataLog[1000][2];

// Custom functions (run only when called)
void search(int duration) {
  // This function collects sensor data over an interval and saves the data to a log
  // @ Param duration - duration of the search in seconds
  
  // Set the ending time to the current time plus the duration in milliseconds
  int endTime = duration*1000 + millis();
  
  while(millis() < endTime) {
    // Collect data
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Distance in centimeters - between 30 and 450
    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.0343 /2;

    // Print out distance
    Serial.println(distance);

    // Add distance to the log along with a timestamp
    dataLog[numDataPoints, millis()];

    // Update the counter for the number of data points that have been measured
    numDataPoints += 1;
  }
}


// Runs on start
void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin,INPUT);

  Serial.begin(9600);
  
}

// Runs for duration of program
void loop() {
  search();
}
