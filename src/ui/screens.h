#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
    lv_obj_t *page1;
    lv_obj_t *page2;
    lv_obj_t *pri_motor_meter;
    lv_obj_t *sec_motor_meter;
    lv_obj_t *pri_motor_power_lbl;
    lv_obj_t *sec_motor_power_lbl;
    lv_obj_t *power_lbl;
    lv_obj_t *power_unit_lbl;
    lv_obj_t *power_bar;
    lv_obj_t *int_temp_lbl;
    lv_obj_t *obj0;
    lv_obj_t *soc_lbl;
    lv_obj_t *bat_temp_lbl;
    lv_obj_t *bat_mode_lbl;
    lv_obj_t *trip_regen_lbl;
    lv_obj_t *obj1;
    lv_obj_t *trip_lbl;
    lv_obj_t *trip_used_lbl;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_PAGE1 = 1,
    SCREEN_ID_PAGE2 = 2,
};

void create_screen_page1();
void tick_screen_page1();

void create_screen_page2();
void tick_screen_page2();

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/