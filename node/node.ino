/* YourDuino Multiple DS18B20 Temperature Sensors on 1 wire
  Connections:
  DS18B20 Pinout (Left to Right, pins down, flat side toward you)
  - Left   = Ground
  - Center = Signal (Pin 2):  (with 3.3K to 4.7K resistor to +5 or 3.3 )
  - Right  = +5 or +3.3 V
  
  // Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

   Based on:
   
/*-----( Import needed libraries )-----*/
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>

// Declare Pins
#define ONE_WIRE_BUS_PIN 3
#define DHTPIN 2

// Define constants
#define DHTTYPE DHT22

/*-----( Declare objects )-----*/
// Initialize DHT sensor for normal 16mhz Arduino
DHT dht( DHTPIN, DHTTYPE );

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire( ONE_WIRE_BUS_PIN );

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors( &oneWire );

/*-----( Declare Variables )-----*/
// Assign the addresses of your 1-Wire temp sensors.
// See the tutorial on how to obtain these addresses:
// http://www.hacktronics.com/Tutorials/arduino-1-wire-address-finder.html

DeviceAddress Probe01 = { 0x28, 0xFF, 0x7D, 0xBF, 0x3C, 0x04, 0x00, 0x9C }; 


void setup() {
  // start serial port to show results
  Serial.begin( 115200 );
  //Serial.println( "FermTemp v0.1 " );
  //Serial.print( "DallasTempLib version: " );
  //Serial.println( DALLASTEMPLIBVERSION );
  
  dht.begin();
  
  // Initialize the Temperature measurement library
  sensors.begin( );
  
  // set the resolution to 10 bit (Can be 9 to 12 bits .. lower is faster)
  sensors.setResolution( Probe01, 10 );
  
  //Serial.print( "Number of 1wire devices found: " );  
  //Serial.println( sensors.getDeviceCount( ) ); 
}

void loop() {
  // Wait 5 seconds between measurements
  delay( 5000 );
  
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float ahum = dht.readHumidity();
  // Read temperature as Ce
  float atempc = dht.readTemperature();
  // Read temperature as Fahrenheit
  float atempf = dht.readTemperature(true);
  
  // Check if any reads failed and exit early (to try again).
  if (isnan( ahum ) || isnan( atempc ) || isnan( atempf )) {
    Serial.println( "Failed to read from DHT sensor!" );
    return;
  }
  
  float tempc = sensors.getTempC( Probe01 );
  
  if (tempc == -127.00) {
    Serial.println( "Failed to read from DS18B20 probe!" );
    return;
  } 
  float tempf = DallasTemperature::toFahrenheit( tempc );
  
  // Compute heat index
  // Must send in temp in Fahrenheit!
  float aindex = dht.computeHeatIndex( atempf, ahum );
  
  // Command all devices on bus to read temperature  
  sensors.requestTemperatures( );

  // Arduino's don't support printf of floats, so hacky workaround is to convert them to strings
  char str_ahum [7]; 
  char str_atempc [7]; 
  char str_atempf [7]; 
  char str_aindex [7]; 
  char str_tempc [7]; 
  char str_tempf [7]; 
  dtostrf( ahum, 5, 2, str_ahum );
  dtostrf( atempc, 5, 2, str_atempc );
  dtostrf( atempf, 5, 2, str_atempf );
  dtostrf( aindex, 5, 2, str_aindex );
  dtostrf( tempc, 5, 2, str_tempc );
  dtostrf( tempf, 5, 2, str_tempf );

  char buffer [73];
  sprintf( buffer, "ahum:%s,atempc:%s,atempf:%s,aindex:%s,tempc:%s,tempf:%s\n", str_ahum, str_atempc, str_atempf, str_aindex, str_tempc, str_tempf );

  Serial.print( buffer );
}
