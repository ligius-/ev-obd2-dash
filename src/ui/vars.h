#ifndef EEZ_LVGL_UI_VARS_H
#define EEZ_LVGL_UI_VARS_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// enum declarations



// Flow global variables

enum FlowGlobalVariables {
    FLOW_GLOBAL_VARIABLE_PRI_MOTOR_POWER = 0,
    FLOW_GLOBAL_VARIABLE_PRI_MOTOR_POWER_STR = 1,
    FLOW_GLOBAL_VARIABLE_SEC_MOTOR_POWER = 2,
    FLOW_GLOBAL_VARIABLE_SEC_MOTOR_POWER_STR = 3,
    FLOW_GLOBAL_VARIABLE_POWER = 4,
    FLOW_GLOBAL_VARIABLE_POWER_STR = 5,
    FLOW_GLOBAL_VARIABLE_INT_TEMP = 6,
    FLOW_GLOBAL_VARIABLE_BAT_TEMP = 7,
    FLOW_GLOBAL_VARIABLE_SOC = 8,
    FLOW_GLOBAL_VARIABLE_BAT_MODE = 9
};

// Native global variables

extern float get_var_pri_motor_power();
extern void set_var_pri_motor_power(float value);
extern const char *get_var_pri_motor_power_str();
extern void set_var_pri_motor_power_str(const char *value);
extern float get_var_sec_motor_power();
extern void set_var_sec_motor_power(float value);
extern const char *get_var_sec_motor_power_str();
extern void set_var_sec_motor_power_str(const char *value);
extern float get_var_power();
extern void set_var_power(float value);
extern const char *get_var_power_str();
extern void set_var_power_str(const char *value);
extern const char *get_var_int_temp();
extern void set_var_int_temp(const char *value);
extern const char *get_var_bat_temp();
extern void set_var_bat_temp(const char *value);
extern const char *get_var_soc();
extern void set_var_soc(const char *value);
extern const char *get_var_bat_mode();
extern void set_var_bat_mode(const char *value);


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_VARS_H*/