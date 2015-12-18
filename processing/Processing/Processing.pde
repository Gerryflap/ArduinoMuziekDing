import controlP5.*;
import ddf.minim.*;
import processing.serial.*;

/* 6 options in the values array:
 0 = on/off (play/pause) -> push button
 1 = cue slider -> we do that ourselves from behind the laptop (GUI)
     0 > music only, 1 > both, 2 > pulse only
 2 = pulse button (0-4) -> push button
 3 = volume (0-40) -> analog slider
 4 = BPM (89-119) -> analog slider
 (5 = music button -> not necessary in hi-fi prototype)
*/
 
int[] values = new int[5];
int volume = 0;
String buffer; // A string named buffer is declared
Serial port; // Serial is the name of the class and our port is called port
Minim minim;
AudioPlayer[] player = new AudioPlayer[7]; // 7 possible BPMs, so 7 samples
ControlP5 gui; // Define the new GUI
Textarea BPM;
int index = 0;

void setup() { 
  
  port = new Serial(this, Serial.list()[Serial.list().length - 1], 9600); // Open the last port in the list at 9600 Baud
  //port = new Serial(this, Serial.list()[0], 9600); // Open the first port in the list (port 0) at 9600 Baud
  size(500, 400);
  minim = new Minim(this); // pass this to Minim so it can load files from the data directory

  // load all files, located in the data folder
  player[0] = minim.loadFile("Albert - 89 BPM3.wav");
  player[1] = minim.loadFile("Albert - 94 BPM3.wav");
  player[2] = minim.loadFile("Albert - 99 BPM3.wav");
  player[3] = minim.loadFile("Albert - 104 BPM.mp3");
  player[4] = minim.loadFile("Albert - 109 BPM3.wav");
  player[5] = minim.loadFile("Albert - 114 BPM3.wav");
  player[6] = minim.loadFile("Albert - 119 BPM3.wav");

  noStroke();
  //Create the new GUI
  gui = new ControlP5(this);
  //Add a Button
  gui.addButton("Music")
    //Set the position of the button : (X,Y)
    .setPosition(50, 50)
      //Set the size of the button : (X,Y)
      .setSize(100, 100)
        //Set the pre-defined Value of the button : (int)
        .setValue(0)
          //set the way it is activated : RELEASE the mouseboutton or PRESS it
          .activateBy(ControlP5.RELEASE);
  ;
  gui.addButton("Both")
    .setPosition(200, 50)
      .setSize(100, 100)
        .setValue(1)
          .activateBy(ControlP5.RELEASE);
  ;
  gui.addButton("Pulse")
    .setPosition(350, 50)
      .setSize(100, 100)
        .setValue(2)
          .activateBy(ControlP5.RELEASE);
  ;
  gui.addButton("Sync")
    .setPosition(200, 200)
      .setSize(100, 50)
        .setValue(2)
          .activateBy(ControlP5.RELEASE);
  ;
  BPM = gui.addTextarea("BPM")
    .setPosition(50, 280)
      .setText("BPM: ")
         .setSize(400, 300)
          .setFont(createFont("Arial", 65))
            .setColorValue(0xff003652);
  ;
}

// The draw section (loops over and over again)
void draw() {
    background(255); // Set the background color to white

  while (port.available () > 0) { // Execute the code between the curly brackets when there is incoming serial data
    serialEvent(port.read()); // The serial data is being read
  }
  OnOff();
  setPulse();
  setVolume();
  setBPM();
}

void OnOff() {
  // If the button is "on" and the cue slides is on "music" or "both"
  if (values[0] == 1 && (values[1] == 0 || values[1] == 1)) {
    // play the file from start to finish.
    // if you want to play the file again, 
    // you need to call rewind() first.
    //player.play();
  } else if (values[0] == 0) {
    // stop playing
   // minimStop();
  }
}

public void Music(int value) {
  port.write("C0");
  values[1] = 0;
}

public void Both(int value) {
  // This is the place for the code, that is activated by the buttonb
  port.write("C1"); // write to Arduino for the pulse
  values[1] = 1;
}

public void Pulse(int value) {
  // This is the place for the code, that is activated by the buttonb
  port.write("C2"); // write to Arduino
  values[1] = 2;
}

public void Sync(int value) {
  port.write("SBPM"); 
}

void setPulse() {
  // this is done in Arduino
}

void setVolume() {
  // Set the volume gain in decibel
  int volume = int(map(values[3], 0, 40, -40, 0));
  //player[1].setGain(volume); 

  //player[1].play();
}

void setBPM() { 
  BPM.setText("BPM: " + values[4]);
  // set the right index in the array of audio samples
  index = (values[4]-89)/5;
  // set corresponding music file
}

void minimStop()
{
  // always close Minim audio classes when you are done with them
  for (int i = 0; i < player.length; i++) {
    player[i].close();
  }
  minim.stop();
  super.stop();
}
