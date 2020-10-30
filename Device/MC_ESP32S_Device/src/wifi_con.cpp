#include <Arduino.h>
#include <WiFi.h>
#include "config.h"

/*
When wifi got disconnected in the middle, This method will use to reconnect wifi
*/
bool setWIFI()
{
    WiFi.begin(SSID, PASSWORD);
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.print("WIFI GOT CONNECT");
        return true;
    }

    Serial.println("WIFI GOT FAIL");
    return false;
}
/*
In the initial step,
This method need to execute to connect wifi
*/
void tryToConnectWIFI()
{
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print("wifi trying to connect");
    }

    Serial.print("WIFI GOT CONNECT");
}

