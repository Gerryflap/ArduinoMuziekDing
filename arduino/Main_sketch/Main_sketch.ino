
const int PULSE_TIME = 100;           //Pulse time in milliseconds
const int PULSE_MOTOR_PIN = 3;        //The pulse motor output
const int BPM_INPUT = A0;             //The analog input for BPM
const int PLAY_BUTTON_PIN = A1;       //Pin of play input
const int CONSTANT_VOLTAGE_PIN = 10;  //Pin with a constant voltage




boolean isMotorOn = false;
unsigned long lastMotorOn = 0;    //Holds the last time that the motor was on in milliseconds
double bpm;
boolean isPulseOn = false;
boolean isMusicOn = true;
boolean isPlaying = false;
boolean playButtonInputHandled = false;


/**
 * The get functions to get state information:
 */

int getBPM() {
  double input = analogRead(BPM_INPUT);
  input = min(input, 10);
  input = max(input, 0);
  input = (int) (7 * input/10.0);
  input = 5.0 * input + 89;
  return input;
}

int getPulseStrength() {
  if (isPulseOn) {
    return 200;
  } else {
    return 0;
  }
}


/**
 * The main loop update functions.
 * These functions interact with the pins and update out model.
 */
void checkForPlayButtonUpdate() {
  boolean currentState = analogRead(PLAY_BUTTON_PIN) > 1000;
  if (currentState = HIGH && !playButtonInputHandled) {
    isPlaying = !isPlaying;
    playButtonInputHandled = true;
    sendPlayStatusUpdate(isPlaying);
  } else if (currentState == LOW) {
    playButtonInputHandled = false;
  }
}

void updateMotor() {
  if (isPlaying) {
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
  } else {
    lastMotorOn = 0;
  }
}

/**
 * The serial connection methods.
 * These methods handle communication between the computer and the aruino.
 */

 void sendBPM() {
  Serial.print("B");
  Serial.println(getBPM());  
}

void sendPlayStatusUpdate(boolean value) {
  Serial.print("O");
  Serial.println(value);
}

void receiveFromComputer() {
 if (Serial.available()) {
    String received = Serial.readString();
    if (received.charAt(0) == 'C') {
      int mode = received.substring(1,2).toInt();
      if (mode == 0) {
        isPulseOn = false;
      } else {
        isPulseOn = true;
      }
    }
  }
}

/**
 * The main arduino functions
 */

void setup() {
  Serial.begin(9600);
  //I put port 10 as a constant high port.
  digitalWrite(CONSTANT_VOLTAGE_PIN, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  sendBPM();  
  checkForPlayButtonUpdate();
  updateMotor();

}



