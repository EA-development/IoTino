#include <Settings.h>
#include <Adafruit_VL53L0X.h>
#include <EEPROM.h>

//TODO disconnect recognition

//macros
#define DEBUG (debug&&Serial&&DEBUG_GENERAL)
#define DEBUG_CONST (Serial&&DEBUG_GENERAL)
#define A(n) (n==0?A0:n==1?A1:n==2?A2:n==3?A3:n==4?A4:n==5?A5:n==6?A6:n==7?A7:n==8?A8:0)
#define E(n) (n==0?E0:n==1?E1:n==2?E2:n==3?E3:n==4?E4:n==5?E5:n==6?E6:n==7?E7:n==8?E8:0)
#define PART(n, x) (A(n)*pow(10, E(n))*pow(x, n))

//instances
Adafruit_VL53L0X lox = Adafruit_VL53L0X();


double map(double value, bool ir, bool percent) {
  double full = EEPROM.readDouble(FULL+(ir?VALUE_SIZE:0))*MAP_PRECISION;
  double low = EEPROM.readDouble(LOW_+(ir?VALUE_SIZE:0))*MAP_PRECISION;
  double empty = EEPROM.readDouble(EMPTY+(ir?VALUE_SIZE:0))*MAP_PRECISION;
  double min = 0*MAP_PRECISION;
  double max = PAPER_SIZE*MAP_PRECISION;
  value *= MAP_PRECISION;
  /*if(value > empty)
    return -1;*/
  double mapped = (map(value, low, full, min, max))/(double)MAP_PRECISION;
  if(percent)
    mapped = mapped*100/(double)PAPER_SIZE;
  if(percent)
    if(mapped > 100)
      mapped = 100;
  return mapped;
}

void lidarInit(bool debug = true) {
  if(DEBUG) {
    Serial.println();
    Serial.println(F("Lidar init started"));
    Serial.println(F("Adafruit VL53L0X start init"));
  }
  if(!Wire.begin(LIDAR_SDA, LIDAR_SCL)) {
    if(DEBUG) {
      Serial.println(F("SPI Connection failed"));
      Serial.print(F("retry: ")); Serial.println(F(LIDAR_FORCE_INIT?"true":"false"));
    }
    while(LIDAR_FORCE_INIT&&!Wire.begin(LIDAR_SDA, LIDAR_SCL));
  }
  if (!lox.begin()) {
    if(DEBUG) {
      Serial.println(F("Failed to boot VL53L0X"));
      Serial.print(F("retry: ")); Serial.println(F(LIDAR_FORCE_INIT?"true":"false"));
    }
    while(LIDAR_FORCE_INIT&&!lox.begin());
  }
  if(DEBUG) {
    Serial.print(F("Adafruit VL53L0X init ")); Serial.println(F(lox.Status!=-5?"successfull":"failed"));
    Serial.println(F("Lidar init finished"));
    Serial.println();
  }
}

double getLidarDistance(bool debug = false, bool mapValue = true) {
  if(DEBUG)
    Serial.print("Lidar: \t");
  double value = centralMean([](){VL53L0X_RangingMeasurementData_t measure;lox.rangingTest(&measure, false);if(measure.RangeStatus==4)return-1;else return(int)measure.RangeMilliMeter;}, LIDAR_SAMPLES, LIDAR_AVERAGE_PERCENTAGE, -1, 5, DEBUG?"lidar out of range":"");
  value+=LIDAR_VALUE_CORRECTION;
  if(DEBUG) {
    Serial.print("distance: ");
    Serial.print(value);
    Serial.print("mm\t");
  }
  if(!mapValue)
    return value;
  double percent = map(value, false, true);
  value = map(value, false, false);
  if(DEBUG) {
    Serial.print(F("mapped distance: "));
    Serial.print(value);
    Serial.print(F("mm\t"));
    Serial.print(F("fill level: "));
    Serial.print(percent);
    Serial.print(F("%\t"));
  }
  return value;
}

void irInit(bool debug = true) {
  if(DEBUG)
    Serial.println(F("IR init started"));
  pinMode(IR_ANALOG, INPUT);
  if(DEBUG)
    Serial.println(F("IR init finished"));
}

short getIrDistanceRaw() {
  return median([](){return modal([](){return analogRead(IR_ANALOG);}, IR_SAMPLES);}, IR_CYCLES);
}

short getIrDistance(bool debug = false, bool mapValue = true) {
  if(DEBUG)
    Serial.print(F("IR: \t"));
  int raw = getIrDistanceRaw();
  if(DEBUG) {
    Serial.print(F("raw: "));
    Serial.print(raw);
    Serial.print(F("\t"));
  }
  double value_ = 0;
  for(int i = 0; i <= GRADE; i++) {
    double part = PART(i, raw);
    if(DEBUG && PRINT_PARTS) {
      Serial.print(F("part x^"));
      Serial.print(i);
      Serial.print(F(": "));
      Serial.print(part);
      Serial.print(F("\t"));
    }
    value_+=part;
  }
  if(DEBUG) {
    Serial.print(F("distance: "));
    Serial.print(value_);
    Serial.print(F("mm\t"));
  }
  if(!mapValue)
    return value_;
  double percent = map(value_, true, true);
  value_ = map(value_, true, false);
  if(DEBUG) {
    Serial.print(F("mapped distance: "));
    Serial.print(value_);
    Serial.print(F("mm\t"));
    Serial.print(F("fill level: "));
    Serial.print(percent);
    Serial.print(F("%\t"));
  }
  return value_;
}

void printLines() {
  for(int i = 0; i < 10; i++)
    Serial.println();
}

byte waitEnter() {
  while(Serial.available())
    Serial.read();
  Serial.println(F("Press [enter] to continue"));
  Serial.println(F("Press [s] to skip calibration step"));
  Serial.println(F("Press [e] to end calibration"));
  while(!Serial.available()&&Serial);
  printLines();
  String response = Serial.readString();
  if(response == "s")
    return 1;
  if(response == "e")
    return 2;
  return 0;
}

void printCalibrationValue(String fillLevel, String sensor) {
  Serial.print(fillLevel+F(": "));
  Serial.print(EEPROM.readDouble((fillLevel=="full"?FULL:fillLevel=="low"?LOW_:EMPTY)+(sensor=="Ir"?VALUE_SIZE:0))); Serial.print("mm\t");
}

void printCalibrationValues(String sensor) {
  Serial.print(sensor+F(":\t"));
  printCalibrationValue("full", sensor);
  printCalibrationValue("low", sensor);
  printCalibrationValue("empty", sensor);
  Serial.println();
}

void printCalibrationValues() {
  Serial.println(F("current calibration values:"));
  printCalibrationValues("Lidar");
  printCalibrationValues("Ir");
}

bool printCalibrateState(String sensor, float complete) {
  Serial.print(sensor+F(" calibration: "));
  Serial.println(complete<0?"waiting":complete==0?"started":complete>=1?"finished":String((int)round((complete*100)))+"% complete");
  if(complete >= 1)
    return true;
  return false;
}

void printCalibrateText(String fillLevel, String instruction, float complete) {
  printLines();
  Serial.println(fillLevel+F(" calibration started"));
  Serial.println(F("this may take around 7 minutes to complete"));
  Serial.println(F("Please do not disturb the sensors while calibrating"));
  printCalibrateState("Lidar", complete);
  if(!printCalibrateState("Ir   ", complete-1))
    return;
  printLines();
  Serial.println(fillLevel+F(" calibration ended"));
 }

bool calibrateCycle(String fillLevel, String instruction) {
  printCalibrationValues();
  Serial.println();
  Serial.print(F("Put ")); Serial.println(instruction+F(" infront of the sensors"));
  byte response = waitEnter();
  if(response == 1) {
    printLines();
    Serial.println(fillLevel+F(" calibration skiped"));
    return false;
  }
  if(response == 2) {
    printLines();
    return true;
  }
  int v = 0;
  int* i = &v;
  double lidarValue;
  double irValue;
  if(!LIDAR)
    v = 10;
  if(LIDAR) lidarValue = /*getLidarDistance(false, false);*/centralAverage([i, fillLevel, instruction](){printCalibrateText(fillLevel, instruction, *i/10.); (*i)++;; return getLidarDistance(false, false);}, LIDAR_CALIBRATION_SAMPLES, 80);
  if(IR) irValue = /*getIrDistance(false, false);*/modal([i, fillLevel, instruction](){printCalibrateText(fillLevel, instruction, *i/10.); (*i)++; return getIrDistance(false, false);}, IR_CALIBRATION_SAMPLES);
  printCalibrateText(fillLevel, fillLevel, *i/10.);
  int address = fillLevel=="full"?FULL:fillLevel=="low"?LOW_:EMPTY;
  if(LIDAR) EEPROM.writeDouble(address, lidarValue);
  if(IR) EEPROM.writeDouble(address+VALUE_SIZE, irValue);
  EEPROM.commit();
  return false;
}

void calibrate() {
  if(!Serial)
    return;
  printLines();
  Serial.println(F("Calibration Menu"));
  printCalibrationValues();
  Serial.println(F("enter [c] to calibrate"));
  Serial.println(F("Press [enter] to continue"));
  while(!Serial.available());
  printLines();
  if(Serial.readString() != "c")
    return;
  if(calibrateCycle("full", "a full roll of toilett paper")) return;
  if(calibrateCycle("low", "a roll of toilett paper with one layer of paper left")) return;
  if(calibrateCycle("empty", "an empty roll of toilett paper")) return;
  EEPROM.commit();
  printLines();
  Serial.println(F("Calibration finished"));
  printCalibrationValues();
  waitEnter();
}

void setup() {
  Serial.begin(115200);
  // wait until serial port opens for native USB devices
  while (!Serial && FORCE_SERIAL)
    delay(1);
  if(DEBUG_SETUP&&DEBUG_CONST) {
    Serial.println();
    Serial.println(F("Setup started"));
  }
  if(LIDAR) lidarInit(DEBUG_SETUP&&DEBUG_LIDAR);
  if(IR) irInit(DEBUG_SETUP&&DEBUG_IR);
  Serial.print(F("EEPROM init\tEEPROM size: ")); Serial.println(EEPROM_SIZE);
  EEPROM.begin(EEPROM_SIZE);
  if(DEBUG_SETUP&&DEBUG_CONST) {
    Serial.println(F("Setup finished"));
    Serial.println();
  }
  calibrate();
}

void loop() {
  if(DEBUG_TIMECODE)
    printTimeCode(TIMECODE_FORMAT);
  if(LIDAR) getLidarDistance(DEBUG_VALUES&&DEBUG_LIDAR);
  if(IR) getIrDistance(DEBUG_VALUES&&DEBUG_IR);
  if(DEBUG_CONST)
    Serial.println();
}
