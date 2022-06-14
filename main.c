#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
const char* ssid = "enerhack";
const char* password = "internetaccess";
WiFiUDP Udp;
unsigned int localUdpPort = 9931;  // local port to listen on
char incomingPacket[600];  // buffer for incoming packets
char  requestAcVol[] = "api/getacvoltage/0";  // a reply string to send back
int sendtime = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println();

  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");

  Udp.begin(localUdpPort);
  //Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
}



void loop() {
  //delay(5000);
  Serial.print("Alfa online, waiting for instructions.\n");
  String start = Serial.readString();
  while(start != "Alfa") {
    start = Serial.readString();
  }
  Serial.print("satart sending : api/getdcpower/all \n");

  String combined = "api/getdcpower/all";
  udpsend(combined);
  int packetSize = Udp.parsePacket();   //Reset Udp to zero?
  while(packetSize == 0)
  {   
      packetSize = Udp.parsePacket();
  }

    
  //receiving incoming UDP packets
  Serial.printf("Received %d bytes from %s, port %d\n" , packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
  int len = Udp.read(incomingPacket,501);
  if(len >0)
  {
    incomingPacket[len]= 0;
  }
  Serial.printf("UDP packet contents: %s\n", incomingPacket);
  
  int i = 0;
  for(i = 0; incomingPacket[i] != '['; i++) {
    
  }
  i++;
  char temp[256];
  int j = i;
  for(j = i; incomingPacket[j] != ']'; j++) {
     temp[j-i] = incomingPacket[j];
     //Serial.printf(temp[j-i]
  }

  temp[j-1] = '\0';
  
  //double value = root["VALUE"];
    Serial.printf("value of your command is \n");
    Serial.println(temp);
    //clean the received packet and await for next new packet.
    packetSize = 0;
//sendback a reply, to the IP adress and port we got the packet from
  Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
  Udp.write("ack");
  Udp.endPacket();
  
      
//  func.remove(0);
  //para.remove(0);

  for(int i = 0; i < 501; i++) {
    incomingPacket[i] = '\0';
  }
}
  void udpsend(String apiCall)
  {
    const char ip[]="192.168.1.217";    //217
    sendtime = sendtime +1;
    char charBuf[501];
    apiCall.toCharArray(charBuf, 501) ;    //overflow if len exceed 256
    Serial.print("sending new api %i  time", sendtime);
    Serial.print(charBuf);
    Udp.beginPacket(ip,9931);
    Udp.write(charBuf);
    Udp.endPacket();
    delay(100);
  return;
  }
