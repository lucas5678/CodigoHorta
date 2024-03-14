#ifndef REQUEST_H
#define REQUEST_H

#include <Ethernet.h>
#include <ArduinoJson.h>

class Request {
  private:
    EthernetClient client;
    EthernetServer server;
    DynamicJsonDocument doc;

  public:
    Request() : server(80), doc(1024) {}

    void send(int analogValue) {
      // Adiciona o valor ao documento JSON
      doc["analogValue"] = analogValue;

      // Conecta ao servidor
      if (client.connect("testedeapi", 8080)) {
        client.println("POST /valoranalogico HTTP/1.1");
        client.println("Host: testedeapi");
        client.println("Content-Type: application/json");
        client.println("Connection: close");
        client.print("Content-Length: ");
        client.println(measureJson(doc));
        client.println();
        serializeJson(doc, client);
      }
    }

    String receive() {
      String message = "";

      // Aguarda a resposta
      while (client.connected()) {
        if (client.available()) {
          String line = client.readStringUntil('\r');

          // Analisa a resposta
          DeserializationError error = deserializeJson(doc, line);

          // Verifica se houve algum erro na análise
          if (error) {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
            return "";
          }

          // Extrai o valor da chave "message"
          message = doc["message"];
        }
      }

      // Fecha a conexão
      client.stop();

      return message;
    }

    String listen() {
      String action = "";

      // Escuta as conexões de entrada
      client = server.available();

      if (client) {
        while (client.connected()) {
          if (client.available()) {
            String line = client.readStringUntil('\r');

            // Analisa a requisição
            DeserializationError error = deserializeJson(doc, line);

            // Verifica se houve algum erro na análise
            if (error) {
              Serial.print(F("deserializeJson() failed: "));
              Serial.println(error.f_str());
              return "";
            }

            // Extrai o valor da chave "acao"
            action = doc["acao"];
          }
        }
      }

      return action;
    }
};

#endif
