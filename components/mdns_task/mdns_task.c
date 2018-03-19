/*
 * mdns.c
 *
 *  Created on: 23.04.2017
 *      Author: michaelboeckling
 */


#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "mdns.h"
#include "wifi.h"

#define TAG "mDNS"
#define MDNS_HOSTNAME "esp32-radio"
#define MDNS_INSTANCE "ESP32 Web Radio"

void mdns_task(EventGroupHandle_t wifi_event_group)
{
    // mdns_server_t definition is moved to a private include ...
    // so it not anymore visible
    //mdns_server_t * mdns = NULL;
    mdns_txt_item_t items[] = { {"board", "esp32"} };

    while(1) {
        /* Wait for the callback to set the CONNECTED_BIT in the
           event group.
        */
        xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT,
                            false, true, portMAX_DELAY);
        // We go below if we receive a connected_bit event

            ESP_LOGI(TAG, "starting mDNS");

            //esp_err_t err = mdns_init(TCPIP_ADAPTER_IF_STA, &mdns);
            //if (err) {
            //    ESP_LOGE(TAG, "Failed starting MDNS: %u", err);
            //    continue;
            //}
            ESP_ERROR_CHECK( mdns_init() );

            ESP_ERROR_CHECK( mdns_hostname_set( MDNS_HOSTNAME ) );
            ESP_ERROR_CHECK( mdns_instance_name_set( MDNS_INSTANCE ) );

            ESP_ERROR_CHECK( mdns_service_add(NULL, "_http", "_tcp", 80, items, 1) );
            ESP_ERROR_CHECK( mdns_service_instance_name_set("_http", "_tcp", "ESP32 WebServer") );

        // sleep 10 seconds
        vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
}
