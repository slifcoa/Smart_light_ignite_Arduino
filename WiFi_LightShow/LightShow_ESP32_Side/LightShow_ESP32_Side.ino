//Define GPIO pins used for Serial Communication
#define RXD2 16
#define TXD2 17

#include <WiFi.h>

const char* ssid = "Trump2020";
const char* password = "Danny@1961";

bool ledOn = false;

const int baudRate = 115200;

WiFiServer server(80);

void setup() {
  
  // Initialize the Serial Ports
  Serial.begin(baudRate);
  Serial.println("Initializing Esp32");
  Serial1.begin(baudRate, SERIAL_8N1, RXD2, TXD2);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");   
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");   
  Serial.println(WiFi.localIP());

  server.begin();  
}

void loop() {
  // put your main code here, to run repeatedly:

   WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("LIGHT=ON")) {
          Serial1.write("ON");          
        }
        if (currentLine.endsWith("LIGHT=OFF")) {
          Serial1.write("OFF");                // GET /L turns the LED off
        }
        if (currentLine.endsWith("BOX=OPEN")) {
          Serial1.write("OPEN");          
        }
        if (currentLine.endsWith("BOX=CLOSE")) {
          Serial1.write("CLOSE");                // GET /L turns the LED off
        }
        
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
