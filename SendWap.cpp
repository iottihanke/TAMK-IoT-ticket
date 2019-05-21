//
// Wapice rest api demo 23.3.2019 KN
//   
//        read username, password, ID from file: settings.json     




#include "SendWap.h"
#include "mqtt.h"

using std::cout;
using std::endl;

///////////////////// CURL ////////////////////////
void send_to_Wap(const char user[],const char psw[],const char sensor1[], const char path1[],float num1,const char quant1[])

{
  CURLcode ret;
  CURL *hnd;
  struct curl_slist *slist1;

  std::string wap_url = "https://wapice.iotlab.tamk.fi/api/v1/process/write/";

//  std::string url_id = wap_url + devID;
  
  std::string url_id = wap_url + devid1;

  slist1 = NULL;
  slist1 = curl_slist_append(slist1, "Content-Type: application/json");

  char sensor[80]=" ";
	strcpy(sensor,sensor1);

  char path[80]="Demo1/";
	strcat(path,path1);


  char quant [10]=" ";
	strcpy(quant,quant1);


  char pdata[250] ;

//cout << " \n +++++++++ "<< num1 << "\n"<< endl;

sprintf(pdata,"[{\"name\":\"%s\",\"path\":\"%s\",\"v\":%f,\"unit\":\"%s\"}]",sensor,path,num1,quant);


  std::string jsonstr = "{ }";


jsonstr=pdata;


	//printf( "\n"); 
	//printf(st); 
	//printf( "\n"); 


  hnd = curl_easy_init();

  curl_easy_setopt(hnd, CURLOPT_URL, url_id.c_str());

  curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);

  curl_easy_setopt(hnd, CURLOPT_USERNAME,usern1);
  curl_easy_setopt(hnd, CURLOPT_PASSWORD,psw1);


//	cout << " +++++++++ "<< psw1 << "\n"<< endl;
//  curl_easy_setopt(hnd, CURLOPT_USERNAME,user_name);
//  curl_easy_setopt(hnd, CURLOPT_PASSWORD,passwd);


  curl_easy_setopt(hnd, CURLOPT_POSTFIELDS,jsonstr.c_str());

  curl_easy_setopt(hnd, CURLOPT_POSTFIELDSIZE_LARGE, (curl_off_t)71);
  curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/7.38.0");
  curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, slist1);
  curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
  curl_easy_setopt(hnd, CURLOPT_SSH_KNOWNHOSTS, "/home/pi/.ssh/known_hosts");
  curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
  curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);
 
  printf( "\n  Send to Wapice= "); 
  printf( jsonstr.c_str());
  printf( "\n\n");

  ret = curl_easy_perform(hnd);

  printf( "\n\n");


  curl_easy_cleanup(hnd);
  hnd = NULL;
  curl_slist_free_all(slist1);
  slist1 = NULL;

 
}

