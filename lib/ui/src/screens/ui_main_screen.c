#include "../ui.h"

lv_obj_t *ui_splash_screen = NULL;
lv_obj_t *ui_main_screen = NULL;
lv_obj_t *ui_label_debug = NULL;

ui_icon_t menu_icons[] = {
    {&lower_jaw_64x64, "Lower Jaw", 1},
    {&upper_jaw_64x64, "Upper Jaw", 2},
    {&multcase_tray_64x64, "Multcase Tray", 3},
    {&universal_scan_64x64, "Universal Scan", 4},
    {&correction_scan_64x64, "Correction Scan", 5},
};

static void splash_timer_cb(lv_timer_t *timer);
static void menu_item_event_cb(lv_event_t *e);

/**
 * @brief Splash screen timer callback
 *
 * @param timer
 */
static void splash_timer_cb(lv_timer_t *timer)
{
    // 1. Create and load the main screen
    ui_main_screen_init();
    lv_disp_load_scr(ui_main_screen);

    // 2. Clean up the splash screen to free memory
    if (ui_splash_screen != NULL)
    {
        lv_obj_del(ui_splash_screen);
        ui_splash_screen = NULL;
    }
}

/**
 * @brief Menu item event callback
 *
 * @param e
 */
static void menu_item_event_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) != LV_EVENT_CLICKED)
        return;

    ui_icon_t *icon = (ui_icon_t *)lv_event_get_user_data(e);
    if (!icon)
        return;

    // Navigate by icon id
    switch (icon->id)
    {
    case 1:
        _ui_screen_change(&ui_Screen1, LV_SCR_LOAD_ANIM_FADE_ON, 150, 0, &ui_Screen1_screen_init);
        break;
    case 2:
        _ui_screen_change(&ui_time, LV_SCR_LOAD_ANIM_FADE_ON, 150, 0, &ui_time_screen_init);
        break;
    case 3:
    case 4:
    case 5:
        _ui_screen_change(&ui_working, LV_SCR_LOAD_ANIM_FADE_ON, 150, 0, &ui_working_screen_init);
        break;
    default:
        break;
    }
}

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
 * @brief Create and load the main screen
 *
 */
void ui_main_screen_init(void)
{
    ui_main_screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_img_src(ui_main_screen, &background, 0);
    lv_obj_clear_flag(ui_main_screen, LV_OBJ_FLAG_SCROLLABLE);

    lv_disp_t *disp = lv_disp_get_default();
    if (!disp) {
        return;
    }

    lv_coord_t scr_w = lv_disp_get_hor_res(disp);
    lv_coord_t gap = 10;
    lv_coord_t item_w = (scr_w - (gap * 4)) / 3;
    if (item_w < 72) item_w = 72;
    lv_coord_t item_h = item_w + 28;

    lv_obj_t *menu_row = lv_obj_create(ui_main_screen);
    lv_obj_set_size(menu_row, scr_w, item_h + 8);
    lv_obj_align(menu_row, LV_ALIGN_CENTER, 0, 30);

    lv_obj_set_style_bg_opa(menu_row, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(menu_row, 0, 0);
    lv_obj_set_style_pad_top(menu_row, 8, 0);
    lv_obj_set_style_pad_bottom(menu_row, 8, 0);
    lv_obj_set_style_pad_column(menu_row, gap, 0);

    lv_obj_set_layout(menu_row, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(menu_row, LV_FLEX_FLOW_ROW);
    lv_obj_set_scroll_dir(menu_row, LV_DIR_HOR);
    lv_obj_set_scrollbar_mode(menu_row, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scroll_snap_x(menu_row, LV_SCROLL_SNAP_CENTER);
    lv_obj_add_flag(menu_row, LV_OBJ_FLAG_SCROLL_ONE);

    const uint32_t menu_count = sizeof(menu_icons) / sizeof(menu_icons[0]);
    for (uint32_t i = 0; i < menu_count; i++) {
        lv_obj_t *card = lv_btn_create(menu_row);
        lv_obj_set_size(card, item_w, item_h);

        lv_obj_t *img = lv_img_create(card);
        lv_img_set_src(img, menu_icons[i].image);
        lv_obj_center(img);

        lv_obj_add_event_cb(card, menu_item_event_cb, LV_EVENT_CLICKED, &menu_icons[i]);
    }

    lv_obj_update_layout(menu_row);
}