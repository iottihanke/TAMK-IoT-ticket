//  KN 23.3.2019


//
//   MQTT client setup
//
#define in_topic 	"IoTTi_out_kuo1"
//#define host_addr 	"193.167.167.59"
#define host_addr 	"10.10.206.200"
#define port_number 	1883


#include <mosquittopp.h>
#include <math.h>

        extern long count1;

class mqtt : public mosqpp::mosquittopp
{

	char buf[251]; 

	public:
		mqtt(const char *id, const char *host, int port);
		~mqtt(){};

		void on_connect(int rc);
		void on_message(const struct mosquitto_message *message);
		void on_subscribe(int mid, int qos_count, const int *granted_qos);
		void IoTTi(void);


};

