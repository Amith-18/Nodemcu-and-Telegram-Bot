#include <ESP8266WiFi.h>

#include <WiFiClientSecure.h>

#include <UniversalTelegramBot.h>

#include <DHT.h>

#include <MQ135.h>

#include <BMP180.h>

#include <Wire.h>

#define DHTPIN D6

#define PIN_MQ135 A0

#define DHTTYPE DHT11

MQ135 mq135_sensor(PIN_MQ135); // define analog pin for MQ135 sensor //const int mq135_pin = A0; // define analog pin for MQ135 sensor

DHT dht(DHTPIN, DHTTYPE);

BMP180 myBMP(BMP180_ULTRAHIGHRES);

// Initialize WiFi connection

char ssid[] = " 5G"; // your network SSID (name) 
char password[] = "*****"; // your network password

// Initialize Telegram BOT

#define BOTtoken "*********" // your telegram bot token

WiFiClientSecure client;

UniversalTelegramBot bot(BOTtoken, client);

//Checks for new messages every 1 second.

int botRequestDelay = 1000;

unsigned long lastTimeBotRan;

void handleNewMessages(int numNewMessages) {

Serial.println("handleNewMessages");

Serial.println(String(numNewMessages));

for (int i = 0; i < numNewMessages; i++) {

String chat_id = String(bot.messages[i].chat_id);

String text = bot.messages[i].text;

String from_name = bot.messages[i].from_name;

// Your Chat ID get it from here: https://t.me/chatidx_bot

if (chat_id == "********") { // Chat id

  if (text == "/temperature") {

    int t = dht.readTemperature();

    String temp = "Temperature : ";

    temp += int(t);

    temp += "°C";

    Serial.println(from_name);

    bot.sendMessage(chat_id, temp, "");

  }

  if (text == "/humidity") {

    int h = dht.readHumidity();

    String temp = "Humidity: ";

    temp += int(h);

    temp += "%";

    bot.sendMessage(chat_id, temp, "");

  }
  if  (text == "/airquality"){

    int s = mq135_sensor.getPPM();

    String temp = "AirQuality: ";

    temp += int(s);

    temp +="PPM";

    bot.sendMessage(chat_id, temp, "");

  }
  if (text == "/pressure"){

    float p = myBMP.getPressure();

    String temp = "Pressure: ";

    temp += float(p);

    temp +="Hg";

    bot.sendMessage(chat_id, temp, "");
  }


} else {
  bot.sendMessage(chat_id, "Unauthorized User", "");
}

if (text == "/start") {

  String welcome = "Welcome  " + from_name + "\n\n Choose your option\n";

  welcome += "/temperature : Get Temperature\n";

  welcome += "/humidity : Get Humiditiy\n";

  welcome += "/airquality : Get Airquality\n";

  welcome += "/pressure : Get Pressure\n";


  bot.sendMessage(chat_id, welcome, "Markdown");

}
}

}

void setup() {

Serial.begin(115200);

dht.begin();

client.setInsecure();

// WiFo Connected

WiFi.mode(WIFI_STA);

WiFi.disconnect();

delay(100);

// Attempt to connect to WiFi network:

Serial.print("Connecting Wifi: ");

Serial.println(ssid);

WiFi.begin(ssid, password);

while (WiFi.status() != WL_CONNECTED) {

Serial.print(".");

delay(500);
}

Serial.println("");

Serial.println("WiFi connected");

Serial.print("IP address: ");

Serial.println(WiFi.localIP());

}

void loop() {

int t = dht.readTemperature();

int h = dht.readHumidity();

int s = mq135_sensor.getPPM();

float p = myBMP.getTemperature();

if (millis() > lastTimeBotRan + botRequestDelay) {

int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

while (numNewMessages) {

  Serial.println("Got Response");

  handleNewMessages(numNewMessages);

  numNewMessages = bot.getUpdates(bot.last_message_received + 1);

}

lastTimeBotRan = millis();
}

}