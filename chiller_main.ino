#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>


//define the potentiometer pin on analog 1
#define POTPIN A0

//Sensor on digital pin 2
#define DHTPIN 2

//Sensor is a DHT22 sensor
#define DHTTYPE DHT22

//No real OLED pin on this board, but we need it for the library to work properly
#define OLED_RESET 4
Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);

//Initialize the dht object
DHT dht(DHTPIN, DHTTYPE);


//Initialize our variables
float humid;
float tempF;
float tempC;
int tempSelect;

void setup()
{
    //Initialize the I2C interface and sensor
	Wire.begin();
    dht.begin();

    //Initialize the display
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}

void loop()
{
    getTempHumid();
    //getTempSetting();
    display.display();
    delay(2000);	
}

void getTempHumid() 
{   
    humid = dht.readHumidity();
    tempC = dht.readTemperature();
    tempF = dht.readTemperature(true);

    if (isnan(humid) || isnan(tempC) || isnan(tempF)) {
        display.clearDisplay();
        display.setTextColor(WHITE);
        display.setTextSize(1);
        display.setCursor(5,0);
        display.print("Failed to read from DHT sensor!");
        return;
    }

    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(0, 1);
    display.print("Cellar");
    display.setTextSize(1);
    display.setCursor(0,20);
    display.print("Humidity: ");
    display.print(humid);
    display.print(" %\t");
    display.setCursor(0,30);
    display.print("Temperature: ");
    display.print(tempC);
    display.print(" C");
    display.setCursor(0,40);
    display.print("Temperature: ");
    display.print(tempF);
    display.print(" F");
}

void getTempSetting()
{
    tempSelect = map(analogRead(POTPIN), 0, 1024, 50, 70);
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(0,20);
    display.print(tempSelect);
}