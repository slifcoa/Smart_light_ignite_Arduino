#define RXD0 3
#define TXD0 1

const int baudRate = 115200;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(baudRate);
  Serial1.begin(baudRate, SERIAL_8N1, RXD0, TXD0);
  Serial1.write("hey");
}

void loop() {
  // put your main code here, to run repeatedly:

}
