import ddf.minim.*;
import processing.serial.*; // Importing the serial library

/* 6 options in the values array:
 0 = on/off (play/pause) -> push button
 1 = cue slider -> we do that ourselves from behind the laptop (GUI)
 2 = pulse button -> push button
 3 = volume -> analog slider
 4 = BPM (89-119) -> analog slider
 (5 = music button -> not necessary in hi-fi prototype)
 */
int[] values = new int[5];
int volume = 0;
String buffer; // A string named buffer is declared
Serial port; // Serial is the name of the class and our port is called port
Minim minim;
AudioPlayer[] player = new AudioPlayer[7];

void setup() { 
  port = new Serial(this, Serial.list()[0], 9600); // Open the first port in the list (port 0) at 9600 Baud
  size(400, 400);
  background(255); // Set the background color to white
  minim = new Minim(this); // pass this to Minim so it can load files from the data directory

  // load all files, located in the data folder
  player[3] = minim.loadFile("Albert - 104 BPM.mp3");
}

// The draw section (loops over and over again)
void draw() {
  while (port.available () > 0) { // Execute the code between the curly brackets when there is incoming serial data
    serialEvent(port.read()); // The serial data is being read
  }
  
  // On/off button
  if (values[0] == 1) {
    // play the file from start to finish.
    // if you want to play the file again, 
    // you need to call rewind() first.
    //player.play();
  } else if (values[0] == 0) {
    // stop playing
  }
  
  // Cue slider: 0 = music only, 1 = both, 2 = pulse only
  if (values[1] == 0) {
    // stop pulse (done in Arduino)
    player.play();
  } else if (values[1] == 1) {
    player.play();
    // start pulse (done in Arduino)
  } else if (values[1] == 2) {
    stop();
    // start pulse (done in Arduino)
  }

  // Set the volume gain in decibel
  values[3] = constrain(values[3], 0, 40);
  float volume = map(values[3], 0, 40, -40, 0);
  player.setGain(-15); 
  
  player.play();
}

void serialEvent(int serial) { // Function declaration, the integer serial gets the values of the serial data that's being send via port.read()
  try { // Using the try-catch structure in case of errors in the transmission
    if (serial != 10) { // Condition, when the sended data is not newline, the value is stored in the buffer 
      buffer += char(serial);
    } else {  // If the value is a newline, then this new line is filtered by removing it from the string
      char c = buffer.charAt(0);
      buffer = buffer.substring(1);
      buffer = buffer.substring(0, buffer.length()-1); // Filtering the carriage returns and removing it from the buffer

      // On/off
      if (c == 'O') values[0] = Integer.parseInt(buffer); // Convert the string 'buffer' into an integer
      // Cue slider
      else if (c == 'C') values[1] = Integer.parseInt(buffer); // Convert the string 'buffer' into an integer
      // Pulse
      else if (c == 'P') values[2] = Integer.parseInt(buffer); // Convert the string 'buffer' into an integer
      // Volume
      else if (c == 'V') values[3] = Integer.parseInt(buffer); // Convert the string 'buffer' into an integer
      // BPM
      else if (c == 'B') values[4] = Integer.parseInt(buffer); // Convert the string 'buffer' into an integer

      buffer = ""; // The value of the string 'buffer' is cleared
    }
  } 
  catch(Exception e) { //when errors in the transmission occur, than print the sentence: "There is no valid data" in the message window
    println("There is no valid data");
  }
}

void stop()
{
  // always close Minim audio classes when you are done with them
  player.close();
  minim.stop();
  super.stop();
}
