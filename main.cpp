#include "mqtt_arduino1.h"

//////////////////////////////////////  23.3.2019 //////////////////////////////
///
///	g++ main.cpp SendTamk.cpp  mqtt_arduino1.cpp -std=c++0x -pthread -lmosquitto -lmosquittopp -lcurl -ljsoncpp
///
///////////////////////////////////////////////////////////////////////////////

#include <signal.h>
#include <sys/time.h>

#include <stdio.h>
#include <string.h>
#include <atomic>
#include <iostream>

#include <fstream>
#include <jsoncpp/json/json.h>

#include <mutex>
#include <condition_variable>

#include <thread>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>

#include "mqtt_main.h"

long  count =0;

std::atomic < long>  time_i;

std::atomic <long> time_b;
std::atomic <unsigned char> tmr_flag;


using namespace std;

mutex xmut,mut2;
std::condition_variable cv;

///////////////////////////////////////////////////////////////
/////
/////		MQTT thread 
/////
///////////////////////////////////////////////////////////////

void mqtt_loop()
{
        int rc;

	mqtt *mqttHdl_2 = new mqtt(in_topic, host_addr, port_number);
	//mqtt *mqttHdl_2 = new mqtt("IoTTi_out_kuo1", "193.167.167.59", 1883);

	mosqpp::lib_init();

	while(1)
	{

		rc = mqttHdl_2->loop();
		if(rc)	{
				mqttHdl_2->reconnect();
				usleep(1e5);
			}

	}

	mosqpp::lib_cleanup();
}

//////////////////////////////////// mqtt thread end /////////////




//////////////////////////////////////////////////////////////////
/////
/////		Timer thread
/////
/////////////////////////////////////////////////////////////////

/////////////////////////////////// timer routine ///////////////


void timer_routine (int signum)
{
	long a;
    	time_b++;


    if (time_b>1000){ time_b=0; tmr_flag=1;} ;   // DB time sending rate  = 10 sec 

	a=time_b;
	time_i=a;	


}


void timer_start()
{
	struct sigaction sa;
 	struct itimerval timer;

 	/* Install timer_handler as the signal handler for SIGVTALRM. */
 	memset (&sa, 0, sizeof (sa));
 	sa.sa_handler = &timer_routine;
 	sigaction (SIGALRM, &sa, NULL);

 	/* Configure the timer to expire after 250 msec... */
 	timer.it_value.tv_sec = 0;
 	timer.it_value.tv_usec = 100000;

 	/* ... and every 3 000 msec after that. */
 	timer.it_interval.tv_sec = 0;	    // Timer time set sec	
 	timer.it_interval.tv_usec = 10000;  // Timer time set usec

 	/* Start a virtual timer. It counts down whenever this process is executing. */
	setitimer (ITIMER_REAL, &timer, NULL);

	while(1);
}

//////////////////////////////////// timer thread end /////////////////


///////////////////////////////////////////////////////////////////////////
////////
///////			Send URL
///////
//////////////////////////////////////////////////////////////////////////


void Send_url(void)

{
  while(1)
	{ 
	
	if(tmr_flag==1){ 
		 
		 printf ("Sending %ld count\n",count);
 		 		
		 count++; 
			if (count>6){ count=0; };

		 tmr_flag=0;
			}	 	
		
	}

}


//////////////////////////////////// send URL thread end /////////////////


int main(int argc, char *argv[])
{
	int i;
	tmr_flag=0;


/////////////////////////////////// read settings ////////////////
    ifstream ifs("iot_ticket_user.json");
    Json::Reader reader;
    Json::Value obj;

	bool parsingSuccessful = reader.parse( ifs, obj );

	if ( !parsingSuccessful )
	{
    		// report to the user the failure and their locations in the document.
    		std::cout  << "\n\n Failed to parse settings\n"
               << reader.getFormattedErrorMessages();
    
	}

	else

	{
    		std::cout  << "\n\n Settings reading OK \n";

    		reader.parse(ifs, obj);     // Reader can also read strings
    		//cout << "    username: " << obj["username"].asString() << endl;
    		//cout << "    psw: " << obj["passwd"].asString() << endl;
    		//cout << "    devID: " << obj["devID"].asString() << endl;
    		//cout << "\n\n" << endl;   
	};

	std::string hp2;

	hp2=obj["username"].asString();
	strncpy(usern1,hp2.c_str(),hp2.length());
			usern1[hp2.length()]='\0';

	cout << "    Username:  " << usern1 << endl; 

	hp2=obj["passwd"].asString();
	strncpy(psw1,hp2.c_str(),hp2.length());
			psw1[hp2.length()]='\0';

	cout << "    Password:  " << psw1 << endl; 

	hp2=obj["devID"].asString();
	strncpy(devid1,hp2.c_str(),hp2.length());
			devid1[hp2.length()]='\0';

	cout << "    device ID: " << devid1 << endl; 	
	cout << "\n\n" << endl;

	thread mqtt_th (mqtt_loop);
        thread send_th(Send_url);
       	thread tmr_th(timer_start);


	printf ("\n   Subscribed to listen MQTT messages \n");
	printf   ("   Sending to IoT ticket when message arrived \n\n");


	mqtt_th.join();
        send_th.join();
	tmr_th.join();	

	return 0;
}