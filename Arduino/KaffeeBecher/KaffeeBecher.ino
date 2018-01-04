int redLedPin = 7;
int blueLedPin = 6;
int greenLedPin = 5;
int sensorPin = 2;

void setup() {
digitalWrite(greenLedPin, HIGH);
delay(1000);
digitalWrite(greenLedPin, LOW);

}

void loop() {
  int reading = analogRead(sensorPin);
  float voltage = reading * 5.0;
  voltage /= 1024.0;
  float temperatureC = (voltage -0.5) * 100;

  if (temperatureC >= 65.0){ 
    digitalWrite(redLedPin, HIGH);
  }
    else { 
    digitalWrite(redLedPin, LOW);
  } 

  
  if (temperatureC < 65.0 && temperatureC > 55.0) { 
    digitalWrite(greenLedPin, HIGH);
  }
    else {
    digitalWrite(greenLedPin, LOW);
    }
  if (temperatureC < 55.0)
    digitalWrite(blueLedPin, HIGH);
    else digitalWrite(blueLedPin, LOW);
    


}
