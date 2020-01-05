#include <U8g2lib.h>
//#include <U8x8lib.h>
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
//#include <Wire.h>
#include <DallasTemperature.h>
#include <OneWire.h>

//Dispay setup
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0);

//Data linija senzorana povezana na pin 2
#define SENSOR_DATA_BUS 2

//Relej inovi 3 i 4

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

  //config releja
  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH);

  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);

  //init sd card
  pinMode(6, OUTPUT);
  SD.begin(6);
  
  
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

	//Float za vlagu
	float soil_val = read_soil_moisture();
	
	//handle
	//process_inputs(temp_val, soil_val);
	output_values(temp_val, soil_val);

  //File pointer
  File data;
  data = SD.open("data.txt", FILE_WRITE);

	data.print(temp_val);
  data.print("|");
  data.print(soil_val);
  data.close();

	delay(2000);
}

/*
 * Funkcija za ispis na ekran
 * @param temp_val - temperatura
 * @param soil_val - vlaga
 */
void output_values(float temp_val, float soil_val){

	//Pravimo char array za drawStr funkciju
	char temp_buffer[5];
  dtostrf(temp_val, 6, 2, temp_buffer);

	char soil_buffer[5];
  dtostrf(soil_val, 6, 2, soil_buffer);

	//Ispis na ekran
	temp_out(temp_buffer,0);
	delay(2000);
	temp_out(soil_buffer,1);  
}

/*
 * Funkcija za kontrolu toka i logovanja
 * @param temp_val - temperatura
 * @param soil_val - vlaga
 
void process_inputs(float temp_val, float soil_val){

	if(temp_val>= 30)digitalWrite(3, HIGH);
	else digitalWrite(3, LOW);

	if(temp_val<=20)digitalWrite(3, LOW);

}
*/
/*
 * Funkcija za citanje vlage zemlje preko senzora
 * @return float - soil_val
 */
float read_soil_moisture(){

	Serial.print("Hvatam vlagu");

	//citanje vlaznosti
	int soil_val = analogRead(SENSOR_PIN);

	//konverzija
	soil_val = map(soil_val, 550, 0, 0, 100);

	//ispis
	Serial.print("Vlaga: ");
	Serial.print(soil_val);
	return soil_val;
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

    if(tempC != DEVICE_DISCONNECTED_C) return tempC;

    //Exception
  	else Serial.println("Greska prilikom citanja!");
  	return 0.0;
}

/*
 * funkcija za ispis na OLED ekran
 */
void temp_out(String text_buffer, int type) {
  //Obrisi
  u8g2.clearBuffer();
  
  //Ispis
  if(type == 0){
    u8g2.drawStr(0, 15, "Temperatura:");
    u8g2.drawStr(0, 30, text_buffer.c_str());
    u8g2.drawStr(35, 30, " C");
    u8g2.sendBuffer();
  }

  //Ispis
  u8g2.drawStr(0, 15, "Vlaga:");
  u8g2.drawStr(0, 30, text_buffer.c_str());
  u8g2.sendBuffer();
}
