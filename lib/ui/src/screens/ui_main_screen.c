#include "../ui.h"

lv_obj_t *ui_main_screen = NULL;
lv_obj_t *ui_label_debug = NULL;
lv_obj_t *ui_label_item = NULL; // label to display the selected menu item

ui_icon_t menu_icons[] = {
    {&lower_jaw_64x64, "Lower Jaw", 1},
    {&upper_jaw_64x64, "Upper Jaw", 2},
    {&multcase_tray_64x64, "Multcase Tray", 3},
    {&universal_scan_64x64, "Universal Scan", 4},
    {&correction_scan_64x64, "Correction Scan", 5},
};

static void menu_item_event_cb(lv_event_t *e);

/**
 * @brief Create and load the main screen
 *
 */
void ui_main_screen_init(void)
{
    // setup the main screen and configure the layout (flex column center) and styles
    ui_main_screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_img_src(ui_main_screen, &background, 0);
    lv_obj_clear_flag(ui_main_screen, LV_OBJ_FLAG_SCROLLABLE);

    static lv_style_t style_main_screen;
    lv_style_init(&style_main_screen);
    lv_style_set_text_font(&style_main_screen, &lv_font_montserrat_20);
    lv_style_set_text_color(&style_main_screen, UI_COLOR_LILA);
    lv_obj_add_style(ui_main_screen, &style_main_screen, 0);

    lv_obj_set_layout(ui_main_screen, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(ui_main_screen, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_main_screen,
                          LV_FLEX_ALIGN_CENTER,  // Main axis (Vertical: distribute from center)
                          LV_FLEX_ALIGN_CENTER,  // Cross axis (Horizontal: center align items)
                          LV_FLEX_ALIGN_CENTER); // Track alignment

    // setup the item label to display the selected menu item
    ui_label_item = lv_label_create(ui_main_screen);
    lv_label_set_text(ui_label_item, "Lower Jaw");

    lv_coord_t gap = 10;
    lv_coord_t item_w = (SCREEN_WIDTH - (gap * 4)) / 3;
    if (item_w < 72)
        item_w = 72;
    lv_coord_t item_h = item_w + 28;

    lv_obj_t *menu_row = lv_obj_create(ui_main_screen);
    lv_obj_set_size(menu_row, SCREEN_WIDTH, item_h + 8);
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
    for (uint32_t i = 0; i < menu_count; i++)
    {
        lv_obj_t *card = lv_btn_create(menu_row);
        lv_obj_set_size(card, item_w, item_h);

        lv_obj_t *img = lv_img_create(card);
        lv_img_set_src(img, menu_icons[i].image);
        lv_obj_center(img);

        lv_obj_add_event_cb(card, menu_item_event_cb, LV_EVENT_CLICKED, &menu_icons[i]);
    }

    lv_obj_update_layout(menu_row);

    // create a label for debugging purposes
    ui_label_debug = lv_label_create(ui_main_screen);
    lv_label_set_text(ui_label_debug, "Main Screen");
    lv_obj_align(ui_label_debug, LV_ALIGN_BOTTOM_MID, 0, -20);
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

    switch (icon->id)
    {
    case 1:
        //
        break;
    case 2:
        //
        break;
    case 3:
    case 4:
    case 5:

        break;
    default:
        break;
    }
}