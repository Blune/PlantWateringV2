#include <ESP8266WebServer.h>

int led= 2;
bool led_status = false;
const char* ssid = "ESP8266_Server";
const char* pass = "blablablabla"; // >= 8 characters!

IPAddress ip(192,168,4,1);
IPAddress gateway(192,168,4,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);
 
String headAndTitle = "<head><style>"
                        ".button {"
                          "border: none;"
                          "color: white;"
                          "width: 350px;"
                          "padding: 20px;"
                          "text-align: center;"
                          "margin: 20px 200px;"
                        "}"
                        ".greenButton {background-color: green; font-size: 64px;}"
                        ".redButton {background-color: red; font-size: 64px;}"
                        ".blueButton {background-color: blue; font-size: 50px;}"
                      "</style>"
                      "</head><meta http-equiv=\"refresh\" content=\"5\"></head>"
                      "</BR></BR><h1 align=\"center\">Control your ESP8266 by Browser</h1></div>"
                      "<div align=\"center\">Control three LEDs and get measured value, AP - ModeS</BR></BR></div>";

String led0_1= "<a href=\"/led0_on\"><button class=\"button greenButton\">LED0 On</button></a>";
String led0_0= "<a href=\"/led0_off\"><button class=\"button redButton\">LED0 Off</button></a>";

void setup(){
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  
  WiFi.softAPConfig(ip, gateway, subnet); 
  WiFi.softAP(ssid, pass);  
 
  delay(500); 

  server.on("/",handleRoot);
  server.on("/led0_on", led0on);
  server.on("/led0_off", led0off);
  server.begin();
}
  
void loop(){
  server.handleClient(); 
}

void handleRoot() {
  led0off();
}

void led0on(){
  switchLEDAndSend(1);
}

void led0off(){
  switchLEDAndSend(0);
}

void switchLEDAndSend(bool state){
  led_status = state;
  
  String message = "";
  message += headAndTitle;
  message += "<div align=\"center\";>";
  
  digitalWrite(led, state);
  (led_status==true)?(message += led0_0):(message += led0_1);
  
  float measuredValue = analogRead(A0)/1024.0 * 3.3;
  message += "</BR><button class=\"button blueButton\">";
  message += "Voltage [V]: </BR>";
  message += String(measuredValue, 2);
  message += "</button>";
  message += "</div>";
  server.send(200, "text/html", message); 
}
