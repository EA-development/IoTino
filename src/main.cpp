#include <Arduino.h>
#include <Adafruit_VL53L0X.h>
#include <array>

#define LIDAR_AVERAGE 100
#define LIDAR_VALUEPERCENTAGE 0.5

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
std::array<uint16_t, LIDAR_AVERAGE> values;

void setup() {
  Serial.begin(115200);

  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }
  Wire.begin(18, 19);
  Serial.println("Adafruit VL53L0X test");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  // power 
  Serial.println(F("VL53L0X API Simple Ranging example\n\n")); 
}


void loop() {
  VL53L0X_RangingMeasurementData_t measure;
  short count = LIDAR_AVERAGE;
  
  Serial.print("Reading a measurements... ");
  
  for(int i = 0; i < LIDAR_AVERAGE; i++) {
    lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

    if (measure.RangeStatus != 4) {  // phase failures have incorrect data
      values[i] = measure.RangeMilliMeter;
    } else {
      count--;
      Serial.println(" out of range ");
    }
  }

  std::sort(std::begin(values), std::end(values));  //sorting
  short mid = floor(count/2);
  short range = mid*LIDAR_VALUEPERCENTAGE;
  short low = mid - range - count%2==0?1:0;
  short high = mid + range;
  double value = 0;
  for(int i = low; i <= high; i++)
    value+=values.at(i);
  Serial.print(values.at(mid));
  value/=high-low+1;
  Serial.print("Distance (mm): "); Serial.println(value);

}
