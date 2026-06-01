
#include <stdio.h>
#include <string.h>
#include <errno.h>         

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"

#include "nvs_flash.h"

#include "esp_log.h"

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"

#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include "lwip/inet.h"      


//MOTOR PINS
#define pin1 GPIO_NUM_14
#define pin2 GPIO_NUM_27
#define pin3 GPIO_NUM_26
#define pin4 GPIO_NUM_25

static const char *TAG = "udp_car";

//move forward 
static void move_forward(void)
{
    ESP_LOGI(TAG, "Moving forward");

    gpio_set_level(pin1, 1);
    gpio_set_level(pin2, 0);

    gpio_set_level(pin3, 1);
    gpio_set_level(pin4, 0);
}

//move backward
static void move_backward(void)
{
    ESP_LOGI(TAG, "Moving backward");

    gpio_set_level(pin1, 0);
    gpio_set_level(pin2, 1);

    gpio_set_level(pin3, 0);
    gpio_set_level(pin4, 1);
}

//turn left
static void turn_left(void)
{
    ESP_LOGI(TAG, "Turning left");

    gpio_set_level(pin1, 0);
    gpio_set_level(pin2, 1);

    gpio_set_level(pin3, 1);
    gpio_set_level(pin4, 0);
}

//turn right
static void turn_right(void)
{
    ESP_LOGI(TAG, "Turning right");

    gpio_set_level(pin1, 1);
    gpio_set_level(pin2, 0);

    gpio_set_level(pin3, 0);
    gpio_set_level(pin4, 1);
}

//stop car
static void stop_car(void)
{
    ESP_LOGI(TAG, "Stopping car");

    gpio_set_level(pin1, 0);
    gpio_set_level(pin2, 0);

    gpio_set_level(pin3, 0);
    gpio_set_level(pin4, 0);
}

//UDP SERVER
static void udp_server_init(void *pvParameters)
{
    
    vTaskDelay(pdMS_TO_TICKS(5000));

    int udp_socket = socket(
        AF_INET,
        SOCK_DGRAM,
        IPPROTO_IP
    );

    if (udp_socket < 0)
    {
        ESP_LOGE(
            TAG,
            "Unable to create socket: errno=%d",
            errno
        );

        vTaskDelete(NULL);
        return;
    }

    ESP_LOGI(TAG, "Socket Created");

    char rx_buffer[128];

    struct sockaddr_in server_addr =
    {
        .sin_family = AF_INET,
        .sin_port = htons(1234),
        .sin_addr.s_addr = htonl(INADDR_ANY)
    };

    int err = bind(
        udp_socket,
        (struct sockaddr *)&server_addr,
        sizeof(server_addr)
    );

    if (err < 0)
    {
        ESP_LOGE(
            TAG,
            "Bind Failed errno=%d",
            errno
        );

        close(udp_socket);

        vTaskDelete(NULL);
        return;
    }

    ESP_LOGI(TAG, "UDP Server Listening On Port 1234");


    while (1)
    {
        struct sockaddr_in source_addr;

        socklen_t socklen = sizeof(source_addr);

        int len = recvfrom(
            udp_socket,
            rx_buffer,
            sizeof(rx_buffer) - 1,
            0,
            (struct sockaddr *)&source_addr,
            &socklen
        );

        if (len < 0)
        {
            ESP_LOGE(
                TAG,
                "Receive Error errno=%d",
                errno
            );

            continue;
        }

        rx_buffer[len] = '\0';

        ESP_LOGI(
            TAG,
            "Received from %s:%d -> %s",
            inet_ntoa(source_addr.sin_addr),
            ntohs(source_addr.sin_port),
            rx_buffer
        );

        if(strcmp(rx_buffer, "F") == 0)
        {
            move_forward();
        }

        else if(strcmp(rx_buffer, "B") == 0)
        {
            move_backward();
        }

        else if(strcmp(rx_buffer, "L") == 0)
        {
            turn_left();
        }

        else if(strcmp(rx_buffer, "R") == 0)
        {
            turn_right();
        }

        else if(strcmp(rx_buffer, "S") == 0)
        {
            stop_car();
        }

        else
        {
            ESP_LOGW(
                TAG,
                "Unknown Command"
            );
        }

    }



    close(udp_socket);

    vTaskDelete(NULL);
}


static void wifi_init(void *pvParameters)
{

    esp_netif_init();

    esp_event_loop_create_default();

    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

    esp_wifi_init(&cfg);

    wifi_config_t wifi_config =
    {
        .sta =
        {
            .ssid = "bava",
            .password = "9790346382"
        }
    };



    esp_wifi_set_mode(
        WIFI_MODE_STA
    );


    esp_wifi_set_config(
        WIFI_IF_STA,
        &wifi_config
    );


    esp_wifi_start();

    ESP_LOGI(
        TAG,
        "WiFi Started"
    );


    esp_wifi_connect();

    ESP_LOGI(
        TAG,
        "Connecting To WiFi..."
    );

    vTaskDelete(NULL);
}

void app_main(void)
{
    //Motor pin configuration
    gpio_config_t pin_config =
    {
        .pin_bit_mask =
            (1ULL << pin1) |
            (1ULL << pin2) |
            (1ULL << pin3) |
            (1ULL << pin4),

        .mode = GPIO_MODE_OUTPUT,

        .pull_up_en = GPIO_PULLUP_DISABLE,

        .pull_down_en = GPIO_PULLDOWN_DISABLE,

        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_config(&pin_config);
    //NVS flash
    nvs_flash_init();

    // Create WiFi Task
    xTaskCreate(
        wifi_init,
        "wifi_init",
        4096,
        NULL,
        5,
        NULL
    );

    // Create UDP Server Task
    xTaskCreate(
        udp_server_init,
        "udp_server_init",
        4096,
        NULL,
        5,
        NULL
    );
}

