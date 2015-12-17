
const int PULSE_TIME = 100;
const int PULSE_MOTOR_PIN = 3;

double a;
boolean isMotorOn = false;
unsigned long lastMotorOn = 0; 
double bpm;



int getBPM() {
  return analogRead(A0);
}

int getPulseStrength() {
  return 200;
}

void sendBPM() {
  Serial.print("B");
  Serial.println(getBPM());  
}

void updateMotor() {
  double pulseFrequency = (60 * 1000.0)/((double) getBPM());
  int currentTime = millis();
  if (currentTime - lastMotorOn > pulseFrequency) {
    lastMotorOn = currentTime;
    isMotorOn = true;
    analogWrite(PULSE_MOTOR_PIN, getPulseStrength());    
  } else if (currentTime - lastMotorOn > PULSE_TIME) {
    if (isMotorOn) {
      analogWrite(PULSE_MOTOR_PIN, 0); 
      isMotorOn = false;
    }
  }
}


void setup() {
  Serial.begin(9600);
  digitalWrite(10, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  sendBPM();
  updateMotor();

}



