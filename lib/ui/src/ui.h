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

#define SCREEN_CENTER_X (SCREEN_WIDTH / 2)
#define SCREEN_CENTER_Y (SCREEN_HEIGHT / 2)

// GENERAL SETTINGS
#define SLEEP_TIMEOUT_MS 10000 // go to sleep after 10 seconds of inactivity

// COLORS
#define UI_COLOR_BLACK lv_color_hex(0x000000)
#define UI_COLOR_WHITE lv_color_hex(0xFFFFFF)
#define UI_COLOR_LILA lv_color_hex(0x622181)       // #622181
#define UI_COLOR_LIGHT_GRAY lv_color_hex(0xD3D3D3) // #D3D3D3

    // IMAGES AND IMAGE SETS
    LV_IMG_DECLARE(splash_screen);
    LV_IMG_DECLARE(background);
    LV_IMG_DECLARE(lower_jaw_64x64);
    LV_IMG_DECLARE(upper_jaw_64x64);
    LV_IMG_DECLARE(multcase_tray_64x64);
    LV_IMG_DECLARE(universal_scan_64x64);
    LV_IMG_DECLARE(correction_scan_64x64);
    LV_IMG_DECLARE(busy);
    LV_IMG_DECLARE(back);
    LV_IMG_DECLARE(start_scan);
    LV_IMG_DECLARE(play);
    LV_IMG_DECLARE(hr_on);
    LV_IMG_DECLARE(hr_off);
    LV_IMG_DECLARE(enhanced_global_scan_on);
    LV_IMG_DECLARE(enhanced_global_scan_off);
    LV_IMG_DECLARE(service_position);

    // SCREEN: ui_splash_screen
    void ui_splash_screen_init(void);

    extern lv_obj_t *ui_splash_screen; // splash screen object

    // SCREEN: ui_main_screen
    void ui_main_screen_init(void);
    uint8_t ui_main_screen_get_icon_id(void);
    void ui_main_screen_knob_rotate(int dir);
    void ui_main_screen_knob_activate_selected(void);

    extern lv_obj_t *ui_main_screen;                 // main screen object
    extern lv_obj_t *ui_main_menu_selected_btn;      // the currently selected button in the menu container
    extern uint32_t ui_main_menu_selected_btn_index; // the index of the currently selected button in the menu container
    extern ui_icon_t main_menu_icons[];              // array of main menu icons

    // SCREEN: ui_conf_screen
    void ui_conf_screen_init(uint8_t main_menu_icon_id);
    void ui_conf_screen_destroy(void);

    extern lv_obj_t *ui_conf_screen; // configuration screen object
    extern bool state_enable_hr;     // default state for HR icon, true=ON, false=OFF
    extern bool state_egs;           // default state for Enhanced Global Scan icon, true=ON, false=OFF

    // SCREEN: ui_scan_screen
    void ui_scan_screen_init(uint8_t main_menu_icon_id);
    void ui_scan_screen_destroy(void);

    extern lv_obj_t *ui_scan_screen;    // scan screen object
    extern bool state_scan_in_progress; // default state for scanning, false=not scanning, true=scanning

    // SCREEN: ui_sleep_screen
    void ui_sleep_screen_init(void);
    void ui_sleep_screen_destroy(void);
    void ui_sleep_screen_enter(void);
    void ui_sleep_screen_wakeup(lv_event_t *e);
    void ui_sleep_screen_knob_rotate(void);

    extern lv_obj_t *ui_sleep_screen; // sleep screen object

    extern bool is_sleeping;               // flag to indicate if the device is in sleep mode
    extern lv_timer_t *screen_sleep_timer; // Timer to check for sleep timeout
    extern lv_obj_t *ui_initial_actions;

    // Function declarations in ui.c
    void LVGL_knob_event(ui_knob_event_t event);
    void LVGL_button_event(ui_button_event_t event);
    void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /* _UI_H */
