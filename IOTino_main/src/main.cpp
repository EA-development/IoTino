#include <Settings.h>
#include <Adafruit_VL53L0X.h>

//TODO disconnect recognition

//macros
#define DEBUG (debug&&Serial&&DEBUG_GENERAL)
#define DEBUG_CONST (Serial&&DEBUG_GENERAL)
#define A(n) (n==0?A0:n==1?A1:n==2?A2:n==3?A3:n==4?A4:n==5?A5:n==6?A6:n==7?A7:n==8?A8:0)
#define E(n) (n==0?E0:n==1?E1:n==2?E2:n==3?E3:n==4?E4:n==5?E5:n==6?E6:n==7?E7:n==8?E8:0)
#define PART(n, x) (A(n)*pow(10, E(n))*pow(x, n))

//instances
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

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

double getLidarDistance(bool debug = true) {
  if(DEBUG)
    Serial.print("Lidar: \t");
  double value = centralMean([](){VL53L0X_RangingMeasurementData_t measure;lox.rangingTest(&measure, false);if(measure.RangeStatus==4)return-1;else return(int)measure.RangeMilliMeter;}, LIDAR_SAMPLES, LIDAR_AVERAGE_PERCENTAGE, -1, 5, DEBUG?"lidar out of range":"");
  value+=LIDAR_VALUE_CORRECTION;
  if(DEBUG) {
    Serial.print("distance: ");
    Serial.print(value);
    Serial.print("mm\t");
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

short getIrDistance(bool debug = false) {
  if(DEBUG)
    Serial.print(F("IR: \t"));
  int raw = getIrDistanceRaw();
  if(DEBUG) {
    Serial.print(F("raw: "));
    Serial.print(raw);
    Serial.print(F("\t"));
  }
  double value = 0;
  for(int i = 0; i <= GRADE; i++) {
    double part = PART(i, raw);
    if(DEBUG && PRINT_PARTS) {
      Serial.print(F("part x^"));
      Serial.print(i);
      Serial.print(F(": "));
      Serial.print(part);
      Serial.print(F("\t"));
    }
    value+=part;
  }
  if(DEBUG)
    Serial.print(F("distance: "));
    Serial.print(value);
    Serial.print(F("mm\t"));
  return value;
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
  if(DEBUG_SETUP&&DEBUG_CONST) {
    Serial.println(F("Setup finished"));
    Serial.println();
  }
}

void loop() {
  if(DEBUG_TIMECODE)
    printTimeCode(TIMECODE_FORMAT);
  if(LIDAR) getLidarDistance(DEBUG_VALUES&&DEBUG_LIDAR);
  if(IR) getIrDistance(DEBUG_VALUES&&DEBUG_IR);
  if(DEBUG_CONST)
    Serial.println();
}
