#include <Arduino.h>
#include <wifi.h>

using namespace std;

#include "http.h"
#include "time.h"

#include "sensor_readings.h"
#include "wifi.h"
#include "config.h"
#include "calAndXmlUtils.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <queue>


/*
Initialize the relevant variables
datetimes,xmlstring,code_num,ids
*/
String datetime;
int begin = 115200;
int code_num = 0;
char xmlchar[2100];
String id_;
char datetime_[32] = {};
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
/*
Define arrays to store data whenever connection gone fail
*/
queue<String> ids;
queue<String> dates;
queue<int> codes;
queue<double> temps;
queue<double> h_m;
queue<double> pres;
queue<double> l_buffer;
queue<double> temp_sds;
queue<double> h_sds;
queue<double> p_sds;
queue<double> l_sds;
/*
Define variables to store readings
*/
double t_m = 0;
double h_m = 0;
double p_m = 0;
double li_m = 0;
double t_sd = 0;
double h_sd = 0;
double p_sd = 0;
double l_sd = 0;
const int daylightOffset_sec = 0;
const char *ntpserver = "pool.ntp.org";
const long gmtOffset_sec = 19800;
String datetimeformat = "%Y-%m-%dT%H:%M:%S%z";

/*
In the initial step we need to strat the all sensors and connect the node mcu to wifi.
So it will ever go beond while it connect to wifi.
then sensors got started.henneed to configure the time we 
*/
void setup()
{
  Serial.begin(begin);
  tryToCnnectWIFI()
  start();
  configTime(gmtOffset_sec, daylightOffset_sec, ntpserver);
}

void loop()
{

while (!codes.empty())
  {
    Serial.println("Ready to process element in buffers");
    if (WiFi.status() == WL_CONNECTED)
    {
      char xmlcap[2100];


      double t_m = temps.front();
      double h_m = h_m.front();
      double p_m = pres.front();
      double l_m = l_buffer.front();
      double t_sd = temp_sds.front();
      double h_sd = h_sds.front();
      double p_sd = p_sds.front();
      double l_sd = l_sds.front();

      String id_processing = (String)ids.front();
      Serial.println(id_processing);
      String date_processing = (String)dates.front();
      int code_processing = codes.front();
      /*
      Generate the cap xml string
      */
      capXML(xmlcap,t_m, h_m, p_m, l_m,t_sd, h_sd, p_sd, l_sd,id_processing, date_processing);
      /*
      Send data to server and error recognizing
      */
      bool sent = sendDataToServer(xmlcap, code_processing))
      if (sent)
      {
        pop();
        Serial.println("Successfully add");
      }
      else
      {
        Serial.printf("Fail sending due to network issue");
        break;
      }
    }
    else
    {
      Serial.println("Wifi Disconnected.!!!!");
      break;
    }
  }



  int iter = 0;
  
  
  int rounds_per_send = 900;
  int rounddelay = 1000;


  double t_iter_buffer[rounds_per_send];
  double h_iter_buffer[rounds_per_send];
  double p_iter_buffer[rounds_per_send];
  double l_iter_buffer[rounds_per_send];
  /*
  senso reading added to the buffer from a delay gap of 1 second
  */
  while (iter < rounds_per_send)
  {
    t_iter_buffer[iter] = round(readT() * 100) / 100.00;
    h_iter_buffer[iter] = round(readH() * 100) / 100.00;
    p_iter_buffer[iter] = round(readP() * 100) / 100.00;
    l_iter_buffer[iter] = round(readLI() * 100) / 100.00;
    delay(rounddelay);
    iter++;
  }

  /*
  Calculate the mean and the standard deviation
  */

  t_m = mean(t_iter_buffer, rounds_per_send);h_m = mean(h_iter_buffer, rounds_per_send);
  p_m = mean(p_iter_buffer, rounds_per_send);li_m = mean(l_iter_buffer, rounds_per_send);
  t_sd = sd(t_iter_buffer, rounds_per_send, t_m);h_sd = sd(h_iter_buffer, rounds_per_send, h_m);
  p_sd = sd(p_iter_buffer, rounds_per_send, p_m);l_sd = sd(l_iter_buffer, rounds_per_send, li_m);

  getTime(datetime_);
  id_ = String(code_num);
  datetime = String(datetime_);
  /*
  Generate the xml according to the cap protocol
  */
  capXML(xmlchar,t_m, h_m, p_m, li_m,t_sd, h_sd, p_sd, l_sd,id_, datetime);

/*
Send the post data and error handling.
*/
  if (WiFi.status() != WL_CONNECTED)
  {
    if (connect_to_wifi())
    {
      if (!sendDataToServer(xmlchar, code_num))
      {
        append();
        Serial.printf("%i added", code_num);
      }
    }
    else
    {
      append();
      Serial.printf("%i added", code_num);
    }
  }
  else
  {
    if (!sendDataToServer(xmlchar, code_num))
    {
      append();
      Serial.printf("%i added", code_num);
    }
    else
    {
      Serial.printf("Send post done")
    }
  }
  code_num++;
  
}


void getTime(char *timestamp)
{
  struct tm info;
  if (getLocalTime(&info))
  {
    char string[32];
    time_t t = time(NULL);
    strftime(string, sizeof(string), datetimeformat, localtime(&t));
    sprintf(timestamp, timeStr);
    
  }
  else{
    Serial.println("Error while getting the time");
  }
  
  
}
/*
Remove the last element in the buffer.
This happen after that we take the buffer values and read and it sent t the server.
*/
void pop()
{
  codes.pop();ids.pop();dates.pop();
  temps.pop();h_m.pop();pres.pop();l_buffer.pop();
  temp_sds.pop();h_sds.pop();p_sds.pop();l_sds.pop();
}
/*
This method is used put elements to buffer.
This happen when connections issues get occurred.
*/
void append()
{
  codes.push(code_num);ids.push(id_);dates.push(datetime);
  temps.push(t_m);h_m.push(h_m);pres.push(p_m);l_buffer.push(li_m);
  temp_sds.push(t_sd);h_sds.push(h_sd);p_sds.push(p_sd);l_sds.push(l_sd);
}
