#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged TO GPIO 23
#define ONE_WIRE_BUS 2
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
// Number of temperature devices found
int numberOfDevices;
// We'll use this variable to store a found device address
DeviceAddress tempDeviceAddress; 

const char* ssid = "PRODUCAO";  
const char* password =  "Frango@9800";

WiFiClient espClient;
PubSubClient client(espClient);
const char* mqtt_server ="192.168.3.200";
unsigned long lastmillis = 0;
int timeToSend = 3000;

void setup(){
  // start serial port
  Serial.begin(115200);
  delay(1000);
Serial.println("inicializando");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi..");
  int restart = 0;
      while (WiFi.status() != WL_CONNECTED && restart < 30) {
        delay(350);
        Serial.print(".");
        restart ++;
    }
if(restart >= 30){
 WiFiManager wifiManager;
 wifiManager.setConfigPortalTimeout(240);
//Cria um AP (Access Point) com: ("nome da rede", "senha da rede")
  if (!wifiManager.autoConnect("Sensors_ds18b20", "ds18b20123")) {
    Serial.println(F("Falha na conexao. Resetar e tentar novamente..."));
    delay(500);
    ESP.restart();
  }  
}

  Serial.println(F("Conectado na rede Wifi."));
  Serial.print(F("Endereco IP: "));
  Serial.println(WiFi.localIP());

client.setServer(mqtt_server, 1883);
client.setCallback(callback);

  // Start up the library
  sensors.begin();
  // Grab a count of devices on the wire
  numberOfDevices = sensors.getDeviceCount();
  
  // locate devices on the bus
  Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(numberOfDevices, DEC);
  Serial.println(" devices.");

  // Loop through each device, print out address
  for(int i=0;i<numberOfDevices; i++){
    // Search the wire for address
    if(sensors.getAddress(tempDeviceAddress, i)){
      Serial.print("Found device ");
      Serial.print(i, DEC);
      Serial.print(" with address: ");
      printAddress(tempDeviceAddress);
      Serial.println();
    } else {
      Serial.print("Found ghost device at ");
      Serial.print(i, DEC);
      Serial.print(" but could not detect address. Check power and cabling");
    }
  }
}

void loop(){ 
  client.loop(); 
 if (!client.connected()) {
  Serial.println("CLIENTE DESCONECTADO!");
 reconnect(); 
 }

if(millis() - lastmillis > timeToSend){
  lastmillis = millis();
sendTemperaturesWithAddress();
}
 
} // end loop

// function to print a device address
void printAddress(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++){
    if (deviceAddress[i] < 16) Serial.print("0");
      Serial.print(deviceAddress[i], HEX);
  }
}


void sendTemperaturesWithAddress() {
  StaticJsonDocument<256> doc;  // Documento JSON
  bool dataInsert = false;

  sensors.requestTemperatures();  // Solicita as temperaturas

  for (int i = 0; i < numberOfDevices; i++) {
    if (sensors.getAddress(tempDeviceAddress, i)) {
      char addressString[17];  // Buffer para armazenar o endereço em HEX
      getAddressAsString(tempDeviceAddress, addressString);

      float tempC = sensors.getTempC(tempDeviceAddress);
      Serial.print("Address: ");
      Serial.print(addressString);
      Serial.print(" - Temp C: ");
      Serial.println(tempC);

      doc[addressString] = tempC;  // Adiciona endereço e valor ao JSON
      dataInsert = true;
    }
  }

  String json;
  if (dataInsert) {
    serializeJson(doc, json);  // Converte o documento JSON em string
  } else {
    json = "{\"error\": \"no devices found\"}";
  }

  client.publish("sensors_ds18b20/address", json.c_str());  // Publica JSON
}

// Função auxiliar para converter o endereço do dispositivo em uma string hexadecimal
void getAddressAsString(DeviceAddress deviceAddress, char* buffer) {
  for (uint8_t i = 0; i < 8; i++) {
    sprintf(buffer + i * 2, "%02X", deviceAddress[i]);
  }
  buffer[16] = '\0';  // Finaliza a string
}



void callback(char* topic, byte* payload, unsigned int length) {
 Serial.print("Message Recebida no Topic [");
 Serial.print(topic);
 Serial.print("] ");
 String PayLoad;
 int resultado=0;

  for (int i = 0; i < length; i++) {
   //Serial.print((char)payload[i]);
   PayLoad+=(char)payload[i];
  }
  Serial.println();
Serial.println(PayLoad);

if (strcmp(topic,"reset_ds18b20")==0){
 int resultado = PayLoad.toInt();
   if(resultado){
  ESP.restart();}
  }

if (strcmp(topic,"timeToSend_ds18b20")==0){
 timeToSend = PayLoad.toInt();
  }

} // end callback