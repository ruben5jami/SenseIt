#include <Wifi.h>

void Wifi::initCommunication(const char* ssid, const char* password) {
    Serial.println();
    Serial.print("connecting to ");
    Serial.println(ssid);
    WiFi.disconnect();
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

String Wifi::sendMessage(JsonObject& json, String url) {

  String line = "";

  Serial.print("Trying to Establish connection with  ");
  Serial.print(CommonValues::host);
  Serial.print(url);
  Serial.print('\n');
  const int httpPort = 80;

  if (!client.connect(CommonValues::host, httpPort)) {
    Serial.println("connection failed");
    return "";
  }

  int length = json.measureLength();

  Serial.println("connection successs");

  // POST URI
  client.print(String("POST ") + url + " HTTP/1.1\r\n" +
               "Host: " + CommonValues::host + "\r\n" +
               "Connection: close\r\n" +
               "Content-Type: application/json\r\n" +
               "Content-Length:" + String(length) + "\r\n\r\n");

  //POST message body
  String out;
  json.printTo(out);
  Serial.println("sending json: ");
  client.println(out);

  //print respone
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return "";
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    line = client.readStringUntil('\r');
  }

  Serial.println();
  Serial.println("closing connection");

  return line;
}

String Wifi::receiveMessage(String url) {
  Serial.print("Trying to Establish connection with  ");
  Serial.print(CommonValues::host);
  Serial.print(url);
  Serial.print('\n');

  const int httpPort = 80;

  if (!client.connect(CommonValues::host, httpPort)) {
    Serial.println("connection failed");
    return "";
  }

  Serial.println("connection successs");

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                "Host: " + CommonValues::host + "\r\n" +
                "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return "";
    }
  }

  String response = "";

  // Read all the lines of the reply from server and print them to Serial
  int isBody = 0;

  while(client.available()){
     String line = client.readStringUntil('\r');
     if (isBody == 0) {
       if (line == "\n") {
         isBody = 1;
       }
     }
     else {
       response += line;
     }

  }

  return response;

}
