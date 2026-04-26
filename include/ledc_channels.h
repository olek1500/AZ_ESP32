#pragma once

// LEDC channel 0 is reserved by the camera driver (LEDC_CHANNEL_0)
#define LEDC_MOT1_CH    1   // GPIO 2  – motor,  1000Hz, 10-bit
#define LEDC_MOT2_CH    2   // GPIO 14 – motor,  1000Hz, 10-bit
#define LEDC_MOT3_CH    3   // GPIO 12 – motor,  1000Hz, 10-bit
#define LEDC_FLASH_CH   4   // GPIO 4  – flash,  5000Hz, 8-bit
#define LEDC_MOT4_CH    5   // GPIO 13 – motor,  1000Hz, 10-bit
#define LEDC_AUX_CH     6   // auxiliary channel for analogwrite command
