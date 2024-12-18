
int reboot = 0;
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
   Serial.println(" MQTT connection... ");
  
    if (client.connect("sensors_ds18b20")) {
     Serial.println("connected MQTT");
      // Once connected, publish an announcement...
     client.publish("sensors_ds18b20", "sensors_ds18b20 Online");

 //------- subscribes --------------//
client.subscribe("reset_ds18b20");
client.subscribe("timeToSend_ds18b20");
 //------- subscribes --------------//

delay(100);

    } else {
     Serial.print("FALHA, client status = ");
     Serial.print(client.state());
     Serial.print(" NOVA TENTATIVA, Nº");
     Serial.print(reboot);
     Serial.println(" - RESET EM 5 SEGUNDOs");
     reboot++;                         
    Serial.println("tentando se conectar ao server..");  
    delay(1000); 
      if(reboot >= 5){ ESP.restart();}
    }
  }
}
