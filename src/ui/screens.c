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

static lv_meter_scale_t * scale0;
static lv_meter_indicator_t * indicator1;
static lv_meter_indicator_t * indicator2;
static lv_meter_indicator_t * indicator3;
static lv_meter_scale_t * scale4;
static lv_meter_indicator_t * indicator5;
static lv_meter_indicator_t * indicator6;
static lv_meter_indicator_t * indicator7;

void create_screen_page1() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.page1 = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 240);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // pri_motor_meter
            lv_obj_t *obj = lv_meter_create(parent_obj);
            objects.pri_motor_meter = obj;
            lv_obj_set_pos(obj, 0, 40);
            lv_obj_set_size(obj, 160, 160);
            {
                lv_meter_scale_t *scale = lv_meter_add_scale(obj);
                scale0 = scale;
                lv_meter_set_scale_ticks(obj, scale, 19, 1, 3, lv_color_hex(0xff818181));
                lv_meter_set_scale_major_ticks(obj, scale, 9, 2, 5, lv_color_hex(0xffa4a4a4), 8);
                lv_meter_set_scale_range(obj, scale, -100, 100, 180, 90);
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_needle_line(obj, scale, 2, lv_color_hex(0xffffffff), 0);
                    indicator1 = indicator;
                }
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_arc(obj, scale, 10, lv_color_hex(0xff6e0000), 5);
                    indicator2 = indicator;
                    lv_meter_set_indicator_start_value(obj, indicator, 20);
                    lv_meter_set_indicator_end_value(obj, indicator, 80);
                }
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_arc(obj, scale, 10, lv_color_hex(0xff005500), 5);
                    indicator3 = indicator;
                    lv_meter_set_indicator_start_value(obj, indicator, -80);
                    lv_meter_set_indicator_end_value(obj, indicator, -20);
                }
            }
            lv_obj_set_style_bg_opa(obj, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_TICKS | LV_STATE_DEFAULT);
            lv_obj_set_style_text_opa(obj, 180, LV_PART_TICKS | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_TICKS | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_width(obj, 40, LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
        }
        {
            // sec_motor_meter
            lv_obj_t *obj = lv_meter_create(parent_obj);
            objects.sec_motor_meter = obj;
            lv_obj_set_pos(obj, 161, 41);
            lv_obj_set_size(obj, 160, 160);
            {
                lv_meter_scale_t *scale = lv_meter_add_scale(obj);
                scale4 = scale;
                lv_meter_set_scale_ticks(obj, scale, 19, 1, 3, lv_color_hex(0xff818181));
                lv_meter_set_scale_major_ticks(obj, scale, 9, 2, 5, lv_color_hex(0xffa4a4a4), 8);
                lv_meter_set_scale_range(obj, scale, 100, -100, 180, 270);
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_needle_line(obj, scale, 2, lv_color_hex(0xffffffff), 0);
                    indicator5 = indicator;
                }
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_arc(obj, scale, 10, lv_color_hex(0xff6e0000), 5);
                    indicator6 = indicator;
                    lv_meter_set_indicator_start_value(obj, indicator, 80);
                    lv_meter_set_indicator_end_value(obj, indicator, 20);
                }
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_arc(obj, scale, 10, lv_color_hex(0xff005500), 5);
                    indicator7 = indicator;
                    lv_meter_set_indicator_start_value(obj, indicator, -20);
                    lv_meter_set_indicator_end_value(obj, indicator, -80);
                }
            }
            lv_obj_set_style_bg_opa(obj, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_TICKS | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_TICKS | LV_STATE_DEFAULT);
            lv_obj_set_style_text_opa(obj, 180, LV_PART_TICKS | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_width(obj, 40, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        }
        {
            // pri_motor_power_lbl
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.pri_motor_power_lbl = obj;
            lv_obj_set_pos(obj, 57, 112);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "");
        }
        {
            // sec_motor_power_lbl
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.sec_motor_power_lbl = obj;
            lv_obj_set_pos(obj, 219, 113);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "");
        }
        {
            // power_lbl
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.power_lbl = obj;
            lv_obj_set_pos(obj, 107, 41);
            lv_obj_set_size(obj, 100, LV_SIZE_CONTENT);
            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_40, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "");
        }
        {
            // power_unit_lbl
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.power_unit_lbl = obj;
            lv_obj_set_pos(obj, 207, 46);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "kW");
        }
        {
            // power_bar
            lv_obj_t *obj = lv_bar_create(parent_obj);
            objects.power_bar = obj;
            lv_obj_set_pos(obj, 0, 8);
            lv_obj_set_size(obj, 320, 23);
            lv_bar_set_range(obj, -200, 200);
            lv_bar_set_mode(obj, LV_BAR_MODE_SYMMETRICAL);
            lv_obj_set_style_radius(obj, 5, LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_anim_time(obj, 5, LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff00ff00), LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0xffff0000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_HOR, LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_stop(obj, 200, LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_main_stop(obj, 50, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        }
        {
            // int_temp_lbl
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.int_temp_lbl = obj;
            lv_obj_set_pos(obj, 28, 214);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "");
        }
        {
            lv_obj_t *obj = lv_img_create(parent_obj);
            objects.obj0 = obj;
            lv_obj_set_pos(obj, 11, 214);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_img_set_src(obj, &img_temperature);
            lv_obj_set_style_img_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_img_recolor(obj, lv_color_hex(0xfffcfcfc), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_img_recolor_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_img_create(parent_obj);
            lv_obj_set_pos(obj, 148, 202);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_img_set_src(obj, &img_konkapp_batt2);
        }
        {
            // soc_lbl
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.soc_lbl = obj;
            lv_obj_set_pos(obj, 185, 214);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "");
        }
        {
            // bat_temp_lbl
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.bat_temp_lbl = obj;
            lv_obj_set_pos(obj, 114, 214);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "");
        }
        {
            // bat_mode_lbl
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.bat_mode_lbl = obj;
            lv_obj_set_pos(obj, 250, 214);
            lv_obj_set_size(obj, 55, LV_SIZE_CONTENT);
            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "");
        }
        {
            // trip_regen_lbl
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.trip_regen_lbl = obj;
            lv_obj_set_pos(obj, 168, 174);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff80ff80), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "+5.8");
        }
        {
            lv_obj_t *obj = lv_line_create(parent_obj);
            objects.obj1 = obj;
            lv_obj_set_pos(obj, 0, 202);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            static lv_point_t line_points[] = {
                { 10, 0 },
                { 310, 0 }
            };
            lv_line_set_points(obj, line_points, 2);
            lv_obj_set_style_line_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_line_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_line_opa(obj, 128, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // trip_lbl
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.trip_lbl = obj;
            lv_obj_set_pos(obj, 129, 158);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Trip kWh");
        }
        {
            // trip_used_lbl
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.trip_used_lbl = obj;
            lv_obj_set_pos(obj, 129, 174);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffff8080), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "-33.2");
        }
    }
    
    tick_screen_page1();
}

void tick_screen_page1() {
    {
        lv_meter_indicator_t *indicator;
        
        lv_ll_t *indicators = &((lv_meter_t *)objects.pri_motor_meter)->indicator_ll;
        int index = 0;
        for (indicator = _lv_ll_get_tail(indicators); index > 0 && indicator != NULL; indicator = _lv_ll_get_prev(indicators, indicator), index--);
        
        if (indicator) {
            int32_t new_val = get_var_pri_motor_power();
            int32_t cur_val = indicator->start_value;
            if (new_val != cur_val) {
                tick_value_change_obj = objects.pri_motor_meter;
                lv_meter_set_indicator_value(objects.pri_motor_meter, indicator, new_val);
                tick_value_change_obj = NULL;
            }
        }
    }
    {
        lv_meter_indicator_t *indicator;
        
        lv_ll_t *indicators = &((lv_meter_t *)objects.sec_motor_meter)->indicator_ll;
        int index = 0;
        for (indicator = _lv_ll_get_tail(indicators); index > 0 && indicator != NULL; indicator = _lv_ll_get_prev(indicators, indicator), index--);
        
        if (indicator) {
            int32_t new_val = get_var_sec_motor_power();
            int32_t cur_val = indicator->start_value;
            if (new_val != cur_val) {
                tick_value_change_obj = objects.sec_motor_meter;
                lv_meter_set_indicator_value(objects.sec_motor_meter, indicator, new_val);
                tick_value_change_obj = NULL;
            }
        }
    }
    {
        const char *new_val = get_var_pri_motor_power_str();
        const char *cur_val = lv_label_get_text(objects.pri_motor_power_lbl);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.pri_motor_power_lbl;
            lv_label_set_text(objects.pri_motor_power_lbl, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_sec_motor_power_str();
        const char *cur_val = lv_label_get_text(objects.sec_motor_power_lbl);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.sec_motor_power_lbl;
            lv_label_set_text(objects.sec_motor_power_lbl, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_power_str();
        const char *cur_val = lv_label_get_text(objects.power_lbl);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.power_lbl;
            lv_label_set_text(objects.power_lbl, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = get_var_power();
        int32_t cur_val = lv_bar_get_value(objects.power_bar);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.power_bar;
            lv_bar_set_value(objects.power_bar, new_val, LV_ANIM_ON);
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
}

void create_screen_page2() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.page2 = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 240);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 140, 112);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Hello!");
        }
    }
    
    tick_screen_page2();
}

void tick_screen_page2() {
}



typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_page1,
    tick_screen_page2,
};
void tick_screen(int screen_index) {
    tick_screen_funcs[screen_index]();
}
void tick_screen_by_id(enum ScreensEnum screenId) {
    tick_screen_funcs[screenId - 1]();
}

void create_screens() {
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    
    create_screen_page1();
    create_screen_page2();
}
