#include <TinyGPS++.h>
#include <SoftwareSerial.h>
static const int RxPin =3, TxPin=2;
static const uint32_t GPSBaud=9600;
SoftwareSerial myserial(6,5);//Tx = 5 Rx =6 myserial is for gsm
TinyGPSPlus gps;
SoftwareSerial ss(RxPin,TxPin);//ss is for gps
float latt, longi;
#define x A1
#define y A2
#define z A3
int xsample=0;
int ysample=0;
int zsample=0;
#define samples 10
#define minVal -100
#define MaxVal 100



int flag=0;

void setup()
{
//Serial.begin(9600);
for(int i=0;i<samples;i++)
{
xsample+=analogRead(x);
ysample+=analogRead(y);
zsample+=analogRead(z);
}
xsample/=samples;
ysample/=samples;
zsample/=samples;
Serial.println(xsample);
Serial.println(ysample);
Serial.println(zsample);
delay(1000);

Serial.println("System Ready..");
pinMode(9,OUTPUT);
Serial.begin(115200);
myserial.begin(9600);
ss.begin(GPSBaud);
}
void gsm(float latt, float longi)
{
myserial.begin(9600);
Serial.println("Initialising");
delay(1000);
myserial.println("AT");
updateserial();



myserial.println("AT+CMGF=1");
updateserial();
myserial.println("AT+CMGS=\"+918460579134\"");
updateserial();
myserial.println(latt);
myserial.println(longi);
myserial.printIn("http://maps.google.com/maps?&z=15&mrt=yp&t=k&q=");
myserial.println("Accident detected at this location. Please send
help");
myserial.write(26);
updateserial();
}
void updateserial()
{
delay(500);
while(Serial.available())
{
myserial.write(Serial.read());
}
while(myserial.available())
{
Serial.write(myserial.read());
}
}
void gpsfun()
{
//Serial.println("entered");
while(ss.available()>0)
{
//Serial.println("entered");
if (gps.encode(ss.read()))
{



if(gps.location.isValid())
{
latt=gps.location.lat();
longi=gps.location.lng();
}
}
}
}
void loop()
{
if(flag<1)
{
int value1=analogRead(x);
int value2=analogRead(y);
int value3=analogRead(z);
int xValue=xsample-value1;
int yValue=ysample-value2;
int zValue=zsample-value3;
Serial.print("x=");
Serial.println(xValue);
Serial.print("y=");
Serial.println(yValue);
Serial.print("z=");
Serial.println(zValue);
digitalWrite(9,LOW);
delay(100);
if(xValue < minVal || xValue > MaxVal || yValue < minVal || yValue >
MaxVal || zValue < minVal || zValue > MaxVal)
{
gpsfun();
Serial.println("accident");
gsm(latt,longi);
Serial.println("SMS Sent");
digitalWrite(9,HIGH);
delay(1000);

flag=1;
}
}
}
