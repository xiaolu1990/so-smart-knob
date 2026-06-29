#include "../ui.h"

lv_obj_t *ui_subscreen_1 = NULL;

static lv_obj_t *ui_label_debug = NULL;
static lv_obj_t *ui_btn_back = NULL;
static lv_obj_t *ui_spinner = NULL;

static void btn_back_event_cb(lv_event_t *e);

/**
 * @brief Initialize the subscreen 1
 *
 */
void ui_subscreen_1_init(void)
{
    // -- setup the subscreen 1 and configure its layout and style --
    ui_subscreen_1 = lv_obj_create(NULL);
    lv_obj_set_style_bg_img_src(ui_subscreen_1, &background, 0);

    static lv_style_t style_subscreen_1;
    lv_style_init(&style_subscreen_1);
    lv_style_set_text_font(&style_subscreen_1, &lv_font_montserrat_20);
    lv_style_set_text_color(&style_subscreen_1, UI_COLOR_LILA);
    lv_obj_add_style(ui_subscreen_1, &style_subscreen_1, 0);

    lv_obj_set_layout(ui_subscreen_1, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(ui_subscreen_1, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_subscreen_1,
                          LV_FLEX_ALIGN_CENTER,      // Main axis (Vertical: distribute from center)
                          LV_FLEX_ALIGN_CENTER,      // Cross axis (Horizontal: center align items)
                          LV_FLEX_ALIGN_CENTER);     // Track alignment
    lv_obj_set_style_pad_row(ui_subscreen_1, 20, 0); // Set vertical padding between items

    // -- create a spinner to indicate the scanning process --
    ui_spinner = lv_spinner_create(ui_subscreen_1, 1000, 60);
    lv_obj_set_size(ui_spinner, 100, 100);
    lv_obj_set_style_arc_color(ui_spinner, UI_COLOR_LILA, LV_PART_INDICATOR); 
    lv_obj_set_style_arc_color(ui_spinner, UI_COLOR_LIGHT_GRAY, LV_PART_MAIN); 

    // -- create a back button to return to the main screen --
    ui_btn_back = lv_btn_create(ui_subscreen_1);
    lv_obj_set_size(ui_btn_back, 80, 80); 
    lv_obj_set_style_bg_opa(ui_btn_back, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(ui_btn_back, 0, 0);
    
    // -- create a label for the back button --
    lv_obj_t *label_btn_back = lv_label_create(ui_btn_back);
    lv_label_set_text(label_btn_back, LV_SYMBOL_LEFT "Back");
    lv_obj_center(label_btn_back);
    lv_obj_set_style_text_color(label_btn_back, UI_COLOR_LILA, 0);
    lv_obj_set_style_text_font(label_btn_back, &lv_font_montserrat_24, 0);

    lv_obj_add_event_cb(ui_btn_back, btn_back_event_cb, LV_EVENT_CLICKED, NULL);

    // -- create a label for debugging purposes --
    ui_label_debug = lv_label_create(ui_subscreen_1);
    lv_label_set_text(ui_label_debug, "Subscreen 1");
    lv_obj_align(ui_label_debug, LV_ALIGN_BOTTOM_MID, 0, -20);
}

/**
 * @brief Destroy the subscreen 1
 *
 */
void ui_subscreen_1_destroy(void)
{
    if (ui_subscreen_1 != NULL)
    {
        lv_obj_del(ui_subscreen_1);
        ui_subscreen_1 = NULL;
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

    // Clean up the subscreen 1 to free memory
    ui_subscreen_1_destroy();
}