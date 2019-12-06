#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define PIN_POWER1 D6

const char* ssid = "deltoro"; // Enter your WiFi name
const char* password =  "idiotilla"; // Enter WiFi password
const char* mqttServer = "192.168.1.102";
const int mqttPort = 1883;
const char* mqttUser = "deltoro";
const char* mqttPassword = "idiotilla";

WiFiClient espClient;
PubSubClient client(espClient);
 
void setup() {
  pinMode(PIN_POWER1, OUTPUT);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  // llama a reconnect() en el "main" y se conecta MQTT
}
/*****************************************************************************************************************************************/
void callback(char* topic, byte* payload, unsigned int length) {

  char cadenaLeida[50];
  int i = 0;
   
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (; i < length; i++) { 
    Serial.print(cadenaLeida[i]=(char)payload[i]); 
  }
  cadenaLeida[i]='\0';
 
  if (0==strcmp(topic,"cmnd/persianadormitorio/POWER1")){ //--------------------------------------- comparo el topic
   Serial.println();
   Serial.println("topic POWER1 ejecutado"); 
   if (0==strcmp(cadenaLeida,"ON")) {digitalWrite(PIN_POWER1, HIGH);}
   if (0==strcmp(cadenaLeida,"1")) {digitalWrite(PIN_POWER1, HIGH);}
   if (0==strcmp(cadenaLeida,"OFF")) {digitalWrite(PIN_POWER1, LOW);}
   if (0==strcmp(cadenaLeida,"0")) {digitalWrite(PIN_POWER1, LOW);}
  }
  Serial.println();
  Serial.println("-----------------------"); 
  
}
/***********************************************************Conecta y envia*******************************************************************/
void reconnect() {
    Serial.println("Connecting to MQTT..."); 
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
      Serial.println("connected");  
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
    }
  //client.publish("cmnd/persianadormitorio/POWER1", "TOGGLE"); //Topic name --> publicando un MSG en el topic
  client.subscribe("cmnd/persianadormitorio/POWER1");         //Subscrito a un topic 
  client.subscribe("cmnd/persianadormitorio/POWER2");
}
/*********************************************************************************************************************************************/
void loop() {
  if (!client.connected()){
    reconnect();
  }else{
    client.loop();
  }

//  if (client.connected()){client.publish("cmnd/persianadormitorio/POWER1", "TOGGLE");}
//  delay(500);
 
  
}
