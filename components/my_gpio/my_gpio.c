
#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <esp_log.h>
#include <driver/gpio.h>
#include "esp_system.h"

#define TAG "gpio"
void gpio_init(gpio_num_t PIN_GPIO)
{
    gpio_config_t valve_cfg = {
        .pin_bit_mask = (1<<PIN_GPIO),  
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE, 
        .mode = GPIO_MODE_OUTPUT,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&valve_cfg); 
    gpio_set_level(PIN_GPIO, 0);

}

void gpio_control( gpio_num_t PIN_GPIO, int control_num)
{
    switch (control_num)
    {
    case 1:
        gpio_set_level(PIN_GPIO, 1);
        ESP_LOGI(TAG, "Pin %d level up", PIN_GPIO);
        break;
    case 0:
        gpio_set_level(PIN_GPIO, 0);
        ESP_LOGI(TAG,"Pin %d level down",PIN_GPIO);
        break;
    default:
        ESP_LOGW(TAG,"Unknown control code");
        break;
    }
    

}