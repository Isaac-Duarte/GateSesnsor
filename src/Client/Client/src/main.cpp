#include <Arduino.h>
#include <AnalogWrite.h>

#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>

#include <WebSocketsClient.h>

#include <AnalogWrite.h>
#include <Config.h>

WiFiMulti WiFiMulti;
WebSocketsClient webSocket;

/**
 * @brief Sets color of the common anode
 * 
 * @param red Red value
 * @param green Green value
 * @param blue Blue value
 */
void setColor(int red, int green, int blue) {
  analogWrite(redPin, 255 - red);
  analogWrite(greenPin, 255 - green);
  analogWrite(bluePin, 255 - blue);
}

/**
 * @brief Handles websocket events from client
 * 
 * @param type The event type
 * @param payload The data sent from the client
 * @param length Length of the payload
 */
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.println("[WSc] Disconnected!");
      break;
    case WStype_CONNECTED:
    {
      Serial.print("[WSc] Connected to url: ");
			Serial.println((char*)payload);
    }
    case WStype_TEXT: {
      int number = atoi((char*)payload);

      if (number == 1) {
        setColor(255, 0, 0);
      }
      else {
        setColor(0, 255, 0);
      }
    }
    default:
      break;
  }
}

/**
 * @brief Connects to WiFi
 * Configure in Config.h
 * 
 */
void connectToWifi() {
  Serial.print("Conntecting to ");
  Serial.print(wifiSSID);

  WiFi.begin(wifiSSID, wifiPasswsord);

  bool color = false;

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

    if (color == false) {
      color = true;
      setColor(1, 41, 64);
    }
    else {
      color = false;
      setColor(255, 255, 255);
    }
  }

  Serial.println("\nConnected!");
}

void setup() {
  // Open up serial for debugging
  Serial.begin(115200);

  connectToWifi();
  
	webSocket.begin(socketIp, socketPort, "/");
	webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(5000);
} 

void loop() {
  webSocket.loop();
}