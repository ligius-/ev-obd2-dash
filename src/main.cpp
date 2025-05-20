#include <Arduino.h>
#include <BluetoothSerial.h>
#include <ELMduino.h>
#include <esp32_smartdisplay.h>
#include <esp_task_wdt.h>
#include <lvgl.h>

#include "../include/lv_conf.h"
#include "ui/ui.h"
#include "ui/vars.h"

#define DEBUG_PORT Serial

#define DEMO_MODE 0

// Bluetooth Serial
BluetoothSerial SerialBT;
ELM327 myELM327;

enum PriorityLevel {
  PRIORITY_HIGH = 0,
  PRIORITY_MEDIUM = 1,
  PRIORITY_LOW = 2
};

// Update rates for each priority level (in ms)
#define RATE_HIGH 50
#define RATE_MEDIUM 1500
#define RATE_LOW 5000

// Define the group within each priority level
typedef struct {
  uint8_t group;        // Group ID within this priority level
  uint8_t totalGroups;  // Total number of groups at this priority level
} PriorityGroup;

// PID calculation method types
enum CalcMethod {
  UNSIGNED_MULT,    // buildUnsignedPayloadFromChars * multiplier
  SIGNED_MULT16,    // buildSigned16Payload * multiplier
  UNSIGNED_OFFSET,  // buildSignedPayloadFromChars + offset
  POWER_CALC,       // Special calculation for power (kW) = value1 * value2 / 1000
  MOTOR_POWER,      // Motor power (kW) = (RPM * Torque) / 9.549
  HVB_TEMP_MODE,    // Special case for battery temp mode
};

// PID information structure
typedef struct {
  const char *name;      // Human-readable name
  const char *pid;       // Full PID with 22 prefix
  CalcMethod method;     // Calculation method
  float param1;          // First parameter (multiplier or offset)
  float param2;          // Second parameter if needed
  uint8_t startOffset;   // Start position in payload
  uint8_t numChars;      // Number of chars to read
  PriorityLevel level;   // Priority level (HIGH, MEDIUM, LOW)
  PriorityGroup group;   // Group information within the priority level
  uint32_t lastUpdate;   // Last update timestamp
  float value;           // Current value
  bool valid;            // Is the value valid?
  lv_obj_t *displayObj;  // Primary LVGL object to display this value
  int dependsOn[2];      // Index of PIDs this one depends on (-1 if none)
  char strValue[16];     // String prefix for display
  char suffix[8];        // String suffix for display
} PID_t;

// The order matters for dependencies (dependent PIDs should come after their dependencies)
PID_t pids[] = {
    // Basic readings (independent values)
    // HIGH PRIORITY GROUP - HVB Power components (1 group)
    {"HVB Voltage", "22480D", UNSIGNED_MULT, 0.01, 0, 6, 4, PRIORITY_HIGH, {0, 1}, 0, 0, false, NULL, {-1, -1}, "", "V"},
    {"HVB Current", "22480B", SIGNED_MULT16, 0.02, 0, 6, 4, PRIORITY_HIGH, {0, 1}, 0, 0, false, NULL, {-1, -1}, "", "A"},

    // MEDIUM PRIORITY GROUP - Motor values (2 groups)
    {"Pri Mo RPM", "221E2C", UNSIGNED_MULT, 1, 0, 6, 4, PRIORITY_MEDIUM, {0, 2}, 0, 0, false, NULL, {-1, -1}, "", "Rpm"},
    {"Pri Mo Torque", "22481C", SIGNED_MULT16, 0.1, -200, 6, 4, PRIORITY_MEDIUM, {0, 2}, 0, 0, false, NULL, {-1, -1}, "", "Nm"},
    {"Sec Mo RPM", "224821", UNSIGNED_MULT, 1, 0, 6, 4, PRIORITY_MEDIUM, {1, 2}, 0, 0, false, NULL, {-1, -1}, "", "Rpm"},
    {"Sec Mo Torque", "22481A", SIGNED_MULT16, 0.1, -200, 6, 4, PRIORITY_MEDIUM, {1, 2}, 0, 0, false, NULL, {-1, -1}, "", "Nm"},

    // LOW PRIORITY GROUP - Various measurements (4 groups)
    {"HVB Temp", "224800", UNSIGNED_OFFSET, -50, 0, 6, 2, PRIORITY_LOW, {0, 4}, 0, 0, false, NULL, {-1, -1}, "", "°C"},
    {"SOC", "224801", UNSIGNED_MULT, 0.002, 0, 6, 4, PRIORITY_LOW, {1, 4}, 0, 0, false, NULL, {-1, -1}, "", " %"},
    {"Int Temp", "22DD04", UNSIGNED_OFFSET, -40, 0, 6, 2, PRIORITY_LOW, {2, 4}, 0, 0, false, NULL, {-1, -1}, "", "°C"},
    {"HVB Mode", "2248E0", HVB_TEMP_MODE, 0, 0, 6, 2, PRIORITY_LOW, {3, 4}, 0, 0, false, NULL, {-1, -1}, "", ""},

    // Calculated values (dependent on other PIDs)
    {"HVB Power", "", POWER_CALC, 0.001, 0, 0, 0, PRIORITY_HIGH, {0, 1}, 0, 0, false, NULL, {0, 1}, "", ""},
    {"Pri Mo Power", "", MOTOR_POWER, 0, 0, 0, 0, PRIORITY_MEDIUM, {0, 2}, 0, 0, false, NULL, {2, 3}, "", " kW"},
    {"Sec Mo Power", "", MOTOR_POWER, 0, 0, 0, 0, PRIORITY_MEDIUM, {1, 2}, 0, 0, false, NULL, {4, 5}, "", " kW"}};

// Track the current group being processed for each priority level
typedef struct {
  uint8_t currentGroup;
  uint32_t nextUpdateTime;
  uint32_t updateRate;
} PriorityState;

PriorityState priorityStates[3];

// Forward declarations
uint8_t ctoi(uint8_t value);
uint64_t buildUnsignedPayloadFromChars(uint8_t startOffset, uint8_t numPayChars);
int16_t buildSigned16Payload(uint8_t startOffset);
void updatePID(int index);
void createUI();
void updateDisplayForPID(int index);
void fillBatteryMode(uint8_t mode, char buffer[32]);
void displayConnectionError();

const int NUM_PIDS = sizeof(pids) / sizeof(PID_t);

void my_log_cb(const char *buf) {
  Serial.print(buf);
  Serial.flush();
}

void setup() {
  // Initialize priority states
  priorityStates[PRIORITY_HIGH].currentGroup = 0;
  priorityStates[PRIORITY_HIGH].nextUpdateTime = 0;
  priorityStates[PRIORITY_HIGH].updateRate = RATE_HIGH;

  priorityStates[PRIORITY_MEDIUM].currentGroup = 0;
  priorityStates[PRIORITY_MEDIUM].nextUpdateTime = 0;
  priorityStates[PRIORITY_MEDIUM].updateRate = RATE_MEDIUM;

  priorityStates[PRIORITY_LOW].currentGroup = 0;
  priorityStates[PRIORITY_LOW].nextUpdateTime = 0;
  priorityStates[PRIORITY_LOW].updateRate = RATE_LOW;

  Serial.begin(115200);
  Serial.println("ESP32 EV Data Monitor with LVGL");

  lv_log_register_print_cb(my_log_cb);

  // Initialize display

  smartdisplay_init();

  auto display = lv_disp_get_default();
  lv_disp_set_rotation(display, LV_DISP_ROT_270);

  lv_obj_t *label = lv_label_create(lv_scr_act());
  lv_label_set_text(label, "Initializing bluetooth...");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

  lv_timer_handler();

  // Initialize Bluetooth
  if (!DEMO_MODE) {
    // SerialBT.enableSSP();
    //SerialBT.respondPasskey(1234);
    SerialBT.begin("ESP32_EV_Monitor", true, true);
  }

  Serial.println("Bluetooth initialized, connecting to ELM327...");
  lv_label_set_text(label, "Bluetooth initialized, connecting to ELM327...");
  lv_timer_handler();

  if (!DEMO_MODE) {
    // SerialBT.setPin("1234");
    if (!SerialBT.connect("OBDII")) {
      DEBUG_PORT.println("Couldn't connect to OBD scanner - Phase 1");
      lv_label_set_text(label, "Couldn't connect to OBD scanner - Phase 1");
      lv_timer_handler();
      while (1);
    }

    // Connect to ELM327 adapter
    if (!myELM327.begin(SerialBT, false, 1000)) {
      Serial.println("Couldn't connect to OBD scanner - phase 2");
      lv_label_set_text(label, "Couldn't connect to OBD scanner - Phase 2");
      lv_timer_handler();
      while (1);
    }
  }

  Serial.println("Connected to ELM327");
  lv_label_set_text(label, "Connected to ELM327");
  lv_timer_handler();

  delay(100);

  ui_init();
}

// Process all PIDs in a specific priority group
void processPriorityGroup(PriorityLevel level, uint8_t groupId, unsigned long currentTime) {
  for (int i = 0; i < NUM_PIDS; i++) {
    // Only process PIDs that match the priority level and group, and are not calculated
    if (pids[i].level == level &&
        pids[i].group.group == groupId &&
        pids[i].dependsOn[0] == -1) {
      // Serial.printf("%8d group %d level %d pid %d\n", currentTime, groupId, level, i);
      updatePID(i);
    }
  }
}

// Process all calculated values
void processCalculatedValues(unsigned long currentTime) {
  for (int i = 0; i < NUM_PIDS; i++) {
    if (pids[i].dependsOn[0] != -1) {  // This is a calculated value
      // Check if dependencies are valid
      bool dependenciesValid = true;
      for (int j = 0; j < 2; j++) {
        if (pids[i].dependsOn[j] != -1 && !pids[pids[i].dependsOn[j]].valid) {
          dependenciesValid = false;
          break;
        }
      }

      // If dependencies valid, calculate
      if (dependenciesValid) {
        // Get the priority level and update rate
        PriorityLevel level = pids[i].level;
        uint32_t updateRate = priorityStates[level].updateRate;

        if (currentTime - pids[i].lastUpdate >= updateRate) {
          // Calculate the value based on the method
          if (pids[i].method == POWER_CALC) {
            // Power (kW) = voltage * current * 0.001
            pids[i].value = pids[pids[i].dependsOn[0]].value *
                            pids[pids[i].dependsOn[1]].value *
                            pids[i].param1;
          } else if (pids[i].method == MOTOR_POWER) {
            // Motor power (kW) = (RPM * Torque) / 9.549 / 1000
            pids[i].value = (pids[pids[i].dependsOn[0]].value *
                             pids[pids[i].dependsOn[1]].value) /
                            9.549 * 0.001;
          }

          pids[i].valid = true;
          pids[i].lastUpdate = currentTime;

          // Update the display for this value
          updateDisplayForPID(i);
        }
      }
    }
  }
}

int demoTick;
auto lv_last_tick = millis();

void loop() {
  unsigned long currentTime = millis();

  // Process each priority level
  for (int levelInt = PRIORITY_HIGH; levelInt <= PRIORITY_LOW; levelInt++) {
    PriorityLevel level = static_cast<PriorityLevel>(levelInt);

    // Check if it's time to process this priority level
    if (currentTime >= priorityStates[level].nextUpdateTime) {
      // Process all PIDs in the current group for this priority level
      processPriorityGroup(level, priorityStates[level].currentGroup, currentTime);

      // Move to the next group and schedule next update
      uint8_t totalGroups = 0;

      // Find the total number of groups for this priority level
      for (int i = 0; i < NUM_PIDS; i++) {
        if (pids[i].level == level && pids[i].dependsOn[0] == -1) {
          totalGroups = pids[i].group.totalGroups;
          break;
        }
      }

      // Advance to next group in rotation
      if (totalGroups > 0) {
        priorityStates[level].currentGroup = (priorityStates[level].currentGroup + 1) % totalGroups;
      }

      // Schedule next update based on priority level
      priorityStates[level].nextUpdateTime = currentTime + priorityStates[level].updateRate;
    }
  }

  // Process all calculated values
  processCalculatedValues(currentTime);

  // Debug print of power values every second for testing
  static unsigned long lastDebugPrint = 0;
  if (currentTime - lastDebugPrint >= 1000) {
    lastDebugPrint = currentTime;

    if (pids[10].valid && pids[11].valid && pids[12].valid) {
      Serial.printf("Power values - HVB: %f  kW, Pri: %f kW, Sec: %f kW\n", pids[10].value, pids[11].value, pids[12].value);
    }
  }

  if (currentTime % 20 == 0) {
    demoTick++;
    demoTick %= 250;
  }

  // Update the ticker
  // lv_tick_inc(currentTime - lv_last_tick);
  lv_last_tick = currentTime;

  // lv_task_handler();
  lv_timer_handler();
  ui_tick();
}

void updatePID(int index) {
  if (DEMO_MODE) {
    // DEMO MODE

    switch (index) {
      case 0:  // HvbV
        pids[index].value = 350;
        break;
      case 1:  // HVb C
        pids[index].value = abs((demoTick % 250) - 125) * -8 + 500;
        break;
      case 2:  // PriRpm
        pids[index].value = 1500;
        break;
      case 3:  // PriTq
        pids[index].value = ((demoTick % 250) - 150) * 5;
        break;
      case 4:  // SecRpm
        pids[index].value = 1500;
        break;
      case 5:  // SecTq
        pids[index].value = ((demoTick % 100) - 30) * 5;
        break;
      case 6:  // HvbT
        pids[index].value = 15 + (demoTick % 40) / 5;
        break;
      case 7:  // SoC
        pids[index].value = 90.3 - (demoTick % 32) / 10;
        break;
      case 8:  // IntT
        pids[index].value = 18 + (demoTick % 80) / 10;
        break;
      case 9:  // Hvb Mode
        pids[index].value = demoTick % 5;
        break;
      default:
        break;
    }

    // Serial.println(pids[index].pid);

    pids[index].valid = true;
    pids[index].lastUpdate = millis();

    updateDisplayForPID(index);

  } else {
    // NOT DEMO MODE

    // myELM327.timeout_ms = 200;
    myELM327.sendCommand_Blocking(pids[index].pid);
    if (myELM327.nb_rx_state != ELM_SUCCESS) {
      Serial.printf("Failed to send command: %s code %d", pids[index].pid, myELM327.get_response());
      delay(200);
      return;
    }

    // if (myELM327.nb_rx_state == ELM_SUCCESS) {
    // Calculate the value based on the method
    if (pids[index].method == UNSIGNED_MULT) {
      uint64_t raw = buildUnsignedPayloadFromChars(pids[index].startOffset, pids[index].numChars);
      pids[index].value = raw * pids[index].param1;
    } else if (pids[index].method == SIGNED_MULT16) {
      int16_t raw = buildSigned16Payload(pids[index].startOffset);
      if (raw == -32768) {
        // sometimes we get 0x8000 back, no idea why
        return;
      }
      pids[index].value = raw * pids[index].param1;
    } else if (pids[index].method == UNSIGNED_OFFSET) {
      int64_t raw = buildUnsignedPayloadFromChars(pids[index].startOffset, pids[index].numChars);
      pids[index].value = raw + pids[index].param1;
    } else if (pids[index].method == HVB_TEMP_MODE) {
      // Special case for battery temp mode
      uint64_t raw = buildUnsignedPayloadFromChars(pids[index].startOffset, pids[index].numChars);
      pids[index].value = (float)raw;  // Use raw value for mode
    }

    pids[index].valid = true;
    pids[index].lastUpdate = millis();

    // Update the display for this PID
    updateDisplayForPID(index);

    // Serial.print(pids[index].name);
    // Serial.print(": ");
    // Serial.println(pids[index].value);
    // } else {
    // myELM327.printError();
    // pids[index].valid = false;
    // }
  }
}

void updateDisplayForPID(int index) {
}

void fillBatteryMode(uint8_t mode, char buffer[32]) {
  // Value mapping: 0='Off':1='Eq':2='CFlow':3='CFan':4='CMod':5='CMax':15='Heat'
  switch (mode) {
    case 0:
      strcpy(buffer, "Off");
      break;
    case 1:
      strcpy(buffer, "Eq");
      break;
    case 2:
      strcpy(buffer, "Flow");
      break;
    case 3:
      strcpy(buffer, "Fan");
      break;
    case 4:
      strcpy(buffer, "Mod");
      break;
    case 5:
      strcpy(buffer, "Max");
      break;
    case 15:
      strcpy(buffer, "Heat");
      break;
    default:
      strcpy(buffer, "?");
      break;
  }
}

// Helper functions
uint8_t ctoi(uint8_t value) {
  if (value >= 'A')
    return value - 'A' + 10;
  else
    return value - '0';
}

uint64_t buildUnsignedPayloadFromChars(uint8_t startOffset, uint8_t numPayChars) {
  uint64_t response = 0;
  for (uint8_t i = 0; i < numPayChars; i++) {
    uint8_t payloadIndex = startOffset + i;
    uint8_t bitsOffset = 4 * (numPayChars - i - 1);
    response = response | ((uint64_t)ctoi(myELM327.payload[payloadIndex]) << bitsOffset);
  }
  return response;
}

int16_t buildSigned16Payload(uint8_t startOffset) {
  uint8_t highNibble1 = ctoi(myELM327.payload[startOffset]);
  uint8_t lowNibble1 = ctoi(myELM327.payload[startOffset + 1]);
  uint8_t highNibble2 = ctoi(myELM327.payload[startOffset + 2]);
  uint8_t lowNibble2 = ctoi(myELM327.payload[startOffset + 3]);

  uint8_t byte1 = (highNibble1 << 4) | lowNibble1;
  uint8_t byte2 = (highNibble2 << 4) | lowNibble2;

  uint16_t combined = (byte1 << 8) | byte2;

  // Serial.print("Received: ");
  // for (uint8_t i = 0; i < 4; i++) {
  //   Serial.print(myELM327.payload[startOffset + i]);
  // }
  // Serial.print(" decoded: ");
  // Serial.println((int16_t)combined);

  return (int16_t)combined;
}

char *buildTextForPid(char *buffer, uint8_t pid, uint8_t precision = 0) {
  sprintf(buffer, "%s%.*f%s", pids[pid].strValue, precision, pids[pid].value, pids[pid].suffix);
  return buffer;
}

float get_var_pri_motor_power() {
  return pids[11].value;
}
void set_var_pri_motor_power(float value) {}

char pri_motor_power_str[30] = {0};
const char *get_var_pri_motor_power_str() {
  return buildTextForPid(pri_motor_power_str, 11);
}
void set_var_pri_motor_power_str(const char *value) {}

float get_var_sec_motor_power() {
  return pids[12].value;
}
void set_var_sec_motor_power(float value) {}

char sec_motor_power_str[30] = {0};
const char *get_var_sec_motor_power_str() {
  return buildTextForPid(sec_motor_power_str, 12);
}
void set_var_sec_motor_power_str(const char *value) {}

float get_var_power() {
  return pids[10].value;
}
void set_var_power(float value) {}

char power_str[30] = {0};
const char *get_var_power_str() {
  return buildTextForPid(power_str, 10, 1);
}
void set_var_power_str(const char *value) {}

char int_temp_str[30] = {0};
const char *get_var_int_temp() {
  return buildTextForPid(int_temp_str, 8);
}
void set_var_int_temp(const char *value) {}

char bat_temp_str[30] = {0};
const char *get_var_bat_temp() {
  return buildTextForPid(bat_temp_str, 6);
}
void set_var_bat_temp(const char *value) {}

char soc_str[30] = {0};
const char *get_var_soc() {
  return buildTextForPid(soc_str, 7);
}
void set_var_soc(const char *value) {}

char bat_mode_str[30] = {0};
const char *get_var_bat_mode() {
  fillBatteryMode(pids[9].value, bat_mode_str);
  return bat_mode_str;
}
void set_var_bat_mode(const char *value) {}

// lv_anim_t pri_motor_meter_anim;
// static void setValueIndicator(void * indic, int32_t v)
// {
//     lv_meter_set_indicator_value((lv_meter_t *)objects.pri_motor_meter, indic, v);
// }

// void tick_screen_page1() {
//     {
//         lv_meter_indicator_t *indicator;

//         lv_ll_t *indicators = &((lv_meter_t *)objects.pri_motor_meter)->indicator_ll;
//         int index = 0;
//         for (indicator = _lv_ll_get_tail(indicators); index > 0 && indicator != NULL; indicator = _lv_ll_get_prev(indicators, indicator), index--);

//         if (indicator) {
//             int32_t new_val = get_var_pri_motor_power();
//             int32_t cur_val = indicator->start_value;
//             if (new_val != cur_val) {

//                 lv_anim_init(&pri_motor_meter_anim);
//                 lv_anim_set_exec_cb(&pri_motor_meter_anim, setValueIndicator);
//                 lv_anim_set_var(&pri_motor_meter_anim, indicator);
//                 // lv_anim_set_time(&pri_motor_meter_anim, 500);
//                 tick_value_change_obj = objects.pri_motor_meter;
//                 //lv_meter_set_indicator_value(objects.pri_motor_meter, indicator, new_val);
//                 lv_anim_set_values(&pri_motor_meter_anim, cur_val, new_val);
//                 lv_anim_start(&pri_motor_meter_anim);
//                 tick_value_change_obj = NULL;
//             }
//         }
//     }