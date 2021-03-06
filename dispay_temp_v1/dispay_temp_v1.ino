#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
//Data linija senzorana povezana na pin 2
#define SENSOR_DATA_BUS 2

//Relej inovi 3 i 4

//Setup za oneWire instancu koja ce komunicirati sa svim OneWire uredjajima
OneWire oneWire(SENSOR_DATA_BUS);

//Njemu referencu saljemo senzoru 
DallasTemperature sensors(&oneWire);

//soil senzor config
int SENSOR_PIN = A0;

//File pointer
File data;

int kill = 0;

void setup() {

  //config releja
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);  //init sd card
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
	process_inputs(temp_val, soil_val);
	output_values(temp_val, soil_val);
  if(kill == 0){
    log_values(temp_val, soil_val);
  }
 
}

/*
 * Funkcija za zapis na karticu
 * @param temp_val - temperatura
 * @param soil_val - vlaga
 */
void log_values(float temp_val, float soil_val){

  data = SD.open("data.txt", FILE_WRITE);

  data.print(temp_val);
  data.print("|");
  data.print(soil_val);
  data.print("\n");
  data.close();

  delay(2000);
  
}

/*
 * Funkcija za ispis na ekran
 * @param temp_val - temperatura
 * @param soil_val - vlaga
 */
void output_values(float temp_val, float soil_val){
  Serial.print(soil_val);
  Serial.print(temp_val);
	
}

/*
 * Funkcija za kontrolu toka i logovanja
 * @param temp_val - temperatura
 * @param soil_val - vlaga
*/ 
void process_inputs(float temp_val, float soil_val){

	if(temp_val>= 26.0){
    digitalWrite(3, LOW);
    kill = 1;
	}
  if(temp_val<= 26.0){
  digitalWrite(3, HIGH);
 }
 
 if(soil_val <=0){
  digitalWrite(4, LOW);
 }
  if(soil_val > 0){
  digitalWrite(4, HIGH);
 }


}

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
  Serial.println("\nHvatam temperaturu");
  sensors.requestTemperatures();

   Serial.println("Temperatura za device 1 (index 0) je: ");
   Serial.println(tempC);

    if(tempC != DEVICE_DISCONNECTED_C) return tempC;

    //Exception
  	else Serial.println("Greska prilikom citanja!");
  	return 0.0;
}
