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
      //else if (c == 'C') values[1] = Integer.parseInt(buffer); // Convert the string 'buffer' into an integer
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
