#include <Arduino.h>
#include "DHT.h"
#include <Wire.h>
#include <Adafruit_BMP085.h>

#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

Adafruit_BMP085 bmpVariable;

/*
Start the sensors.
Handle err if sensor start not happen
*/
void start()
{
    dht.begin();
    if (!bmpVariable.begin())
    {
        Serial.println("Could not find sensors");
        while (1){}        
    }
}

/*
Declare variable to use when measuring the light intensiveness
*/
const int ldrPin = A0;

/*
Read the light intensity sensor data and
handle errors
*/
float readLI()
{
    float ldrVal = analogRead(ldrPin) * 100 / 3000;
    if (ldrVal > 100)
    {
        return 100.00;
    }
    return ldrVal;
}

/*
Read the heat sensor data and
handle errors
*/
float readH()
{
    float h = dht.readHumidity();
    if (isnan(h))
    {
        Serial.println("Wrong H value");
        return 0;
    }
    return h;
}

/*
Read the temparature sensor data and
handle errors
*/
float readT()
{
    float t = dht.readTemperature();
    if (isnan(t))
    {
        Serial.println("Wrong temp value"));
        return 0;
    }
    return t;
}

/*
Read the presure sensor data and
handle errors
*/
float readP()
{
    float p = bmpVariable.readPressure();
    if (isnan(p))
    {
        Serial.println("Wrong presure values");
        return 0;
    }
    return p;
}

