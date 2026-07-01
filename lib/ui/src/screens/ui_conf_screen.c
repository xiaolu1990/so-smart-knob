#include "../ui.h"

#include <math.h>

lv_obj_t *ui_conf_screen = NULL;

static lv_obj_t *ui_label_debug = NULL;

#define CONF_ICON_COUNT 5
#define CONF_ICON_RING_RADIUS 156   // distance from center
#define CONF_START_ANGLE_DEG -90.0f // starting angle offset for the first icon (top of the circle)
#define DEG_TO_RAD 0.01745329252f

ui_icon_t conf_menu_icons_default[] = {
    {&hr_on, "HR ON", 0},
    {&enhanced_global_scan_on, "Enhanced Global Scan ON", 1},
    {&service_position, "Service Position", 2},
    {&start_scan, "Start Scan", 3},
    {&back, "Back", 4},
}; // default icons displayed in the configuration screen

ui_icon_t conf_menu_icons_extra[] = {
    {&hr_off, "HR OFF", 0},
    {&enhanced_global_scan_off, "Enhanced Global Scan OFF", 1},
    {&play, "Play", 2},
    {&busy, "Busy", 3},
}; // extra icons that can be used later in the configuration screen

static void menu_icon_click_event_cb(lv_event_t *e);

void ui_conf_screen_init(uint8_t main_menu_icon_id)
{
    // setup the configuration screen and configure the layout (flex column center) and styles
    ui_conf_screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_img_src(ui_conf_screen, &background, 0);

    static lv_style_t style_conf_screen;
    lv_style_init(&style_conf_screen);
    lv_style_set_text_font(&style_conf_screen, &lv_font_montserrat_20);
    lv_style_set_text_color(&style_conf_screen, UI_COLOR_LILA);
    lv_obj_add_style(ui_conf_screen, &style_conf_screen, 0);

    // -- display image of the selected main menu icon in the center of the screen --
    lv_obj_t *selected_icon_img = lv_img_create(ui_conf_screen);
    lv_img_set_src(selected_icon_img, main_menu_icons[main_menu_icon_id].image);
    lv_img_set_zoom(selected_icon_img, 384); // 1.5x zoom
    lv_obj_center(selected_icon_img);

    // -- create a menu container to hold the menu icons --
    lv_obj_t *conf_menu_container = lv_obj_create(ui_conf_screen);
    lv_obj_set_size(conf_menu_container, SCREEN_WIDTH, SCREEN_HEIGHT);
    lv_obj_center(conf_menu_container);
    lv_obj_set_style_bg_opa(conf_menu_container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(conf_menu_container, 0, 0);
    lv_obj_set_style_pad_all(conf_menu_container, 0, 0);

    const float angle_step = 360.0f / CONF_ICON_COUNT;

    // arrange the icons in a circular layout around the center of the screen
    for (uint32_t i = 0; i < CONF_ICON_COUNT; i++)
    {
        const float angle_deg = CONF_START_ANGLE_DEG + (angle_step * i);
        const float angle_rad = angle_deg * DEG_TO_RAD;

        lv_coord_t pos_x = SCREEN_CENTER_X + (CONF_ICON_RING_RADIUS * cosf(angle_rad)) - 32; // 32 is half of button width
        lv_coord_t pos_y = SCREEN_CENTER_Y + (CONF_ICON_RING_RADIUS * sinf(angle_rad)) - 32; // 32 is half of button height

        lv_obj_t *button = lv_btn_create(conf_menu_container);
        lv_obj_set_size(button, 64, 64);
        lv_obj_set_pos(button, pos_x, pos_y);
        lv_obj_set_style_radius(button, LV_RADIUS_CIRCLE, LV_PART_MAIN);
        lv_obj_set_style_bg_opa(button, LV_OPA_TRANSP, 0);
        lv_obj_set_style_border_width(button, 0, 0);

        lv_obj_t *img = lv_img_create(button); // place image on the button
        lv_img_set_src(img, conf_menu_icons_default[i].image);
        lv_img_set_zoom(img, 109); // 42% zoom, which is approximately 64px
        lv_obj_center(img);

        lv_obj_add_event_cb(button, menu_icon_click_event_cb, LV_EVENT_CLICKED, &conf_menu_icons_default[i]);
    }

    // -- setup the debug label to display debug information --
    ui_label_debug = lv_label_create(ui_conf_screen);
    lv_obj_align(ui_label_debug, LV_ALIGN_BOTTOM_MID, 0, -20);
    lv_label_set_text(ui_label_debug, "Configuration Screen");
}

void ui_conf_screen_destroy(void)
{
    if (ui_conf_screen != NULL)
    {
        lv_obj_del(ui_conf_screen);
        ui_conf_screen = NULL;
        ui_label_debug = NULL;
    }
}

static void menu_icon_click_event_cb(lv_event_t *e)
{
    lv_obj_t *target = lv_event_get_target(e);
    ui_icon_t *icon = (ui_icon_t *)lv_event_get_user_data(e);
    if (!icon)
        return;

    if (lv_event_get_code(e) == LV_EVENT_CLICKED)
    {
        switch (icon->id)
        {
        case 1:
            // Handle HR ON action
            break;
        case 2:
            // Handle HR OFF action
            break;
        case 3:
            // Handle Service Position action
            break;
        case 4:
            // Handle back icon action, return to main screen
            if (ui_main_screen == NULL)
                ui_main_screen_init();
                
            lv_disp_load_scr(ui_main_screen);
            // Clean up the configuration screen to free memory
            ui_conf_screen_destroy();
            break;
        default:
            break;
        }
    }
}