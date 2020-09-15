/*
    * General config
*/
const int secondsToUpdate = 1000 * 60 * 5;

const char* authToken = "***";
const char* accountSID = "***";
const char* phoneNumber = "**";

const char* textMessage = "[Gate]: Warning the gate has been opened for more than five minutes!";

const char* phoneNumbers[] = {
    "+***",
    "+**",
    "+**" 
};

const int phoneNumberCount = sizeof(phoneNumbers)  / sizeof(int);

/*
    * Wifi config
*/
const int socketPort = 1337;
const char wifiSSID[] = "****";
const char wifiPasswsord[] = "****";


const char apSSID[] = "Gate Sesnsor";
const char apPassword[] = "supersecretpassword";

/**
 * @brief The pin for the reed switch
 * 
 */
const int reedSwitchPin = 5;

/*
    * Common annode config
*/
const int redPin = 21;
const int greenPin = 22;
const int bluePin = 23;