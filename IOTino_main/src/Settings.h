#include <Util.h>

//pins
#define LIDAR_SDA 18
#define LIDAR_SCL 19
#define IR_ANALOG 34

//general
#define FORCE_SERIAL true
#define TIMECODE_FORMAT TIMECODE_FORMAT_SEPERATE
#define MAP_PRECISION 1000
#define PAPER_SIZE 80

#define MIN_FILL_LEVEL 50
#define RESEND_TIME  2*60*60*1000

//debug
#define DEBUG_GENERAL true
#define DEBUG_SETUP true
#define DEBUG_TIMECODE true
#define DEBUG_VALUES true
#define DEBUG_ERROR true
#define DEBUG_LIDAR true
#define DEBUG_IR true

//activation settings
#define LIDAR true
#define IR true

//LIDAR
#define LIDAR_FORCE_INIT true
#define LIDAR_SAMPLES 1000
#define LIDAR_AVERAGE_PERCENTAGE 77
#define LIDAR_VALUE_CORRECTION -30
#define LIDAR_CALIBRATION_SAMPLES 1

//IR
#define IR_SAMPLES 1000
#define IR_CYCLES 50
#define IR_CALIBRATION_SAMPLES 10

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

//EEPROM
#define EEPROM_SIZE (FILL_LEVELS*SENSORS*VALUE_SIZE)
#define FILL_LEVELS 3
#define SENSORS 2
#define VALUE_SIZE sizeof(double)
#define FULL 0
#define LOW_ FULL+SENSORS*VALUE_SIZE
#define EMPTY LOW_+SENSORS*VALUE_SIZE
