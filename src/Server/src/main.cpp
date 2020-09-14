#include <Arduino.h>
#include <Config.h>

#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>

#include <WebSocketsServer.h>

#include <twilio.hpp>
#include <AnalogWrite.h>


WiFiMulti WiFiMulti;
WebSocketsServer webSocket = WebSocketsServer(socketPort);
Twilio *twilio;

// Variable to track the sensor state 
int reedState = 0;

// For sending data to client
char buf[16];

// These are to handle the timer
int interval;
bool startTimer;
long lastTrigger;
long now;

/**
 * @brief Handles websocket events from client
 * 
 * @param num Client Id
 * @param type The event type
 * @param payload The data sent from the client
 * @param length Length of the payload
 */
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED:
    {
      IPAddress ip = webSocket.remoteIP(num);
      Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
    }
    default:
      break;
  }
}

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

/**
 * @brief Sends a text message using Twilio
 * 
 * @param wantedPhoneNumber The number to send the text mesage to
 * @param message The message for the text 
 */
void sendTextMessage(const char* wantedPhoneNumber, const char* message) {
  Serial.print("Sending message to ");
  Serial.println(wantedPhoneNumber);

  String response;
  bool success = twilio->send_message(wantedPhoneNumber, phoneNumber, message, response);

  if (success) {
    Serial.print("Successfully sent the message to ");
    Serial.println(wantedPhoneNumber);
  }
  else {
    Serial.print("Couldn't send the message to ");
    Serial.println(wantedPhoneNumber);
    Serial.println(response);
  }
    
}

/**
 * @brief Updates the timer and common anode
 * 
 */
void updateTimer() {
  Serial.println(reedState);
  switch (reedState) {
    case 0:
      startTimer = false;
      setColor(0, 255, 0);
      break;
    case 1:
      lastTrigger = millis();
      startTimer = true;
      setColor(255, 0, 0);
      break;
  }

  if (startTimer && (millis() - lastTrigger) > (secondsToUpdate)) {
    for (int i = 0; i < phoneNumberCount; i = i + 1) {
        sendTextMessage(phoneNumbers[i], textMessage);
    }

    startTimer = false;
  }
}

void setup() {
  // Open up serial for debugging
  Serial.begin(115200);

  // Setup reed switch
  pinMode(reedSwitchPin, INPUT_PULLUP);
  
  connectToWifi();
  Serial.println(WiFi.localIP());
  // Start websocket
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  // Create Tilio service 
  twilio = new Twilio(accountSID, authToken);

  interval = 0;
  updateTimer();
}

void loop() {
  if (reedState != digitalRead(reedSwitchPin))
  {
    reedState = digitalRead(reedSwitchPin); 
    updateTimer();

    ltoa(reedState, buf, 10);
	  webSocket.broadcastTXT(buf);
  }

  webSocket.loop();
}