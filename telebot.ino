//d1 dht,d2 led,rx fan,A0 flamesenser,D5 Buzzer
#include "DHT.h"
#define DHTPIN 5          // pin d1
#define DHTTYPE DHT11     // DHT11
DHT dht(DHTPIN, DHTTYPE);

#include "CTBot.h"
CTBot myBot;

String ssid = "Wifi Name";     
String pass = "Wifi Password"; 
String token = "Your Telegram Token";   // REPLACE token WITH YOUR TELEGRAM BOT TOKEN
uint8_t led = 4;        // d2 connect led .. inbuilt on then replace 4 with 2    
uint8_t fan=3;                     // RX
int flame_detected;
uint8_t buzz=14;							

void setup() {
	Serial.begin(115200);
	Serial.println("Starting TelegramBot...");
  dht.begin();

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
	digitalWrite(led, LOW); // turn off the led (inverted logic!)
  pinMode(fan,OUTPUT);
  digitalWrite(fan, LOW);
  pinMode(buzz,OUTPUT);
  digitalWrite(buzz,LOW);
}

void loop() {
	// a variable to store telegram message data
	TBMessage msg;
  flame_detected = analogRead(A0);
  if (flame_detected <= 800)
  {
    myBot.sendMessage(msg.sender.id, "Fire in house");
    Serial.println("Fire Detected!!");
    Serial.println(flame_detected);
    digitalWrite(buzz,HIGH);
    delay(1000);
  }
	// if there is an incoming message...
	if (myBot.getNewMessage(msg)) {

		if (msg.text.equalsIgnoreCase("LIGHT ON")) {              // if the received message is "LIGHT ON"...
			digitalWrite(led, HIGH); // turn on the LED (inverted logic!)
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
			digitalWrite(led, LOW);// turn off the led (inverted logic!)
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
          float tempC = dht.readTemperature();
          float hum = dht.readHumidity();
         String temp;
      temp=(String)"Current Temperature: " +tempC+(String)"  Current Humidity"+hum;
      myBot.sendMessage(msg.sender.id,temp );
      myBot.sendMessage(msg.sender.id, "Status Completed");
    }
    else if (msg.text.equalsIgnoreCase("devices")) {
     Serial.println("Checking Connected Devices");
      myBot.sendMessage(msg.sender.id, "Let me check connected Devices");
      myBot.sendMessage(msg.sender.id, "Connected Devices are Light and Fan");
    }
    else if (msg.text.equalsIgnoreCase("check temperature") | msg.text.equalsIgnoreCase("check humidity") | msg.text.equalsIgnoreCase("check environment") )
    {
      float tempC = dht.readTemperature();
      float hum = dht.readHumidity();
      String temp;
      temp=(String)"Current Temperature: " +tempC+(String)"  Current Humidity"+hum;
      Serial.println(tempC);
      myBot.sendMessage(msg.sender.id,temp );
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
