// code for send data on Thingspeak
#include <WiFi.h>
#include <ThingSpeak.h>
//#include<WiFiClient.h>
const char *ssid = "Infinix NOTE 12";  //Infinix NOTE 12
const char *password = "payal46002"; //payal46002
const char *api = "E8C0IAKX5AXGIDUJ";
const int channel = 2483271;
WiFiServer server(80);

WiFiClient client;
#include<DHT.h>
#define Type DHT11
int sensepin = 18;
DHT HT(sensepin,Type);
float humidity;
float temperature_C;
float temperature_F;
void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(921600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  ThingSpeak.begin(client);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(2000);
    Serial.print(WiFi.status());
  }
  Serial.print("connected");
  HT.begin();
  delay(1000);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  humidity = HT.readHumidity();
  temperature_C = HT.readTemperature();
  temperature_F = HT.readTemperature(true);
  Serial.print("humidity :");
  Serial.print(humidity);
  delay(2);
  Serial.print("  temperature_C : ");
  Serial.print(temperature_C);
  delay(10);
  Serial.print("  temperature_F : ");
  Serial.println(temperature_F);
  delay(10);
 int a = ThingSpeak.writeField(channel, 1, temperature_C, api);
 //int b = ThingSpeak.writeField(channel, 2, temperature_F, api);
 //int c = ThingSpeak.writeField(channel, 3, humidity, api);
  //checkThingSpeakStatus(a, b, c);
 if ( a==200)    //|| b==200 || c==200)
 {
 Serial.print("Success/n");
 }
 else
 {
 Serial.print("problem occured");
 }
 delay(5000);
}

