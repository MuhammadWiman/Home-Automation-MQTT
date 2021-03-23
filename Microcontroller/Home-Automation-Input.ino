/*
    Develop by      : Fahmi Nurfadilah 
    Email           : fahmi.nurfadilah1412@gmail.com
    Project         : IoT General
    Version         : 1.2
*/

#include <PubSubClient.h>
#include <ESP8266WiFi.h>

// Update these with values suitable for your network.

const char* ssid = "LSKK_Lantai2"; /// ssid/nama wifi yeng terkoneksi internet
const char* password = "lskk12345"; //paassword wifi
const char* mqtt_server = "167.205.7.19";// mqtt server
const char* mqtt_user = "/workshopmitra:workshopmitra"; //user mqtt
const char* mqtt_pass = "passwordnyaqwerty";//password mqtt
const char* mqtt_pub_topic = "Received";//antrian data input
const char* CL = "LSKK-HA-Saklar-1";//nama alat

String statusDevice[4] = {"1", "1", "1", "1"};

int input1 = D1 ;
int input2 = D2 ;
int input3 = D3 ;
int input4 = D4 ;


int instate1 = 0;
int instate2 = 0;
int instate3 = 0;
int instate4 = 0;


int prevState1 = 0;
int prevState2 = 0;
int prevState3 = 0;
int prevState4 = 0;


char message [100];

WiFiClient espClient;
PubSubClient client(espClient);

byte mac[6];
String MACAddress;

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

String mac2String(byte ar[]) {
  String s;
  for (byte i = 0; i < 6; ++i)
  {
    char buf[3];
    sprintf(buf, "%2X", ar[i]);
    s += buf;
    if (i < 5) s += ':';
  }
  return s;
}

void printMACAddress() {
  WiFi.macAddress(mac);
  MACAddress = mac2String(mac);
  Serial.println(MACAddress);
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  printMACAddress();
  const char* CL;
  CL = MACAddress.c_str();
  Serial.println(CL);
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(CL, mqtt_user, mqtt_pass)) {
      Serial.println("connected");
    } else {Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      ESP.restart();
      delay(5000);

    }
  }
}

void setup()
{
  Serial.begin(115200);
  //pinMode(input, INPUT);
  pinMode(input1, INPUT_PULLUP);
  pinMode(input2, INPUT_PULLUP);
  pinMode(input3, INPUT_PULLUP);
  pinMode(input4, INPUT_PULLUP);
  setup_wifi();
  printMACAddress();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}
void loop() {
  String pubmsg = "";
  String value = String(statusDevice[0] + statusDevice[1] + statusDevice[2] + statusDevice[3]);
  String input_guid = "5183a824-3609-11eb-adc1-0242ac120002";
  String input_value = value;
  int callstate[4] = {instate1,instate2,instate3,instate4};
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  instate1 = digitalRead(input1);
  instate2 = digitalRead(input2);
  instate3 = digitalRead(input3);
  instate4 = digitalRead(input4);
  
  //Serial.print("Publish message: ");
  //pubmsg = input_guid + "#" + input_value;
  //Serial.println(pubmsg);
  //client.publish(mqtt_pub_topic, pubmsg.c_str());
  if (instate1 == 1) {
    if(prevState1 != instate1){
      statusDevice[0] = "0";
      Serial.println("Input detected!");
      pubmsg = input_guid + "#" + input_value;
      Serial.println(pubmsg);
      client.publish(mqtt_pub_topic, pubmsg.c_str());
      Serial.println("Success send to RMQ");
      prevState1 = callstate[0];
    }
  }
  if (instate1 == 0) {
    if(prevState1 != instate1){
      statusDevice[0] = "1";
      Serial.println("Input not detected!");
      pubmsg = input_guid + "#" + input_value;
      Serial.println(pubmsg);
      client.publish(mqtt_pub_topic, pubmsg.c_str());
      Serial.println("Success send to RMQ");
      prevState1 = callstate[0];
  }
}
if (instate2 == 1) {
    if(prevState2 != instate2){
      statusDevice[1] = "0";
      Serial.println("Input detected!");
      pubmsg = input_guid + "#" + input_value;
      Serial.println(pubmsg);
      client.publish(mqtt_pub_topic, pubmsg.c_str());
      Serial.println("Success send to RMQ");
      prevState2 = callstate[1];
    }
  }
  if (instate2 == 0) {
    if(prevState2 != instate2){
      statusDevice[1] = "1";
      Serial.println("Input not detected!");
      pubmsg = input_guid + "#" + input_value;
      Serial.println(pubmsg);
      client.publish(mqtt_pub_topic, pubmsg.c_str());
      Serial.println("Success send to RMQ");
      prevState2 = callstate[1];
  }
}
if (instate3 == 1) {
    if(prevState3 != instate3){
      statusDevice[2] = "0";
      Serial.println("Input detected!");
      pubmsg = input_guid + "#" + input_value;
      Serial.println(pubmsg);
      client.publish(mqtt_pub_topic, pubmsg.c_str());
      Serial.println("Success send to RMQ");
      prevState3 = callstate[2];
    }
  }
  if (instate3 == 0) {
    if(prevState3 != instate3){
      statusDevice[2] = "1";
      Serial.println("Input not detected!");
      pubmsg = input_guid + "#" + input_value;
      Serial.println(pubmsg);
      client.publish(mqtt_pub_topic, pubmsg.c_str());
      Serial.println("Success send to RMQ");
      prevState3 = callstate[2];
  }
}
if (instate4 == 1) {
    if(prevState4 != instate4){
      statusDevice[3] = "0";
      Serial.println("Input detected!");
      pubmsg = input_guid + "#" + input_value;
      Serial.println(pubmsg);
      client.publish(mqtt_pub_topic, pubmsg.c_str());
      Serial.println("Success send to RMQ");
      prevState4 = callstate[3];
    }
  }
  if (instate4 == 0) {
    if(prevState4 != instate4){
      statusDevice[3] = "1";
      Serial.println("Input not detected!");
      pubmsg = input_guid + "#" + input_value;
      Serial.println(pubmsg);
      client.publish(mqtt_pub_topic, pubmsg.c_str());
      Serial.println("Success send to RMQ");
      prevState4 = callstate[3];
  }
}
}
