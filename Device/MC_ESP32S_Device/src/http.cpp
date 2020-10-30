#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "config.h"
/*
This method is used to send post requests from nodemcu 32s board to flask server
It send a xml in body according to cap protocol
*/

bool sendDataToServer(char *capxmlstring)
{
    /*
    Initialize a http client
    */
    HTTPClient http;
    /*
    Return false whe wifi is not connected
    */
    if (WiFi.status() != WL_CONNECTED)
    {
        return false;
    }
    /*
    Initialize the variable that include the server request link
    */
    String to_link = "/insert"
    String url_to_send = (String)server + to_link;
    http.begin((String)url_to_send);

    /*
    Send the data as a post request with cap xml data in the body
    */

    int responseCode = http.POST(capxmlstring);
   /*
   If error happen throgh the network.
   return false
   */
    if (responseCode != HTTP_CODE_OK)
    {        
        http.errorToString(responseCode).c_str();
        http.end();
        return false;
    }
    http.end();
    /*
    return true if no error
    */
    return true;
}