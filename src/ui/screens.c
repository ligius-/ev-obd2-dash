#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

#include <string.h>

objects_t objects;
lv_obj_t *tick_value_change_obj;
uint32_t active_theme_index = 0;

static void event_handler_cb_screen1_obj0(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            int32_t value = lv_arc_get_value(ta);
            set_var_power(value);
        }
    }
}

static void event_handler_cb_screen1_pri_mo(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            int32_t value = lv_arc_get_value(ta);
            set_var_pri_mot_tq(value);
        }
    }
}

static void event_handler_cb_screen1_sec_mo(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            int32_t value = lv_arc_get_value(ta);
            set_var_sec_mot_tq(value);
        }
    }
}

void create_screen_screen1() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.screen1 = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 240, 320);
    lv_obj_set_style_bg_color(obj, lv_color_hex(theme_colors[active_theme_index][0]), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_arc_create(parent_obj);
            objects.obj0 = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 240, 240);
            lv_arc_set_range(obj, -200, 200);
            lv_arc_set_mode(obj, LV_ARC_MODE_SYMMETRICAL);
            lv_obj_add_event_cb(obj, event_handler_cb_screen1_obj0, LV_EVENT_ALL, 0);
            lv_obj_set_style_arc_width(obj, 16, LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_width(obj, 16, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_opa(obj, 150, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_color(obj, lv_color_hex(theme_colors[active_theme_index][2]), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // pri_mo
            lv_obj_t *obj = lv_arc_create(parent_obj);
            objects.pri_mo = obj;
            lv_obj_set_pos(obj, 20, 20);
            lv_obj_set_size(obj, 200, 200);
            lv_arc_set_range(obj, -200, 200);
            lv_arc_set_bg_start_angle(obj, 40);
            lv_arc_set_bg_end_angle(obj, 140);
            lv_arc_set_mode(obj, LV_ARC_MODE_SYMMETRICAL);
            lv_arc_set_rotation(obj, 90);
            lv_obj_add_event_cb(obj, event_handler_cb_screen1_pri_mo, LV_EVENT_ALL, 0);
            lv_obj_set_style_bg_opa(obj, 50, LV_PART_KNOB | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(theme_colors[active_theme_index][1]), LV_PART_KNOB | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 1, LV_PART_KNOB | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 1, LV_PART_KNOB | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_left(obj, 1, LV_PART_KNOB | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 1, LV_PART_KNOB | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_color(obj, lv_color_hex(theme_colors[active_theme_index][3]), LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_width(obj, 5, LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_width(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_color(obj, lv_color_hex(theme_colors[active_theme_index][2]), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_opa(obj, 150, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // sec_mo
            lv_obj_t *obj = lv_arc_create(parent_obj);
            objects.sec_mo = obj;
            lv_obj_set_pos(obj, 20, 20);
            lv_obj_set_size(obj, 200, 200);
            lv_arc_set_range(obj, -200, 200);
            lv_arc_set_bg_start_angle(obj, 40);
            lv_arc_set_bg_end_angle(obj, 140);
            lv_arc_set_mode(obj, LV_ARC_MODE_SYMMETRICAL);
            lv_arc_set_rotation(obj, 270);
            lv_obj_add_event_cb(obj, event_handler_cb_screen1_sec_mo, LV_EVENT_ALL, 0);
            lv_obj_set_style_bg_opa(obj, 50, LV_PART_KNOB | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 1, LV_PART_KNOB | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_left(obj, 1, LV_PART_KNOB | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 1, LV_PART_KNOB | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(theme_colors[active_theme_index][1]), LV_PART_KNOB | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 1, LV_PART_KNOB | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_color(obj, lv_color_hex(theme_colors[active_theme_index][3]), LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_width(obj, 5, LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_width(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_color(obj, lv_color_hex(theme_colors[active_theme_index][2]), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_opa(obj, 150, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // hvb_power_lbl
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.hvb_power_lbl = obj;
            lv_obj_set_pos(obj, 50, 93);
            lv_obj_set_size(obj, 110, LV_SIZE_CONTENT);
            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_40, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(theme_colors[active_theme_index][1]), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj1 = obj;
            lv_obj_set_pos(obj, 160, 97);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(theme_colors[active_theme_index][1]), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "kW");
        }
        {
            // int_temp_lbl
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.int_temp_lbl = obj;
            lv_obj_set_pos(obj, 24, 280);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(theme_colors[active_theme_index][1]), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "");
        }
        {
            lv_obj_t *obj = lv_image_create(parent_obj);
            objects.obj2 = obj;
            lv_obj_set_pos(obj, 6, 280);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_image_set_src(obj, &img_temperature);
            lv_obj_set_style_image_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_image_recolor(obj, lv_color_hex(theme_colors[active_theme_index][1]), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_image_recolor_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_image_create(parent_obj);
            lv_obj_set_pos(obj, 158, 275);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_image_set_src(obj, &img_konkapp_batt2);
        }
        {
            // soc_lbl
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.soc_lbl = obj;
            lv_obj_set_pos(obj, 93, 137);
            lv_obj_set_size(obj, 55, LV_SIZE_CONTENT);
            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
            lv_obj_set_style_text_color(obj, lv_color_hex(theme_colors[active_theme_index][1]), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "");
        }
        {
            // bat_temp_lbl
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.bat_temp_lbl = obj;
            lv_obj_set_pos(obj, 190, 280);
            lv_obj_set_size(obj, 40, LV_SIZE_CONTENT);
            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
            lv_obj_set_style_text_color(obj, lv_color_hex(theme_colors[active_theme_index][1]), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "");
        }
        {
            // bat_mode_lbl
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.bat_mode_lbl = obj;
            lv_obj_set_pos(obj, 175, 296);
            lv_obj_set_size(obj, 55, LV_SIZE_CONTENT);
            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
            lv_obj_set_style_text_color(obj, lv_color_hex(theme_colors[active_theme_index][1]), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "");
        }
        {
            lv_obj_t *obj = lv_image_create(parent_obj);
            objects.obj3 = obj;
            lv_obj_set_pos(obj, 68, 278);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_image_set_src(obj, &img_car_battery_icons8);
            lv_obj_set_style_image_recolor(obj, lv_color_hex(theme_colors[active_theme_index][1]), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_image_recolor_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // lvbsoc_lbl
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.lvbsoc_lbl = obj;
            lv_obj_set_pos(obj, 98, 279);
            lv_obj_set_size(obj, 55, LV_SIZE_CONTENT);
            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
            lv_obj_set_style_text_color(obj, lv_color_hex(theme_colors[active_theme_index][1]), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "");
        }
        {
            // lvbv_lbl
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.lvbv_lbl = obj;
            lv_obj_set_pos(obj, 97, 296);
            lv_obj_set_size(obj, 55, LV_SIZE_CONTENT);
            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
            lv_obj_set_style_text_color(obj, lv_color_hex(theme_colors[active_theme_index][1]), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "");
        }
        {
            // pri_mot_tq_lbl
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.pri_mot_tq_lbl = obj;
            lv_obj_set_pos(obj, 56, 173);
            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
            lv_obj_set_style_text_color(obj, lv_color_hex(theme_colors[active_theme_index][1]), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "");
        }
        {
            // sec_mot_tq_lbl
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.sec_mot_tq_lbl = obj;
            lv_obj_set_pos(obj, 126, 173);
            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
            lv_obj_set_style_text_color(obj, lv_color_hex(theme_colors[active_theme_index][1]), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "");
        }
        {
            // bat_temp_lbl_1
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.bat_temp_lbl_1 = obj;
            lv_obj_set_pos(obj, 134, 189);
            lv_obj_set_size(obj, 40, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(theme_colors[active_theme_index][1]), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "");
        }
        {
            // pri_mot_temp_lbl
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.pri_mot_temp_lbl = obj;
            lv_obj_set_pos(obj, 66, 189);
            lv_obj_set_size(obj, 40, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(theme_colors[active_theme_index][1]), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "");
        }
        {
            // cds_lbl
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.cds_lbl = obj;
            lv_obj_set_pos(obj, -2, 296);
            lv_obj_set_size(obj, 55, LV_SIZE_CONTENT);
            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
            lv_obj_set_style_text_color(obj, lv_color_hex(theme_colors[active_theme_index][1]), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "");
        }
        {
            // kwh_per_100km_lbl
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.kwh_per_100km_lbl = obj;
            lv_obj_set_pos(obj, 97, 238);
            lv_obj_set_size(obj, 55, LV_SIZE_CONTENT);
            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
            lv_obj_set_style_text_color(obj, lv_color_hex(theme_colors[active_theme_index][1]), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj4 = obj;
            lv_obj_set_pos(obj, 154, 240);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(theme_colors[active_theme_index][1]), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "kWh /");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj5 = obj;
            lv_obj_set_pos(obj, 154, 251);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(theme_colors[active_theme_index][1]), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "100 kM");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj6 = obj;
            lv_obj_set_pos(obj, 64, 246);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(theme_colors[active_theme_index][1]), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Usage:");
        }
    }
    
    tick_screen_screen1();
}

void tick_screen_screen1() {
    {
        int32_t new_val = get_var_power();
        int32_t cur_val = lv_arc_get_value(objects.obj0);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.obj0;
            lv_arc_set_value(objects.obj0, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = get_var_pri_mot_tq();
        int32_t cur_val = lv_arc_get_value(objects.pri_mo);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.pri_mo;
            lv_arc_set_value(objects.pri_mo, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = get_var_sec_mot_tq();
        int32_t cur_val = lv_arc_get_value(objects.sec_mo);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.sec_mo;
            lv_arc_set_value(objects.sec_mo, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_power_str();
        const char *cur_val = lv_label_get_text(objects.hvb_power_lbl);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.hvb_power_lbl;
            lv_label_set_text(objects.hvb_power_lbl, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_int_temp();
        const char *cur_val = lv_label_get_text(objects.int_temp_lbl);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.int_temp_lbl;
            lv_label_set_text(objects.int_temp_lbl, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_soc();
        const char *cur_val = lv_label_get_text(objects.soc_lbl);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.soc_lbl;
            lv_label_set_text(objects.soc_lbl, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_bat_temp();
        const char *cur_val = lv_label_get_text(objects.bat_temp_lbl);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.bat_temp_lbl;
            lv_label_set_text(objects.bat_temp_lbl, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_bat_mode();
        const char *cur_val = lv_label_get_text(objects.bat_mode_lbl);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.bat_mode_lbl;
            lv_label_set_text(objects.bat_mode_lbl, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_lvb_soc_str();
        const char *cur_val = lv_label_get_text(objects.lvbsoc_lbl);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.lvbsoc_lbl;
            lv_label_set_text(objects.lvbsoc_lbl, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_lvb_v_str();
        const char *cur_val = lv_label_get_text(objects.lvbv_lbl);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.lvbv_lbl;
            lv_label_set_text(objects.lvbv_lbl, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_pri_mot_tq_str();
        const char *cur_val = lv_label_get_text(objects.pri_mot_tq_lbl);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.pri_mot_tq_lbl;
            lv_label_set_text(objects.pri_mot_tq_lbl, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_sec_mot_tq_str();
        const char *cur_val = lv_label_get_text(objects.sec_mot_tq_lbl);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.sec_mot_tq_lbl;
            lv_label_set_text(objects.sec_mot_tq_lbl, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_sec_mot_temp_str();
        const char *cur_val = lv_label_get_text(objects.bat_temp_lbl_1);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.bat_temp_lbl_1;
            lv_label_set_text(objects.bat_temp_lbl_1, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_pri_mot_temp_str();
        const char *cur_val = lv_label_get_text(objects.pri_mot_temp_lbl);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.pri_mot_temp_lbl;
            lv_label_set_text(objects.pri_mot_temp_lbl, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_cds_str();
        const char *cur_val = lv_label_get_text(objects.cds_lbl);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.cds_lbl;
            lv_label_set_text(objects.cds_lbl, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_kwh_per_100kmh();
        const char *cur_val = lv_label_get_text(objects.kwh_per_100km_lbl);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.kwh_per_100km_lbl;
            lv_label_set_text(objects.kwh_per_100km_lbl, new_val);
            tick_value_change_obj = NULL;
        }
    }
}

void change_color_theme(uint32_t theme_index) {
    active_theme_index = theme_index;
    
    lv_obj_set_style_bg_color(objects.screen1, lv_color_hex(theme_colors[theme_index][0]), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_set_style_arc_color(objects.obj0, lv_color_hex(theme_colors[theme_index][2]), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_set_style_bg_color(objects.pri_mo, lv_color_hex(theme_colors[theme_index][1]), LV_PART_KNOB | LV_STATE_DEFAULT);
    
    lv_obj_set_style_arc_color(objects.pri_mo, lv_color_hex(theme_colors[theme_index][3]), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    
    lv_obj_set_style_arc_color(objects.pri_mo, lv_color_hex(theme_colors[theme_index][2]), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_set_style_bg_color(objects.sec_mo, lv_color_hex(theme_colors[theme_index][1]), LV_PART_KNOB | LV_STATE_DEFAULT);
    
    lv_obj_set_style_arc_color(objects.sec_mo, lv_color_hex(theme_colors[theme_index][3]), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    
    lv_obj_set_style_arc_color(objects.sec_mo, lv_color_hex(theme_colors[theme_index][2]), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_set_style_text_color(objects.hvb_power_lbl, lv_color_hex(theme_colors[theme_index][1]), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_set_style_text_color(objects.obj1, lv_color_hex(theme_colors[theme_index][1]), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_set_style_text_color(objects.int_temp_lbl, lv_color_hex(theme_colors[theme_index][1]), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_set_style_image_recolor(objects.obj2, lv_color_hex(theme_colors[theme_index][1]), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_set_style_text_color(objects.soc_lbl, lv_color_hex(theme_colors[theme_index][1]), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_set_style_text_color(objects.bat_temp_lbl, lv_color_hex(theme_colors[theme_index][1]), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_set_style_text_color(objects.bat_mode_lbl, lv_color_hex(theme_colors[theme_index][1]), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_set_style_image_recolor(objects.obj3, lv_color_hex(theme_colors[theme_index][1]), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_set_style_text_color(objects.lvbsoc_lbl, lv_color_hex(theme_colors[theme_index][1]), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_set_style_text_color(objects.lvbv_lbl, lv_color_hex(theme_colors[theme_index][1]), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_set_style_text_color(objects.pri_mot_tq_lbl, lv_color_hex(theme_colors[theme_index][1]), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_set_style_text_color(objects.sec_mot_tq_lbl, lv_color_hex(theme_colors[theme_index][1]), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_set_style_text_color(objects.bat_temp_lbl_1, lv_color_hex(theme_colors[theme_index][1]), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_set_style_text_color(objects.pri_mot_temp_lbl, lv_color_hex(theme_colors[theme_index][1]), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_set_style_text_color(objects.cds_lbl, lv_color_hex(theme_colors[theme_index][1]), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_set_style_text_color(objects.kwh_per_100km_lbl, lv_color_hex(theme_colors[theme_index][1]), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_set_style_text_color(objects.obj4, lv_color_hex(theme_colors[theme_index][1]), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_set_style_text_color(objects.obj5, lv_color_hex(theme_colors[theme_index][1]), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_set_style_text_color(objects.obj6, lv_color_hex(theme_colors[theme_index][1]), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_invalidate(objects.screen1);
}

uint32_t theme_colors[2][4] = {
    { 0xffffffff, 0xff000000, 0xffe0e0e0, 0xffe39426 },
    { 0xff000000, 0xffffffff, 0xff4f4f4f, 0xffb1a410 },
};


typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_screen1,
};
void tick_screen(int screen_index) {
    tick_screen_funcs[screen_index]();
}
void tick_screen_by_id(enum ScreensEnum screenId) {
    tick_screen_funcs[screenId - 1]();
}

void create_screens() {
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    
    create_screen_screen1();
}
