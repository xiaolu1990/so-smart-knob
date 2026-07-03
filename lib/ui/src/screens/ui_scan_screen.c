#include "../ui.h"

lv_obj_t *ui_scan_screen = NULL;

bool state_scan_in_progress = false; // default state for scanning, false=not scanning, true=scanning

static lv_obj_t *ui_container_scan_strategy = NULL;
static lv_obj_t *ui_img_scan_strategy = NULL;
static lv_obj_t *ui_spinner_scan_busy = NULL;
static lv_obj_t *ui_spinner_scan_busy_static = NULL; // static arc to show when scanning is not in progress
static lv_obj_t *ui_scan_screen_menu_container = NULL;
static lv_obj_t *ui_label_debug = NULL;

static const char *icon_label_texts[] = {
    LV_SYMBOL_PLAY, LV_SYMBOL_STOP, LV_SYMBOL_HOME};

static void menu_icon_click_event_cb(lv_event_t *e);

void ui_scan_screen_init(uint8_t main_menu_icon_id)
{
    // setup the scan screen and configure the layout and styles
    ui_scan_screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_img_src(ui_scan_screen, &background, 0);

    static lv_style_t style_scan_screen;
    lv_style_init(&style_scan_screen);
    lv_style_set_text_font(&style_scan_screen, &lv_font_montserrat_20);
    lv_style_set_text_color(&style_scan_screen, UI_COLOR_LILA);
    lv_obj_add_style(ui_scan_screen, &style_scan_screen, 0);

    lv_obj_set_layout(ui_scan_screen, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(ui_scan_screen, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_scan_screen,
                          LV_FLEX_ALIGN_CENTER,      // Main axis (Vertical: distribute from center)
                          LV_FLEX_ALIGN_CENTER,      // Cross axis (Horizontal: center align items)
                          LV_FLEX_ALIGN_CENTER);     // Track alignment
    lv_obj_set_style_pad_row(ui_scan_screen, 20, 0); // Set vertical padding between items

    // -- create a container for scan strategy image and progress --
    ui_container_scan_strategy = lv_obj_create(ui_scan_screen);
    lv_obj_set_size(ui_container_scan_strategy, 200, 200);
    lv_obj_set_style_bg_opa(ui_container_scan_strategy, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(ui_container_scan_strategy, 0, 0);

    ui_img_scan_strategy = lv_img_create(ui_container_scan_strategy);
    lv_obj_center(ui_img_scan_strategy);
    lv_img_set_src(ui_img_scan_strategy, main_menu_icons[main_menu_icon_id].image);
    lv_img_set_zoom(ui_img_scan_strategy, 320); // 1.25x zoom

    ui_spinner_scan_busy = lv_spinner_create(ui_container_scan_strategy, 1500, 60);
    lv_obj_set_size(ui_spinner_scan_busy, 150, 150);
    lv_obj_add_flag(ui_spinner_scan_busy, LV_OBJ_FLAG_HIDDEN); // initially hide the spinner, it will be shown when scanning starts
    lv_obj_center(ui_spinner_scan_busy);
    lv_obj_set_style_arc_color(ui_spinner_scan_busy, UI_COLOR_LILA, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(ui_spinner_scan_busy, UI_COLOR_LIGHT_GRAY, LV_PART_MAIN);

    // create a normal static arc as default
    ui_spinner_scan_busy_static = lv_arc_create(ui_container_scan_strategy);
    lv_obj_set_size(ui_spinner_scan_busy_static, 150, 150);
    lv_obj_center(ui_spinner_scan_busy_static);
    lv_arc_set_bg_angles(ui_spinner_scan_busy_static, 0, 360); // Make background a full circle
    lv_arc_set_angles(ui_spinner_scan_busy_static, 0, 60);     // Indicator length is 60 degrees
    lv_obj_set_style_arc_color(ui_spinner_scan_busy_static, UI_COLOR_LIGHT_GRAY, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(ui_spinner_scan_busy_static, UI_COLOR_LIGHT_GRAY, LV_PART_MAIN);
    lv_obj_remove_style(ui_spinner_scan_busy_static, NULL, LV_PART_KNOB); // Remove the grab handle knob

    // -- create menu container (flex row layout) to hold the menu icons --
    ui_scan_screen_menu_container = lv_obj_create(ui_scan_screen);
    lv_obj_set_size(ui_scan_screen_menu_container, SCREEN_WIDTH, 100);
    lv_obj_set_layout(ui_scan_screen_menu_container, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(ui_scan_screen_menu_container, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_scan_screen_menu_container,
                          LV_FLEX_ALIGN_CENTER,                        // Main axis (Horizontal: distribute from center)
                          LV_FLEX_ALIGN_CENTER,                        // Cross axis (Vertical: center align items)
                          LV_FLEX_ALIGN_CENTER);                       // Track alignment
    lv_obj_set_style_pad_column(ui_scan_screen_menu_container, 30, 0); // Set horizontal padding between items
    lv_obj_set_style_bg_opa(ui_scan_screen_menu_container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(ui_scan_screen_menu_container, 0, 0);

    for (uint32_t i = 0; i < 3; i++)
    {
        lv_obj_t *button = lv_btn_create(ui_scan_screen_menu_container);
        lv_obj_set_size(button, 80, 80);
        lv_obj_set_style_radius(button, LV_RADIUS_CIRCLE, LV_PART_MAIN);
        lv_obj_set_style_bg_color(button, UI_COLOR_LILA, LV_PART_MAIN);
        lv_obj_set_style_bg_opa(button, LV_OPA_50, 0);
        lv_obj_set_style_border_width(button, 0, 0);

        lv_obj_t *button_label = lv_label_create(button);
        lv_label_set_text(button_label, icon_label_texts[i]);
        lv_obj_center(button_label);
        lv_obj_set_style_text_font(button_label, &lv_font_montserrat_34, 0);
        lv_obj_set_style_text_color(button_label, UI_COLOR_LIGHT_GRAY, 0);

        lv_obj_add_event_cb(button, menu_icon_click_event_cb, LV_EVENT_CLICKED, (void *)(uintptr_t)i);
    }

    // -- setup the debug label to display debug information --
    ui_label_debug = lv_label_create(ui_scan_screen);
    lv_obj_add_flag(ui_label_debug, LV_OBJ_FLAG_FLOATING); // remove it from flex layout so it doesn't affect the layout of other elements
    lv_obj_align(ui_label_debug, LV_ALIGN_BOTTOM_MID, 0, -20);
    lv_label_set_text(ui_label_debug, "Scan Screen");

    lv_obj_update_layout(ui_scan_screen); // Update the layout to ensure all child objects are positioned correctly
}

void ui_scan_screen_destroy(void)
{
    if (ui_scan_screen != NULL)
    {
        lv_obj_del(ui_scan_screen);
        state_scan_in_progress = false;
        ui_scan_screen = NULL;
        ui_img_scan_strategy = NULL;
        ui_spinner_scan_busy = NULL;
        ui_scan_screen_menu_container = NULL;
        ui_label_debug = NULL;
    }
}

static void menu_icon_click_event_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) != LV_EVENT_CLICKED)
        return;

    uint32_t idx = (uint32_t)(uintptr_t)lv_event_get_user_data(e);

    switch (idx)
    {
    case 0: // Play
        state_scan_in_progress = true;
        lv_obj_add_flag(ui_spinner_scan_busy_static, LV_OBJ_FLAG_HIDDEN);   // hide the static spinner arc when scanning stops
        lv_obj_clear_flag(ui_spinner_scan_busy, LV_OBJ_FLAG_HIDDEN); // show the spinner when scanning starts

        if (state_enable_hr && state_egs)
            lv_label_set_text_static(ui_label_debug, "Scan Started\nHR ON, EGS ON");
        else if (state_enable_hr && !state_egs)
            lv_label_set_text_static(ui_label_debug, "Scan Started\nHR ON, EGS OFF");
        else if (!state_enable_hr && state_egs)
            lv_label_set_text_static(ui_label_debug, "Scan Started\nHR OFF, EGS ON");
        else
            lv_label_set_text_static(ui_label_debug, "Scan Started\nHR OFF, EGS OFF");

        lv_obj_update_layout(ui_scan_screen);
        break;
    case 1: // Stop
        state_scan_in_progress = false;
        lv_obj_clear_flag(ui_spinner_scan_busy_static, LV_OBJ_FLAG_HIDDEN); // show the static spinner arc when scanning stops
        lv_obj_set_style_arc_color(ui_spinner_scan_busy_static, UI_COLOR_LILA, LV_PART_MAIN);
        lv_obj_set_style_arc_color(ui_spinner_scan_busy_static, UI_COLOR_LILA, LV_PART_INDICATOR);
        lv_obj_add_flag(ui_spinner_scan_busy, LV_OBJ_FLAG_HIDDEN); // hide the spinner when scanning stops

        lv_label_set_text_static(ui_label_debug, "Scan Stopped");

        lv_obj_update_layout(ui_scan_screen);
        break;
    case 2: // Home
        if (ui_main_screen == NULL)
            ui_main_screen_init();

        lv_disp_load_scr(ui_main_screen);
        ui_scan_screen_destroy();
        break;
    default:
        break;
    }
}
