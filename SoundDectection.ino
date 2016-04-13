int previousValue_1 = 0;
int previousValue_2 = 0;
int previousValue_3 = 0;

int sensorValue_1 = 0;
int sensorValue_2 = 0;
int sensorValue_3 = 0;

//PINS
int FRONT = 2;
int LEFT = 3;
int RIGHT = 4;  
int BACK = 5;

//Pin connected to ST_CP of 74HC595
int latchPin = 8; //green
//Pin connected to SH_CP of 74HC595
int clockPin = 12; //yellow
////Pin connected to DS of 74HC595
int dataPin = 11; //blue

int vibTime = 300;
int soundMag = 55;

// the setup routine runs once when you press reset:
void setup() {

  pinMode(2, OUTPUT);

  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  //Each mic will follow the same pattern
  //Detect any change in sound
  sensorValue_1 = analogRead(A0);

  //Test to see if the sound is within the differential to respond
  if(calcDiff(sensorValue_1, previousValue_1) > soundMag) {
    //Write to the vibrating disc from the shift register
    registerWrite(0,HIGH);
    //Allow it to vibrate
    delay(vibTime);
  } else {
    //Otherwise ensure that it is off
    registerWrite(0,LOW);
  }
  
  sensorValue_2 = analogRead(A2);
  if(calcDiff(sensorValue_2, previousValue_2) > soundMag) {
    registerWrite(1,HIGH);
    delay(vibTime);
  } else {
    registerWrite(1,LOW);
  }

  // read the input on analog pin 0:
  sensorValue_3 = analogRead(A3);


  Serial.println(calcDiff(sensorValue_3, previousValue_3));
  if(calcDiff(sensorValue_3, previousValue_3) > soundMag) {
    registerWrite(2,HIGH);
    delay(vibTime);
  } else {
    registerWrite(2,LOW);
  }
  
  delay(5);        // delay in between reads for stability

  previousValue_1 = sensorValue_1;
  previousValue_2 = sensorValue_2;
  previousValue_3 = sensorValue_3;

  
}

int calcDiff(int val1, int val2) {
  int diff = val1 - val2;
  return abs(diff);
}

void registerWrite(int whichPin, int whichState) {
// the bits you want to send
  byte bitsToSend = 0;

  // turn off the output so the pins don't light up
  // while you're shifting bits:
  digitalWrite(latchPin, LOW);

  // turn on the next highest bit in bitsToSend:
  bitWrite(bitsToSend, whichPin, whichState);

  // shift the bits out:
  shiftOut(dataPin, clockPin, MSBFIRST, bitsToSend);

    // turn on the output so the LEDs can light up:
  digitalWrite(latchPin, HIGH);

}

