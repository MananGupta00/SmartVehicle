#include <SoftwareSerial.h>
SoftwareSerial mySerial(9, 10);

const int igRelay=4;
const int tamperRelay=5;
const int IRin=6;
const int FuelIN=A7;
//defined pins
bool turned= true;
bool parkingmode= true;
int gpsCount=0;
int oldgpslat=0;
int oldgpslon=0;


//registered phone no.
String phno="+919758183355"; 


void setup() {
  pinMode(igRelay,OUTPUT);
  pinMode(IRin,INPUT);
  pinMode(tamperRelay,INPUT);
  pinMode(FuelIN,INPUT);
  Serial.begin(9600); 

  mySerial.begin(9600); 
  mySerial.println("AT+CNMI=2,2,0,0,0\r"); // AT Command to receive a live SMS
  delay(100);
}

void RecieveMessage()
{
  String phn="";
  String info="",mesg="";
    //To access message phone no.
  if(mySerial.available()!=0)
    { mySerial.readStringUntil('\n');
      info = mySerial.readStringUntil('\n');
      mesg= mySerial.readStringUntil('\n');
      phn= info.substring(7,20);

      if(phn==phno)
      { 
      Serial.print("message- ");
       int l=mesg.length();
            mesg= mesg.substring(0,l-1);
      Serial.println(mesg);

     mesg="LOCATE";  //Testing
      
      if(mesg=="TURNOFF")
      {
        turned= false;
        SendSMS("Security turned off");
      }
      else if(mesg=="TURNON")
      {
        SendSMS("Security turned on");
        turned= true;
      }
      else if(mesg=="LOCATE")
       {
        String msg= bluetooth();
        int index= msg.indexOf("Lat", 4);
        msg=msg.substring(0,index);
        Serial.println(msg);
        SendSMS(msg);
        }
      }
    } 
  }

void SendSMS(String msg)
  {
    Serial.print("sending- "+msg);
  mySerial.println("AT+CMGF=1");    
  delay(500);
  mySerial.println("AT+CMGS=\"+919758183355\"\r"); 
  mySerial.print(" Hello! I am Bike Security. ");
  mySerial.print(msg);
  delay(200);
  mySerial.println((char)26); //the stopping character Ctrl+Z
  }
    
void gsm(String msg){
// transferring data to gsm modem
     SendSMS(msg);
  }

void alert(String type){
   gsm(type);
  }

void tamperWire(){
   int untemp= digitalRead(tamperRelay);
   delay(2);
   if(untemp==0)
    {
     alert("Ignition Wire Tampering ALERT");
     }
  }

void checkPark(){
   int untemp= digitalRead(tamperRelay);
    delay(2);
    if(untemp==1)
    {
     parkingmode=true;
     }
  }

String bluetooth(){
  String reads="";
    Serial.readStringUntil('\n');
    delay(100);
  if(Serial.available()!=0)
    reads = Serial.readStringUntil('\n');
    delay(5);
    return reads;
  }


void checkfinger(){
  String incoming= bluetooth();
  
  if (incoming=="finger")   
      ignite();
  else if(incoming=="alert")
      alert("Unauthentic FingerPrint ALERT");
  else if(incoming!="")
      {
        if(gpsCount==0)
      checkGPS(incoming); //Checking for location change
      else
      gpsCount--;
      }
  }

int getGPSLat(String txt){
      int lat;
      int index= txt.indexOf("lon");
      String latt= txt.substring(3, index);
      
      Serial.print("Latitude- ");
      Serial.println(latt);
      
      lat= latt.toInt();
      return lat;
  }
int getGPSLon(String txt){
      int lon;
      int index= txt.indexOf("lon");
      String latt= txt.substring(index);
      
      Serial.print("Longitude- ");
      Serial.println(latt);
      
      lon= latt.toInt();
      return lon;
  
  }

void checkGPS(String txt){
  
    gpsCount=10;
    int latitude= getGPSLat(txt);
    int longitude= getGPSLon(txt);
    
    if((latitude>=(oldgpslat+0.0000001))||(latitude<=(oldgpslat-0.0000001))||(longitude>=oldgpslon+0.0000001)||(longitude<=(oldgpslon-0.0000001)))
    alert("Vehicle Theft Alert");
    oldgpslat=latitude;
    oldgpslon=longitude;
    
}

void irsensor(){
  int dist = digitalRead(IRin);
  delay(20);
  if(dist==0){
  alert("Vehicle Lift Alert");
  }
  }

void ignite(){
 
 Serial.print("Igniting");
 parkingmode=false;
 digitalWrite(igRelay,HIGH);
 delay(500);
  }

void loop() {
  RecieveMessage();

  if(turned==true)
  {    
    if(parkingmode==true){
      //checkfinger();
      irsensor();
      //tamperWire();
     }
    else
      checkPark();
  }
  else{
    digitalWrite(igRelay,HIGH);
    }
}
