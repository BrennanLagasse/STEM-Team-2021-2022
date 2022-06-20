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

  Serial.println(endTime);
  Serial.print(millis());

  while(millis() < endTime) {
    Serial.println(millis());
  }
  
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

    // Check if the distance value is valid. If so, save it. If not, trash it.
    if(distance > 0 && distance < 500) {
      // Print out distance
      //Serial.println(distance);

      // Add distance to the log along with a timestamp
      dataLog[numDataPoints][0] = distance;
      dataLog[numDataPoints][1] = millis();

      // Update the counter for the number of data points that have been measured
      numDataPoints += 1;
    }
    else {
      // Warn the user about a value out of bounds
      Serial.println("Value detected out of sensor range.");
    }
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
  search(10);

  Serial.println("Done searching");
  
  // Landing range
  int groupSize = 10;
  
  // Stores the minimum difference between the minimum and maximum values of a group. Set to maximum (450) by default for ease of use.
  double minDiff = 450;
  
  // Stores the index of the middle value of this group. Set to 0 by default.
  int minIndex = 0;
  
  // Increase to increase precision, decrease to increase speed
  int incrementSize = 5;
  
  // Process data with minimum/maximum difference
  for(int i = 0; i <= numDataPoints - groupSize; i += incrementSize) {
    // Set to opposite extreme values by default
    double total = 0
    
    for(int j = i; j < groupSize + i; j++) {
      total+=dataLog[j,0];
    }
    
    // Find the diffence over the region
    double diff = localMax - localMin;
    
    // Update best position if the difference is smaller than the current lowest
    if(diff < minDiff) {
      minDiff = diff;
      minIndex = i;
    }
  }
}
