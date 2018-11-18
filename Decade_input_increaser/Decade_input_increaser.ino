#include <Thread.h>
#include <ThreadController.h>

//String serialStr; // String used for feedback over serial

Thread inputThread = Thread(); // Used threading to time the scanning of switches state independent of main loop

bool decPinState = false; // Used to "clock" 4017
int decPin = 3; // Used pin 3 for clock

int decReturnPin = 4; // The returning signal

int decCounter = 0; // Used to control the amount of switches to traverse

bool input1 = false; // Placeholders for
bool input2 = false; // the state of
bool input3 = false; // the switches

ThreadController controller = ThreadController();


void setup() {

  //Serial.begin(9600); // Used for feedback

  inputThread.enabled = true; // Enabling threading
  inputThread.setInterval(1); // Interval in ms to run the "scan command"
  inputThread.onRun(count); // Specifying the function to run

  controller.add(&inputThread);

  // Setting the pin states
  pinMode(decPin, OUTPUT);
  pinMode(decReturnPin, INPUT);

  //These are used for visual feedback in the video:
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);

}

void loop() {

  controller.run();

}

void count() {
  
  //Serial.println(decCounter);

  decPinState ? decPinState = false : decPinState = true; // Toggles pin 3 between LOW and HIGH,
  digitalWrite(decPin, decPinState);                      // mimmicking a clock signal

  if (!decPinState) { //The 4017 traverses when clock goes high, so the scanning will happen between the shifts
    switch (decCounter) { // This set which switch the 4017 is feeding to (used to differentiate which var to set)
      case 0:
        //Serial.println("Case 0!");
        input1 = digitalRead(decReturnPin); // Because the 4017 is at its first "step"
        break;                              // the switch for "input1" is determing the voltage on pin 3
      case 1:
        //Serial.println("Case 1!");
        input2 = digitalRead(decReturnPin); // As above
        break;
      case 2:
        //Serial.println("Case 2!");
        input3 = digitalRead(decReturnPin); // As above
        break;
      default: // Should not be in used in this code, but it doesn't harm to keep here.
        //Serial.println("What?!");
        break;
    }

    //These are used for visual feedback in the video, turns the LED on, when button is pressed.
    digitalWrite(A0, !input1);
    digitalWrite(A1, !input2);
    digitalWrite(A2, !input3);

    //serialStr = "input is: ";
    //serialStr += input1;
    //serialStr += input2;
    //serialStr += input3;

    //Serial.println(serialStr);

    decCounter++; // INcreases by 1, used to traverse the case switch

    if (decCounter > 2) {
      decCounter = 0; // When decCounter surpasses 2 it gets reset to 0.
    }


  }

}
