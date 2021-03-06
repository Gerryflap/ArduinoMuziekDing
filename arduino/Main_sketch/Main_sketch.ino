
const double PULSE_TIME = 0.25;        //Pulse time 
const int PULSE_MOTOR_PIN = 3;        //The pulse motor output
const int BPM_INPUT = A0;             //The analog input for BPM
const int VOLUME_INPUT = A2;
const int PLAY_BUTTON_PIN = A1;       //Pin of play input
const int CONSTANT_VOLTAGE_PIN = 10;  //Pin with a constant voltage




boolean isMotorOn = false;
unsigned long lastMotorOn = 0;    //Holds the last time that the motor was on in milliseconds
double bpm;
double volume;
boolean isPulseOn = false;
boolean isMusicOn = true;
boolean isPlaying = false;
boolean playButtonInputHandled = false;
int bpmAnalogCounter = 0;
int volumeAnalogCounter = 0;
double pulseFrequency;


/**
 * The get functions to get state information:
 */

int getBPM() {
  
  return bpm;
}

int getPulseStrength() {
  if (isPulseOn) {
    return 255;
  } else {
    return 0;
  }
}


/**
 * The main loop update functions.
 * These functions interact with the pins and update out model.
 */
void checkForPlayButtonUpdate() {
  int value = analogRead(PLAY_BUTTON_PIN);
  //Serial.println(value);
  boolean currentState = value > 200;
  if (currentState == HIGH && !playButtonInputHandled) {
    isPlaying = !isPlaying;
    playButtonInputHandled = true;
    sendPlayStatusUpdate(isPlaying);
  } else if (currentState == LOW) {
    playButtonInputHandled = false;
  }
}

void updateBPM() {
  double input = analogRead(BPM_INPUT);
  input *= 7.0/1024;
  input = min(input, 6);
  input = max(input, 0);
  input = (int) input;
  input = 5.0 * input + 89;
  if (input != bpm) {
    if (bpmAnalogCounter < 10) {
      bpmAnalogCounter += 1;
    } else {
      bpm = input;
      sendBPM();
      bpmAnalogCounter = 0;
      pulseFrequency = (60 * 1000.0)/((double) getBPM());
    }
  }
  if (input == bpm) {
    bpmAnalogCounter = 0;
  }
}

void updateVolume() {
  double input = analogRead(VOLUME_INPUT);
  
  input *= 40.0/1024;
  input = min(input, 40);
  input = max(input, 0);
  input = (int) input;
  if (input != volume) {
    if (volumeAnalogCounter < 10) {
      volumeAnalogCounter += 1;
    } else {
      volume = input;
      volumeAnalogCounter = 0;
      sendVolume();
    }
  }
  if (input == volume) {
    volumeAnalogCounter = 0;
  }
}

void updateMotor() {
  if (isPlaying) {
    //Calculates the time between 2 pulses given the BPM
    int currentTime = millis();
  
    
    if (currentTime - lastMotorOn > pulseFrequency) {
      //if the motor should be on again, turn it on and update the isMotorOn value
      lastMotorOn = currentTime;

      isMotorOn = true;
      analogWrite(PULSE_MOTOR_PIN, getPulseStrength());
    } else if (currentTime - lastMotorOn > PULSE_TIME * pulseFrequency) {
     
      //If the motor should be off...
      if (isMotorOn) {
        //... and if the motor is on, stop it and set isMotorOn to false;
        analogWrite(PULSE_MOTOR_PIN, 0); 
        isMotorOn = false;
      }
    }
  } else {
    lastMotorOn = 0;
    if (isMotorOn) {
        analogWrite(PULSE_MOTOR_PIN, 0); 
        isMotorOn = false;
    }
  }
}

void syncMotor(unsigned long syncTime) {
    lastMotorOn = syncTime - pulseFrequency;
    isMotorOn = false;
    analogWrite(PULSE_MOTOR_PIN, 0);
    updateMotor();
}

/**
 * The serial connection methods.
 * These methods handle communication between the computer and the aruino.
 */

void sendBPM() {
  Serial.print("B");
  Serial.println(getBPM());  
}

void sendVolume() {
  Serial.print("V");
  Serial.println((int) volume);  
}

void sendPlayStatusUpdate(boolean value) {
  Serial.print("O");
  Serial.println(value);
}

void receiveFromComputer() {
 if (Serial.available()) {
    unsigned long currentTime = millis();
    String received = Serial.readString();
    if (received.charAt(0) == 'C') {
      int mode = received.substring(1,2).toInt();
      if (mode == 0) {
        isPulseOn = false;
      } else {
        isPulseOn = true;
      }
    } else if (received.equals("SBPM")) {
      syncMotor(currentTime);
    } else if (received.equals("UPDATE")) {
      sendPlayStatusUpdate(isPlaying);
      sendBPM();
      sendVolume();
    }
  }
}

/**
 * The main arduino functions
 */

void setup() {
  Serial.begin(9600);
  pinMode(9, OUTPUT);
  pinMode(CONSTANT_VOLTAGE_PIN, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(PULSE_MOTOR_PIN, OUTPUT);
  pinMode(PLAY_BUTTON_PIN, INPUT);
  pinMode(VOLUME_INPUT, INPUT);
  pinMode(BPM_INPUT, INPUT);
  analogReference(DEFAULT);
  //I put port 10 as a constant high port.
  digitalWrite(CONSTANT_VOLTAGE_PIN, HIGH);  
  digitalWrite(11, HIGH);
  digitalWrite(9, HIGH);

}

void loop() {
  // put your main code here, to run repeatedly:
  
  receiveFromComputer();
  checkForPlayButtonUpdate();
  updateBPM();
  updateVolume();
  updateMotor();
  

}



