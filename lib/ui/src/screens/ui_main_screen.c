#include "../ui.h"

lv_obj_t *ui_main_screen = NULL;
lv_obj_t *ui_main_menu_selected_btn = NULL;   // the currently selected button in the menu container
uint32_t ui_main_menu_selected_btn_index = 0; // the index of the currently selected button in the menu container

ui_icon_t main_menu_icons[] = {
    {&lower_jaw_64x64, "Lower Jaw", 0},
    {&upper_jaw_64x64, "Upper Jaw", 1},
    {&multcase_tray_64x64, "Multcase Tray", 2},
    {&universal_scan_64x64, "Universal Scan", 3},
    {&correction_scan_64x64, "Correction Scan", 4},
};

static lv_obj_t *ui_menu_container = NULL; // container to hold the menu items
static lv_obj_t *ui_label_debug = NULL;
static lv_obj_t *ui_label_item = NULL; // label to display the selected menu item

static void menu_icon_click_event_cb(lv_event_t *e);
static void menu_container_scroll_event_cb(lv_event_t *e);
static void update_menu_state(lv_obj_t *container, bool update_btn_style, bool update_label);
static void change_brightness_event_cb(lv_event_t *e);

/**
 * @brief Create and load the main screen
 *
 */
void ui_main_screen_init(void)
{
    // -- setup the main screen and configure the layout (flex column center) and styles --
    ui_main_screen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_main_screen, LV_OBJ_FLAG_SCROLLABLE);

    static lv_style_t style_main_screen;
    lv_style_init(&style_main_screen);
    lv_style_set_text_font(&style_main_screen, &lv_font_montserrat_20);
    lv_style_set_text_color(&style_main_screen, UI_COLOR_LILA);
    
    lv_obj_add_style(ui_main_screen, &style_main_screen, 0);

    // -- create a tileview to hold the main screen and settings screen tiles --
    lv_obj_t *tv_main = lv_tileview_create(ui_main_screen);
    lv_obj_set_size(tv_main, SCREEN_WIDTH, SCREEN_HEIGHT);
    lv_obj_set_style_anim_time(tv_main, 200, 0);
    lv_obj_set_scrollbar_mode(tv_main, LV_SCROLLBAR_MODE_OFF);

    // -- create a tile for the main screen and set it as the initial tile --
    lv_obj_t *tile_main = lv_tileview_add_tile(tv_main, 0, 0, LV_DIR_BOTTOM);

    lv_obj_add_style(tile_main, &style_main_screen, 0);
    lv_obj_set_style_bg_img_src(tile_main, &background, 0);
    lv_obj_clear_flag(tile_main, LV_OBJ_FLAG_SCROLLABLE);


    lv_obj_set_layout(tile_main, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(tile_main, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(tile_main,
                          LV_FLEX_ALIGN_CENTER,  // Main axis (Vertical: distribute from center)
                          LV_FLEX_ALIGN_CENTER,  // Cross axis (Horizontal: center align items)
                          LV_FLEX_ALIGN_CENTER); // Track alignment
    lv_obj_set_style_pad_row(tile_main, 20, 0);  // Set vertical padding between items

    // -- setup the item label to display the selected menu item --
    ui_label_item = lv_label_create(tile_main);
    lv_obj_set_style_text_font(ui_label_item, &lv_font_montserrat_30, 0);

    // -- create a container to hold the menu items and display as a card view --
    ui_menu_container = lv_obj_create(tile_main);
    lv_coord_t gap = 21;
    lv_coord_t item_w = (SCREEN_WIDTH - (gap * 4)) / 3; // 3 items + 4 gaps

    // ensure the first and last items are centered in the container by adding padding to the left and right
    lv_obj_set_style_pad_left(ui_menu_container, (SCREEN_WIDTH - item_w) / 2, 0);
    lv_obj_set_style_pad_right(ui_menu_container, (SCREEN_WIDTH - item_w) / 2, 0);
    lv_obj_add_flag(ui_menu_container, LV_OBJ_FLAG_OVERFLOW_VISIBLE);
    lv_obj_add_flag(ui_menu_container, LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_add_flag(ui_menu_container, LV_OBJ_FLAG_SCROLL_CHAIN_VER);

    lv_coord_t item_h = item_w;
    lv_obj_set_size(ui_menu_container, SCREEN_WIDTH, item_h + 20);
    lv_obj_set_style_bg_opa(ui_menu_container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(ui_menu_container, 0, 0);

    lv_obj_set_layout(ui_menu_container, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(ui_menu_container, LV_FLEX_FLOW_ROW);
    lv_obj_set_scroll_dir(ui_menu_container, LV_DIR_HOR);
    lv_obj_set_scrollbar_mode(ui_menu_container, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scroll_snap_x(ui_menu_container, LV_SCROLL_SNAP_CENTER);
    lv_obj_add_flag(ui_menu_container, LV_OBJ_FLAG_SCROLL_ONE);
    lv_obj_set_style_pad_column(ui_menu_container, gap, 0);

    const uint32_t menu_items_cnt = sizeof(main_menu_icons) / sizeof(main_menu_icons[0]);
    for (uint32_t i = 0; i < menu_items_cnt; i++)
    {
        lv_obj_t *button = lv_btn_create(ui_menu_container);
        lv_obj_set_size(button, item_w, item_h);
        lv_obj_set_style_bg_opa(button, LV_OPA_TRANSP, 0);
        lv_obj_set_style_border_width(button, 0, 0);

        lv_obj_add_flag(button, LV_OBJ_FLAG_SCROLL_ON_FOCUS); // Ensure the button scrolls into view when focused

        lv_obj_t *img = lv_img_create(button); // place image on the button
        lv_img_set_src(img, main_menu_icons[i].image);
        lv_obj_center(img);

        lv_obj_add_event_cb(button, menu_icon_click_event_cb, LV_EVENT_CLICKED, &main_menu_icons[i]);
    }

    lv_obj_add_event_cb(ui_menu_container, menu_container_scroll_event_cb, LV_EVENT_SCROLL, NULL);
    lv_obj_add_event_cb(ui_menu_container, menu_container_scroll_event_cb, LV_EVENT_SCROLL_END, NULL);

    // lv_obj_update_layout(ui_menu_container);
    // update_menu_state(ui_menu_container, true, true);

    lv_event_send(ui_menu_container, LV_EVENT_SCROLL_END, NULL); // manually trigger a scroll event to ensure the first item is centered

    // -- create a label for debugging purposes --
    ui_label_debug = lv_label_create(ui_main_screen);
    lv_label_set_text(ui_label_debug, "Main Screen");
    lv_obj_align(ui_label_debug, LV_ALIGN_BOTTOM_MID, 0, -20);
    
    // -- create a second tile for the settings screen --
    lv_obj_t *tile_settings = lv_tileview_add_tile(tv_main, 0, 1, LV_DIR_TOP);

    lv_obj_t *slider_brightness = lv_slider_create(tile_settings);
    lv_obj_center(slider_brightness);
    lv_slider_set_range(slider_brightness, 5, 100);
    lv_obj_add_event_cb(slider_brightness, change_brightness_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    lv_obj_t *label_ui_settings = lv_label_create(tile_settings);
    lv_label_set_text(label_ui_settings, "Display Brightness");
    lv_obj_align(label_ui_settings, LV_ALIGN_CENTER, 0, 40);
}

/**
 * @brief Get the index of the currently selected menu item
 *
 * @return uint8_t
 */
uint8_t ui_main_screen_get_icon_id(void)
{
    return ui_main_menu_selected_btn_index;
}

/**
 * @brief Handle knob rotation events on the main screen
 *
 * @param event
 */
void ui_main_screen_knob_rotate(ui_knob_event_t event)
{
    if (ui_menu_container == NULL)
        return;

    if (event != UI_KNOB_LEFT && event != UI_KNOB_RIGHT)
        return;

    lv_coord_t step = lv_obj_get_width(lv_obj_get_child(ui_menu_container, 0)) + lv_obj_get_style_pad_column(ui_menu_container, 0);

    lv_coord_t current_x = lv_obj_get_scroll_x(ui_menu_container);
    lv_coord_t target_x = current_x + (event == UI_KNOB_LEFT ? step : -step);

    // lv_obj_scroll_to_x(ui_menu_container, target_x, LV_ANIM_ON);
    lv_obj_scroll_to_x(ui_menu_container, target_x, LV_ANIM_OFF);
}

/**
 * @brief Handle knob press events on the main screen
 *
 * @param event
 */
void ui_main_screen_knob_press(ui_button_event_t event)
{
    if (event == UI_BUTTON_SINGLE_CLICK)
    {
        if (ui_main_menu_selected_btn == NULL)
            return;

        if (!is_sleeping)
            lv_event_send(ui_main_menu_selected_btn, LV_EVENT_CLICKED, NULL);
    }
    else if (event == UI_BUTTON_LONG_PRESS_START)
    {
        // Handle long press start event if needed
    }
    else if (event == UI_BUTTON_LONG_PRESS_HOLD)
    {
        // Handle long press hold event if needed
    }
    else if (event == UI_BUTTON_LONG_PRESS_UP)
    {
        // Handle long press up event if needed
    }
}

/**
 * @brief Callback function when menu icon is clicked
 *
 * @param e
 */
static void menu_icon_click_event_cb(lv_event_t *e)
{
    lv_obj_t *target = lv_event_get_target(e);
    if (target != ui_main_menu_selected_btn)
        return;

    if (lv_event_get_code(e) != LV_EVENT_CLICKED)
        return;

    ui_icon_t *icon = (ui_icon_t *)lv_event_get_user_data(e);
    if (!icon)
        return;

    ui_conf_screen_init(icon->id);
    lv_disp_load_scr(ui_conf_screen);
}

/**
 * @brief Menu container scroll event callback
 * @details - update the selected item label based on the centered menu item when scrolling occurs
 *
 * @param e
 */
static void menu_container_scroll_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *container = lv_event_get_target(e);

    if (code == LV_EVENT_SCROLL)
    {
        update_menu_state(container, true, false);
    }
    else if (code == LV_EVENT_SCROLL_END)
    {
        update_menu_state(container, true, true);
    }
}

/**
 * @brief Update the menu state
 * @details - update the zoom, opacity, and label of the menu items based on their distance from the center
 *
 * @param container The menu container
 * @param update_btn_style Whether to update the style of the buttons
 * @param update_label Whether to update the label of the centered item
 */
static void update_menu_state(lv_obj_t *container, bool update_btn_style, bool update_label)
{
    if (container == NULL)
        return;

    uint32_t child_cnt = lv_obj_get_child_cnt(container);
    if (child_cnt == 0)
        return;

    lv_area_t cont_area;
    lv_obj_get_coords(container, &cont_area);
    lv_coord_t container_center_x = (cont_area.x1 + cont_area.x2) / 2;

    lv_coord_t effect_range = lv_obj_get_width(container) / 2;
    if (effect_range < 1)
        effect_range = 1;

    uint32_t selected_btn_idx = 0;
    lv_coord_t best_dist = LV_COORD_MAX;

    for (uint32_t i = 0; i < child_cnt; i++)
    {
        lv_obj_t *btn = lv_obj_get_child(container, i);

        lv_area_t btn_area;
        lv_obj_get_coords(btn, &btn_area);
        lv_coord_t btn_center_x = (btn_area.x1 + btn_area.x2) / 2;

        lv_coord_t dist = LV_ABS(btn_center_x - container_center_x);
        if (dist < best_dist)
        {
            best_dist = dist;
            selected_btn_idx = i;
            ui_main_menu_selected_btn = btn;
        }

        if (dist > effect_range)
            dist = effect_range;

        // disable all buttons click property first
        lv_obj_clear_flag(btn, LV_OBJ_FLAG_CLICKABLE);

        if (update_btn_style)
        {
            lv_obj_t *img = lv_obj_get_child(btn, 0); // get the image child of the button

            // make all buttons semi-transparent and smaller
            lv_img_set_zoom(img, 220); // 0.85x zoom
            lv_obj_set_style_opa(btn, LV_OPA_30, 0);
        }
    }

    // enable only selected
    if (ui_main_menu_selected_btn && update_btn_style)
    {
        lv_obj_add_flag(ui_main_menu_selected_btn, LV_OBJ_FLAG_CLICKABLE);

        // make the selected button fully opaque and larger
        lv_img_set_zoom(lv_obj_get_child(ui_main_menu_selected_btn, 0), 384); // 1.5x zoom
        lv_obj_set_style_opa(ui_main_menu_selected_btn, LV_OPA_COVER, 0);
    }

    ui_main_menu_selected_btn_index = selected_btn_idx;

    if (update_label && ui_label_item)
        lv_label_set_text(ui_label_item, main_menu_icons[selected_btn_idx].text);
}

/**
 * @brief Event callback for changing the display brightness
 * @param e The LVGL event
 */
static void change_brightness_event_cb(lv_event_t *e)
{
    lv_obj_t *slider = lv_event_get_target(e);
    int32_t brightness = lv_slider_get_value(slider);

    ui_set_display_brightness(brightness);
}