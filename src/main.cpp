#include <Arduino.h>
#include <Adafruit_VL53L0X.h>
#include <array>
#include <map>

//pins
#define LIDAR_SDA 18
#define LIDAR_SCL 19
#define IR_ANALOG 34

//settings
#define LIDAR false
#define IR true
//LIDAR
#define LIDAR_SAMPLES 1000
#define LIDAR_VALUE_CORRECTION -30
//IR
#define IR_SAMPLES 10000 //because it cans
#define IR_CYCLES 10
//IR mapping
#define A0 +9.42450609268053
#define A1 +1.76515427480637
#define A2 -5.03610159944910
#define A3 +6.88283703313403
#define A4 -5.02366498969399
#define A5 +2.08076080866994
#define A6 -4.87776360389973
#define A7 +5.99683334169558
#define A8 -2.97424332815192
#define E0 +0
#define E1 -1
#define E2 -4
#define E3 -7
#define E4 -10
#define E5 -13
#define E6 -17
#define E7 -21
#define E8 -25
#define GRADE 8
#define PRINT_PARTS false

//macros
#define DEBUG debug&&Serial
#define A(n) (n==0?A0:n==1?A1:n==2?A2:n==3?A3:n==4?A4:n==5?A5:n==6?A6:n==7?A7:n==8?A8:0)
#define E(n) (n==0?E0:n==1?E1:n==2?E2:n==3?E3:n==4?E4:n==5?E5:n==6?E6:n==7?E7:n==8?E8:0)
#define PART(n, x) (A(n)*pow(10, E(n))*pow(x, n))

//instances
Adafruit_VL53L0X lox = Adafruit_VL53L0X();
uint16_t values[LIDAR_SAMPLES];
std::map<short, short> amounts;

void lidarInit(bool force = false, bool debug = true) {
  if(DEBUG)
    Serial.println(F("Adafruit VL53L0X start init"));
  Wire.begin(LIDAR_SDA, LIDAR_SCL);
  if (!lox.begin()) {
    if(DEBUG) {
      Serial.println(F("Failed to boot VL53L0X"));
      Serial.print(F("retry: ")); Serial.println(F(force?"true":"false"));
    }
    while(force&&!lox.begin());
  }
  if(DEBUG) {
    Serial.print(F("Adafruit VL53L0X init ")); Serial.println(F(lox.Status!=-5?"successfull":"failed"));
  }
}
   

double getLidarDistance(bool debug = true) { //Average
  VL53L0X_RangingMeasurementData_t measure;
  short count = LIDAR_SAMPLES;
  
  //get data
  for(int i = 0; i < LIDAR_SAMPLES; i++) {
    lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!
    if (measure.RangeStatus != 4) {  // phase failures have incorrect data
      values[i] = measure.RangeMilliMeter;
    } else {
      count--;
      if(DEBUG)
        Serial.println(F("WARNING: lidar out of range"));
    }
  }

  //average
  double value = 0;
  for(int i = 0; i < count; i++)
    value+=values[i];
  value/=count;

  //correct value
  value+=LIDAR_VALUE_CORRECTION;

  return value;
}

void irInit() {
  pinMode(IR_ANALOG, INPUT);
}

short getIrValue() { //modal wert
  amounts.clear();
  for(int i = 0; i < IR_SAMPLES; i++)
    amounts[analogRead(IR_ANALOG)]++;
  short value = 0;
  for(auto const& x : amounts)
    if(x.second > amounts[value])
      value = x.first;
  return value;
}

short getIrDistanceRaw() {
  amounts.clear();
  for(int i = 0; i < IR_CYCLES; i++)
    amounts[getIrValue()]++;
  short value = 0;
  for(auto const& x : amounts)
    if(x.second > amounts[value])
      value = x.first;
  return value;
}

short getIrDistance(bool debug = false) {
  int raw = getIrDistanceRaw();
  if(DEBUG) {
    Serial.print(raw);
    Serial.print("\t");
  }
  double value = 0;
  for(int i = 0; i <= GRADE; i++) {
    double part = PART(i, raw);
    if(DEBUG && PRINT_PARTS) {
      Serial.print(part);
      Serial.print("\t");
    }
    value+=part;
  }
  if(DEBUG)
    Serial.println(value);
  return value;
}

void setup() {
  Serial.begin(115200);
  // wait until serial port opens for native USB devices
  while (!Serial)
    delay(1);
  Serial.println();
  if(LIDAR) lidarInit();
  if(IR) irInit();
}

void loop() {
  Serial.print(millis());
  Serial.print("\t");
  if(LIDAR) Serial.println(getLidarDistance());
  if(IR) getIrDistance(true);
}
