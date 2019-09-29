//Automatic Water Level control system by Team 3 WIZnet IoT contest
/*Arduino with WIZnet Ethernet Shield
 */

#include <SPI.h>
#include <Ethernet.h>
#include <LiquidCrystal.h>

int level1=A1;
int level2=A2;
int level3=A3;
int level4=A4;
int level5=A5;
int motor=9;

int a;
int b;
int c;
int d;
int e;
int r=0;

int z=111; // Adjust this value from 100 to 1023 if your circuit do not show correct value. 

void motor1(int);

LiquidCrystal lcd(7,6,2,3,4,5);

void setup()
{
pinMode(level1,INPUT);
pinMode(level2,INPUT);
pinMode(level3,INPUT);
pinMode(level4,INPUT);
pinMode(level5,INPUT);
pinMode(motor,OUTPUT);

Serial.begin(9600);

lcd.begin(16, 2); // if you are using 16x2 line LCD, then replace these values and also adjust all LCD text for 1st and 2nd line in the loop below. 
}

void loop()
{
a=analogRead(level1);
b=analogRead(level2);
c=analogRead(level3);
d=analogRead(level4);
e=analogRead(level5);
lcd.clear();
lcd.setCursor(0,0);              
lcd.print("TEAM3 Project");   
lcd.setCursor(0,1);              
lcd.print("H20 Flow monitor");

if(e>z && d>z && c>z && b>z && a>z )
{
Serial.print("Full tank----");
Serial.println("motor off");
motor1(0);
digitalWrite(motor,HIGH);
lcd.clear();
lcd.setCursor(1,2);
lcd.print("Tank: 100% FULL");delay(150);
}

else
{
  if(e<z && d>z && c>z && b>z && a>z ){
lcd.clear();
lcd.setCursor(1,2);
lcd.print("Tank: 80% FULL");delay(250);
motor1(1);
}

if(e<z && d<z && c>z && b>z && a>z )
{
lcd.clear();
lcd.setCursor(1,2);
lcd.print("Tank: 60% FULL");delay(250);
motor1(1);
}
if(e<z && d<z && c<z && b>z && a>z )
{
lcd.clear();
lcd.setCursor(1,2);
lcd.print("Tank: 40% FULL");delay(250);
motor1(1);
}
if(e<z && d<z && c<z && b<z && a>z )
{
lcd.clear();
lcd.setCursor(1,2);
lcd.print("Tank: 20% FULL");delay(250);
motor1(1);
}

if(e<z && d<z && c<z && b<z && a<z )
{
Serial.print("Tank is empty");delay(1000);
motor1(1);
lcd.clear();
lcd.setCursor(3,2);
lcd.print("Tank is EMPTY");delay(1000);
}
}  
lcd.clear();
}

void motor1( int t){
Serial.println("Entering motor loop");
Serial.print("value of t:  ");
Serial.println(t);

if(t==0)
{
Serial.print("motor off");
digitalWrite(motor,HIGH);
lcd.clear();
lcd.setCursor(0,3);
lcd.print("Pump (OFF)");

}
else
{
lcd.clear();
lcd.setCursor(0,3);
digitalWrite(motor,LOW);
delay(200);
Serial.print("motor on");
lcd.print("Pump  (ON)");

}
}

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };   //physical mac address
byte ip[] = { 192, 168, 1, 178 };                      // ip in lan (that's what you need to use in your browser. ("192.168.1.178")
byte gateway[] = { 192, 168, 1, 1 };                   // internet access via router
byte subnet[] = { 255, 255, 255, 0 };                  //subnet mask
EthernetServer server(80);                             //server port     
String readString;

void setup() {
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  pinMode(led, OUTPUT);
  microservo.attach(7);
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  // Create a client connection
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {   
      if (client.available()) {
        char c = client.read();
     
        //read char by char HTTP request
        if (readString.length() < 100) {
          //store characters to string
          readString += c;
          //Serial.print(c);
         }

         //if HTTP request has ended
         if (c == '\n') {          
           Serial.println(readString); //print to serial monitor for debuging
     
           client.println("HTTP/1.1 200 OK"); //send new page
           client.println("Content-Type: text/html");
           client.println();     
           client.println("<HTML>");
           client.println("<HEAD>");
           client.println("<meta name='apple-mobile-web-app-capable' content='yes' />");
           client.println("<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />");
           client.println("<link rel='stylesheet' type='text/css' href='Team 3 home' />");
           client.println("<TITLE>Team 3</TITLE>");
           client.println("</HEAD>");
           client.println("<BODY>");
           client.println("<H1>Team 3</H1>");
           client.println("<hr />");
           client.println("<br />");  
           client.println("<H2>Arduino with Ethernet Shield</H2>");
           client.println("<br />");  
           client.println("<a href=\"/?button1on\"\">Turn On moter</a>");
           client.println("<a href=\"/?button1off\"\">Turn Off moter</a><br />");   
           client.println("<br />");     
           client.println("<br />");
           client.println("<br />"); 
           client.println("</BODY>");
           client.println("</HTML>");
     
           delay(1);
           //stopping client
           client.stop();
           //controls the Arduino if you press the buttons
           if (readString.indexOf("?button1on") >0){
               digitalWrite(motor, HIGH);
           }
           if (readString.indexOf("?button1off") >0){
               digitalWrite(motor, LOW);
           }
           
         }
       }
    }
}
}
