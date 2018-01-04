
const byte channel_A = 47;
const byte channel_B = 49;
#define interfacePin DAC0
#define startPin 45
#define ledRed 51
#define ledGreen 53
volatile int virtualPosition = 0;
volatile int oldVP = 0;

void setup() {
  Serial.begin(9600);

  analogWriteResolution(12); //Sets the resolution of the DAC to 12Bits (2^10 = 4096)
  
  pinMode(ledGreen, OUTPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(interfacePin, OUTPUT);
  pinMode(startPin, INPUT_PULLUP);
  pinMode(channel_A, INPUT_PULLUP);
  pinMode(channel_B, INPUT_PULLUP);
  
//Holds the program and lets the dual LED blink red until the start button is pressed

  while (digitalRead(startPin) == HIGH) { 
    digitalWrite(ledRed, HIGH);        
    delay(1000);
    digitalWrite(ledRed, LOW);
    delay(1000);
  }
  
//When the LED turns constant green the intterupts are attached and the analog output starts at the voltage corresponding to virtualPosition = 0
  
  digitalWrite(ledRed, LOW);
  digitalWrite(ledGreen, HIGH);
  
  attachInterrupt(digitalPinToInterrupt(channel_A), ISRa, CHANGE);
  attachInterrupt(digitalPinToInterrupt(channel_B), ISRb, CHANGE);
}



void loop(){

//Prints the virtualPosition over USB everytime it changes to make sure the program runs correct.

if(oldVP != virtualPosition){
  oldVP = virtualPosition;
  Serial.print(virtualPosition);
  Serial.print('\n'); 
}

//Resets the virtualPosition everytime it gets over 239 or below 1 to indicate that one full rotation is done.

if (virtualPosition > 239) {
    virtualPosition = 0;
  }
  else if (virtualPosition < 1) {
    virtualPosition = 240;
  }

// Converts the virtualPosition in an analog Voltage for a  neuralynx channel to read. The Arduino due uses a 12Bit DAC, hence the 4096.

analogWrite(interfacePin, 4096 / 240 * virtualPosition);


}

void ISRa() {

  //Debouncing: lets the ISR only increment to virtualPosition if a set amount of time has past since the last change.
  
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();
  if (interruptTime - lastInterruptTime > 5) {

  //Set of conditions to increment virtualPosition on every CHANGE to read the full resolution of the teethed encoder wheel.
  
    if (digitalRead(channel_A) == HIGH && (digitalRead(channel_B) == LOW)) {
      virtualPosition  ++;
    }
    else if (digitalRead(channel_A) == LOW && digitalRead(channel_B) == HIGH) {
      virtualPosition ++;
    }
    else {
      virtualPosition --;
    }


  }
}

void ISRb() {

  //Debouncing: lets the ISR only increment to virtualPosition if a set amount of time has past since the last change.
  
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();
  if (interruptTime - lastInterruptTime > 5) {

 //Set of conditions to increment the virtualPosition on every CHANGE to read the full resolution of the teethed encoder wheel.
 
    if (digitalRead(channel_B) == HIGH && (digitalRead(channel_A) == HIGH)) {
      virtualPosition ++;
    }
    else if (digitalRead(channel_B) == LOW && digitalRead(channel_A) == LOW) {
      virtualPosition ++;
    }
    else {
      virtualPosition --;
    }


  }
}
