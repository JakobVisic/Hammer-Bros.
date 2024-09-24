/*
██╗  ██╗ █████╗ ███╗   ███╗███╗   ███╗███████╗██████╗     ██████╗ ██████╗  ██████╗ ███████╗   
██║  ██║██╔══██╗████╗ ████║████╗ ████║██╔════╝██╔══██╗    ██╔══██╗██╔══██╗██╔═══██╗██╔════╝   
███████║███████║██╔████╔██║██╔████╔██║█████╗  ██████╔╝    ██████╔╝██████╔╝██║   ██║███████╗   
██╔══██║██╔══██║██║╚██╔╝██║██║╚██╔╝██║██╔══╝  ██╔══██╗    ██╔══██╗██╔══██╗██║   ██║╚════██║   
██║  ██║██║  ██║██║ ╚═╝ ██║██║ ╚═╝ ██║███████╗██║  ██║    ██████╔╝██║  ██║╚██████╔╝███████║██╗


   __   __   __ _  __  ____    _  _  __  ____  __  ___ 
 _(  ) / _\ (  / )/  \(  _ \  / )( \(  )/ ___)(  )/ __)
/ \) \/    \ )  ((  O )) _ (  \ \/ / )( \___ \ )(( (__ 
\____/\_/\_/(__\_)\__/(____/   \__/ (__)(____/(__)\___)


Hammer Bros.

Jakob Visic

OCADU DIGF-6037 Creation & Computation

A 2 player game where the first player to score 3 points wins.
Use your Hammer to hit the sensore to change your Orange lights brightness.
Match your organge light's brightness with the brightness of the light in the center to score a point.
There is a 1 second delay (on both players sides) after ever hit, so time your hits accordingly.
Once a player wins, the lights will fade 3 times, all lights will light up, and then the game will restart.

*/


// ----------------------------------
// GLOBAL VARIABLES
// ----------------------------------
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

int p2PointPin1 = 12;  // these pins were originally pin 7 and 8 instead of 12 and 11 but for some reason pin 7 and 8 stopped working so i switched them
int p2PointPin2 = 11;  // even after trouble shooting, trying different lights, wires, and etc, it didnt work.
int p2PointPin3 = 9;

// initialize points to 0
int P1points = 0;
int P2points = 0;

// controls the range that the sensor has to read (the high the number, the easier it is to get a point)
int rangeValue = 30; 
int timeBetweenHits = 1000;

int p1mappedSensorValue = 0;
int p2mappedSensorValue = 0;

int goalSensorRangeMax;
int goalSensorRangeMin;

// sets the variable to winner, 0 means there is no winner.
int winner = 0;
int gameFinished = 0;


// ----------------------------------
// SETUP
// ----------------------------------
void setup() {
  // initialize serial communication: 
  Serial.begin(9600);

  delay(1000);
  // create a random seed so that random() function is different every load
  randomSeed(analogRead(0));

  // randomize the first value
  delay(1000);
  goalSensorValue = random(25, 256);


  Serial.print("GOAL VALUE ");
  Serial.println(goalSensorValue);
}


// ----------------------------------
// MAIN LOOP
// ----------------------------------
void loop() {
  // read sensor pin and store value in a variable:
  if (winner == 0) {
    P1sensorValue = analogRead(P1SensorPin); 
    P2sensorValue = analogRead(P2SensorPin);
  } 

  // write the goalSensor value to the GOALpin
  analogWrite(GOALpin, goalSensorValue);

  // set the min and max ranges based on rangeValue variable (this effects the difficulty of the game)
  goalSensorRangeMax = goalSensorValue + rangeValue;
  goalSensorRangeMin = goalSensorValue - rangeValue;

  // map sensor value  
  p1mappedSensorValue = map(P1sensorValue, 0, 1023, 0, 255);
  p2mappedSensorValue = map(P2sensorValue, 0, 1023, 0, 255);

  // call functions
  if (winner == 0) {
    // call only if there is no winner
    checkIfHit();
    checkPlayerPoints();
  }

  if (gameFinished == 0){
    winnerCheck();
  }
}

// ----------------------------------
// FUNCTIONS
// ----------------------------------

// ------ Check if the sensor was hit and set a delay between hits --------
void checkIfHit () {
    // set a timer between hits, preventing players from holding down hammer and cheating
  if (p1mappedSensorValue > 25 && gameFinished == 0) { // if the Sensor value is above 25, and the game isnt finished, then 
    analogWrite(P1LEDpin, p1mappedSensorValue);        // set the players light to the mapped sensor value
  
    timeBetweenHits = random(1000, 3000);
    delay(timeBetweenHits);                            // set a delay so the player cannot hit again unti the delay is done

    analogWrite(P1LEDpin, 0);                          // set the light back to 0 so that the player knows they can hit again
  } 
  if (p2mappedSensorValue > 25 && gameFinished == 0) { 
    analogWrite(P2LEDpin, p2mappedSensorValue);        
  
    timeBetweenHits = random(1000, 3000);
    delay(timeBetweenHits);                            // set a delay so the player cannot hit again unti the delay is done

    analogWrite(P2LEDpin, 0);                          // set the light back to 0 so that the player knows they can hit again
  }
}


// ------ Check if the player got points --------
void checkPlayerPoints () {
  // ------ PLAYER ONE POINT CHECK --------
  if (P1points == 0 && p1mappedSensorValue > goalSensorRangeMin && p1mappedSensorValue < goalSensorRangeMax) {
    // if the player has 0 points, and the mapped sensor value is within the goalRange, and nobody has won the game, then

    // turn on the first point pin
    analogWrite(p1PointPin3, 255);
    P1points++;
    // print the points
    // Serial.print("POINTS = ");
    // Serial.println(P1points);

    // create a new goal
    goalSensorValue = random(256);
    // Serial.print("GOAL VALUE ");
    // Serial.println(goalSensorValue);
    return;
    // Serial.print(newGoalSensorValue);
  } else if  (P1points == 1 && p1mappedSensorValue > goalSensorRangeMin && p1mappedSensorValue < goalSensorRangeMax) {
    analogWrite(p1PointPin2, 255);
    P1points++;
    // Serial.print("POINTS = ");
    // Serial.println(P1points);

    goalSensorValue = random(256);
    // Serial.print("GOAL VALUE ");
    // Serial.println(goalSensorValue);
   
    return;
  } else if  (P1points == 2 && p1mappedSensorValue > goalSensorRangeMin && p1mappedSensorValue < goalSensorRangeMax) {

    analogWrite(p1PointPin3, 255);
    P1points++;

    // Serial.print("POINTS = ");
    // Serial.println(P1points);

    winner = 1;
    return;
  }

// ------ PLAYER TWO POINT CHECK --------
  if (P2points == 0 && p2mappedSensorValue > goalSensorRangeMin && p2mappedSensorValue < goalSensorRangeMax) {
    // if the player has 0 points, and the mapped sensor value is within the goalRange, and nobody has won the game, then
    
    // turn on the first point pin
    analogWrite(p2PointPin1, 255);
    P2points++;
    // print the points
    // Serial.print("p2 POINTS = ");
    // Serial.println(P2points);

    // create a new goal
    goalSensorValue = random(256);
    // Serial.print("GOAL VALUE ");
    // Serial.println(goalSensorValue);
    return;
    // Serial.print(newGoalSensorValue);
  } else if  (P2points == 1 && p2mappedSensorValue > goalSensorRangeMin && p2mappedSensorValue < goalSensorRangeMax) {
    analogWrite(p2PointPin2, p2mappedSensorValue);
    P2points++;
    // Serial.print("POINTS = ");
    // Serial.println(P2points);

    goalSensorValue = random(256);
    // Serial.print("GOAL VALUE ");
    // Serial.println(goalSensorValue);
    
    return;
  } else if  (P2points == 2 && p2mappedSensorValue > goalSensorRangeMin && p2mappedSensorValue < goalSensorRangeMax) {

    analogWrite(p2PointPin3, p2mappedSensorValue);
    P2points++;

    // Serial.print("POINTS = ");
    // Serial.println(P2points);

    winner = 2;
    return;
  }
}


// ------- WINNER --------
void winnerCheck () {
  if (winner == 1) {
    // Serial.println("Player 1 wins");

    for (int i = 0; i < 4; i++) {
      analogWrite(p1PointPin1, 255);
      analogWrite(p1PointPin2, 255);
      analogWrite(p1PointPin3, 255);
      analogWrite(P1LEDpin, 255);
      delay(250);
      for (int i = 255; i>0; i = i - 10) {
          analogWrite(p1PointPin1, i);
          analogWrite(p1PointPin2, i);
          analogWrite(p1PointPin3, i);
          analogWrite(P1LEDpin, i);
          delay(50);
      }
    }

    delay(2000);
    gameFinished = 1;
    restartGame();
  } else if (winner == 2) {
    // Serial.println("Player 2 wins");

    for (int i = 0; i < 4; i++) {
      analogWrite(p2PointPin1, 255);
      analogWrite(p2PointPin2, 255);
      analogWrite(p2PointPin3, 255);
      analogWrite(P2LEDpin, 255);
      delay(250);
      for (int i = 255; i>0; i = i - 10) {
          analogWrite(p2PointPin1, i);
          analogWrite(p2PointPin2, i);
          analogWrite(p2PointPin3, i);
          analogWrite(P2LEDpin, i);
          delay(50);
      }
    }

    delay(2000);
    gameFinished = 1;
    restartGame();
  }
}

// ------- RESTART GAME FUNCTION --------
void restartGame () {  

  // light up all lights and then set to lights to 0 brightness to signify new round
  analogWrite(p1PointPin1, 255);
  analogWrite(p1PointPin2, 255);
  analogWrite(p1PointPin3, 255);

  analogWrite(p2PointPin1, 255);
  analogWrite(p2PointPin2, 255);
  analogWrite(p2PointPin3, 255);

  analogWrite(P1LEDpin, 255);
  analogWrite(P2LEDpin, 255);
  analogWrite(GOALpin, 255);

  delay(2000);

  analogWrite(p1PointPin1, 0);
  analogWrite(p1PointPin2, 0);
  analogWrite(p1PointPin3, 0);

  analogWrite(p2PointPin1, 0);
  analogWrite(p2PointPin2, 0);
  analogWrite(p2PointPin3, 0);

  analogWrite(P1LEDpin, 0);
  analogWrite(P2LEDpin, 0);
  analogWrite(GOALpin, 0);


  // create a new random goal 
  goalSensorValue = random(25, 256);

  // reset points
  P1points = 0;
  P2points = 0;
  // reset winner to 0 - (no winner)
  winner = 0;
  // set game finished to 0 
  gameFinished = 0;
}
