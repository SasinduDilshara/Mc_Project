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
    return round((total / len_array) * 100) / 100.00;
}
/*

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
    return round(sqrt(variance) * 100) / 100.00;
}