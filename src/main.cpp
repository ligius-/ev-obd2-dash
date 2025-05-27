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

// uncomment below if you don't want to connect to BT and just want to simulate numbers
#define DEMO_MODE 0
// uncomment below if you want to connect directly to MAC aa:bb:cc:11:22:33 otherwise it will connect to "OBDII"
#define BT_QUICK_CONNECT
// uncomment below if you want a lot of log output
// #define DEBUG

// at which voltage read from CDS (pin 34) the theme switches to dark (higher values = darker)
#define CDS_BRIGHT_MILLIVOLTS (150)

// Bluetooth Serial
BluetoothSerial SerialBT;
ELM327 myELM327;

enum PriorityLevel {
  PRIORITY_HIGH = 0,
  PRIORITY_MEDIUM_HIGH = 1,
  PRIORITY_MEDIUM_LOW = 2,
  PRIORITY_LOW = 3
};

// Update rates for each priority level (in ms)
#define RATE_HIGH 50
#define RATE_MEDIUM_HIGH 150
#define RATE_MEDIUM_LOW 1000
#define RATE_LOW 5000

// Define the group within each priority level
typedef struct {
  uint8_t group;        // Group ID within this priority level
  uint8_t totalGroups;  // Total number of groups at this priority level
} PriorityGroup;

// PID calculation method types
enum CalcMethod {
  UNSIGNED_MULT_OFFSET,  // buildUnsignedPayloadFromChars * multiplier(param1) + offset (param2)
  SIGNED_MULT16,         // buildSigned16Payload * multiplier
  MULTIPLIER_CALC,       // Special calculation for power (kW) = value1 * value2 / 1000;
  DIVIDER_CALC,          // First parameter is the multiplier for everything
  MOTOR_POWER_CALC,      // Motor power (kW) = (RPM * Torque) / 9.549
  HVB_TEMP_MODE_CALC,    // Special case for battery temp mode
};

#define PID_LIST                                                                                                                            \
  /* Basic readings (independent values) */                                                                                                 \
  /* HIGH PRIORITY GROUP - HVB Power components */                                                                                          \
  X(HVB_VOLTAGE, "7e4", "22480D", UNSIGNED_MULT_OFFSET, 0.01, 0, 6, 4, PRIORITY_HIGH, 0, 1, PID_NONE, PID_NONE, "", "V")                    \
  X(HVB_CURRENT, "7e4", "2248F9", SIGNED_MULT16, 0.1, 0, 6, 4, PRIORITY_HIGH, 0, 1, PID_NONE, PID_NONE, "", "A")                            \
                                                                                                                                            \
  /* MEDIUM HIGH PRIORITY GROUP - Motor values */                                                                                           \
  /*X(PRI_MO_RPM, "7e6",  "221E2C", UNSIGNED_MULT_OFFSET, 1, 0, 6, 4, PRIORITY_MEDIUM_HIGH, 0, 2, PID_NONE, PID_NONE, "", "Rpm")     */     \
  X(PRI_MO_TORQUE, "7e6", "22481C", SIGNED_MULT16, 0.1, -200, 6, 4, PRIORITY_MEDIUM_HIGH, 0, 2, PID_NONE, PID_NONE, "", " Nm")              \
  /*X(SEC_MO_RPM, "7e7", "224821", UNSIGNED_MULT_OFFSET, 1, 0, 6, 4, PRIORITY_MEDIUM_HIGH, 1, 2, PID_NONE, PID_NONE, "", "Rpm")      */     \
  X(SEC_MO_TORQUE, "7e7", "22481A", SIGNED_MULT16, 0.1, -200, 6, 4, PRIORITY_MEDIUM_HIGH, 1, 2, PID_NONE, PID_NONE, "", " Nm")              \
                                                                                                                                            \
  /* MEDIUM LOW PRIORITY GROUP */                                                                                                           \
  X(VEHICLE_SPEED_HR, "7e0", "221505", UNSIGNED_MULT_OFFSET, 0.0078125, 0, 6, 4, PRIORITY_MEDIUM_LOW, 0, 1, PID_NONE, PID_NONE, "", "km/h") \
                                                                                                                                            \
  /* LOW PRIORITY GROUP - Various measurements */                                                                                           \
  X(HVB_TEMP, "7e4", "224800", UNSIGNED_MULT_OFFSET, 1, -50, 6, 2, PRIORITY_LOW, 0, 10, PID_NONE, PID_NONE, "", "°C")                       \
  X(SOC, "7e4", "224801", UNSIGNED_MULT_OFFSET, 0.002, 0, 6, 4, PRIORITY_LOW, 1, 10, PID_NONE, PID_NONE, "", " %")                          \
  X(INT_TEMP, "7e2", "22DD04", UNSIGNED_MULT_OFFSET, 1, -40, 6, 2, PRIORITY_LOW, 2, 10, PID_NONE, PID_NONE, "", "°C")                       \
  X(HVB_MODE, "7e6", "2248E0", HVB_TEMP_MODE_CALC, 0, 0, 6, 2, PRIORITY_LOW, 3, 10, PID_NONE, PID_NONE, "", "")                             \
  X(LVB_SOC, "726", "224028", UNSIGNED_MULT_OFFSET, 1, 0, 6, 2, PRIORITY_LOW, 4, 10, PID_NONE, PID_NONE, "", "%")                           \
  X(LVB_VOLTAGE, "726", "22402a", UNSIGNED_MULT_OFFSET, 0.05, 6, 6, 2, PRIORITY_LOW, 5, 10, PID_NONE, PID_NONE, "", "V")                    \
  /*X(LVB_CURRENT, "726", "22402b", UNSIGNED_MULT_OFFSET, 1, -127, 6, 2, PRIORITY_LOW, 6, 10, PID_NONE, PID_NONE, "", "A")      */          \
  /*X(COOL_HEAT_POWER, "7e6", "2248de", UNSIGNED_MULT_OFFSET, 0.001, 0, 6, 4, PRIORITY_LOW, 7, 10, PID_NONE, PID_NONE, "", "kW")*/          \
  X(PRI_MO_TEMP, "7e6", "22481f", SIGNED_MULT16, 1, 0, 6, 4, PRIORITY_LOW, 8, 10, PID_NONE, PID_NONE, "", "°C")                             \
  X(SEC_MO_TEMP, "7e7", "224820", SIGNED_MULT16, 1, 0, 6, 4, PRIORITY_LOW, 9, 10, PID_NONE, PID_NONE, "", "°C")                             \
                                                                                                                                            \
  /* Calculated values (dependent on other PIDs) */                                                                                         \
  X(HVB_POWER, "", "", MULTIPLIER_CALC, 0.001, 0, 0, 0, PRIORITY_HIGH, 0, 1, HVB_VOLTAGE, HVB_CURRENT, "", "kW")                            \
  /*X(PRI_MO_POWER, "", "", MOTOR_POWER_CALC, 0, 0, 0, 0, PRIORITY_MEDIUM, 0, 2, PRI_MO_RPM, PRI_MO_TORQUE, "", " kW")*/                    \
  /*X(SEC_MO_POWER, "", "", MOTOR_POWER_CALC, 0, 0, 0, 0, PRIORITY_MEDIUM, 1, 2, SEC_MO_RPM, SEC_MO_TORQUE, "", " kW")*/                    \
  X(POWER_PER_100KM, "", "", DIVIDER_CALC, 100, 0, 6, 4, PRIORITY_MEDIUM_LOW, 0, 2, HVB_POWER, VEHICLE_SPEED_HR, "", "")

// First usage - Enum definition
// Define X for enum usage
#define X(id, header, pid, method, p1, p2, start, num, level, g1, g2, d1, d2, pre, suf) id,
typedef enum {
  PID_NONE = -1,  // Special value for "no dependency"
  PID_LIST
      PID_COUNT
} PID_ID;
#undef X  // Important: Undefine X before redefining it

// PID information structure
typedef struct {
  PID_ID id;
  const char *header;    // OBD2 header
  const char *pid;       // OBD2 PID with 22 prefix
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
  PID_ID dependsOn[2];   // PIDs this one depends on (-1 if none)
  char strValue[16];     // String prefix for display
  char suffix[8];        // String suffix for display
} PID_t;

// Redefine X for struct initialization
#define X(id, header, pid, method, p1, p2, start, num, level, g1, g2, d1, d2, pre, suf) \
  {id, header, pid, method, p1, p2, start, num, level, {g1, g2}, 0, 0, false, NULL, {d1, d2}, pre, suf},

// Create the array using PID_LIST
PID_t pids[] = {
    PID_LIST};
#undef X  // Clean up by undefining X again

PID_t *getPID(PID_ID id) {
  if (id < 0 || id >= PID_COUNT)
    return NULL;
  return &pids[id];
}

// Track the current group being processed for each priority level
typedef struct {
  uint8_t currentGroup;
  uint32_t nextUpdateTime;
  uint32_t updateRate;
} PriorityState;

PriorityState priorityStates[4];

uint32_t cds_value;

// Forward declarations
uint8_t ctoi(uint8_t value);
uint64_t buildUnsignedPayloadFromChars(uint8_t startOffset, uint8_t numPayChars);
int16_t buildSigned16Payload(uint8_t startOffset);
void updatePID(int index);
void updateDisplayForPID(int index);
void fillBatteryMode(uint8_t mode, char buffer[32]);
void handleBrightness();

const int NUM_PIDS = sizeof(pids) / sizeof(PID_t);

void my_log_cb(lv_log_level_t level, const char *buf) {
  Serial.printf("[%d] %s\n", level, buf);
  Serial.flush();
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

void keyRequestCallback() {
  SerialBT.respondPasskey(1234);
}

void confirmRequestCallback(uint32_t numVal) {
  SerialBT.confirmReply(true);
}

void setup() {
  // Initialize priority states
  priorityStates[PRIORITY_HIGH].currentGroup = 0;
  priorityStates[PRIORITY_HIGH].nextUpdateTime = 0;
  priorityStates[PRIORITY_HIGH].updateRate = RATE_HIGH;

  priorityStates[PRIORITY_MEDIUM_HIGH].currentGroup = 0;
  priorityStates[PRIORITY_MEDIUM_HIGH].nextUpdateTime = 0;
  priorityStates[PRIORITY_MEDIUM_HIGH].updateRate = RATE_MEDIUM_HIGH;

  priorityStates[PRIORITY_MEDIUM_LOW].currentGroup = 0;
  priorityStates[PRIORITY_MEDIUM_LOW].nextUpdateTime = 0;
  priorityStates[PRIORITY_MEDIUM_LOW].updateRate = RATE_MEDIUM_LOW;

  priorityStates[PRIORITY_LOW].currentGroup = 0;
  priorityStates[PRIORITY_LOW].nextUpdateTime = 0;
  priorityStates[PRIORITY_LOW].updateRate = RATE_LOW;

  Serial.begin(115200);
  Serial.println("ESP32 EV Data Monitor with LVGL");

  // lv_log_register_print_cb(my_log_cb);

  // Initialize display

  smartdisplay_init();

  lv_obj_t *label = lv_label_create(lv_scr_act());
  lv_label_set_text(label, "Initializing bluetooth...");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
  lv_timer_handler();

  // Initialize Bluetooth
  if (!DEMO_MODE) {
    SerialBT.end();
    SerialBT.disableSSP();
    // SerialBT.setPin("1234", 4);
    // memcpy(pinCode, "1234", 4);
    // esp_bt_gap_set_pin(ESP_BT_PIN_TYPE_FIXED, 4, pinCode);
    // SerialBT._pin_code_len = 4;
    if (!SerialBT.begin("ESP32_EV_Monitor", true, true)) {
      lv_label_set_text(label, "Couldn't start BT");
      lv_tick_inc(50);
      lv_timer_handler();
      while (1);
    }
  }

  Serial.println("Bluetooth initialized, connecting to ELM327...");
  lv_label_set_text(label, "BT initialized, connecting to ELM327...");
  lv_tick_inc(50);
  lv_timer_handler();

  if (!DEMO_MODE) {
    SerialBT.onKeyRequest(keyRequestCallback);
    SerialBT.onConfirmRequest(confirmRequestCallback);
    SerialBT.setPin("1234", 4);

#ifdef BT_QUICK_CONNECT
    uint8_t addr[6] = {0xaa, 0xbb, 0xcc, 0x11, 0x22, 0x33};
    if (!SerialBT.connect(addr, 1, ESP_SPP_SEC_ENCRYPT | ESP_SPP_SEC_AUTHENTICATE, ESP_SPP_ROLE_MASTER)) {
      // if (!SerialBT.connect(BTAddress("aabbcc112233"))) {
#else
    if (!SerialBT.connect("OBDII")) {
#endif
      DEBUG_PORT.println("Cannot connect to OBD scanner - 1");
      lv_label_set_text(label, "Cannot connect to OBD scanner - 1");
      lv_tick_inc(50);
      lv_timer_handler();
      while (1);
    }

    // Connect to ELM327 adapter
    if (!myELM327.begin(SerialBT, false, 1000)) {
      Serial.println("Cannot connect to OBD scanner - 2");
      lv_label_set_text(label, "Cannot connect to OBD scanner - 2");
      lv_tick_inc(50);
      lv_timer_handler();
      while (1);
    }
  }

  Serial.println("Connected to ELM327");
  lv_label_set_text(label, "Connected to ELM327, reading all values");
  lv_tick_inc(1);
  lv_timer_handler();

  // read all values once
  for (int i = 0; i < NUM_PIDS; i++) {
    updatePID(i);
  }

  ui_init();
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
          if (pids[i].method == MULTIPLIER_CALC) {
            // A * B * p1 - Power (kW) = voltage * current * 0.001
            pids[i].value = pids[pids[i].dependsOn[0]].value *
                            pids[pids[i].dependsOn[1]].value *
                            pids[i].param1;
          } else if (pids[i].method == MOTOR_POWER_CALC) {
            // TODO: convert to multiplier_calc
            // Motor power (kW) = (RPM * Torque) / 9.549 / 1000
            pids[i].value = (pids[pids[i].dependsOn[0]].value *
                             pids[pids[i].dependsOn[1]].value) /
                            9.549 * 0.001;
          } else if (pids[i].method == DIVIDER_CALC) {
            // A/B * p1
            pids[i].value = (pids[pids[i].dependsOn[0]].value /
                             pids[pids[i].dependsOn[1]].value) *
                            pids[i].param1;
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

int demoTick = 0;
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

    if (pids[HVB_POWER].valid) {
      Serial.printf("Power values - HVB: %f kW V: %fV I: %fA Speed: %f km/h \n",
                    pids[HVB_POWER].value, pids[HVB_VOLTAGE].value, pids[HVB_CURRENT].value, pids[VEHICLE_SPEED_HR].value);
    }

    handleBrightness();
  }

  if (currentTime % 20 == 0) {
    demoTick++;
    demoTick %= 250;
  }

  // Update the ticker
  lv_tick_inc(currentTime - lv_last_tick);
  lv_last_tick = currentTime;

  // lv_task_handler();
  lv_timer_handler();
  ui_tick();
}

const char *lastHeader;
void updatePID(int index) {
  if (DEMO_MODE) {
    // DEMO MODE

    switch (index) {
      case HVB_VOLTAGE:  // HvbV
        pids[index].value = 350;
        break;
      case HVB_CURRENT:  // HVb C
        pids[index].value = abs((demoTick % 250) - 125) * -8 + 500;
        break;
      // case PRI_MO_RPM:  // PriRpm
      //   pids[index].value = 1500;
      //   break;
      case PRI_MO_TORQUE:  // PriTq
        pids[index].value = ((demoTick % 250) - 150) * 5;
        break;
      // case 4:  // SecRpm
      //   pids[index].value = 1500;
      //   break;
      case SEC_MO_TORQUE:  // SecTq
        pids[index].value = ((demoTick % 100) - 30) * 5;
        break;
      case HVB_TEMP:  // HvbT
        pids[index].value = 15 + (demoTick % 40) / 5;
        break;
      case SOC:  // SoC
        pids[index].value = 90.3 - (demoTick % 32) / 10;
        break;
      case INT_TEMP:  // IntT
        pids[index].value = 18 + (demoTick % 80) / 10;
        break;
      case HVB_MODE:  // Hvb Mode
        pids[index].value = demoTick % 5;
        break;
      case LVB_SOC:
        pids[index].value = 75;
        break;
      case LVB_VOLTAGE:
        pids[index].value = 13.5;
        break;
      // case LVB_CURRENT:
      //   pids[index].value = -3.6;
      //   break;
      // case COOL_HEAT_POWER:
      //   pids[index].value = 15 - (demoTick % 40) / 5;
      //   break;
      case PRI_MO_TEMP:
        pids[index].value = 40 + (demoTick % 40) / 5;
        break;
      case SEC_MO_TEMP:
        pids[index].value = 50 + (demoTick % 40) / 6;
        break;
      case VEHICLE_SPEED_HR:
        pids[index].value = 90 + (demoTick % 20) / 3;
        break;
      default:
        if (pids[index].dependsOn[0] == -1) {
          Serial.printf("Warning, no demo value defined PID index %i", index);
        }
        break;
    }

    pids[index].valid = true;
    pids[index].lastUpdate = millis();

    updateDisplayForPID(index);

  } else {
    // NOT DEMO MODE

    const char *header = pids[index].header;
    if (header != NULL && strcmp(header, "") != 0 && (lastHeader == NULL || strcmp(header, lastHeader) != 0)) {
      char command[20];
      sprintf(command, SET_HEADER, header);
      myELM327.sendCommand_Blocking(command);
      // TODO: error checking
      lastHeader = header;
    }

    myELM327.sendCommand_Blocking(pids[index].pid);

    if (myELM327.nb_rx_state != ELM_SUCCESS) {
      Serial.printf("Failed to send command: %s code %d\n", pids[index].pid, myELM327.get_response());

      myELM327.sendCommand_Blocking("AT SH 726");
      myELM327.sendCommand_Blocking(pids[index].pid);
      uint32_t startTime = millis();
      if (myELM327.nb_rx_state != ELM_SUCCESS) {
        Serial.printf("Failed to send command again: %s code %d\n", pids[index].pid, myELM327.get_response());
        delay(200);
        return;
      }
    }

#ifdef DEBUG
    if (myELM327.nb_rx_state == ELM_SUCCESS) {
#endif
      // Calculate the value based on the method
      if (pids[index].method == UNSIGNED_MULT_OFFSET) {
        uint64_t raw = buildUnsignedPayloadFromChars(pids[index].startOffset, pids[index].numChars);
        pids[index].value = raw * pids[index].param1 + pids[index].param2;
      } else if (pids[index].method == SIGNED_MULT16) {
        int16_t raw = buildSigned16Payload(pids[index].startOffset);
        if (raw == -32768) {
          // sometimes we get 0x8000 back, no idea why
          return;
        }
        pids[index].value = raw * pids[index].param1;
      } else if (pids[index].method == HVB_TEMP_MODE_CALC) {
        // Special case for battery temp mode
        uint64_t raw = buildUnsignedPayloadFromChars(pids[index].startOffset, pids[index].numChars);
        pids[index].value = (float)raw;  // Use raw value for mode
      }

      pids[index].valid = true;
      pids[index].lastUpdate = millis();

      // Update the display for this PID
      updateDisplayForPID(index);

#ifdef DEBUG
      Serial.print(pids[index].name);
      Serial.print(": ");
      Serial.println(pids[index].value);
    } else {
      myELM327.printError();
      pids[index].valid = false;
    }
#endif
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

#ifdef DEBUG
  Serial.print("Received: ");
  for (uint8_t i = 0; i < 4; i++) {
    Serial.print(myELM327.payload[startOffset + i]);
  }
  Serial.print(" decoded: ");
  Serial.println((int16_t)combined);
#endif

  return (int16_t)combined;
}

uint32_t lastTheme = THEME_ID_LIGHT;
void handleBrightness() {
#ifdef BOARD_HAS_CDS
  cds_value = analogReadMilliVolts(CDS);
  if (cds_value > CDS_BRIGHT_MILLIVOLTS) {
    if (lastTheme != THEME_ID_DARK) {
      analogWrite(GPIO_BCKL, 100);
      change_color_theme(THEME_ID_DARK);
      lastTheme = THEME_ID_DARK;
    }
  } else {
    if (lastTheme != THEME_ID_LIGHT) {
      analogWrite(GPIO_BCKL, 255);  // maximum brightness
      change_color_theme(THEME_ID_LIGHT);
      lastTheme = THEME_ID_LIGHT;
    }
  }
#endif
}

char *buildTextForPid(char *buffer, uint8_t pid, uint8_t precision = 0, boolean useSuffix = true) {
  sprintf(buffer, "%s%.*f%s", pids[pid].strValue, precision, pids[pid].value, useSuffix ? pids[pid].suffix : "");
  return buffer;
}

// String buffer for each PID value
#define MAX_PID_STR_LENGTH 30
struct PidStringBuffer {
  char buffer[MAX_PID_STR_LENGTH];
};

// Array of buffers for all PIDs
PidStringBuffer pidStrBuffers[PID_COUNT];

// Template for generating getter functions for float values
#define DEFINE_FLOAT_GETTER(name, pid_id) \
  float get_var_##name() {                \
    return pids[pid_id].value;            \
  }                                       \
  void set_var_##name(float value) {}

// Template for generating getter functions for string values
#define DEFINE_STRING_GETTER_STR(name, pid_id, precision, useSuffix)                    \
  const char *get_var_##name##_str() {                                                  \
    return buildTextForPid(pidStrBuffers[pid_id].buffer, pid_id, precision, useSuffix); \
  }                                                                                     \
  void set_var_##name##_str(const char *value) {}

#define DEFINE_STRING_GETTER(name, pid_id, precision, useSuffix)                        \
  const char *get_var_##name() {                                                        \
    return buildTextForPid(pidStrBuffers[pid_id].buffer, pid_id, precision, useSuffix); \
  }                                                                                     \
  void set_var_##name(const char *value) {}

// Template for defining both float and string getters
#define DEFINE_PID_ACCESSORS(name, pid_id, precision, suffix) \
  DEFINE_FLOAT_GETTER(name, pid_id)                           \
  DEFINE_STRING_GETTER_STR(name, pid_id, precision, suffix)

DEFINE_PID_ACCESSORS(power, HVB_POWER, 1, false)
// DEFINE_PID_ACCESSORS(pri_motor_power, PRI_MO_POWER, 0, true)
// DEFINE_PID_ACCESSORS(sec_motor_power, SEC_MO_POWER, 0, true)

DEFINE_STRING_GETTER(int_temp, INT_TEMP, 0, true)
DEFINE_STRING_GETTER(bat_temp, HVB_TEMP, 0, true)
DEFINE_STRING_GETTER(soc, SOC, 1, true)
DEFINE_PID_ACCESSORS(pri_mot_tq, PRI_MO_TORQUE, 0, true)
// DEFINE_STRING_GETTER_STR(coolheat_pwr, COOL_HEAT_POWER, 0, true)
DEFINE_STRING_GETTER_STR(lvb_soc, LVB_SOC, 0, true)
DEFINE_STRING_GETTER_STR(lvb_v, LVB_VOLTAGE, 1, true)
// DEFINE_STRING_GETTER_STR(lvb_current, LVB_CURRENT, 1, true)
DEFINE_STRING_GETTER_STR(pri_mot_temp, PRI_MO_TEMP, 0, true)
DEFINE_STRING_GETTER_STR(sec_mot_temp, SEC_MO_TEMP, 0, true)

DEFINE_STRING_GETTER(kwh_per_100kmh, POWER_PER_100KM, 1, false)

const char *get_var_bat_mode() {
  fillBatteryMode(pids[HVB_MODE].value, pidStrBuffers[HVB_MODE].buffer);
  return pidStrBuffers[HVB_MODE].buffer;
}
void set_var_bat_mode(const char *value) {}

float get_var_sec_mot_tq() {
  // XXX: negative value is required to move the right way
  return -pids[SEC_MO_TORQUE].value;
}
void set_var_sec_mot_tq(float value) {}
DEFINE_STRING_GETTER_STR(sec_mot_tq, SEC_MO_TORQUE, 0, true)

char cds_str[30] = {0};
const char *get_var_cds_str() {
  sprintf(cds_str, "%u", cds_value);
  return cds_str;
}
void set_var_cds_str(const char *value) {}
