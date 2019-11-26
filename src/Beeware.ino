#include <SPI.h>
#include "Display.h"
#include "OPENSCALE.h"
#include "MAX17043.h"
#include "SI7021.h"
#include "PC9088.h"

// SDA = D0;
// SCL = D1;

// EPD_CS_PIN = A2
// EPD_CLK_PIN = A3
// EPD_DIN_PIN = A5

// SPI_SS = A2
// SPI_SCK = A3
// SPI_MISO = A4;
// SPI_MOSI = A5;

// Pin definition
#define DEBUG_BUTTON        D2
#define SI7021_CONTROL_PIN 	D3
#define EPD_RESET_PIN       A0
#define EPD_DC_PIN          A1
#define EPD_CS_PIN          A2
#define EPD_BUSY_PIN        D7

// #define PC9088_READ_PIN 		A4
// #define PC9088_CONTROL_PIN 	D6

SYSTEM_THREAD(DISABLED);
SYSTEM_MODE(AUTOMATIC);

STARTUP(WiFi.selectAntenna(ANT_EXTERNAL));
STARTUP(System.enableFeature(FEATURE_RETAINED_MEMORY));
STARTUP(System.disable(SYSTEM_FLAG_PUBLISH_RESET_INFO));

Display display(SPI, EPD_DC_PIN, EPD_CS_PIN, EPD_RESET_PIN, EPD_BUSY_PIN);
MAX17043 powerModule(Wire);
OPENSCALE weightSensor(Serial1);
SI7021 tempSensor(Wire, SI7021_CONTROL_PIN);
//PC9088 lightSensor(PC9088_READ_PIN, PC9088_CONTROL_PIN);

const int TZ = -8;

const int HIGH_POWER_MODE_INTERVAL_MS = 60000; // 1 minute
const int MEDIUM_POWER_MODE_INTERNAL_SEC = 300;   // 5 minutes
const int LOW_POWER_MODE_INTERVAL_SEC = 600;  // 10 minutes

const int SOC_LOW = 40;             // 40% charge dropped below low limit
const int SOC_HIGH = 80;            // 80% enough power to keep running

const unsigned long UPDATE_LIMIT = 900000; // 15 minutes
retained unsigned long lastUpdated;

bool debug = false;
double voltage = 0;
double soc = 0;
int alert = 0;
double strength = 0;
double quality = 0;
double humidity = 0;
double tempf = 0;
double lightV = 0;
double lightR = 0;
double weight = 0;

void setup()
{
  //pinMode(SI7021_CONTROL_PIN, OUTPUT);
  pinMode(DEBUG_BUTTON, INPUT_PULLDOWN);

  //if (true)
  if(digitalRead(DEBUG_BUTTON) == HIGH)
  {
    debug = true;

    // debug output
    Serial.begin(9600);

   while(!Serial.isConnected()) // wait for Host to open serial port
     	Particle.process();

    Serial.printf("%s\n", "Connected Debug");
  }

  RGB.control(true);
  RGB.brightness(0);
  RGB.control(false);

  debug = true;

  // set time zone to western us
  Time.zone(TZ);
  updateTime();

  tempSensor.turnOn();

  if (debug)
    Serial.println("Temp Module On");

  // power monitoring
   Wire.begin();
   powerModule.Initialize();

  if (debug)
    Serial.println("Power Module On");

  // weight sensor
  Serial1.begin(9600, SERIAL_8N1); // start uart

  if (debug)
    Serial.println("Weight Module On");

  int status = display.Setup();

  if (status != 0 && debug)
  {
    Serial.print("Display Statup Failed: ");
    Serial.println(status);
  }

  if (debug)
  {
    if (!tempSensor.verify())
    {
        Serial.println("Temperature Sensor Fault");
    }
  }

  //lightSensor.turnOn();
}

void printDebug(DeviceData &data)
{
  Serial.print("Voltage: ");
  Serial.print(data.Voltage);
  Serial.println(" V");

  Serial.print("Percentage: ");
  Serial.print(data.Charge);
  Serial.println(" %");

  Serial.print("Alert: ");
  Serial.println(data.Alert);

  Serial.print("Signal Strength: ");
  Serial.print(data.Strength);
  Serial.println("%");

  Serial.print("Signal Quality: ");
  Serial.print(data.Quality);
  Serial.println("%");

  Serial.print("Temp:");
  Serial.print(data.Temp);
  Serial.println("F");

  Serial.print("Humidity:");
  Serial.print(data.Humidity);
  Serial.println("%");

  Serial.print("Light Voltage: ");
  Serial.print(data.Light);
  Serial.println();

  Serial.print("Weight: ");
  Serial.print(data.Weight);
  Serial.println("lb");

  Serial.printf("%s\n", "...");
}

String buildJson(DeviceData &data)
{
  String json = "{";

  json.concat("\"Voltage\":");
  json.concat(data.Voltage);

  json.concat(",\"Charge\":");
  json.concat(data.Charge);

  json.concat(",\"Alert\":");
  json.concat(data.Alert);

  json.concat(",\"SS\":");
  json.concat(data.Strength);

  json.concat(",\"QS\":");
  json.concat(data.Quality);

  json.concat(",\"Temp\":");
  json.concat(data.Temp);

  json.concat(",\"Humidity\":");
  json.concat(data.Humidity);

  json.concat(",\"Light\":");
  json.concat(data.Light);

  json.concat(",\"Weight\":");
  json.concat(data.Weight);

  json.concat("}");

  return json;
}

void loop()
{
  collect();

  publish();

  if (soc < SOC_LOW)
  {
    System.sleep(SLEEP_MODE_DEEP, LOW_POWER_MODE_INTERVAL_SEC);
  }

  if (soc < SOC_HIGH)
  {
    System.sleep(SLEEP_MODE_DEEP, MEDIUM_POWER_MODE_INTERNAL_SEC);
  }

  delay(HIGH_POWER_MODE_INTERVAL_MS);
}

void collect()
{
  voltage = powerModule.getVoltage();
  soc = powerModule.getSOC();
  alert = powerModule.getAlert();

  if (WiFi.ready())
  {
    WiFiSignal signal = WiFi.RSSI();
    strength = signal.getStrengthValue();
    quality = signal.getQuality();
  }
  else
  {
    strength = 0;
    quality = 0;
  }

  tempf = tempSensor.readTemperatureF();
  humidity = tempSensor.readHumidity();
  //lightV = lightSensor.readVolt();
  //lightR = lightSensor.readLight();
  weight = weightSensor.readWeight();
}

void publish()
{
  Particle.connect();
  waitFor(Particle.connected, 10000);

  DeviceData data;
  data.Voltage = voltage;
  data.Charge = soc;
  data.Alert = alert;
  data.Strength = strength;
  data.Quality = quality;
  data.Humidity = humidity;
  data.Light = lightR;
  data.Temp = tempf;
  data.Weight = weight;

  String jsonData = buildJson(data);

  particle::Future<bool> successfuture = Particle.publish("sensordata", jsonData, 60, PRIVATE, WITH_ACK);

  while(!successfuture.isDone())
  {
    delay(1000);
  }

  if (debug)
  {
     if (successfuture.isSucceeded())
       Serial.println("Publish Succeeded");
  }
  else
  {
    if (debug)
      Serial.println("Publish Failed");

    printDebug(data);
  }

  unsigned long time = millis();

  if (time <= lastUpdated)
  {
    lastUpdated = 0;

    int status = display.Update(data);

    if (status != 0 && debug)
    {
      Serial.print("Display Statup Failed: ");
      Serial.println(status);
    }
  }
  else
  {
    if ((lastUpdated - time) >  UPDATE_LIMIT)
    {
      int status = display.Update(data);

      if (status != 0 && debug)
      {
        Serial.print("Display Statup Failed: ");
        Serial.println(status);
      }

      lastUpdated = time;
    }
  }
}

void updateTime()
{
  time_t lastSyncTimestamp;
  unsigned long lastSync = Particle.timeSyncedLast(lastSyncTimestamp);

  if (isDateGreater(Time.now(), lastSyncTimestamp))
  {
    if (lastSyncTimestamp > 0)
    {
      Serial.print("Time received from Particle Device Cloud was: ");
      Serial.println(Time.timeStr(lastSyncTimestamp));
    }

    // Request time synchronization from Particle Device Cloud
    Particle.syncTime();

    // Wait until Photon receives time from Particle Device Cloud (or connection to Particle Device Cloud is lost)
    waitUntil(Particle.syncTimeDone);
  }

  if (isDST())
  {
    if (!Time.isDST())
      Time.beginDST();
  }
  else
  {
    if (Time.isDST())
      Time.endDST();
  }
}

bool isDateGreater(time_t a, time_t b)
{
  struct tm *tmpa = gmtime(&a);
  struct tm *tmpb = gmtime(&b);

  if (tmpa->tm_year > tmpb->tm_year)
    return true;

  if (tmpa->tm_mon > tmpb-> tm_mon)
    return true;

  if (tmpa->tm_mday > tmpb-> tm_mday)
    return true;

   return false;
}

bool isDST()
{
  // US Summer Timer calculation (Second Sunday in March to First Sunday November)
  int dayOfMonth = Time.day();
  int month = Time.month();
  int dayOfWeek = Time.weekday();

  if (month < 3 || month > 11)
    return false;

  if (month > 3 && month < 11)
    return true;

  int previousSunday = dayOfMonth - (dayOfWeek - 1);

  if (month == 3)
  {
    if (previousSunday < 8)
      return false;
    else
    {
      int secSinceMidnightUTC = Time.now() % 86400;
      int nonDSTLocalHour = (secSinceMidnightUTC / 3600) + TZ;

      // after 2am local time
      if (nonDSTLocalHour >= 2)
        return true;
      else
        return false;
    }
  }

  if (previousSunday > 0)
    return false;
  else
  {
    int secSinceMidnightUTC = Time.now() % 86400;
    int nonDSTLocalHour = (secSinceMidnightUTC / 3600) + TZ;

    // after 2am local time,
    if (nonDSTLocalHour >= 1)
      return false;
    else
      return true;
  }
}
