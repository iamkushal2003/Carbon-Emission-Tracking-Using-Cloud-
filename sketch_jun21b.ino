#include <ESP8266WiFi.h>

const char *ssid = "Mamameya"; // Enter your WiFi Name
const char *pass = "244466666"; // Enter your WiFi Password

WiFiServer server(80);

double air_quality;

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(550);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  delay(500);
  air_quality = ((analogRead(A0) / 1024.0) * 100.0);
  Serial.println(air_quality);

  WiFiClient client = server.available();
  if (client) { // Check if a client has connected
    if (client.connected()) { // Loop while the client's connected
      // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
      client.println("HTTP/1.1 200 OK");
      client.println("Content-type:text/html");
      client.println("Connection: close");
      client.println("Refresh: 3"); // Update the page after 3 seconds
      client.println();

      // Display the HTML web page
      client.println("<!DOCTYPE html><html>");
      client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
      client.println("<link rel=\"icon\" href=\"data:,\">");
      client.println("</head>");

      // Web Page Heading
      client.println("<body><h1 style=\"color:orange;\"> Air Quality Measurement </h1>");

      // Display pollution content
      client.println("<p style=\"color:blue;\"> CO Level (in percentage) = " + String(air_quality) + " %" + " </p>");
      if (air_quality <= 20.0) {
        client.println("<p style=\"color:green;\"> Normal </p>");
      } else if (air_quality > 20.0 && air_quality < 70.0) {
        client.println("<p style=\"color:purple;\"> Medium </p>");
      } else {
        client.println("<p style=\"font-size:200%; color:red\"> Danger!!! </p>");
      }

      client.println("</body></html>");
      client.stop();
    }
  }
}