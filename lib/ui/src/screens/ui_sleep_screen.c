#include "../ui.h"

lv_obj_t *ui_sleep_screen = NULL;

static lv_obj_t *previous_screen = NULL;
static lv_obj_t *ui_debug_label = NULL;

/**
 * @brief Initialize the sleep screen
 *
 */
void ui_sleep_screen_init(void)
{
    ui_sleep_screen = lv_obj_create(NULL);

    lv_obj_set_style_bg_img_src(ui_sleep_screen, &splash_screen, 0);

    lv_obj_add_event_cb(ui_sleep_screen, ui_sleep_screen_wakeup, LV_EVENT_PRESSED, NULL);

    ui_debug_label = lv_label_create(ui_sleep_screen);
    lv_obj_set_style_text_font(ui_debug_label, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(ui_debug_label, UI_COLOR_LILA, 0);
    lv_label_set_text(ui_debug_label, "Sleep Screen");
    lv_obj_align(ui_debug_label, LV_ALIGN_BOTTOM_MID, 0, -20);
}

/**
 * @brief Destroy the sleep screen
 *
 */
void ui_sleep_screen_destroy(void)
{
    if (ui_sleep_screen != NULL)
    {
        lv_obj_del(ui_sleep_screen);
        ui_sleep_screen = NULL;
    }
}

/**
 * @brief Enter the sleep screen
 *
 */
void ui_sleep_screen_enter(void)
{
    if (is_sleeping)
        return;

    previous_screen = lv_scr_act(); // Save the current screen so it can be restored later after waking up

    if (ui_sleep_screen == NULL)
    {
        ui_sleep_screen_init();
    }
    lv_scr_load(ui_sleep_screen); // Load the sleep screen

    is_sleeping = true; // Set the sleep state
}

/**
 * @brief Callback function for waking up the sleep screen
 *
 * @param e The event object
 */
void ui_sleep_screen_wakeup(lv_event_t *e)
{
    if (lv_event_get_code(e) != LV_EVENT_PRESSED)
        return;

    if (!is_sleeping)
        return;

    is_sleeping = false;                // Reset the sleep state
    lv_disp_trig_activity(NULL);        // Reset LVGL inactivity timer immediately
    lv_timer_reset(screen_sleep_timer); // Reset the sleep timer to avoid immediate re-entry into sleep mode

    lv_scr_load(previous_screen); // Load the previous screen

    ui_sleep_screen_destroy(); // Destroy the sleep screen to free up resources
}

void ui_sleep_screen_knob_rotate(void)
{
    if (ui_sleep_screen == NULL)
        return;

    is_sleeping = false;                // Reset the sleep state
    lv_disp_trig_activity(NULL);        // Reset LVGL inactivity timer immediately
    lv_timer_reset(screen_sleep_timer); // Reset the sleep timer to avoid immediate re-entry into sleep mode

    lv_scr_load(previous_screen); // Load the previous screen

    ui_sleep_screen_destroy(); // Destroy the sleep screen to free up resources
}