/*
Experiment 1: Analog Mapping - 1 Sensor Value


OCADU DIGF-6037 Creation & Computation - taught by Kate Hartman & Nick Puckett


The value of the sensor is mapped to the brightness of the LED.
*/


// initialize pressure sensor pins and variables
int P1sensorValue = 0;
int P2sensorValue = 0;
int P1SensorPin = A0;
int P2SensorPin = A1;


// Sets the goal pin and the sensor value
int GOALpin = 10;
int goalSensorValue;


// initialize player pin
int P1LEDpin = 2;
int P2LEDpin = 3;


// Player pin setup
int p1PointPin1 = 4;
int p1PointPin2 = 5;
int p1PointPin3 = 6;


int p2PointPin1 = 7;
int p2PointPin2 = 8;
int p2PointPin3 = 9;


// initialize points to 0
int P1points = 0;
int P2points = 0;


// controls the range that the sensor has to read (the high the number, the easier it is to get a point)
int rangeValue = 30; 
int timeBetweenHits = 1000;


int p1mappedSensorValue = 0;
int p2mappedSensorValue = 0;


// sets the variable to winner, 0 means there is no winner.
int winner = 0;
int gameFinished = 0;


void(* resetFunc) (void) = 0; //declare reset function @ address 0


// ----------------------------------
// setup
// ----------------------------------
void setup() {
  // initialize serial communication: 
  Serial.begin(9600);
  // goalSensorValue = random(1, 255);


  // create a random seed so that random() function is different every load
  randomSeed(analogRead(2));


  // randomize the first value
  delay(100);
  goalSensorValue = random(25, 256);
  delay(100);


  Serial.print("GOAL VALUE ");
  Serial.println(goalSensorValue);


  
}



// ----------------------------------
// main loop
// ----------------------------------
void loop() {
  // read sensor pin and store value in a variable:
  if (winner == 0) {
    P1sensorValue = analogRead(P1SensorPin); 
    P2sensorValue = analogRead(P2SensorPin);
  } 



  analogWrite(GOALpin, goalSensorValue);
  
  // Serial.println(startTime);


  // map sensor value  
  p1mappedSensorValue = map(P1sensorValue, 0, 1023, 0, 255);
  p2mappedSensorValue = map(P2sensorValue, 0, 1023, 0, 255);


  // set a timer between hits, preventing players from holding down hammer and cheating
  if (p1mappedSensorValue > 25 && gameFinished == 0) { // if the Sensor value is above 25, and the game isnt finished, then 
    analogWrite(P1LEDpin, p1mappedSensorValue);        // set the players light to the mapped sensor value
    Serial.println(p1mappedSensorValue);        
  
    delay(timeBetweenHits);                            // set a delay so the player cannot hit again unti the delay is done


    analogWrite(P1LEDpin, 0);                          // set the light back to 0 so that the player knows they can hit again
    // Serial.println("TIMER END");
  } 
  if (p2mappedSensorValue > 25 && gameFinished == 0) { // if the Sensor value is above 25, and the game isnt finished, then 
    analogWrite(P2LEDpin, p2mappedSensorValue);        // set the players light to the mapped sensor value
    Serial.println(p2mappedSensorValue);        
  
    delay(timeBetweenHits);                            // set a delay so the player cannot hit again unti the delay is done


    analogWrite(P2LEDpin, 0);                          // set the light back to 0 so that the player knows they can hit again
    // Serial.println("TIMER END");
  } 


// set the min and max ranges based on rangeValue variable (this effects the difficulty of the game)
  int goalSensorRangeMax = goalSensorValue + rangeValue;
  int goalSensorRangeMin = goalSensorValue - rangeValue;



// ------ PLAYER ONE POINT CHECK --------
  if (P1points == 0 && p1mappedSensorValue > goalSensorRangeMin && p1mappedSensorValue < goalSensorRangeMax && winner == 0) {
    // if the player has 0 points, and the mapped sensor value is within the goalRange, and nobody has won the game, then


    // turn on the first point pin
    analogWrite(p1PointPin3, 255);
    P1points++;
    // print the points
    Serial.print("POINTS = ");
    Serial.println(P1points);


    // create a new goal
    goalSensorValue = random(256);
    Serial.print("GOAL VALUE ");
    Serial.println(goalSensorValue);
    return;
    // Serial.print(newGoalSensorValue);
  } else if  (P1points == 1 && p1mappedSensorValue > goalSensorRangeMin && p1mappedSensorValue < goalSensorRangeMax && winner == 0) {
    analogWrite(p1PointPin2, 255);
    P1points++;
    Serial.print("POINTS = ");
    Serial.println(P1points);


    goalSensorValue = random(256);
    Serial.print("GOAL VALUE ");
    Serial.println(goalSensorValue);
   
    return;
  } else if  (P1points == 2 && p1mappedSensorValue > goalSensorRangeMin && p1mappedSensorValue < goalSensorRangeMax && winner == 0) {


    analogWrite(p1PointPin3, 255);
    P1points++;


    Serial.print("POINTS = ");
    Serial.println(P1points);


    winner = 1;
    return;
  }


// ------ PLAYER TWO POINT CHECK --------
  if (P2points == 0 && p2mappedSensorValue > goalSensorRangeMin && p2mappedSensorValue < goalSensorRangeMax && winner == 0) {
    // turn on the first point pin
    analogWrite(p2PointPin1, 255);
    P2points++;
    // print the points
    Serial.print("p2 POINTS = ");
    Serial.println(P2points);


    // create a new goal
    goalSensorValue = random(256);
    Serial.print("GOAL VALUE ");
    Serial.println(goalSensorValue);
    return;
    // Serial.print(newGoalSensorValue);
  } else if  (P2points == 1 && p2mappedSensorValue > goalSensorRangeMin && p2mappedSensorValue < goalSensorRangeMax && winner == 0) {
    analogWrite(p2PointPin2, p2mappedSensorValue);
    P2points++;
    Serial.print("POINTS = ");
    Serial.println(P2points);


    goalSensorValue = random(256);
    Serial.print("GOAL VALUE ");
    Serial.println(goalSensorValue);
    
    return;
  } else if  (P2points == 2 && p2mappedSensorValue > goalSensorRangeMin && p2mappedSensorValue < goalSensorRangeMax && winner == 0) {


    analogWrite(p2PointPin3, p2mappedSensorValue);
    P2points++;


    Serial.print("POINTS = ");
    Serial.println(P2points);


    winner = 2;
    return;
  }


// ------- WINNER --------
  if (winner == 1 && gameFinished == 0) {
    // Serial.println("Player 1 wins");


    // light up all player 1 pins, turn off player 2 pins
    analogWrite(p1PointPin1, 255);
    analogWrite(p1PointPin2, 255);
    analogWrite(p1PointPin3, 255);


    analogWrite(p2PointPin1, 0);
    analogWrite(p2PointPin2, 0);
    analogWrite(p2PointPin3, 0);


    analogWrite(P1LEDpin, 255);


    delay(2000);
    gameFinished = 1;
    // resetFunc();  //call reset


  } else if (winner == 2 && gameFinished == 0) {
    // Serial.println("Player 2 wins");


    // light up all player 2 pins, turn off player 1 pins
    analogWrite(p1PointPin1, 0);
    analogWrite(p1PointPin2, 0);
    analogWrite(p1PointPin3, 0);


    analogWrite(p2PointPin1, 255);
    analogWrite(p2PointPin2, 255);
    analogWrite(p2PointPin3, 255);


    analogWrite(P2LEDpin, 255);


    delay(2000);
    gameFinished = 1;
  }
}





