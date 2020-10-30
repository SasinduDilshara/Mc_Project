from flask import Flask, request, render_template
from flask_restful import Api, Resource
from xml.etree import ElementTree


def extract_capxml(capxml):
    '''
This function is used to extract xml Value which will be recieved by http request body.
Following is the structure of CAP protocol XML value

        <?xml version = \"1.0\" encoding = \"utf8\"?>
        <alert>
        <identifier>universal identifier value</identifier>
        <sender> client</sender>
        <sent> timestamp</sent>
        <status> Actual</status>
        <msgType> Alert</msgType>
        <scope> Public</scope>
        <info>
        <category> Environment Reading</category>
        <event> Sensor Readings</event>
        <urgency> Past</urgency>
        <severity> Minor</severity>
        <certainty> Observed</certainty>
        <senderName> ESP 32s</senderName>
        <parameter><valueName>t_mean</valueName><value>%s</value></parameter>
        <parameter><valueName>h_mean</valueName><value>%s</value></parameter>
        <parameter><valueName>p_mean</valueName><value>%s</value></parameter>
        <parameter><valueName>l_mean</valueName><value>%s</value></parameter>
        <parameter><valueName>t_sd</valueName><value>%s</value></parameter>
        <parameter><valueName>h_sd</valueName><value>%s</value></parameter>
        <parameter><valueName>p_sd</valueName><value>%s</value></parameter>
        <parameter><valueName>l_sd</valueName><value>%s</value></parameter>
        </info>"
        </alert>"

'''
    xml = ElementTree.fromstring(xml_str)
    parameters = root.findall('info/parameter')
    datetime = root.find('sent').text.strip()
    '''
    idetifier > 
    This should be unique for each message in the server
    ex:- UUID
    '''
    universal_identifier = root.find('identifier').text.strip()
    '''
    Dictionary to store readings
    '''
    values = dict()
    values['datetime'] = datetime
    values['identifier'] = universal_identifier
    reading_data = {}
    for p in parameters:
        '''
        Take and save mean value and standard deviation
        '''
        name = p.find('valueName').text.strip()
        value = p.find('value').text.strip()
        data[name] = float(value)
    values["reading_data"] = reading_data
    return values