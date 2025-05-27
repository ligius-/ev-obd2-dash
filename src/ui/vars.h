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
    FLOW_GLOBAL_VARIABLE_INT_TEMP = 0,
    FLOW_GLOBAL_VARIABLE_BAT_MODE = 1,
    FLOW_GLOBAL_VARIABLE_BAT_TEMP = 2,
    FLOW_GLOBAL_VARIABLE_SOC = 3,
    FLOW_GLOBAL_VARIABLE_POWER = 4,
    FLOW_GLOBAL_VARIABLE_POWER_STR = 5,
    FLOW_GLOBAL_VARIABLE_CDS_STR = 6,
    FLOW_GLOBAL_VARIABLE_COOLHEAT_PWR_STR = 7,
    FLOW_GLOBAL_VARIABLE_LVB_SOC_STR = 8,
    FLOW_GLOBAL_VARIABLE_LVB_V_STR = 9,
    FLOW_GLOBAL_VARIABLE_LVB_CURRENT_STR = 10,
    FLOW_GLOBAL_VARIABLE_PRI_MOT_TQ = 11,
    FLOW_GLOBAL_VARIABLE_PRI_MOT_TQ_STR = 12,
    FLOW_GLOBAL_VARIABLE_PRI_MOT_TEMP_STR = 13,
    FLOW_GLOBAL_VARIABLE_SEC_MOT_TQ = 14,
    FLOW_GLOBAL_VARIABLE_SEC_MOT_TQ_STR = 15,
    FLOW_GLOBAL_VARIABLE_SEC_MOT_TEMP_STR = 16,
    FLOW_GLOBAL_VARIABLE_KWH_PER_100KMH = 17
};

// Native global variables

extern const char *get_var_int_temp();
extern void set_var_int_temp(const char *value);
extern const char *get_var_bat_mode();
extern void set_var_bat_mode(const char *value);
extern const char *get_var_bat_temp();
extern void set_var_bat_temp(const char *value);
extern const char *get_var_soc();
extern void set_var_soc(const char *value);
extern float get_var_power();
extern void set_var_power(float value);
extern const char *get_var_power_str();
extern void set_var_power_str(const char *value);
extern const char *get_var_cds_str();
extern void set_var_cds_str(const char *value);
extern const char *get_var_coolheat_pwr_str();
extern void set_var_coolheat_pwr_str(const char *value);
extern const char *get_var_lvb_soc_str();
extern void set_var_lvb_soc_str(const char *value);
extern const char *get_var_lvb_v_str();
extern void set_var_lvb_v_str(const char *value);
extern const char *get_var_lvb_current_str();
extern void set_var_lvb_current_str(const char *value);
extern float get_var_pri_mot_tq();
extern void set_var_pri_mot_tq(float value);
extern const char *get_var_pri_mot_tq_str();
extern void set_var_pri_mot_tq_str(const char *value);
extern const char *get_var_pri_mot_temp_str();
extern void set_var_pri_mot_temp_str(const char *value);
extern float get_var_sec_mot_tq();
extern void set_var_sec_mot_tq(float value);
extern const char *get_var_sec_mot_tq_str();
extern void set_var_sec_mot_tq_str(const char *value);
extern const char *get_var_sec_mot_temp_str();
extern void set_var_sec_mot_temp_str(const char *value);
extern const char *get_var_kwh_per_100kmh();
extern void set_var_kwh_per_100kmh(const char *value);


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_VARS_H*/