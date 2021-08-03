/*
    Develop by      : Fahmi Nurfadilah 
    Email           : fahmi.nurfadilah1412@gmail.com
    Project         : LSKK HomeAutomation
    Version         : 2.1
*/

#include <PubSubClient.h>
#include <ESP8266WiFi.h>

// Update these with values suitable for your network.

const char* ssid = "LSKK_Lantai2";
const char* password = "lskk12345";
const char* mqtt_server = "192.168.4.2";
const char* mqtt_user = "/Homeauto:homeauto";
const char* mqtt_pass = "homeauto12345!";
const char* mqtt_pub_topic = "SecuritySensor";//publish pada mqtt
const char* CL = "LSKK-HA-SAKLAR-SECURITY";
String input_name = "BL_BUTTON"; 
int loop_count  = 0 ; //loop count loop

String statusDevice = "1";

const int input1 = D1;





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
  WiFi.mode(WIFI_STA);
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

void watchdogSetup(void) {
  ESP.wdtDisable();
}

void setup()
{
  Serial.begin(115200);
  pinMode(input1, INPUT_PULLUP);



  setup_wifi();
  printMACAddress();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  watchdogSetup();
}

String button_status;
String prevpubmsg = "";
void loop() {
  String pubmsg = "";
  String input_guid = "c9c1127b-348c-481d-876e-3d4af9425f78";
  for (int i = 0; i <= loop_count; i++) {
    if (!client.connected()) {
      reconnect();
    }
    client.loop();
  }
  loop_count++;
  ESP.wdtFeed();
  Serial.print(loop_count);
  Serial.print(". Watchdog fed in approx. ");
  Serial.print(loop_count * 1000);
  Serial.println(" milliseconds.");
  delay(1000);
  int instate1 = digitalRead(input1);
  if(instate1==LOW) button_status="0";
  else button_status="1";
  
  pubmsg = input_guid + "#" + input_name + "#" +button_status;
  //Kirim
  if(pubmsg != prevpubmsg) {
    client.publish(mqtt_pub_topic, pubmsg.c_str());
    Serial.println(pubmsg);
    prevpubmsg=pubmsg;
  }
}
