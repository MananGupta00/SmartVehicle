#include <GSM.h>
#define PINNUMBER ""
int parkingmode= HIGH;
int rel =4;
int dist;
int reads;

//gsm variables
char senderNumber[20];
GSM gsmAccess;
GSM_SMS sms;


void setup() {
  pinMode(rel, OUTPUT);   
  Serial.begin(9600); 

    while (!Serial);
       bool notConnected = true;

  // Start GSM connection
  while (notConnected) {
    if (gsmAccess.begin(PINNUMBER) == GSM_READY) {
      notConnected = false;
    } else {
      Serial.println("Not connected");
      delay(1000);
    }
  }

  Serial.println("GSM initialized");
  Serial.println("Waiting for messages");
}


void SendSMS(int type)
{
  Serial.println("AT+CMGF=1");    
  delay(1000);
  Serial.println("AT+CMGS=\"+919758183355\"\r"); 
  if(type==1)
  Serial.println(" Hello, I am GSM Modem! Someone is trying to lift your vehicle ");
  delay(200);
  Serial.println((char)26); //the stopping character Ctrl+Z
  delay(1000); 
}

void RecieveMessage()
{
char c;

  // If there are any SMSs available()
  if (sms.available()) {
    Serial.println("Message received from:");

    // Get remote number
    sms.remoteNumber(senderNumber, 20);
    Serial.println(senderNumber);

    // An example of message disposal
    // Any messages starting with # should be discarded
    if (sms.peek() == '#') {
      Serial.println("Discarded SMS");
      sms.flush();
    }

    // Read message bytes and print them
    while (c = sms.read()) {
      Serial.print(c);
    }

    Serial.println("\nEND OF MESSAGE");

    // Delete message from modem memory
    sms.flush();
    Serial.println("MESSAGE DELETED");
  }
}
 

void gsm(int type){
    //Give enough time for GSM to register on Network
     SendSMS(type);    //Send one SMS 
  }

void alert(int type){
   gsm(type) ;  
  }

void tamperingWire(){
  
  }


void checkfinger(){
  Serial.println("check");
   while(Serial.available()==0);
  reads=Serial.read();
  if (reads==1)
    {
      parkingmode=LOW;
      ignite();
    }
  else
   alert(2);
   delay(50);
  }

void irsensor(){
  dist = digitalRead(10); 
  delay(50);
  if(dist==0)
  alert(1);
  }
  
void ignite(){
 digitalWrite(rel,HIGH);
 delay(500);
 digitalWrite(rel,LOW);
 delay(500);
    }

void loop() {
    RecieveMessage();
   
/*  if(parkingmode==HIGH){   
   irsensor();  
  tamperWire();
  delay(100);
  }
*/
}
