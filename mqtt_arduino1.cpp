
///////////////////////////  23.3.2019  KN /////////////////////////////



#include <cstdio>
#include <cstring>
#include <string>
#include <functional>     	// c++11

#include <iostream>        	// due to: cout

#include <fstream>
#include <jsoncpp/json/json.h>

#include <mutex>
#include <condition_variable>

using std::cout;
using std::endl;
 


#include "mqtt.h"

#include "mqtt_arduino1.h"
#include "SendTamk.h"
#include "SendWap.h"

using namespace std;

extern mutex xmut;
extern std::condition_variable cv;


//////////////////////////////////////////////////////////////////////////
//////////
//////////    MQTT
//////////
/////////////////////////////////////////////////////////////////////////

mqtt::mqtt(const char *id, const char *host, int port) : mosquittopp(id)
{
	int keepalive = 60;

	/* Connect immediately. This could also be done by calling
	 * mqtt_tempconv->connect(). */
	connect(host, port, (int) keepalive);
};


////////// ON CONNECT ///////////////////////////////////////////////////


void mqtt::on_connect(int rc)
{
	printf("****   MQTT Connected with code %d.  *****\n", rc);
	if(rc == 0){
		/* Only attempt to subscribe on a successful connect. */
		subscribe(NULL, in_topic);
	printf("****   MQTT subscription to topic = ");printf(in_topic);
	printf("    ****\n");

	}
}

//////////////////////////  ON SUBSCRIBE ///////////////////////////////////////

void mqtt::on_subscribe(int mid, int qos_count, const int *granted_qos)
{

	printf("****   MQTT Topic subscription succeeded.   ****\n");

}


//////////////////////////// Message received //////////////////////////////////

void mqtt::on_message(const struct mosquitto_message *message)
{
	
	printf("\n");
	//cout << "Msg received - " << endl;
        printf(" Msg received "); 

	if(!strcmp(message->topic, in_topic))
	{
		memset(buf, 0, 151*sizeof(char));
		/* Copy N-1 bytes to ensure always 0 terminated. */
		memcpy(buf, message->payload, 150*sizeof(char));

		/// ICT course
		if((buf[0]=='I') && (buf[1]=='O') && (buf[2]=='T') && (buf[3]=='J') && (buf[4]=='S'))  /// ICT course
          		{  IoTTi();  };

	}  // IoTTi topic
}

//////////////////////////////// IoTTi IOT course format //////////////////////

								// save data to global table 

void mqtt::IoTTi( void)
{

    Json::Reader reader;
    Json::Value obj;


	std::string js = " ";
	
        char st1[250];

	char sensor_name1[80];
	char sensor_path1[80];
	char sensor_unit1[10];


	//cout << " * " << endl ;
	//cout << "buf = " << buf << endl ;


	int i=0;
	while(buf[i]!=0)
	{
          st1[i]=buf[i+6];	// remove "IOTJS="  from beging of MQTT message
 	//  cout << st1[i] ; 
	  i++;
	}
	st1[i]='\0';


	//cout << "+ " << endl ;
	//cout << "ST1 = " << st1 << endl ;

	
	std::string hp;
	std::string hp2;

	hp=st1;
	
	cout << "hp = " << hp << endl ;

	sprintf(sensor_name1,"-");
	sprintf(sensor_path1,"dummy");
	float sensor_value1=-2000;
	sprintf(sensor_unit1,"-");
	sprintf(sensor_data,"?");


 	if(reader.parse(hp, obj)==true)
	{     // Reader can also read strings
  

		//   Check mqtt message content


		if(obj["S_name"].isString())
		{
			hp2=obj["S_name"].asString();
			strncpy(sensor_name1,hp2.c_str(),hp2.length());
			sensor_name1[hp2.length()]='\0';
 			cout << "  Sensor name: " << sensor_name1 ;
		};

		if(obj["S_value"].isDouble())
		{
			sensor_value1=obj["S_value"].asFloat();
			cout << "Sensor value Float: " << sensor_value1 ;
		};

		if(obj["S_value"].isInt())
		{
			sensor_value1=obj["S_value"].asInt();
			cout << " Sensor value Int: " << sensor_value1 ;
		};


		if(obj["S_path"].isString())
		{
			hp2=obj["S_path"].asString();
			strncpy(sensor_path1,hp2.c_str(),hp2.length());
			sensor_path1[hp2.length()]='\0';
 			cout << " Sensor path: " << sensor_path1 ;
		};

		if(obj["S_unit"].isString())
		{
			hp2=obj["S_unit"].asString();
			strncpy(sensor_unit1,hp2.c_str(),hp2.length());
			sensor_unit1[hp2.length()]='\0';
 			cout << " Sensor unit: " << sensor_unit1 ;
		};

		if(obj["S_data"].isString())
		{
			hp2=obj["S_data"].asString();
			strncpy(sensor_data,hp2.c_str(),hp2.length());
			sensor_data[hp2.length()]='\0';
 			cout << " Sensor data: " << sensor_data ;
		};

		cout << " done " << endl;

		///       send JSON to Wapice  

		
		send_to_Wap("NA","NA",sensor_name1,sensor_path1,sensor_value1,sensor_unit1);	

		;

	} else
	{
		cout << " --- Error in JSON parsing ---- " << endl;
	};

	;				

}







