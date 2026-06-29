#ifndef _UI_H
#define _UI_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "lvgl.h"

#include "ui_helpers.h"
#include "ui_events.h"

    // Typedefs
    typedef struct
    {
        const lv_img_dsc_t *image;
        const char *text;
        uint8_t id;
    } ui_icon_t;

    typedef enum
    {
        UI_KNOB_LEFT = 0,  /*!< EVENT: Rotate to the left */
        UI_KNOB_RIGHT,     /*!< EVENT: Rotate to the right */
        UI_KNOB_H_LIM,     /*!< EVENT: Count reaches maximum limit */
        UI_KNOB_L_LIM,     /*!< EVENT: Count reaches the minimum limit */
        UI_KNOB_ZERO,      /*!< EVENT: Count back to 0 */
        UI_KNOB_EVENT_MAX, /*!< EVENT: Number of events */
        UI_KNOB_NONE,      /*!< EVENT: No event */
    } ui_knob_event_t;

    typedef enum
    {
        UI_BUTTON_PRESS_DOWN = 0,
        UI_BUTTON_PRESS_UP,
        UI_BUTTON_PRESS_REPEAT,
        UI_BUTTON_PRESS_REPEAT_DONE,
        UI_BUTTON_SINGLE_CLICK,
        UI_BUTTON_DOUBLE_CLICK,
        UI_BUTTON_MULTIPLE_CLICK,
        UI_BUTTON_LONG_PRESS_START,
        UI_BUTTON_LONG_PRESS_HOLD,
        UI_BUTTON_LONG_PRESS_UP,
        UI_BUTTON_PRESS_END,
        UI_BUTTON_EVENT_MAX,
        UI_BUTTON_NONE_PRESS,
    } ui_button_event_t;

// SCREEN PROPERTIES
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 480

// COLORS
#define UI_COLOR_BLACK lv_color_hex(0x000000)
#define UI_COLOR_WHITE lv_color_hex(0xFFFFFF)
#define UI_COLOR_LILA lv_color_hex(0x622181) // #622181

    // IMAGES AND IMAGE SETS
    LV_IMG_DECLARE(splash_screen);
    LV_IMG_DECLARE(background);
    LV_IMG_DECLARE(lower_jaw_64x64);
    LV_IMG_DECLARE(upper_jaw_64x64);
    LV_IMG_DECLARE(multcase_tray_64x64);
    LV_IMG_DECLARE(universal_scan_64x64);
    LV_IMG_DECLARE(correction_scan_64x64);

    // SCREEN: ui_splash_screen
    void ui_splash_screen_init(void);

    // SCREEN: ui_main_screen
    void ui_main_screen_init(void);
    void ui_main_screen_knob_rotate(int dir);
    void ui_main_screen_knob_activate_selected(void);

    // SCREEN: ui_subscreen_1
    void ui_subscreen_1_init(void);
    void ui_subscreen_1_destroy(void);

    // SCREEN: ui_subscreen_2
    void ui_subscreen_2_init(void);
    void ui_subscreen_2_destroy(void);

    // SCREEN: ui_subscreen_3
    void ui_subscreen_3_init(void);
    void ui_subscreen_3_destroy(void);

    // SCREEN: ui_subscreen_4
    void ui_subscreen_4_init(void);
    void ui_subscreen_4_destroy(void);

    // SCREEN: ui_subscreen_5
    void ui_subscreen_5_init(void);
    void ui_subscreen_5_destroy(void);

    extern lv_obj_t *ui_splash_screen; // splash screen object
    extern lv_obj_t *ui_main_screen;   // main screen object
    extern lv_obj_t *ui_subscreen_1;   // subscreen 1 object
    extern lv_obj_t *ui_subscreen_2;   // subscreen 2 object
    extern lv_obj_t *ui_subscreen_3;   // subscreen 3 object
    extern lv_obj_t *ui_subscreen_4;   // subscreen 4 object
    extern lv_obj_t *ui_subscreen_5;   // subscreen 5 object

    extern lv_obj_t *ui____initial_actions0;

    // Function declarations in ui.c
    void LVGL_knob_event(ui_knob_event_t event);
    void LVGL_button_event(ui_button_event_t event);
    void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /* _UI_H */
