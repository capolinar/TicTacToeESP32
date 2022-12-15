#include <Keypad.h>
/**********************************************************************
  Filename    : LED Matrix Display
  Description : Use 2 74HC595 to drive the LED Matrix display
  Auther      : www.freenove.com
  Modification: 2020/07/11
**********************************************************************/
int latchPin = 2;          // Pin connected to ST_CP of 74HC595（Pin12）
int clockPin = 4;          // Pin connected to SH_CP of 74HC595（Pin11）
int dataPin = 15;          // Pin connected to DS of 74HC595（Pin14）

// Define the pattern data for a smiling face
const int smilingFace[] = {                       //"^ⅴ^"
  0x1C, 0x22, 0x51, 0x45, 0x45, 0x51, 0x22, 0x14
};
// Define the data of numbers and letters, and save them in flash area
const int data[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // " "
  0x00, 0x00, 0x21, 0x7F, 0x01, 0x00, 0x00, 0x00, // "1"
  0x00, 0x00, 0x23, 0x45, 0x49, 0x31, 0x00, 0x00, // "2"
  0x00, 0x00, 0x22, 0x49, 0x49, 0x36, 0x00, 0x00, // "3"
  0x00, 0x00, 0x0E, 0x32, 0x7F, 0x02, 0x00, 0x00, // "4"
  0x00, 0x00, 0x79, 0x49, 0x49, 0x46, 0x00, 0x00, // "5"
  0x00, 0x00, 0x3E, 0x49, 0x49, 0x26, 0x00, 0x00, // "6"
  0x00, 0x00, 0x60, 0x47, 0x48, 0x70, 0x00, 0x00, // "7"
  0x00, 0x00, 0x36, 0x49, 0x49, 0x36, 0x00, 0x00, // "8"
  0x00, 0x00, 0x32, 0x49, 0x49, 0x3E, 0x00, 0x00, // "9"
  0x00, 0x00, 0x3E, 0x41, 0x41, 0x3E, 0x00, 0x00, // "0"
  0x00, 0x00, 0x3F, 0x44, 0x44, 0x3F, 0x00, 0x00, // "A"
  0x00, 0x00, 0x7F, 0x49, 0x49, 0x36, 0x00, 0x00, // "B"
  0x00, 0x00, 0x3E, 0x41, 0x41, 0x22, 0x00, 0x00, // "C"
  0x00, 0x00, 0x7F, 0x41, 0x41, 0x3E, 0x00, 0x00, // "D"
  0x00, 0x00, 0x7F, 0x49, 0x49, 0x41, 0x00, 0x00, // "E"
  0x00, 0x00, 0x7F, 0x48, 0x48, 0x40, 0x00, 0x00  // "F"
};

// define the symbols on the buttons of the keypad
char keys[4][4] = {
 {'1', '2', '3', 'A'},
 {'4', '5', '6', 'B'},
 {'7', '8', '9', 'C'},
 {'*', '0', '#', 'D'}
};
byte rowPins[4] = {14, 27, 26, 25}; // connect to the row pinouts of the keypad
byte colPins[4] = {13, 21, 22, 23}; // connect to the column pinouts of the keypad
// initialize an instance of class NewKeypad
Keypad myKeypad = Keypad(makeKeymap(keys), rowPins, colPins, 4, 4);



void setup() {
  // set pins to output
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  Serial.begin(115200); // Initialize the serial port and set the baud rate to 115200
  Serial.println("ESP32 is ready! "); // Print the string "ESP32 is ready! "

}

void loop() {
 // Get the character input
 char keyPressed = myKeypad.getKey();
 // If there is a character input, sent it to the serial port
 if (keyPressed) {
 Serial.println(keyPressed);
 }


  // Define a one-byte variable (8 bits) which is used to represent the selected state of 8 column.
int cols;
 // Display the static smiling pattern
 for (int j = 0; j < 500; j++ ) { // repeat 500 times
 cols = 0x01;
 for (int i = 0; i < 8; i++) { // display 8 column data by scanning
 matrixRowsVal(smilingFace[i]);// display the data in this column
 matrixColsVal(~cols); // select this column
 delay(1); // display them for a period of time
 matrixRowsVal(0x00); // clear the data of this column
 cols <<= 1; // shift "cols" 1 bit left to select the next column
 }
 }
 // Display the dynamic patterns of numbers and letters
 for (int i = 0; i < 128; i++) {
  for (int k = 0; k < 10; k++) { // repeat image of each frame 10 times.
    cols = 0x01; // Assign binary 00000001. Means the first column is selected.
      for (int j = i; j < 8 + i; j++) { // display image of each frame
        matrixRowsVal(pgm_read_word_near(data + j));// display the data in this column
        matrixColsVal(~cols); // select this column
        delay(1); // display them for a period of time
        matrixRowsVal(0x00); // close the data of this column
        cols <<= 1; // shift "cols" 1 bit left to select the next column
      }
    }
 }

 
}

void matrixRowsVal(int value) {
  // make latchPin output low level
  digitalWrite(latchPin, LOW);
  // Send serial data to 74HC595
  shiftOut(dataPin, clockPin, LSBFIRST, value);
  // make latchPin output high level, then 74HC595 will update the data to parallel output
  digitalWrite(latchPin, HIGH);
}

void matrixColsVal(int value) {
  // make latchPin output low level
  digitalWrite(latchPin, LOW);
  // Send serial data to 74HC595
  shiftOut(dataPin, clockPin, MSBFIRST, value);
  // make latchPin output high level, then 74HC595 will update the data to parallel output
  digitalWrite(latchPin, HIGH);
}
