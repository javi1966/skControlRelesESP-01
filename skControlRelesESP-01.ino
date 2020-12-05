#include <ESP8266WiFi.h>

#define RELE_1  0
#define RELE_2  2



#define ON  0
#define OFF 1

int rele1 = OFF, rele2 = OFF;

WiFiServer server(80);

void setup() {
  // put your setup code here, to run once:
  pinMode(RELE_1, INPUT);
  pinMode(RELE_2, INPUT);
  delay(500);

  pinMode(RELE_1, OUTPUT);
  pinMode(RELE_2, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  delay(500);

  digitalWrite(RELE_1, OFF);
  digitalWrite(RELE_2, OFF);

  delay(500);

  digitalWrite(LED_BUILTIN, HIGH);

  Serial.begin(115200);


  Serial.println("\r\n");
  Serial.print("Chip ID: 0x");
  Serial.println(ESP.getChipId(), HEX);

  WiFi.mode(WIFI_STA);
  //WiFi.softAP("MOVISTAR_47E8","ndfBakCEvtHwj8jSSEMJ");
  // WiFi.softAP("Wireless-N","z123456z");
  //WiFi.softAP("Hello_IoT", "12345678");
  // WiFi.softAP("AI-THINKER_C0E300");
  WiFi.begin("Wireless-N", "z123456z");
  WiFi.config(IPAddress(192, 168, 1, 245), IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0));

  int timeout = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (++timeout > 100)
    {

      Serial.println("Sin Conexion WIFI");
      int a = 25;
      while (a--) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(100);
        digitalWrite(LED_BUILTIN, LOW);
        delay(100);
      }
      Serial.println("Reset SW");
      ESP.reset();

    }
  }

  
  server.begin();

  

}

void loop() {
  // put your main code here, to run repeatedly:

  WiFiClient client = server.available();

  if (!client) {
    return;
  }
  Serial.print("NUEVO CLIENTE\r\n");

  while (!client.connected()) {

    delay(1);
  }

  String req = client.readStringUntil('\r');
  Serial.print(req);
  Serial.print("\r\n");
  client.flush();



  if (req.indexOf("rele1/off") != -1)
  {
    rele1 = OFF;

    Serial.print("rele1 off\r\n");
  }
  else if (req.indexOf("rele1/on") != -1)
  {
    rele1 = ON;
    Serial.print("rele1 on\r\n");
  }
  else if (req.indexOf("rele2/off") != -1)
  {
    rele2 = OFF;

    Serial.print("rele2 off\r\n");

  }
  else if (req.indexOf("rele2/on") != -1) {

    rele2 = ON;

    Serial.print("rele2 on\r\n");
  }
  else
  {

    Serial.println("invalid request");
    client.stop();
    return;
  }

  digitalWrite(RELE_1, rele1);
  delay(500);
  digitalWrite(RELE_2, rele2);
  delay(500);
  client.flush();

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n";
  s += "Rele1 ";
  s += (rele1)  ? "off\n" : "on\n";
  s += "Rele2 ";
  s += (rele2) ? "off\n" : "on\n";
  s += "</html>\n";


  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");

}
