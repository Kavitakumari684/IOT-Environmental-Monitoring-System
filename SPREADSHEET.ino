#include "WiFi.h"
#include <HTTPClient.h>
#include "time.h"
#include "DHT.h" // Include DHT sensor library

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 0;

const char* ssid ="Infinix NOTE 12";      //Infinix NOTE 12
const char* password = "payal46002";       //payal46002

String GOOGLE_SCRIPT_ID = "AKfycbxGfNRptnosQ7WFtcPywW-ml-rnGXJopfUnXZ7rAYjSEySUywbIR1afGayysSTKis9U_g" ;                                                                       
int count = 0;

#define DHTPIN 18  
#define DHTTYPE DHT11  // Digital pin connected to the DHT sensor              

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  delay(1000);
  Serial.begin(921600);
  delay(1000);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  dht.begin(); // Initialize DHT sensor
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    static bool flag = false;
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
      Serial.println("Failed to obtain time");
      return;
    }
    char timeStringBuff[50];
    strftime(timeStringBuff, sizeof(timeStringBuff), "%A, %B %d %Y %H:%M:%S", &timeinfo);
    String asString(timeStringBuff);
    asString.replace(" ", "-");
    Serial.print("Time:");
    Serial.println(asString);

    float temperature = dht.readTemperature(); // Read temperature from DHT sensor
    float humidity = dht.readHumidity();       // Read humidity from DHT sensor

    String urlFinal = "https://script.google.com/macros/s/" + GOOGLE_SCRIPT_ID + "/exec?" +
                      "date=" + asString +                               //asString + // + String(count);
                      "&sensor=" + String(count)+
                      "&temperature="+String(humidity)+                                // String(temperature) + // Include temperature data
                      "&humidity="+String(temperature)  ;                                 // +String(temperature) ++ //Include humidity data
                                                  

    Serial.print("POST data to spreadsheet:");
    Serial.println(urlFinal);

    HTTPClient http;
    http.begin(urlFinal.c_str());
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET();
    Serial.print("HTTP Status Code: ");
    Serial.println(httpCode);

    String payload;
    if (httpCode > 0) {
      payload = http.getString();
      Serial.println("Payload: " + payload);
    }

    http.end();
  }
  count++;
  delay(1000);
}
