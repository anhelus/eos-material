/**
  Stazione meteo

  Legge i dati di temperatura ed umidità provenienti da un sensore DHT11,
  pubblicandoli presso un broker MQTT.

  creato il 26 giugno 2020
  da Angelo Cardellicchio

  Questo codice è rilasciato sotto licenza MIT.
*/
#include "WiFi.h"
#include "PubSubClient.h"
#include "DHT.h"
#include "Ethernet.h"
#include "PubSubClient.h"

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

IPAddress server(192,168,1,245);
DHT dht(4, DHT11);
float temperature = 0;
float humidity = 0;

// Funzione responsabile del setup iniziale della connessione wireless.
void setup_wifi(char* ssid, char* password) {
    delay(100);
    Serial.print("Connessione alla rete wireless in corso...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print('.');
    }
    Serial.println("Connessione avvenuta con successo.");
    Serial.print("Indirizzo IP: ");
    Serial.println(WiFi.localIP());
}

// Funzione per la gestione della riconnessione al broker MQTT.
void reconnect() {
  while (!client.connected()) {
    Serial.print("Connessione al broker MQTT in corso...");
    if (client.connect("espClient")) {
      Serial.println("Connesso!");
    } else {
      Serial.print("Connessione fallita con rc=");
      Serial.print(client.state());
      Serial.println(". Riprovo in 5 secondi...");
      delay(5000);
    }
  }
}

// Funzione per la conversione del valore letto in stringa. 
char* convertToStringVal(float val)
{
    char valString[8];
    return dtostrf(val, 1, 2, valString);
}

void setup() {
    Serial.begin(115200);
    setup_wifi("Linkem2.4GHz_7710F6", "jus123kio987d");
    // setup_mqtt(client, "broker.mqtt-dashboard.com", 1883);
    client.setServer(server, 1883);
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
    long now = millis();
    if (now - lastMsg > 5000) {
        temperature = dht.readTemperature();
        humidity = dht.readHumidity();
        char tempString[8];
        char humString[8];
        dtostrf(temperature, 1, 2, tempString);
        dtostrf(humidity, 1, 2, humString);

        client.publish("esp32/temperature", tempString);
        Serial.println(temperature);
        client.publish("esp32/humidity", humString);        
        Serial.println(humidity);
    }
    delay(1000);
}
