#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "secret.h"

const char* ssid     = STASSID;
const char* password = STAPSK;

const char* host = "api.github.com";
const uint16_t port = 443;


const int buttonPin = D2;
const int ledPin =  LED_BUILTIN;
int buttonState = 0; 


// SHA1 fingerprint of the certificate
const char fingerprint[] PROGMEM = "29:70:30:74:CA:3C:48:F5:4A:79:C6:2D:11:57:A2:41:2A:2D:7D:5C";

void setup() {
  Serial.begin(9600);

  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  delay(500);
}

void loop() {
  buttonState = digitalRead(buttonPin);

  if (buttonState == LOW) {
    digitalWrite(ledPin, HIGH);
    delay(250);
    return;
  }
  digitalWrite(ledPin, LOW);

  Serial.print("connecting to ");
  Serial.print(host);
  Serial.print(':');
  Serial.println(port);

  // Use WiFiClient class to create TCP connections
  WiFiClientSecure client;

  Serial.printf("[=] Using fingerprint '%s'\n", fingerprint);
  client.setFingerprint(fingerprint);

  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    Serial.println("wait 5 sec...");
    delay(5000);
    return;
  }

  // This will send the request to the server
  String url = "/repos/lostsh/connectedMailbox/dispatches";
  Serial.print("[*] requesting URL: ");
  Serial.println(url);
  String dataPost = "{\"event_type\":\"do-notify\", \"client_payload\":{\"content\":\"" + String("New letter inbox (BYlostsh)") +"\"}}";
  Serial.println(dataPost);
  client.print(String("POST ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: Node-mailboxBYlostsh\r\n" +
               "Authorization: token " + GH_TOKEN + "\r\n" +
               "Content-Length: " + dataPost.length() + "\r\n" +
               "Content-Type: application/json\r\n" +
               "\r\n" +
               dataPost + "\r\n");

  //read back one line from server
  Serial.println("receiving from remote server");
  String line = client.readStringUntil('\r');
  Serial.println(line);

  Serial.println("closing connection");
  client.stop();

  Serial.println("Pause");
  digitalWrite(ledPin, HIGH);
  delay(3600000);
}
