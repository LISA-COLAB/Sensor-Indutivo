#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>


#define SI GPIO_NUM_19


void vTaskReadSI(){

    while(1){
        vTaskDelay(500 / portTICK_PERIOD_MS);
        if(gpio_get_level(SI) == 1) {
            gpio_set_level(GPIO_NUM_2, 1);
            printf("Metal detectado!\n");
            vTaskDelay(500 / portTICK_PERIOD_MS);
            gpio_set_level(GPIO_NUM_2, 0);
        } else {
            printf("Nada\n");
        }
    }
}

void app_main(void) {
    gpio_config_t gpioalert = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL << GPIO_NUM_2),
        .pull_up_en = 0,
        .pull_down_en = 0,
    };
    gpio_config(&gpioalert);

    gpio_config_t gpiosi = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = (1ULL << SI),
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_ENABLE, 
    };
    gpio_config(&gpiosi);

    xTaskCreatePinnedToCore(vTaskReadSI, "read indutor sensor", 2048, NULL, 1, NULL, 0);

}