#include "../ui.h"

lv_obj_t *ui_subscreen_3 = NULL;

static lv_obj_t *ui_label_debug = NULL;
static lv_obj_t *ui_btn_back = NULL;

static void btn_back_event_cb(lv_event_t *e);

/**
 * @brief Initialize the subscreen 3
 *
 */
void ui_subscreen_3_init(void)
{
    // -- setup the subscreen 3 and configure its layout and style --
    ui_subscreen_3 = lv_obj_create(NULL);
    lv_obj_set_style_bg_img_src(ui_subscreen_3, &background, 0);

    static lv_style_t style_subscreen_3;
    lv_style_init(&style_subscreen_3);
    lv_style_set_text_font(&style_subscreen_3, &lv_font_montserrat_20);
    lv_style_set_text_color(&style_subscreen_3, UI_COLOR_LILA);
    lv_obj_add_style(ui_subscreen_3, &style_subscreen_3, 0);

    // -- create a back button to return to the main screen --
    ui_btn_back = lv_btn_create(ui_subscreen_3);
    lv_obj_set_size(ui_btn_back, 80, 80);
    lv_obj_align(ui_btn_back, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *label_btn_back = lv_label_create(ui_btn_back);
    lv_label_set_text(label_btn_back, LV_SYMBOL_LEFT "Back");
    lv_obj_center(label_btn_back);

    lv_obj_add_event_cb(ui_btn_back, btn_back_event_cb, LV_EVENT_CLICKED, NULL);

    // -- create a label for debugging purposes --
    ui_label_debug = lv_label_create(ui_subscreen_3);
    lv_label_set_text(ui_label_debug, "Subscreen 3");
    lv_obj_align(ui_label_debug, LV_ALIGN_BOTTOM_MID, 0, -20);
}

/**
 * @brief Destroy the subscreen 3
 *
 */
void ui_subscreen_3_destroy(void)
{
    if (ui_subscreen_3 != NULL)
    {
        lv_obj_del(ui_subscreen_3);
        ui_subscreen_3 = NULL;
    }
}

/**
 * @brief Event callback for the back button
 *
 * @param e
 */
static void btn_back_event_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) != LV_EVENT_CLICKED)
        return;

    // Navigate back to the main screen
    ui_main_screen_init();
    lv_disp_load_scr(ui_main_screen);

    // Clean up the subscreen 3 to free memory
    ui_subscreen_3_destroy();
}