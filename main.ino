#include <SPI.h>
#include "request.h"

// MAC address for your Ethernet Shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// IP address for your Ethernet Shield
IPAddress ip(192, 168, 1, 177);

Request request;

void setup() {
  Serial.begin(9600);

  // Inicializa o Ethernet Shield com o IP definido
  Ethernet.begin(mac, ip);

  delay(1000);
}

void loop() {
  int analogValue = analogRead(A0); // Lê o valor da entrada analógica A0

  request.send(analogValue);

  String message = request.receive();

  Serial.println(message);

  delay(1000); // Aguarda um segundo antes de ler novamente
}
