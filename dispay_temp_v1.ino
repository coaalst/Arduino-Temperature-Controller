#include <U8g2lib.h>
#include <U8x8lib.h>
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <DallasTemperature.h>
#include <OneWire.h>

//Dispay setup
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0);

//Data linija senzorana povezana na pin 2
#define SENSOR_DATA_BUS 2

//Pin za napajanje senzora
#define SENSOR_POWER_LINE 4

//Setup za oneWire instancu koja ce komunicirati sa svim OneWire uredjajima
OneWire oneWire(SENSOR_DATA_BUS);

//Njemu referencu saljemo senzoru 
DallasTemperature sensors(&oneWire);

//soil senzor config
int SENSOR_PIN = A0;


void setup() {
  //Pokreni lib za ekran
  u8g2.begin();
  u8g2.setFont(u8g2_font_crox4hb_tr);

  //config power pin-a
  pinMode(SENSOR_POWER_LINE, OUTPUT);
  digitalWrite(SENSOR_POWER_LINE, HIGH);
  
  Serial.begin(9600);

  //Pokreni senzor
  sensors.begin();
}

/*
 * Na svakih 5 sek updejtujemo vrednost temperature
 * i ispisujemo je na ekran. Deo za kontrolu releja 
 * jos uvek nije zavrsen.
 * 
 * Troubleshoot preko serial monitora
 */
void loop() {
//Float za temp prima vrednost
 float temp_val = read_temperature();

  //Ako smo uspesno procitali
  if(temp_val != DEVICE_DISCONNECTED_C){

   //Pravimo char array za drawStr funkciju
   String temp_string = String(temp_val);
   char buffer[5];
   temp_string.toCharArray(buffer,5);

   //Ispis na ekran
   display_out(buffer);  
  } 
 
  //Exception
  else Serial.println("Greska prilikom citanja!");

  Serial.print("Hvatam vlagu");
  
  //citanje vlaznosti
  int soil_val = analogRead(SENSOR_PIN);

  //konverzija
  soil_val = map(soil_val, 550, 0, 0, 100);
  
  //ispis
  Serial.print("Vlaga: ");
  Serial.print(soil_val);
  
  delay(5000);
}

/*
 * Funkcija za citanje temperature preko senzora
 * @return float - temp
 */
float read_temperature(){
  //Var za temperaturu
  float tempC = sensors.getTempCByIndex(0);
  
  //Trazimo temperaturu
  Serial.print("Hvatam temperaturu");
  sensors.requestTemperatures();
  Serial.println("uspeh");

   Serial.print("Temperatura za device 1 (index 0) je: ");
   Serial.println(tempC);

   return tempC;
}

/*
 * funkcija za ispis na OLED ekran
 */
void display_out(String text_buffer) {
  //Obrisi
  u8g2.clearBuffer();
  
  //Ispis
  u8g2.drawStr(0, 15, "Temperatura:");
  u8g2.drawStr(0, 30, text_buffer.c_str());
  u8g2.drawStr(35, 30, " C");
  u8g2.sendBuffer();
}
