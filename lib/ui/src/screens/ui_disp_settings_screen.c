#include "../ui.h"

lv_obj_t *ui_disp_settings_screen = NULL;

static void switch_ui_style_cb(lv_event_t *e);

/**
 * @brief Callback function to switch the main screen UI style (card menu or radial menu)
 *
 * @param e
 */
static void switch_ui_style_cb(lv_event_t *e)
{
    lv_obj_t *target = lv_event_get_target(e);
    if (target == NULL)
        return;

    if (lv_event_get_code(e) != LV_EVENT_CLICKED)
        return;

    if (target == lv_obj_get_child(ui_disp_settings_screen, 0)) // Card menu button
    {
        ui_main_screen_style = UI_STYLE_CARD_MENU;
    }
    else if (target == lv_obj_get_child(ui_disp_settings_screen, 1)) // Radial menu button
    {
        ui_main_screen_style = UI_STYLE_RADIAL_MENU;
    }
   
    // After changing the style, go to the main screen
    ui_main_screen_destroy(); // destroy the existing main screen then recreate
    ui_main_screen_init(ui_main_screen_style);
    lv_disp_load_scr(ui_main_screen);

    // Clean up the display settings screen to free memory
    ui_display_settings_screen_destroy();
}

/**
 * @brief Initialize the display settings screen
 *
 */
void ui_display_settings_screen_init(void)
{
    ui_disp_settings_screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(ui_disp_settings_screen, UI_COLOR_LILA, 0);

    static lv_style_t style_disp_settings_screen;
    lv_style_init(&style_disp_settings_screen);
    lv_style_set_text_font(&style_disp_settings_screen, &lv_font_montserrat_20);
    lv_style_set_text_color(&style_disp_settings_screen, UI_COLOR_LILA);

    lv_obj_add_style(ui_disp_settings_screen, &style_disp_settings_screen, 0);

    // create 2 buttons to select the main screen ui style (card menu or radial menu)
    lv_obj_t *btn_card_menu = lv_btn_create(ui_disp_settings_screen);
    lv_obj_set_size(btn_card_menu, 100, 100);
    lv_obj_set_style_bg_color(btn_card_menu, UI_COLOR_YELLOW, 0);
    lv_obj_set_style_radius(btn_card_menu, LV_RADIUS_CIRCLE, 0);
    lv_obj_align(btn_card_menu, LV_ALIGN_CENTER, -100, 0);

    lv_obj_t *label_card_menu = lv_label_create(btn_card_menu);
    lv_label_set_text(label_card_menu, "Card");
    lv_obj_set_style_text_color(label_card_menu, UI_COLOR_LILA, 0);
    lv_obj_center(label_card_menu);

    lv_obj_add_event_cb(btn_card_menu, switch_ui_style_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *btn_radial_menu = lv_btn_create(ui_disp_settings_screen);
    lv_obj_set_size(btn_radial_menu, 100, 100);
    lv_obj_set_style_bg_color(btn_radial_menu, UI_COLOR_YELLOW, 0);
    lv_obj_set_style_radius(btn_radial_menu, LV_RADIUS_CIRCLE, 0);
    lv_obj_align(btn_radial_menu, LV_ALIGN_CENTER, 100, 0);

    lv_obj_t *label_radial_menu = lv_label_create(btn_radial_menu);
    lv_label_set_text(label_radial_menu, "Radial");
    lv_obj_set_style_text_color(label_radial_menu, UI_COLOR_LILA, 0);
    lv_obj_center(label_radial_menu);

    lv_obj_add_event_cb(btn_radial_menu, switch_ui_style_cb, LV_EVENT_CLICKED, NULL);

    // Create a label for the display settings screen
    lv_obj_t *label = lv_label_create(ui_disp_settings_screen);
    lv_label_set_text(label, "Display Settings");
    lv_obj_align(label, LV_ALIGN_BOTTOM_MID, 0, -20);
}

/**
 * @brief Destroy the display settings screen and free associated resources
 *
 */
void ui_display_settings_screen_destroy(void)
{
    if (ui_disp_settings_screen != NULL)
    {
        lv_obj_del(ui_disp_settings_screen);
        ui_disp_settings_screen = NULL;
    }
}