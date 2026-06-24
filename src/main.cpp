/*
 * SPDX-FileCopyrightText: 2024-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <Arduino.h>
#include <esp_display_panel.hpp>
#include <lvgl.h>
#include "lvgl_v8_port.h"

#include <ESP_Knob.h>
#include <Button.h>
#include <ui.h>

/**
/* To use the built-in examples and demos of LVGL uncomment the includes below respectively.
 * You also need to copy `lvgl/examples` to `lvgl/src/examples`. Similarly for the demos `lvgl/demos` to `lvgl/src/demos`.
 */
// #include <demos/lv_demos.h>
// #include <examples/lv_examples.h>

#define GPIO_NUM_KNOB_PIN_A     6
#define GPIO_NUM_KNOB_PIN_B     5
#define GPIO_BUTTON_PIN         GPIO_NUM_0

using namespace esp_panel::drivers;
using namespace esp_panel::board;

/*Knob event definition*/
ESP_Knob *knob;
static void knob_left_event_cb(int count, void *usr_data)
{
    // Serial.printf("Detect left event, count is %d\n", count);
    lvgl_port_lock(-1);
    LVGL_knob_event(UI_KNOB_LEFT);
    lvgl_port_unlock();
}

static void knob_right_event_cb(int count, void *usr_data)
{
    // Serial.printf("Detect right event, count is %d\n", count);
    lvgl_port_lock(-1);
    LVGL_knob_event(UI_KNOB_RIGHT);
    lvgl_port_unlock();
}

static void single_click_event_cb(void *button_handle, void *usr_data) {
    // Serial.println("Button Single Click");
    lvgl_port_lock(-1);
    LVGL_button_event(UI_BUTTON_SINGLE_CLICK);
    lvgl_port_unlock();
}

static void double_click_event_cb(void *button_handle, void *usr_data)
{
    // Serial.println("Button Double Click");
    lvgl_port_lock(-1);
    LVGL_button_event(UI_BUTTON_DOUBLE_CLICK);
    lvgl_port_unlock();
}

static void long_press_event_cb(void *button_handle, void *usr_data) {
    // Serial.println("Button Long Press Start");
    lvgl_port_lock(-1);
    LVGL_button_event(UI_BUTTON_LONG_PRESS_START);
    lvgl_port_unlock();
}

void setup()
{
    Serial.begin(115200);

    Serial.println("Initializing board");
    Board *board = new Board();
    board->init();
#if LVGL_PORT_AVOID_TEARING_MODE
    auto lcd = board->getLCD();
    // When avoid tearing function is enabled, the frame buffer number should be set in the board driver
    lcd->configFrameBufferNumber(LVGL_PORT_DISP_BUFFER_NUM);
#if ESP_PANEL_DRIVERS_BUS_ENABLE_RGB && CONFIG_IDF_TARGET_ESP32S3
    auto lcd_bus = lcd->getBus();
    /**
     * As the anti-tearing feature typically consumes more PSRAM bandwidth, for the ESP32-S3, we need to utilize the
     * "bounce buffer" functionality to enhance the RGB data bandwidth.
     * This feature will consume `bounce_buffer_size * bytes_per_pixel * 2` of SRAM memory.
     */
    if (lcd_bus->getBasicAttributes().type == ESP_PANEL_BUS_TYPE_RGB) {
        static_cast<BusRGB *>(lcd_bus)->configRGB_BounceBufferSize(lcd->getFrameWidth() * 10);
    }
#endif
#endif
    assert(board->begin());

    Serial.println("Initializing LVGL");
    lvgl_port_init(board->getLCD(), board->getTouch());

    /*knob initialization*/
    Serial.println("Initialize Knob device");
    knob = new ESP_Knob(GPIO_NUM_KNOB_PIN_A, GPIO_NUM_KNOB_PIN_B);
    knob->begin();
    knob->attachLeftEventCallback(knob_left_event_cb);
    knob->attachRightEventCallback(knob_right_event_cb);

    Serial.println("Initialize Button device");
    Button *btn = new Button(GPIO_BUTTON_PIN, false);
    btn->attachSingleClickEventCb(&single_click_event_cb, NULL);
    btn->attachDoubleClickEventCb(&double_click_event_cb, NULL);
    btn->attachLongPressStartEventCb(&long_press_event_cb, NULL);

    Serial.println("Creating UI");
    /* Lock the mutex due to the LVGL APIs are not thread-safe */
    lvgl_port_lock(-1);

    ui_init();

    /* Release the mutex */
    lvgl_port_unlock();
}

void loop()
{
    Serial.println("IDLE loop");
    delay(1000);
}
