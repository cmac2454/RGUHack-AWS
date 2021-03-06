/*
  RGU ATTENDANCE
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Key.h>
#include <Keypad.h>
#include <b64.h>
#include <SPI.h>
#include <Ethernet.h>

String students[10];
int attend[10];
int inputLen = 0;
int IDlen = 7;
String nums = "1234567890";
String input = "";

//Initialize demo array
void arrInit(){
  for (int i = 0; i < 10; i = i + 1) {
    students[i] = "1000000";
    attend[i] = 0;
  }
}

//KeyPad init
const byte rows = 4; //four rows
const byte cols = 4; //three columns
char keys[rows][cols] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[rows] = {A5, A4, A3, A2}; //connect to the row pinouts of the keypad
byte colPins[cols] = {A1, A0, 6, 7}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

//EthernetClient init

byte mac[] = { 0x98, 0x4F, 0xEE, 0x01, 0xCE, 0x36 };

char* url = "rguattend.azurewebsites.net";

EthernetClient client;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial){
  }
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
  }
  pinMode(rowPins[0], INPUT);
  pinMode(rowPins[1], INPUT);
  pinMode(rowPins[2], INPUT);
  pinMode(rowPins[3], INPUT);
  pinMode(colPins[0], INPUT);
  pinMode(colPins[1], INPUT);
  pinMode(colPins[2], INPUT);
  pinMode(colPins[3], INPUT);
  arrInit();
}

void loop() {
  client.connect(url, 80);
  int markFlag = 0;
  char key = keypad.getKey();
  if (key == '#') {
    if (inputLen != IDlen){
      Serial.println("Invalid ID number. Must be 7 digits long.");
      Serial.print("You entered ");
      Serial.print(inputLen);
      Serial.println(" digits.");
      input = "";
      inputLen = 0;
    }
    else {
      Serial.println("Connecting...");
      if (client.connected()){
        Serial.println("Connected.");
        String postQuery = "ID=";
        postQuery += input;
        Serial.println("POST ID QUERY");
        client.println("POST /query.php HTTP/1.1");
        client.println("Host: rguattend.azurewebsites.net");
        client.println("Content-Type: multipart/form-data");
        client.println();
        client.print(postQuery);
        input = "";
        inputLen = 0;
      }
      else {
        Serial.println("Connection failed, please try again.");
      }
    }
  }
  else if(key == '*'){
    Serial.println(input);
  }
  else if(nums.indexOf(key) >= 0 && nums.indexOf(key) < 10) {
    input+=key;
    inputLen = inputLen + 1;
    Serial.println(input);
    Serial.println(inputLen);   
  }

  delay(1);
}
