
#include "CTBot.h"
CTBot myBot;

String ssid = "WIFI_NAME";     
String pass = "WIFI_PASSWORD"; 
String token = "token";   // REPLACE token WITH YOUR TELEGRAM BOT TOKEN
uint8_t led = 2;        // the onboard ESP8266 LED.    
uint8_t fan=3;                     // If you have a NodeMCU you can use the BUILTIN_LED pin
int f=0,l=0;                          // (replace 2 with BUILTIN_LED)							

void setup() {
	Serial.begin(115200);
	Serial.println("Starting TelegramBot...");

	// connect the ESP8266 to the desired access point
	myBot.wifiConnect(ssid, pass);

	// set the telegram bot token
	myBot.setTelegramToken(token);

	// check if all things are ok
	if (myBot.testConnection())
		Serial.println("\ntestConnection OK");
	else
		Serial.println("\ntestConnection NOK");

	// set the pin connected to the LED to act as output pin
	pinMode(led, OUTPUT);
	digitalWrite(led, HIGH); // turn off the led (inverted logic!)
  pinMode(fan,OUTPUT);
  digitalWrite(fan, HIGH);

}

void loop() {
	// a variable to store telegram message data
	TBMessage msg;

	// if there is an incoming message...
	if (myBot.getNewMessage(msg)) {

		if (msg.text.equalsIgnoreCase("LIGHT ON")) {              // if the received message is "LIGHT ON"...
			digitalWrite(led, LOW); // turn on the LED (inverted logic!)
      Serial.println("Light on msg received");
     int l=1;
			myBot.sendMessage(msg.sender.id, "Light is now ON");  // notify the sender
		}
   else if (msg.text.equalsIgnoreCase("FAN ON")) {        // if the received message is "LIGHT OFF"...
     digitalWrite(fan, HIGH);// turn off the led (inverted logic!)
     Serial.println("FAN is now ON");
     int f=1;
      myBot.sendMessage(msg.sender.id, "FAN is now ON"); // notify the sender
    }
    else if (msg.text.equalsIgnoreCase("FAN OFF")) {        // if the received message is "LIGHT OFF"...
     digitalWrite(fan, LOW);// turn off the led (inverted logic!)
     Serial.println("FAN is now OFF");
     int f=0;
      myBot.sendMessage(msg.sender.id, "FAN is now OFF"); // notify the sender
    }
		else if (msg.text.equalsIgnoreCase("LIGHT OFF")) {        // if the received message is "LIGHT OFF"...
			digitalWrite(led, HIGH);// turn off the led (inverted logic!)
     Serial.println("Light of msg received");
     int l=0;
			myBot.sendMessage(msg.sender.id, "Light is now OFF"); // notify the sender
		}
   else if (msg.text.equalsIgnoreCase("status")) {
     Serial.println("Checking Status");
      myBot.sendMessage(msg.sender.id, "Sending you status..Let me check");
      if(digitalRead(fan) == HIGH){
          myBot.sendMessage(msg.sender.id, "fan is on"); }
      if(digitalRead(led) == HIGH){
          myBot.sendMessage(msg.sender.id, "led is on"); }
      myBot.sendMessage(msg.sender.id, "Status Completed");
    }
    else if (msg.text.equalsIgnoreCase("devices")) {
     Serial.println("Checking Connected Devices");
      myBot.sendMessage(msg.sender.id, "Let me check connected Devices");
      myBot.sendMessage(msg.sender.id, "Connected Devices are Light and Fan");
    }
		else {                                                    // otherwise...
			// generate the message for the sender
			String reply;
			reply = (String)"Welcome " + msg.sender.username + (String)". Try LIGHT ON or LIGHT OFF.";
			myBot.sendMessage(msg.sender.id, reply);             // and send it
		}
	}
	// wait 500 milliseconds
	delay(500);
}
