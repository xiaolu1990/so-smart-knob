#include "../ui.h"

lv_obj_t *ui_splash_screen = NULL;
lv_obj_t *ui_main_screen = NULL;
lv_obj_t *ui_label_scan_target = NULL;

static void splash_timer_cb(lv_timer_t *timer);
static void create_main_screen(void);

/**
 * @brief Initialize the main screen
 *
 */
void ui_main_screen_init(void)
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
    create_main_screen();

    // 2. Clean up the splash screen to free memory
    if (ui_splash_screen != NULL)
    {
        lv_obj_del(ui_splash_screen);
        ui_splash_screen = NULL;
    }
}

/**
 * @brief Create and load the main screen
 *
 */
static void create_main_screen(void)
{
    ui_main_screen = lv_obj_create(NULL);
    ui_label_scan_target = lv_label_create(ui_main_screen);

    static lv_style_t style;
    lv_style_init(&style);

    // setup the default style for the main screen
    lv_style_set_text_font(&style, &lv_font_montserrat_20);
    lv_style_set_text_color(&style, UI_COLOR_LILA);
    
    lv_obj_add_style(ui_label_scan_target, &style, 0);

    lv_obj_set_style_bg_img_src(ui_main_screen, &background, 0);

    lv_label_set_text(ui_label_scan_target, "Scan Target");
    lv_obj_align(ui_label_scan_target, LV_ALIGN_CENTER, 0, 0);
  

    lv_scr_load(ui_main_screen);
}