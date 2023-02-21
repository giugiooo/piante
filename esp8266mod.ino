#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <LittleFS.h>

// Wifi network station credentials
#define WIFI_SSID "gm"
#define WIFI_PASSWORD "perlapiantina"

// Telegram BOT Token (Get from Botfather)
#define BOT_TOKEN "6233250501:AAFuVuQdSnJul5rW2N7KKskDbMqVXMApj_s"

const char *SUBSCRIBED_USERS_FILENAME = "/subscribed_users.json"; // Filename for local storage
const unsigned long BULK_MESSAGES_MTBS = 1500;                    // Mean time between send messages, 1.5 seconds
const unsigned int MESSAGES_LIMIT_PER_SECOND = 25;                // Telegram API have limit for bulk messages ~30 messages per second
const unsigned long BOT_MTBS = 2*60000;                              // Mean time between scan messages

WiFiClientSecure secured_client;
X509List cert(TELEGRAM_CERTIFICATE_ROOT);
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
DynamicJsonDocument usersDoc(1500);
unsigned long bot_lasttime; // last time messages' scan has been done


void sendMessageToDennisGiuliaAndCarolina(){
  String message = "You need to feel the water tank to keep your plant alive!";
  bot.sendMessage("1479828415", message, "");
  bot.sendMessage("1903278869", message, "");
  bot.sendMessage("1953797257", message, "");

}

void setup()
{
  Serial.begin(115200);
  Serial.println();

  if (!LittleFS.begin())
  {
    Serial.println("Failed to mount file system");
    return;
  }

  // attempt to connect to Wifi network:
  configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
  secured_client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());

  // Check NTP/Time, usually it is instantaneous and you can delete the code below.
  Serial.print("Retrieving time: ");
  time_t now = time(nullptr);
  while (now < 24 * 3600)
  {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }
  Serial.println(now);
}

void loop()
{
  if (millis() - bot_lasttime > BOT_MTBS)
  {
    sendMessageToDennisGiuliaAndCarolina();

    bot_lasttime = millis();
  }
}