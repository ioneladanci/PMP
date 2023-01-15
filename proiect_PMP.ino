#include <WiFi.h>

const char *WIFI_NAME = "yourAP";
const char *WIFI_PASSWORD  = "yourPassword"; 
WiFiServer server(80);

String header;
int mode=0;
int b1=0;
int b2=0;
int b3=0;

String LED_THREE_STATE = "off";

int max_value=100;
const int led = 4;
const int led_b = 15;
volatile int value=0;

const int microphone =25;

void setup() {
  Serial.begin(115200);
  pinMode(microphone, INPUT);
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  pinMode(led_b, OUTPUT);
  digitalWrite(led_b, LOW);

  Serial.print("Connecting to ");
  Serial.println(WIFI_NAME);
  WiFi.softAP(WIFI_NAME, WIFI_PASSWORD);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin(); 
  Serial.println("Server started");
  delay(10);
}



void loop(){
  WiFiClient client = server.available();  
  control_led();
  if (client) {                            
    Serial.println("New Client is requesting web page");          
    String current_data_line = "";  
    while (client.connected()) {  
      control_led();
      if (client.available()) {             
        char new_byte = client.read();             
        Serial.write(new_byte);                    
        header += new_byte;
        if (new_byte == '\n') {                    
         
          if (current_data_line.length() == 0) 
          {
            
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            if (header.indexOf("1LED=ON") >= 0) 
            {
                mode=1;  
            }
            if(header.indexOf("2LED=OFF") >= 0) {
              mode=0;
            }
            if(header.indexOf("3LED=MUSIC") >= 0) {
              mode=2; 
            }
            if(header.indexOf("LED=BLINK1") >= 0) {
              
              mode=3;   
            }
            if(header.indexOf("LED=BLINK2") >= 0) {
              
              mode=4;   
            }
            if(header.indexOf("LED=BLINK3") >= 0) {
              
              mode=5;   
            }
            control_led();
            
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button {background-color: #8A2BE2; padding: 15px 32px; color: black; border: 5px solid #8A2BE2;}");
            
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");            
            // Web Page Heading
            client.println("</style></head>");
            client.println("<body><center><h1>LED controlling</h1></center>");
            client.println("<form><center>");
            client.println("<p>LED is " + LED_THREE_STATE + "</p>");
            
            client.println ("<button class=\"button\" name=\"1LED\" value=\"ON\" type=\"submit\">LED ON</button>");
            client.println ("<button class=\"button\" name=\"2LED\" value=\"OFF\" type=\"submit\">LED OFF</button></center>");
            client.println("<p></p>");
            client.println("<button class=\"button\" name=\"3LED\" value=\"MUSIC\" type=\"submit\"> MUSIC REACTIVE LED</button>");
            client.println("<p></p>");
            client.println("<button class=\"button\" name=\"LED\" value=\"BLINK1\" type=\"submit\"> BLINK1 </button>");
            client.println("<button class=\"button\" name=\"LED\" value=\"BLINK2\" type=\"submit\"> BLINK2 </button>");
            client.println("<button class=\"button\" name=\"LED\" value=\"BLINK3\" type=\"submit\"> BLINK3 </button>");
            client.println("</form></body></html>");
            client.println();
            
            
            break;
          } 
          else 
          { 
            current_data_line = "";
          }
        } 
        else if (new_byte != '\r') 
        {  
          current_data_line += new_byte;     
        }
        
      }
    }
    
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}

void control_led(){
  if(mode==0){
    Serial.println("LED is OFF");
    LED_THREE_STATE = "OFF";
    digitalWrite(led, LOW);
    digitalWrite(led_b, LOW);
  }
  if(mode==1){
    Serial.println("GET LED is ON");
    LED_THREE_STATE = "ON";
    digitalWrite(led, HIGH);
    digitalWrite(led_b, HIGH); 
  }  
  if (mode==2){
    LED_THREE_STATE = "MUSIC REACTIVE";
    int s1=0;
    s1=digitalRead(microphone);
    delay(15);
    if (s1>max_value)
      max_value=s1;
    int s1_map=map(s1,0,max_value,0,1023);
    if(s1_map>=1){
      digitalWrite(led_b, HIGH);
      Serial.println(s1);
    }
    else{
      digitalWrite(led_b, LOW);
    }
    digitalWrite(led, s1_map);
  }
  if(mode==3){
    blink1();
  }
  if(mode==4){
    blink2();
  }
  if(mode==5){
    blink3();
  }
  
}

void blink1(){
  b1++;
  if((b1 > 30000 and b1 < 45000) or (b1 > 55000 and b1 < 70000)or (b1 > 80000 and b1 < 95000)){
    digitalWrite(led, HIGH);
    digitalWrite(led_b, HIGH); 
  }
   else{
    digitalWrite(led, LOW);
    digitalWrite(led_b, LOW); 
  }
  if(b1 >=95000){
    b1=0;
  }
  
}

void blink2(){
  b2++;
  if(b2 > 10000  ){
    digitalWrite(led, HIGH);
    digitalWrite(led_b, HIGH); 
  }
  else{
    digitalWrite(led, LOW);
    digitalWrite(led_b, LOW); 
  }
  if(b2>=20000){
    b2=0;
  }
  
}

void blink3(){
  b3++;
  if(b3 > 30000  ){
    digitalWrite(led, HIGH);
    digitalWrite(led_b, HIGH); 
  }
  else{
    digitalWrite(led, LOW);
    digitalWrite(led_b, LOW); 
  }
  if(b3>=90000){
    b3=0;
  }
  
}
