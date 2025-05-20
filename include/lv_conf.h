/**
 * @file lv_conf.h
 * Configuration file for LVGL v8.3.6
 */

#ifndef LV_CONF_H
#define LV_CONF_H

#include <stdint.h>

/* Color depth: 1 (1 byte per pixel), 8 (RGB332), 16 (RGB565), 32 (ARGB8888) */
#define LV_COLOR_DEPTH 16

/* Use monochrome graphics for 1-bit color depth displays */
#define LV_COLOR_16_SWAP 1

/* 1: Enable anti-aliasing */
#define LV_ANTIALIAS 0

/* Default display refresh period */
#define LV_DISP_DEF_REFR_PERIOD 33

/* Default Dots Per Inch */
#define LV_DPI 130

/* More compact font storage */
#define LV_FONT_FMT_TXT_LARGE 0

#define LV_USE_PRINTF 1
#define LV_SPRINTF_USE_FLOAT 1

#define LV_USE_SYSMON 1
// #define LV_USE_MEM_MONITOR 1

/* Enable features */
#define LV_USE_ANIMATION 1
#define LV_USE_SHADOW 0
#define LV_USE_BLEND_MODES 0
#define LV_USE_OPA_SCALE 0
#define LV_USE_IMG_TRANSFORM 0
#define LV_USE_GROUP 1
#define LV_USE_FILESYSTEM 0
#define LV_USE_USER_DATA 1
#define LV_USE_PERF_MONITOR 1
#define LV_USE_API_EXTENSION_V6 0
#define LV_USE_API_EXTENSION_V7 0

/* Font usage */
#define LV_FONT_MONTSERRAT_8 0
#define LV_FONT_MONTSERRAT_10 1
#define LV_FONT_MONTSERRAT_12 1
#define LV_FONT_MONTSERRAT_14 0
#define LV_FONT_MONTSERRAT_16 0
#define LV_FONT_MONTSERRAT_18 0
#define LV_FONT_MONTSERRAT_20 0
#define LV_FONT_MONTSERRAT_22 0
#define LV_FONT_MONTSERRAT_24 0
#define LV_FONT_MONTSERRAT_26 1
#define LV_FONT_MONTSERRAT_28 0
#define LV_FONT_MONTSERRAT_30 0
#define LV_FONT_MONTSERRAT_32 0
#define LV_FONT_MONTSERRAT_34 0
#define LV_FONT_MONTSERRAT_36 0
#define LV_FONT_MONTSERRAT_38 0
#define LV_FONT_MONTSERRAT_40 1
#define LV_FONT_MONTSERRAT_42 0
#define LV_FONT_MONTSERRAT_44 0
#define LV_FONT_MONTSERRAT_46 0
#define LV_FONT_MONTSERRAT_48 0

/* Default font */
#define LV_FONT_DEFAULT &lv_font_montserrat_12

/* Enable widgets */
#define LV_USE_LABEL 1
#define LV_USE_IMG 1
#define LV_USE_LINE 1
#define LV_USE_ARC 1
#define LV_USE_BTN 1
#define LV_USE_BTNMATRIX 1
#define LV_USE_BAR 1
#define LV_USE_SLIDER 0
#define LV_USE_TABLE 0
#define LV_USE_CHECKBOX 0
#define LV_USE_DROPDOWN 0
#define LV_USE_ROLLER 0
#define LV_USE_TEXTAREA 1
#define LV_USE_CANVAS 0
#define LV_USE_CHART 0
#define LV_USE_LIST 0
#define LV_USE_WIN 0
#define LV_USE_TABVIEW 0
#define LV_USE_TILEVIEW 0
#define LV_USE_CALENDAR 0
#define LV_USE_SPINBOX 0
#define LV_USE_SWITCH 0
#define LV_USE_MSGBOX 0

#define LV_USE_METER 1
// #define LV_DRAW_COMPLEX 1

/* Define memory size for LVGL */
#define LV_MEM_CUSTOM 0
#define LV_MEM_SIZE (64 * 1024U) /* Size of memory used by LVGL (in bytes) */
#define LV_MEM_ADR 0             /* 0: unused */
#define LV_MEM_ATTR              /* Complier prefix for big arrays */
#define LV_MEM_AUTO_DEFRAG 1     /* Automatically defrag on free */

/* Enable GPU */
#define LV_USE_GPU 0
#define LV_USE_GPU_STM32_DMA2D 0
#define LV_USE_GPU_NXP_PXP 0
#define LV_USE_GPU_NXP_VG_LITE 0
#define LV_USE_GPU_SDL 0

/* Logger settings */
#define LV_USE_LOG 1

// #define LV_LOG_TRACE_TIMER 1

/* Build settings */
#define LV_BUILD_EXAMPLES 0

/* Set the log level */
#ifdef LV_USE_LOG
#define LV_LOG_LEVEL LV_LOG_LEVEL_WARN
#endif

// /*Enable asserts if an operation is failed or an invalid data is found.
//  *If LV_USE_LOG is enabled an error message will be printed on failure*/
// #define LV_USE_ASSERT_NULL          1   /*Check if the parameter is NULL. (Very fast, recommended)*/
// #define LV_USE_ASSERT_MALLOC        1   /*Checks is the memory is successfully allocated or no. (Very fast, recommended)*/
// #define LV_USE_ASSERT_STYLE         1   /*Check if the styles are properly initialized. (Very fast, recommended)*/
// #define LV_USE_ASSERT_MEM_INTEGRITY 1   /*Check the integrity of `lv_mem` after critical operations. (Slow)*/
// #define LV_USE_ASSERT_OBJ           1   /*Check the object's type and existence (e.g. not deleted). (Slow)*/

// /*Add a custom handler when assert happens e.g. to restart the MCU*/
// #define LV_ASSERT_HANDLER_INCLUDE <stdint.h>
// #define LV_ASSERT_HANDLER while(1);   /*Halt by default*/


#define LV_TICK_CUSTOM 1
#if LV_TICK_CUSTOM
#define LV_TICK_CUSTOM_INCLUDE "Arduino.h"      /*Header for the system time function*/
#define LV_TICK_CUSTOM_SYS_TIME_EXPR (millis()) /*Expression evaluating to current system time in ms*/
#endif                                          /*LV_TICK_CUSTOM*/

// /*Define a custom attribute to `lv_tick_inc` function*/
// #define LV_ATTRIBUTE_TICK_INC

// /*Define a custom attribute to `lv_timer_handler` function*/
// #define LV_ATTRIBUTE_TIMER_HANDLER

/*Define a custom attribute to `lv_display_flush_ready` function*/
#define LV_ATTRIBUTE_FLUSH_READY

/*Required alignment size for buffers*/
#define LV_ATTRIBUTE_MEM_ALIGN_SIZE 1

/*Will be added where memories needs to be aligned (with -Os data might not be aligned to boundary by default).
 * E.g. __attribute__((aligned(4)))*/
#define LV_ATTRIBUTE_MEM_ALIGN

/*Attribute to mark large constant arrays for example font's bitmaps*/
#define LV_ATTRIBUTE_LARGE_CONST

/*Compiler prefix for a big array declaration in RAM*/
#define LV_ATTRIBUTE_LARGE_RAM_ARRAY

/*Place performance critical functions into a faster memory (e.g RAM)*/
#define LV_ATTRIBUTE_FAST_MEM

/*Export integer constant to binding. This macro is used with constants in the form of LV_<CONST> that
 *should also appear on LVGL binding API such as Micropython.*/
#define LV_EXPORT_CONST_INT(int_value) struct _silence_gcc_warning /*The default value just prevents GCC warning*/

/*Prefix all global extern data with this*/
#define LV_ATTRIBUTE_EXTERN_DATA


/*Align the stride of all layers and images to this bytes*/
#define LV_DRAW_BUF_STRIDE_ALIGN 1

/*Align the start address of draw_buf addresses to this bytes*/
#define LV_DRAW_BUF_ALIGN

/*The target buffer size for simple layer chunks.*/
#define LV_DRAW_LAYER_SIMPLE_BUF_SIZE    (24 * 1024)   /*[bytes]*/

#endif /* LV_CONF_H */