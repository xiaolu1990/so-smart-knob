#include "../ui.h"

lv_obj_t *ui_splash_screen = NULL;

static void splash_timer_cb(lv_timer_t *timer);

/**
 * @brief Initialize the splash screen
 *
 */
void ui_splash_screen_init(void)
{
    ui_splash_screen = lv_obj_create(NULL);

    // 1. Create a splash screen then navigate to the main screen after a delay
    lv_obj_set_style_bg_img_src(ui_splash_screen, &splash_screen, 0);

    // Create a timer to transition from the splash screen to the main screen after 2 seconds
    lv_timer_t *timer = lv_timer_create(splash_timer_cb, 2000, NULL);
    lv_timer_set_repeat_count(timer, 1); // set the timer to run only once
}

/**
 * @brief Splash screen timer callback
 *
 * @param timer
 */
static void splash_timer_cb(lv_timer_t *timer)
{
    // 1. Create and load the main screen
    ui_main_screen_init(ui_main_screen_style);
    lv_disp_load_scr(ui_main_screen);

    // 2. Clean up the splash screen to free memory
    if (ui_splash_screen != NULL)
    {
        lv_obj_del(ui_splash_screen);
        ui_splash_screen = NULL;
    }
}