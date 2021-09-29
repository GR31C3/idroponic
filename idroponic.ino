//LIBRARY FOR SD CARD
#include <SD.h>
#include <SPI.h>
//SD CARD VARIABLE
int chipSelect=4; //pin of chip select sd board
File myData;  //variable for interect with file object

//LIBRARY FOR HUMIDITY/TEMPERATURE SENSOR
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <DHT_U.h>

//VARIABLE HUM/TEMP SENSOR
#define DHT_pin A0 // pin sensor
#define DHTTYPE DHT11  //define model sensor
DHT dht(DHT_pin, DHTTYPE);

//VARIABLE WATER BUCKET
int redLed=10; //define pin for red led 
int greenLed=9; //define pin for green led 
int blueLed=8; //define pin for blue led 
float maxLev=200.00; //define max water level 
float midLev=100.00; //define middle water level 

//VARIABLE PUMP
int relayPin=A1;  //define pin relay pump// need to connect pump and adapter throgh a relay
int pumpOn=5*60000;  //define value pump turn on
int pumpOff=50*60000;  //define value pump turn off

//DEFINE OVERLAPPING
const unsigned long intervall=60*60000;
unsigned long previusTime=0;

void setup() {
  //define led water level as OUTPUT
  pinMode(redLed, OUTPUT);
  pinMode(blueLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(relayPin, OUTPUT);
  // initialize serial monitor
  Serial.begin(9600);
 //sensor hum/temp initialize
 dht.begin();
 pinMode(10, OUTPUT); //reserv pin 10 as output, don't use for other thinghs required from library
 SD.begin(chipSelect); //initialize SD card with chipselect in pin4
}

void loop() {
  int waterSensor=analogRead(A1);  //define pin for sensor
//LOOP FOR ON/OFF PUMP
 digitalWrite(relayPin,HIGH);   
 delay(pumpOn);
 digitalWrite(relayPin,LOW);
 delay(pumpOff);

//LOOP FOR WATER LEVEL  
 unsigned long currenTime=millis();  //updates frequently
  if(currenTime-previusTime >= intervall){  //event to overlap
  if (waterSensor > maxLev){   // reading level water sensor data and send to serial monitor
    Serial.println("Full");
    digitalWrite(redLed, HIGH);
    digitalWrite(greenLed, LOW);
    digitalWrite(blueLed, LOW);
 }
  else if ((waterSensor > midLev) && (waterSensor <= maxLev)) {
    Serial.println("Middle");
    digitalWrite(redLed, LOW);
    digitalWrite(greenLed, HIGH);
    digitalWrite(blueLed, LOW);
 }
  else{
    Serial.println("No water");
    digitalWrite(redLed, LOW);
    digitalWrite(greenLed, LOW);
    digitalWrite(blueLed, HIGH);
 }
 previusTime=currenTime; //update the timing for the next time around
  }
delay(2000);
float h = dht.readHumidity();
float t = dht.readTemperature();
float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f))  {
      Serial.println("Failed to read from DHT sensor!");
      return;
  }
  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);
  
  myData=SD.open("THData.txt", FILE_WRITE); //open THData.txt on SD card
  if (myData){
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C");
  Serial.print(hif);
  Serial.println(" *F");

  myData.print(h);
  myData.print(",");
  myData.print(t);
  myData.print(",");
  myData.print(f);

  myData.close();
 }
 }
