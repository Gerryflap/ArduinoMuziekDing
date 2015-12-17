
const int PULSE_TIME = 100;       //Pulse time in milliseconds
const int PULSE_MOTOR_PIN = 3;    //The pulse motor output
const int BPM_INPUT = A0;         //The analog input for BPM



boolean isMotorOn = false;
unsigned long lastMotorOn = 0;    //Holds the last time that the motor was on in milliseconds
double bpm;



int getBPM() {
  return analogRead(BPM_INPUT);
}

int getPulseStrength() {
  return 200;
}

void sendBPM() {
  Serial.print("B");
  Serial.println(getBPM());  
}

void updateMotor() {
  //Calculates the time between 2 pulses given the BPM
  double pulseFrequency = (60 * 1000.0)/((double) getBPM());
  int currentTime = millis();

  
  if (currentTime - lastMotorOn > pulseFrequency) {
    //if the motor should be on again, turn it on and update the isMotorOn value
    lastMotorOn = currentTime;
    isMotorOn = true;
    analogWrite(PULSE_MOTOR_PIN, getPulseStrength());    
  } else if (currentTime - lastMotorOn > PULSE_TIME) {
   
    //If the motor should be off...
    if (isMotorOn) {
      //... and if the motor is on, stop it and set isMotorOn to false;
      analogWrite(PULSE_MOTOR_PIN, 0); 
      isMotorOn = false;
    }
  }
}


void setup() {
  Serial.begin(9600);
  //I put port 10 as a constant high port.
  digitalWrite(10, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  sendBPM();
  updateMotor();

}



