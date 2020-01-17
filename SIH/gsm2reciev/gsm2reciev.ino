#include <SoftwareSerial.h>
SoftwareSerial mySerial(9, 10);
int i,j,k,t=0;
char msg[100];
char c;


char phno[]="7088782979";

void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600); 

  mySerial.println("AT+CNMI=2,2,0,0,0\r"); // AT Command to receive a live SMS
  delay(100);
}

void loop() 
{
  if(t==0){
  i=0;
  int x=0;
  k=25;
  j=12;
  while(mySerial.available()>0)
  {
    msg[i++]=mySerial.read(); 
  }
      while(k>j)
      {      
        c=msg[j++];
        if(c==phno[x])
        {
           x++;
          if(x==10)
          {Serial.print(phno); 
          t=1;}      
       }        
        else
        break;
      }
      
  
 delay(1000);
 
 }
}
