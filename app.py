'''
Import relevant packages from flask. 

ElementTree => Parse the XML string and get values
sqlalchemy => Handle database related things

'''
from flask import Flask, request, render_template
from flask_restful import Api, Resource
from xml.etree import ElementTree
from flask_sqlalchemy import SQLAlchemy
from sqlalchemy import desc
from helper.caphelper import extract_capxml
from helper.dbhelper import initialize_insert_data
'''
Initialize the app vaiable
'''
app = Flask(__name__)

'''
Initialize the debug variable.
True when developping.
False when on the production"
'''
DEBUG = True
DEBUG_DATABASE = 'postgresql://postgres:postgres@localhost/mc_semeter_project'
'''
Hosted database URL
'''
HOSTED_DATABASE = ''

if(DEBUG):
    '''
    When developing
    '''
    app.debug = True
    app.config['SQLALCHEMY_DATABASE_URI'] = DEBUG_DATABASE

else:
    '''
    After hosting
    '''
    app.debug = False
    app.config['SQLALCHEMY_DATABASE_URI'] = HOSTED_DATABASE

sqlAlchemyconfigure = 'SQLALCHEMY_TRACK_MODIFICATIONS'

'''
set SQLAlchemy.SQLALCHEMY_TRACK_MODIFICATIONS to false
'''
app.config[sqlAlchemyconfigure] = False

'''
Initializing the database variale
'''
database = SQLAlchemy(app)


'''
Class to create and save object to database.
This class is inherit the inbuild 
'''
class Database(database.Model):
    '''
    Table name of the database
    '''
    __tablename__ = 'microcontroller_sensor_readings'
    '''
    primary key of the table
    '''
    id = database.Column(database.Integer, primary_key=True)
    '''
    Mean value of temparature , h_mean , p_mean , l_mean
    '''
    t_mean = database.Column(database.Float)
    h_mean = database.Column(database.Float)
    p_mean = database.Column(database.Float)
    l_mean = database.Column(database.Float)
    '''
    Standard deviation value of temparature , h_mean , p_mean , l_mean
    '''
    t_sd = database.Column(database.Float)
    h_sd = database.Column(database.Float)
    p_sd = database.Column(database.Float)
    l_sd = database.Column(database.Float)
    '''
    Timestamp when data read by the sensor
    '''
    timestamp = database.Column(database.String(128))
    '''
    unique identifier for debug purpose only
    '''
    code = database.Column(database.String(128))


    def __init__(self, timestamp, t_mean, h_mean,
                 p_mean, l_mean, t_sd, h_sd, p_sd, l_sd,code):

        '''
        Constructor will get the values and create a new row of the table by inserting values
        '''
        '''
        set mean values to table row
        '''
        self.t_mean = t_mean
        self.h_mean = h_mean
        self.p_mean = p_mean
        self.l_mean = l_mean
        '''
        set standard deviation values on table row
        '''
        self.t_sd = t_sd
        self.h_sd = h_sd
        self.p_sd = p_sd
        self.l_sd = l_sd

        '''
        set code      => debug purposes
        set timestamp => timestamp when the sensor reading the data
        '''
        self.code = code
        self.timestamp = timestamp


def data_insert(data):
    '''
    This method will use to insert data into postgresql database table
    create a Database class object and then insert the data 
    '''

    (code, timestamp, t_mean, h_mean, p_mean, l_mean,t_sd, h_sd, p_sd, l_sd) = initialize_insert_data(data)
    try:
        '''
        create and commit data
        '''
        obj = Database(timestamp, t_mean, h_mean, p_mean, l_mean,t_sd, h_sd, p_sd, l_sd,code)
        db.session.add(obj)
        db.session.commit()
        return True
    except:
        return False

@app.route('/')
def index():
    '''
    This will deliver the data to html page and it will show the last ive information updates.
    '''
    html_display = Database.query.order_by(desc(Database.timestamp)).limit(5).all()
    return render_template("dashboard.html", table=html_display)

@app.route('/insert', methods=['POST'])
def post():
    request_body = request.data
    extracted_data = extract_capxml(request_body)
    if DEBUG:
        print("cap extracted_data data : ", extracted_data)
    result = data_insert(extracted_data)
    if result:
        return "DATABASE updated succefully"
    else:
        return "DATABASE update failed!!!"



if __name__ == '__main__':
    app.run()

