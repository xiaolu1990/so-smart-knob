#include "ui.h"
#include "ui_helpers.h"
#include "esp_log.h"

static const char *TAG = "ui";
///////////////////// VARIABLES ////////////////////

lv_obj_t *ui_initial_actions;
lv_group_t *g;

#if LV_COLOR_DEPTH != 16
#error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP != 0
#error "LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
#endif

//********************************* */
/**
 * @brief handle knob event, and update the UI accordingly
 *
 * @param event
 */
void LVGL_knob_event(ui_knob_event_t event)
{
    if (lv_scr_act() == ui_sleep_screen)
    {
        ui_sleep_screen_knob_rotate();
    }

    if (lv_scr_act() == ui_main_screen)
    {
        ui_main_screen_knob_rotate(event);
    }

    event = UI_KNOB_NONE; // Reset the event after handling

    return;
}

/**
 * @brief handle button event, and update the UI accordingly
 *
 * @param event
 */
void LVGL_button_event(ui_button_event_t event)
{
    if (lv_scr_act() == ui_main_screen)
    {
        ui_main_screen_knob_press(event);
    }

    event = UI_BUTTON_NONE_PRESS; // Reset the event after handling
    
    return;
}

/**
 * @brief user modified ui init function, called in main.cpp
 *
 */
void ui_init(void)
{

    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);

    lv_scr_load(lv_obj_create(NULL)); // 清理默认屏幕

    ui_splash_screen_init();
    ui_sleep_screen_init();

    ui_initial_actions = lv_obj_create(NULL);

    lv_disp_load_scr(ui_splash_screen); // only load the splash screen at startup
}
