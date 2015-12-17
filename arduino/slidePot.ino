/*
  Pot slider b10k circuit:
  1: voltage pin
  2: GND
  3: analog read

  read values from the Arduino Pro Mini range from 0 to 20,
  with spikes after changing the slider. We need to implement a
  spike detection; also a distorted output should be recalibrated.
 */

// These constants won't change:
const int sensorPin = A0;    // pin that the analog output is attached to
const int ledPin = 13;       // the powerpin!

// variables:
int sliderValue = 0;         // the slider value

void setup() {
  Serial.begin(9600);      // open the serial port at 9600 bps: 
  // turn on LED to signal the start of the calibration period:
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
}

void loop() {
  // read the sensor:
  sliderValue = analogRead(sensorPin);

  // fade the LED using the calibrated value:
  Serial.print(sliderValue);
  Serial.print('\n');
}
