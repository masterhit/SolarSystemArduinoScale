/****************************************************************************************
 ****************************************************************************************
 *
 *
 *   SOLAR SYSTEM SCALE V3.0
 *   Author: Wandeclayt M.
 *   wandeclayt [at] gmail dot com
 *   
 *   [PORTUGUES]
 *   
 *   A balanca utiliza 4 celulas de carga de 50kg, conectadas a um amplificador HX711.
 *   Após exibir o peso (massa) na Terra, os valores correspondentes a indicacao que
 *   a mesma balanca (calibrada para a aceleracao gravitacional na superficie da Terra)
 *   registraria em varios corpos do Sistema Solar sao calculados e exibidos.
 *   
 *   Eh um projeto simples mas valioso para museus de ciencia, observatorios astronomicos
 *   abertos ao publico e feiras de ciencia.
 *   
 *   [ENGLISH]
 *   
 *   The scale uses 4 x 50kg load cells, connected to a HX711 amplifier.
 *   After displaying the weight (mass) value on Earth, it computes and displays the
 *   readings you would get on the surface of several solar system bodies
 *   with a scale calibrated for the Earth surface gravity acceleration.
 *   
 *   It is a simple yet valuable project for science museums, public observatories or
 *   science fairs.
 *   
 *   PARTS LIST
 *   01 Arduino Board
 *   01 LCD (i2c)
 *   04 50kg Load Cells
 *   01 HX711
 *
 ****************************************************************************************
 ****************************************************************************************/

#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <HX711.h>


/****************************************************************************************
                                HX711 CONFIGURATION
 ****************************************************************************************/

 HX711 scale(A1, A0);          // A1=DATA, A0=CLOCK.

/****************************************************************************************
                                DISPLAY CONFIGURATION
 ****************************************************************************************/

#define I2C_ADDR 0x27  // Define I2C Address for controller
#define BACKLIGHT_PIN 3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

#define  LED_OFF 0
#define  LED_ON  1

LiquidCrystal_I2C  lcd(I2C_ADDR, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);
 
 float  ScaleMeasure;          // weight (mass) scale reading on earth.
 int    ViewTime = 3500;       // display time for each set of weigth readings

/****************************************************************************************
 *                            SURFACE GRAVITY VALUES
 ****************************************************************************************/
 
 const float  Mercury = 0.378;  
 const float  Venus   = 0.907; 
 const float  Earth   = 1;
 const float  Moon    = 0.166; 
 const float  Mars    = 0.377;
 const float  Jupiter = 2.36;  
 const float  Saturn  = 0.916; 
 const float  Uranus  = 0.889; 
 const float  Neptune = 1.12;  
 const float  Pluto   = 0.071;


void setup()
{

  delay(50);
  lcd.begin (16, 2); 
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(LED_OFF);
  delay(50);
  lcd.setBacklight(LED_ON);

  Serial.begin(38400);
  Serial.println("Solar System Scale v2.0");
  Serial.println("Author: Wandeclayt M.");

/****************************************************************************************
 * 
 *                               CALIBRATION FACTOR
 *                                                              
 ****************************************************************************************/

  scale.set_scale(-19000.f);       // 4 cell X 50kg calibration factor.
  //scale.set_scale(-28000.f);     // 1 cell X 50kg 4wire
  scale.tare();

  
/****************************************************************************************
 * 
 *                              INITIALIZATION MESSAGE
 * 
 ****************************************************************************************/

  lcd.clear();
  delay(100);

  lcd.setCursor(0, 0);              
  lcd.print("Ciencia");
  lcd.setCursor(0, 1);
  lcd.print("no Parque");
  delay(2000);

  lcd.clear();
  delay(100);
  
  lcd.setCursor(0, 0);              
  lcd.print("Balanca");
  lcd.setCursor(0, 1);
  lcd.print("SistemaSolarV3.0");
  delay(1000);

  lcd.clear();
  delay(100);
  
  lcd.setCursor(0, 0);              
  lcd.print("author:");
  lcd.setCursor(0, 1);
  lcd.print("Wandeclayt M.");

}

void loop()
{
/****************************************************************************************
 * 
 * DISPLAYS "SCALE READY/BALANCA PRONTA" ON LCD WHILE NO WEIGHT IS DETECTED
 * 
 ****************************************************************************************/

  do {

  ScaleMeasure = scale.get_units(5),1;        // Weight Detection 

  lcd.clear();
  delay(10);
  lcd.home();

  delay(10);
  lcd.setCursor(1, 0);
  lcd.print("BALANCA PRONTA");

  for (int StandByCursor=0; StandByCursor <= 15; StandByCursor++){
  lcd.setCursor(StandByCursor, 1);
  lcd.print("*");
  delay(50);
  }
  
  Serial.println("BALANCA PRONTA");       //Prints on Serial Monitor 
    
  } while (ScaleMeasure <= 5);

  lcd.clear();
  delay(10);
  lcd.home();

  delay(10);
  lcd.setCursor(4, 0);
  lcd.print("MEDINDO");
  
  ScaleMeasure = scale.get_units(30),1;
  
/****************************************************************************************
                            FLOAT TO STRING READING CONVERSION
 ****************************************************************************************/
  
  String DisplayValEarth  = String (ScaleMeasure);
  String DisplayValMoon   = String (ScaleMeasure * Moon);
  String DisplayValMars   = String (ScaleMeasure * Mars);
  String DisplayValJupiter= String (ScaleMeasure * Jupiter);
  String DisplayValSaturn = String (ScaleMeasure * Saturn);
  String DisplayValPluto  = String (ScaleMeasure * Pluto);

  
/****************************************************************************************
                                SERIAL MONITOR BLOCK
 ****************************************************************************************/
  
  Serial.print("Terra: \t");
  Serial.print(DisplayValEarth);
  Serial.print("kg \t | \t");
  Serial.print("Lua: \t");
  Serial.print(DisplayValMoon);
  Serial.println("kg");

  Serial.print("Marte: \t");
  Serial.print(DisplayValMars);
  Serial.print("kg \t | \t");
  Serial.print("Jupiter: \t");
  Serial.print(DisplayValJupiter);
  Serial.println("kg");

  Serial.print("Saturno: ");
  Serial.print(DisplayValSaturn);
  Serial.print("kg \t | \t");
  Serial.print("Plutao: \t");
  Serial.print(DisplayValPluto);
  Serial.println("kg");
  

/****************************************************************************************
                                    LCD DISPLAY BLOCK
 ****************************************************************************************/
  
  lcd.clear();
  delay(10);
  lcd.home();

  lcd.backlight();
  
  lcd.setCursor(0, 0);              // First line
  lcd.print("Terra =");
  lcd.setCursor(8, 0);
  lcd.print(DisplayValEarth);
  lcd.setCursor(14, 0);
  lcd.print("kg");

  lcd.setCursor(0, 1);              // Second line
  lcd.print("Lua   =");
  lcd.setCursor(8, 1);
  lcd.print(DisplayValMoon);
  lcd.setCursor(14, 1);
  lcd.print("kg");

  delay(ViewTime);                     

  lcd.clear();                      
  delay(50);
  lcd.home();

  lcd.setCursor(0, 0);              // First line
  lcd.print("Marte =");
  lcd.setCursor(8, 0);
  lcd.print(DisplayValMars);
  lcd.setCursor(14, 0);
  lcd.print("kg");

  lcd.setCursor(0, 1);              // Second line
  lcd.print("Jupiter=");
  lcd.setCursor(8, 1);
  lcd.print(DisplayValJupiter);
  lcd.setCursor(14, 1);
  lcd.print("kg");
  
  delay(ViewTime);                      

  lcd.clear();                      
  delay(50);
  lcd.home();

  lcd.setCursor(0, 0);              // First line
  lcd.print("Saturno=");
  lcd.setCursor(8, 0);
  lcd.print(DisplayValSaturn);
  lcd.setCursor(14, 0);
  lcd.print("kg");

  lcd.setCursor(0, 1);              // Second line
  lcd.print("Plutao =");
  lcd.setCursor(8, 1);
  lcd.print(DisplayValPluto);
  lcd.setCursor(14, 1);
  lcd.print("kg");
  
  delay(ViewTime);                      

  lcd.clear();                      
  delay(50);
  lcd.home();

}




