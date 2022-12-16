#include <WiFi.h>
#include <PubSubClient.h>
#include <Keypad.h>

// WiFi
const char *ssid = "AplnrWifi"; // Enter your WiFi name
const char *password = "rjcjapolinar";  // Enter WiFi password

// MQTT Broker
const char *mqtt_broker = "broker.emqx.io";
const char *topic = "emqx/c-test";
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const int mqtt_port = 1883;

//LED Matrix
int latchPin = 2;          // Pin connected to ST_CP of 74HC595（Pin12）
int clockPin = 4;          // Pin connected to SH_CP of 74HC595（Pin11）
int dataPin = 15;          // Pin connected to DS of 74HC595（Pin14）


WiFiClient espClient;
PubSubClient client(espClient);

//keypad keys
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

    // set pins to output for LED Matrix
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
 // Set software serial baud to 115200;
 Serial.begin(115200);
 // connecting to a WiFi network
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.println("Connecting to WiFi..");
 }
 Serial.println("Connected to the WiFi network");
 //connecting to a mqtt broker
 client.setServer(mqtt_broker, mqtt_port);
 client.setCallback(callback);
 while (!client.connected()) {
     String client_id = "esp32-client-";
     client_id += String(WiFi.macAddress());
     Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
     if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
         Serial.println("Public emqx mqtt broker connected");
     } else {
         Serial.print("failed with state ");
         Serial.print(client.state());
         delay(2000);
     }
 }
 // publish and subscribe
 client.publish(topic, "Hi EMQX I'm ESP32 ^^");
 client.subscribe(topic);
}


byte *board; //all received messages should only be board updates 

//callback for messages to arduino
void callback(char *topic, byte *payload, unsigned int length) {
 Serial.print("Message arrived in topic: ");
 Serial.println(topic);
 Serial.print("Message:");
 for (int i = 0; i < 9; i++) {
     Serial.print((char) payload[i]);
 }
 Serial.println();
 Serial.println("-----------------------");
}

//functions to read rows and columns of LED Matrix
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

void loop() {
   // Get the character input
 char keyPressed = myKeypad.getKey();
 String str = String(keyPressed);
 if (keyPressed) {
   const char *c = str.c_str();
 client.publish(topic, c);
 }

//LED Matrix LOOP
  int cols;
  //blank board at start of game
  int col8 = 0x00;//right col of board
  int col6= 0x00;//middle
  int col4= 0x00;//left
  for (int j = 0; j < 500; j++ ) //repeat image 500 times so it displays
  {
     cols = 0x01;//first column selected
     matrixRowsVal(col8);
     matrixColsVal(~cols); // select this column
     delay(1); // display them for a period of time
     matrixRowsVal(0x00); // clear the data of this column
     cols <<= 2; // shift "cols" 1 bit left to select the next column
     matrixRowsVal(col6);
     matrixColsVal(~cols); // select this column
     delay(1); // display them for a period of time
     matrixRowsVal(0x00); // clear the data of this column
     cols <<= 2; // shift "cols" 1 bit left to select the next column
     matrixRowsVal(col4);
     matrixColsVal(~cols); // select this column
     delay(1); // display them for a period of time
     matrixRowsVal(0x00); // clear the data of this column
  }

 
 //loop for mqtt
 client.loop();


}


