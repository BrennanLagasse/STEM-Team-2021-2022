
void setup() {
  // Setup pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin,INPUT);

  Serial.begin(9600);
  
  // Run search - only runs once, so it is in setup
  search(10);

  // Size of averagePool
  int poolSize = 50;
  
  // New log with pooled data
  double pooledData[numDataPoints/poolSize][2];
  
  // Increment through pools
  for(int i = 0; i <= numDataPoints - poolSize; i += poolSize) {
    double total = 0;
    // Increment through elements of the pool and average to a single value
    for(int j = i; j < poolSize + i; j++) {
      total += dataLog[j,0][0];
    }
    
    double poolAverage = total/poolSize;
    
    pooledData[i/poolSize][0] = poolAverage;
    pooledData[i/poolSize][1] = (dataLog[i][1] + dataLog[i+poolSize-1][1])/2;
  }
  
  // The optimal landing location is flat and low
  
  // Land time is set to zero by default
  int landTime = 0;
  
  // Choose landing range (should be around the length of the landing module
  int landRange = 5;
  
  // Now that data is pooled, search for a section with minimum deviance
  int minDeviance = 500;
  
  for(int i = 0; i <= pooledData.size() - landRange; i++) {
    int max = 0;
    int min = 500;
    
    for(int j = i; j < i + landRange; j++) {
      if(pooledData[j][0] > max) {
        max = pooledData[j][0];
      }
      if(pooledData[j][0] < min) {
        min = pooledData[j][0];
      }
    }
    
    double deviance = max - min;
    
    if(deviance < minDeviance) {
      minDeviance = deviance;
      landTime = (pooledData[i][1] + pooledData[i + landRange - 1][1])*0.5;
    }
  }
  
  System.out.println("The optimal landing location is at " + landTime + " seconds");
}
