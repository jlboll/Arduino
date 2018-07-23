#include <SPI.h>
#include <Ethernet.h>
#include <DallasTemperature.h>  //library for temperature sensors
#include <OneWire.h>            //library for the onewire bus

#define ONE_WIRE_BUS     7        //the onewire bus is connected to pin 7 on arduino
#define TEMPERATURE_PRECISION  10 //resolution of the sensors is set to 10bit(somehow won´t work)


OneWire oneWire(ONE_WIRE_BUS);               // setup a oneWire instance to communicate with any OneWire devices
// (not just Maxim/Dallas temperature ICs)
DallasTemperature sensors(&oneWire);         // Pass our oneWire reference to Dallas Temperature
// variable to store the number of sensors
bool connected = false;                                   // yes-no variable (boolean) to store if the arduino is connected to the server
int i = 0;                                                // variable to count the sendings to the server
// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x7A, 0xA1 }; // Ardurino mac adresse
byte ip[] = { 192, 168, 1, 10 };
byte server[] = { 192, 168, 1, 30 }; // MinSynology
EthernetClient client;
//---------------------------------------------------------------------------------------------
void setup(void)                                  // setup-function (runs only at the startup of the arduino)
{
  Serial.begin(9600);                               // start the serial port

  sensors.begin();                                  // start up the library
  int numSensors = sensors.getDeviceCount();        // store the number of sensors to the variable numSensors,
  int temparray[numSensors];            // array with "numSensors" storage places for the temperature of each sensor
  // "sensors.getDeviceCount" is a function in the library

  Serial.println("Enumerating and scanning for I2C sensors.");

  if (numSensors > 0)                     // if there is at least one sensor:
  {
    Serial.print("Enumerated ");            //print the number of sensors to serial port
    Serial.print(numSensors);
    Serial.println( " sensors.");

  }
  else                                      //if there is no sensor:
  {
    Serial.println("No sensors enumerated."); // tell the serial port
  }

  Ethernet.begin(mac, ip);

  delay(1000);

  Serial.println("connecting...");

  if (client.connect(server, 80)) {
    Serial.println("connected");
  } else {
    Serial.println("connection failed");
  }
}
//----------------------------------------------------------------------------------------------------------
void loop(void)                     // loop function (runs over and over again)
{
  int numSensors = sensors.getDeviceCount();
  float temparray[numSensors];          // array with "numSensors" storage places for the temperature of each sensor
  // "sensors.getDeviceCount" is a function in the dallas temperature library
  sensors.requestTemperatures();      // send the request for temperature to sensors (all sensors)
  delay(100);
  for (i = 0; i < numSensors; i++)    // as long as "i" ( chosen number, starts at 0) is smaller than
    //numSensors" (number of sensors) do the "for" cycle
  {
    float temp = sensors.getTempCByIndex(i); // take temperature reading from sensor "i" and store it to the variable "temp"
    temparray[i] = temp;                   // store the temperature from sensor i to storage place i in the array
  }
  client.connect(server, 80);
  Serial.println("GET http://192.168.1.30  ");
  client.print("GET http://192.168.1.30/data.php?sensor="); //place your server address here
  for (i = 0; i < numSensors; i++)
  {
    Serial.print("&sensor");
    Serial.print(i);
    Serial.print("=");
    Serial.print(temparray[i]);
    client.print("&sensor");
    client.print(i);
    client.print("=");
    client.print(temparray[i]);

    if (i < numSensors - 1)
    {
      Serial.println(" ");
    }
    else
    {
    }
  }
  client.println("Host:http://192.168.1.30");
  client.println();
  client.stop();

  Serial.println();
  delay(58633);//-2
}
