#include <Arduino.h>

/*
This function is use to read the mean values of sensor readings
sensor reading stored buffer and it len_arraygth is the parameters
*/
double mean(double array[], int len_array)
{
    double total = 0;
    for (int i = 0; i < len_array; i++)
    {
        total += array[i];
    }
    return round((total / len_array));
}

/*
This function is use to read the mean values of sensor readings.
First it calculate the mean and used it
sensor reading stored buffer and it len_arraygth is the parameters
*/

double sd(double array[], int len_array)
{
    double variance = 0;
    double mean = mean(*array,*len_array)
    for (int i = 0; i < len_array; i++)
    {
        variance += pow((array[i] - mean), 2);
    }
    return round(sqrt(variance)) / len_array;
}

/*
This method helps to build the cap protocl based xml string which contain the parameters and parameter values.
*/

void capXML(char *capxml,double t_mean, double h_mean, double p_mean, double l_mean,double t_sd, double h_sd, double p_sd, double l_sd,String identifier, String datetime)
{
    String t_mean = String(t_mean);String h_mean = String(h_mean);String t_sd = String(t_sd);String h_sd = String(h_sd);String p_mean = String(p_mean);String p_sd = String(p_sd);String l_mean = String(l_mean);String l_sd = String(l_sd);

    String capProtocol = "<?xml version = \"1.0\" encoding = \"utf8\"?>"+"<alert>"+"<identifier>%s</identifier>"+"<sender> NODE_MCU</sender>"+"<sent> %s</sent>"+"<status> Actual</status>"+"<msgType> Alert</msgType>"+"<scope> Public</scope>"
+"<info>"+"<category> Env</category>"+"<event> Readings</event>"+"<urgency> Past</urgency>"+"<severity> Minor</severity>"+"<certainty> Observed</certainty>"+"<senderName> NODE_MCU Device</senderName>"
+"<parameter><valueName>%s</valueName><value>%s</value></parameter>"+"<parameter><valueName>%s</valueName><value>%s</value></parameter>"+"<parameter><valueName>%s</valueName><value>%s</value></parameter>"+"<parameter><valueName>%s</valueName><value>%s</value></parameter>"
+"<parameter><valueName>%s</valueName><value>%s</value></parameter>"+"<parameter><valueName>%s</valueName><value>%s</value></parameter>"+"<parameter><valueName>%s</valueName><value>%s</value></parameter>"+"<parameter><valueName>%s</valueName><value>%s</value></parameter>"
+"</info>"+"</alert>";

    sprintf(
        capxml,capProtocol,identifier.c_str(), datetime.c_str(),
        "t_mean", t_mean.c_str(),"h_mean", h_mean.c_str(),"p_mean", p_mean.c_str(),"l_mean", l_mean.c_str(),
"t_sd", t_sd.c_str(),"h_sd", h_sd.c_str(),"p_sd", p_sd.c_str(),"l_sd", l_sd.c_str()

    );
}