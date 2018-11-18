#include <Thread.h>

String serialStr; // String used for feedback over serial

Thread inputThread = Thread(); // Used threading to time the scanning of switches state independent of main loop

bool decPinState = false; // Used to "clock" CD4017
int decPin = 3; // Used pin 3 for clock

int decReturnPin = 4; // The returning signal

int decCounter = 0; // Used to control the amount of switches to traverse

bool input1 = false; // Placeholders for
bool input2 = false; // the state of
bool input3 = false; // the switches

void setup() {

  Serial.begin(9600); // Used for feedback

  inputThread.enabled = true; // Enabling threading
  inputThread.setInterval(200); // Interval in ms to run the "scan command"
  inputThread.onRun(count); // Specifying the function to run

  // Setting the pin states
  pinMode(decPin, OUTPUT);
  pinMode(decReturnPin, INPUT);

  //These are only used for visual feedback in the video:
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);

}

void loop() {

  // Running the thread
  if (inputThread.shouldRun()) {
    inputThread.run();
  }

}

void count() {

  decPinState ? decPinState = false : decPinState = true; // Toggles pin 3 between LOW and HIGH,
  digitalWrite(decPin, decPinState);                      // mimmicking a clock signal

  if (decPinState) { //The CD4017 traverses when clock goes high, so the scanning will happen between the shifts
    switch (decCounter) { // This set wich switch the CD4017 is feeding
      case 1:
        // statements
        input1 = digitalRead(decReturnPin);
        break;
      case 2:
        // statements
        input2 = digitalRead(decReturnPin);
        break;
      case 3:
        // statements
        input3 = digitalRead(decReturnPin);
        break;
      default:
        // statements
        break;
    }
    
    digitalWrite(A0, !input1);
    digitalWrite(A1, !input2);
    digitalWrite(A2, !input3);

    serialStr = "input is: ";
    serialStr += input1;
    serialStr += input2;
    serialStr += input3;

    Serial.println(serialStr);

    decCounter++;

    if (decCounter > 3) {
      decCounter = 0;
    }


  }

}
