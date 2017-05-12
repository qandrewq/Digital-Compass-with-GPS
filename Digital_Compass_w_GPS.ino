#include <SparkFun_MAG3110.h> // This library provides many functions for Sparkfun's MAG3110 Magnetometer
#include <TinyGPS++.h> // This library provides many functions for GPS devices
#include <SoftwareSerial.h> // This library enables an Arduino to have serial communiction on digital pins

static const int RXPin = 8, TXPin = 7; // Choosing pins that transmit and receive serial data
static const uint32_t GPSBaud = 9600; // Setting the baud rate of the GPS

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

MAG3110 mag = MAG3110(); //Instantiate MAG3110

String heading;

void setup() {
  Serial.begin(9600);
  ss.begin(GPSBaud);
  delay(2000);

  mag.initialize(); //Initialize the MAG3110
}

void loop() {

  // -------------------- SET GPS COORDINATES ------------------------ //
  // BEST Library
  static const double TARGET_LAT = 39.509889, TARGET_LON = -84.733204;
  
  // South
  //static const double TARGET_LAT = 39.509630, TARGET_LON = -84.732640;

  // North
  //static const double TARGET_LAT = 39.511803, TARGET_LON = -84.732660;

  // Home
  //static const double TARGET_LAT = 37.579288, TARGET_LON = -122.377886;

  // ----------------------------------------------------------------- //
  int x, y, z;

  magCalibrate(); // Calibrates the magnetometer

  // ----------------------------------------------------------------- //
  //                                                                   //
  // During this time, the user must rotate the magnetometer on a flat //
  // surface a full 360 degrees.                                       //
  //                                                                   //
  // ----------------------------------------------------------------- //

  // Find course to destination (0 to 360)
  double courseToDestination =
    TinyGPSPlus::courseTo(
      gps.location.lat(),
      gps.location.lng(),
      TARGET_LAT, 
      TARGET_LON);

  // Convert course to nautical direction
  const char *cardinalToLondon = TinyGPSPlus::cardinal(courseToDestination);

  Serial.print("Destination Heading: ");
  printStr(gps.location.isValid() ? cardinalToLondon : "*** ", 6);
  Serial.println();
  
  //Serial.println(magDirection);

  mag.readMag(&x, &y, &z);

  // Convert magnetometer reading to 0 to 360 heading
  float magDirection = mag.readHeading() + 85;
  if (magDirection < 0) {
    magDirection = 360 - abs(mag.readHeading());
  }

  // Convert magnetometer reading to nautical direction
  if ((magDirection>-11.25+22.5*0)&&(magDirection<11.25+22.5*0)) {
    heading = "N";
  } else if ((magDirection>-11.25+22.5*1)&&(magDirection<11.25+22.5*1)) {
    heading = "NNW";
  } else if ((magDirection>-11.25+22.5*2)&&(magDirection<11.25+22.5*2)) {
    heading = "NW";
  } else if ((magDirection>-11.25+22.5*3)&&(magDirection<11.25+22.5*3)) {
    heading = "WNW";
  } else if ((magDirection>-11.25+22.5*4)&&(magDirection<11.25+22.5*4)) {
    heading = "W";
  } else if ((magDirection>-11.25+22.5*5)&&(magDirection<11.25+22.5*5)) {
    heading = "WSW";
  } else if ((magDirection>-11.25+22.5*6)&&(magDirection<11.25+22.5*6)) {
    heading = "SW";
  } else if ((magDirection>-11.25+22.5*7)&&(magDirection<11.25+22.5*7)) {
    heading = "SSW";
  } else if ((magDirection>-11.25+22.5*8)&&(magDirection<11.25+22.5*8)) {
    heading = "S";
  } else if ((magDirection>-11.25+22.5*9)&&(magDirection<11.25+22.5*9)) {
    heading = "SSE";
  } else if ((magDirection>-11.25+22.5*10)&&(magDirection<11.25+22.5*10)) {
    heading = "SE";
  } else if ((magDirection>-11.25+22.5*11)&&(magDirection<11.25+22.5*11)) {
    heading = "ESE";
  } else if ((magDirection>-11.25+22.5*12)&&(magDirection<11.25+22.5*12)) {
    heading = "E";
  } else if ((magDirection>-11.25+22.5*13)&&(magDirection<11.25+22.5*13)) {
    heading = "ENE";
  } else if ((magDirection>-11.25+22.5*14)&&(magDirection<11.25+22.5*14)) {
    heading = "NE";
  } else if ((magDirection>-11.25+22.5*15)&&(magDirection<11.25+22.5*15)) {
    heading = "NNE";
  }

  Serial.print("Your heading:  ");
  Serial.print(heading);
  Serial.println();
  //Serial.println(magDirection); 
  Serial.println("--------------------------------------------");
  Serial.println();

  delay(500);
}

// Calibrate magnetometer in order to have a precise heading measurement
void magCalibrate() {
    if(!mag.isCalibrated()) //If we're not calibrated
  {
    if(!mag.isCalibrating()) //And we're not currently calibrating
    {
      //Serial.println("Entering calibration mode");
      mag.enterCalMode(); //This sets the output data rate to the highest possible and puts the mag sensor in active mode
    }
    else
    {
      //Must call every loop while calibrating to collect calibration data
      //This will automatically exit calibration
      //You can terminate calibration early by calling mag.exitCalMode();
      mag.calibrate();
    }
  }
  else
  {
    //Serial.println("Calibrated!");
  }
}

// Code from TinyGPS++ to print GPS heading
static void printStr(const char *str, int len)
{
  int slen = strlen(str);
  for (int i=0; i<len; ++i)
    Serial.print(i<slen ? str[i] : ' ');
  smartDelay(0);
}

// Code from TinyGPS++ to control timing
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}
