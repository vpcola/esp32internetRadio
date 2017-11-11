/*
 * ui.c
 *
 *  Created on: 01.04.2017
 *      Author: michaelboeckling
 */



#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <soc/rmt_struct.h>
#include <esp_system.h>
#include <esp_log.h>
#include <nvs_flash.h>
#include <driver/gpio.h>
#include <stdio.h>
//#include "ws2812.h"
#include "ssd1306.h"
#include "ui.h"

#define TAG "ui"

#define delay_ms(ms) ((ms) / portTICK_RATE_MS)


typedef struct {
    int queue_size;
    QueueHandle_t ui_queue;
} ui_obj_t;

/** UI object instance */
static ui_obj_t *p_ui_obj = NULL;

void task_ui(void *pvParameters)
{
    ui_queue_message_t mesg;
    TickType_t delay = 500 / portTICK_RATE_MS ;

    while(1) {
        xQueueReceive((p_ui_obj->ui_queue), &mesg, delay);

        switch(mesg.event) {
            case UI_NONE:
                delay = portMAX_DELAY;
                break;

            case UI_CONNECTING:
                delay = delay_ms(250);
                break;

            case UI_CONNECTED:
                delay = portMAX_DELAY;
                break;

            case UI_STATION_UPDATE:
                ESP_LOGI(TAG, "station update : %s", (const char *) mesg.data);
                SSD1306_GotoXY(10, 50);
                SSD1306_Puts((const char *) mesg.data, &Font_7x10, SSD1306_COLOR_WHITE);
                break;
        }

        // ESP_LOGI(TAG, "task_ui stack: %d\n", uxTaskGetStackHighWaterMark(NULL));
        // Run oled animation here..
        SSD1306_UpdateScreen();
    }

    vTaskDelete(NULL);
}

void ui_queue_event(ui_event_t evt)
{
    ui_queue_message_t message;
    message.event = evt;

    if(p_ui_obj != NULL)
        xQueueSend(p_ui_obj->ui_queue, &message, 0);
}

void ui_queue_event_message(ui_event_t evt, const char * msg)
{
    ui_queue_message_t message;
    message.event = evt;
    // TODO: Buffer overflow checks
    strncpy(message.data, msg, UI_MESSAGE_LENGTH );

    if(p_ui_obj != NULL)
        xQueueSend(p_ui_obj->ui_queue, &message, 0);
}


int ui_init()
{
    if(p_ui_obj == NULL) {
        p_ui_obj = malloc(sizeof(ui_obj_t));
    } else {
        ESP_LOGE(TAG, "UI already initialized");
        return -1;
    }

    p_ui_obj->queue_size = UI_QUEUE_LENGTH;
    p_ui_obj->ui_queue = xQueueCreate(UI_QUEUE_LENGTH, sizeof(ui_queue_message_t));

    xTaskCreatePinnedToCore(&task_ui, "task_ui", 2048, NULL, 20, NULL, 0);

    return 0;
}
