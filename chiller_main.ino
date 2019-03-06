#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>


//define the potentiometer pin on analog 1
#define POTPIN A0

//define the relay pin on digital 13
#define RELAY_PIN 13

//Sensor on digital pin 2
#define DHTPIN 2

//Sensor is a DHT22 sensor
#define DHTTYPE DHT22

//No real OLED pin on this board, but we need it for the library to work properly
#define OLED_RESET 4
Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);

//define the temp setpoint limits, margin, and delay before chiller can change states
#define MAX_TEMP 70
#define MIN_TEMP 50
#define MARGIN 2
#define CHILLER_DELAY 10000

//define the time to switch display cycles in milliseconds
#define DISPLAY_CYCLE 2000

//Initialize the dht object
DHT dht(DHTPIN, DHTTYPE);


//Initialize our variables
int humid;
int tempF;
int tempSelect;
int currentTime;
int displayTime = 0;
int chillerTime = 0;
bool dispTemp = true;
bool chillerState = false;

void setup()
{
    //Initialize the I2C interface and sensor
	Wire.begin();
    dht.begin();

    //set digital pinmode
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW);

    //Initialize the display
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}

void loop()
{
    getSensors();
    currentTime = millis();

    if (currentTime - chillerTime > CHILLER_DELAY) {    
        if (tempF > tempSelect + MARGIN && chillerState == false) {
            chillerState = true;
            digitalWrite(RELAY_PIN, HIGH);
        }
        else if (tempF < tempSelect - MARGIN && chillerState == true) {
            chillerState = false;
            digitalWrite(RELAY_PIN, LOW);
        
        chillerTime = currentTime;
        }     
    }

    updateDisp(dispTemp);
    display.display();
    
    if (currentTime - displayTime > DISPLAY_CYCLE) {
        if (dispTemp == true) {
            dispTemp = false;
        }
        else {
            dispTemp = true;
        }
        displayTime = currentTime;
    }
    
    delay(1);
}

void getSensors()
{
    tempSelect = map(analogRead(POTPIN), 0, 1020, MIN_TEMP, MAX_TEMP);
    tempF = (int)dht.readTemperature(true);
    humid = (int)dht.readHumidity();
}

void updateDisp(bool dataSelect)
{
    if (isnan(humid) || isnan(tempF)) {
        display.clearDisplay();
        display.setTextColor(WHITE);
        display.setTextSize(1);
        display.setCursor(5,0);
        display.print("Failed to read temperature!");
        return;
    }

    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(0, 1);
    display.print("Set: ");
    display.print(tempSelect);
    display.print(" F");

    if (dataSelect == true) {
        display.setTextSize(5);
        display.setCursor(30,25);
        display.print(tempF);
        display.setTextSize(3);
        display.print((char)247);
    }
    else {
        display.setTextSize(5);
        display.setCursor(20,25);
        display.print(humid);
        display.print("%\t");
    }
}