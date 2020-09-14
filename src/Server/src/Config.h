/*
    * General config
*/
const int secondsToUpdate = 1000 * 60 * 5;

const char* authToken = "5c9b4968f08efea5ae16dd050ceea9f1";
const char* accountSID = "AC32642b2a4b16e9355e0a3371d5314d4a";
const char* phoneNumber = "+14352443156";

const char* textMessage = "[Gate]: Warning the gate has been opened for more than five minutes!";

const char* phoneNumbers[] = {
    "+18015569811", // Isaac
    "+18019462639", // Nary
    "+18019039986" // Sam
};

const int phoneNumberCount = sizeof(phoneNumbers)  / sizeof(int);

/*
    * Wifi config
*/
const int socketPort = 1337;
const char wifiSSID[] = "Apple Wi-Fi Network";
const char wifiPasswsord[] = "3fdd8he48a48v7";

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