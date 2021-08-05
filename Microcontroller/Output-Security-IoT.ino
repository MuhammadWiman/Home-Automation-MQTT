/*
    Develop by      : Fahmi Nurfadilah
    Email           : fahmi.nurfadilah1412@gmail.com
    Updated by      : Ary Setijadi Prihatmanto
    Email           : asetijadi@lskk.ee.itb.ac.id
    Project         : LSKK Homeautomation
    Version         : 2.1
*/

#include <PubSubClient.h>
#include <ESP8266WiFi.h>

// Update these with values suitable for your network.

const char* ssid = "LSKKHomeauto"; //ssid/nama wifi  
const char* password = "1234567890"; //password wifi
const char* mqtt_server = "192.168.0.2"; //server mqtt
const char* mqtt_user = "/homeauto:homeauto"; //user mqtt
const char* mqtt_pass = "homeauto12345!"; //password mqtt
const char* CL = "LSKK-HA-AKTUATOR-SECURITY"; //nama alat
const char* mqtt_pub_topic = "status"; //topik dari data input
const char* output_name = "ALARM";
String statusDevice = "0";
int relay1 = D1 ;


//5183a824-3609-11eb-adc1-0242ac120002 | Lampu Pijar | Punclut | BC:DD:C2:16:CE:BD
//5885f13d-c8c5-47a9-9c30-1f606fb83169 | Lampu tengah | Punclut | A0:20:A6:1B:B5: E
//813b8f84-5e92-462a-a51a-c8e3c017ff28 | Lampu atas | Punclut | 5C:CF:7F:F7:5E:A7

const char* device_guid = "b0e272d9-1472-4640-a5e1-b435abfb2c3c";// masukkan guuid/serial_number
String output_value;

char msg[100];
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

char sPayload[100];
char message [40] ;
char address[40];
void callback(char* topic, byte* payload, unsigned int length) {
  memcpy(sPayload,payload,length);
  memcpy(address,payload,36);
  memcpy(message,&payload[37],length-37);
  Serial.print("Message arrived [");
  Serial.print(sPayload);
  Serial.println("] ");

  Serial.println(device_guid);
  Serial.println(address);
  if(String((char *)address) == String((char *)device_guid)) 
  {
    Serial.println("tersambung");
  }
  else
  { 
    Serial.println("tidak tersambung");
    return;    
  }

  Serial.println(message);

  if (message[0] == '1') {
    digitalWrite(relay1, HIGH);
    Serial.println("relay 1 on");
    statusDevice = "1";

  }
  if (message[0] == '0') {
    digitalWrite(relay1, LOW);
    Serial.println("relay 1 off");
    statusDevice= "0";
    
  }


  Serial.print("Publish message: ");
  String pubmsg = String(device_guid) + "#" + output_name + "#"+ String(statusDevice) ;
  Serial.println(pubmsg);
  client.publish(mqtt_pub_topic, pubmsg.c_str());
  delay(50);
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
      client.subscribe("SecurityAktuator"); //mengambil data dari antrian received
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      ESP.restart();
      delay(5000);

    }
  }
}

void setup()
{
  pinMode(D1, OUTPUT);

  digitalWrite(D1, HIGH);

  //pinMode(input, INPUT);
  Serial.begin(115200);
  setup_wifi();
  printMACAddress();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
