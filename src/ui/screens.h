#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
    lv_obj_t *screen1;
    lv_obj_t *obj0;
    lv_obj_t *pri_mo;
    lv_obj_t *sec_mo;
    lv_obj_t *hvb_power_lbl;
    lv_obj_t *obj1;
    lv_obj_t *int_temp_lbl;
    lv_obj_t *obj2;
    lv_obj_t *soc_lbl;
    lv_obj_t *bat_temp_lbl;
    lv_obj_t *bat_mode_lbl;
    lv_obj_t *obj3;
    lv_obj_t *lvbsoc_lbl;
    lv_obj_t *lvbv_lbl;
    lv_obj_t *pri_mot_tq_lbl;
    lv_obj_t *sec_mot_tq_lbl;
    lv_obj_t *bat_temp_lbl_1;
    lv_obj_t *pri_mot_temp_lbl;
    lv_obj_t *cds_lbl;
    lv_obj_t *kwh_per_100km_lbl;
    lv_obj_t *obj4;
    lv_obj_t *obj5;
    lv_obj_t *obj6;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_SCREEN1 = 1,
};

void create_screen_screen1();
void tick_screen_screen1();

enum Themes {
    THEME_ID_LIGHT,
    THEME_ID_DARK,
};
enum Colors {
    COLOR_ID_BACKGROUND,
    COLOR_ID_FOREGROUND,
    COLOR_ID_DIAL_BG,
    COLOR_ID_SEC_DIAL_ACCENT,
};
void change_color_theme(uint32_t themeIndex);
extern uint32_t theme_colors[2][4];
extern uint32_t active_theme_index;

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/