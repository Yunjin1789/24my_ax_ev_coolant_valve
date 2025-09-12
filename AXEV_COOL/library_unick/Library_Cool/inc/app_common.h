#ifndef APP_COMMON_H_
#define APP_COMMON_H_

/* driver header */
#include <gpio_device.h>
#include <timer_device.h>
#include <softTimerTask.h>
#include <adc_device.h>
#include <spi_device.h>
#include <measureTask.h>
#include <flash_device.h>
#include <wdt_device.h>
#include <lin.h>
#include <lin_cfg.h>
#include <lin_hw_cfg.h>

/* application header */
#include <main.h>
#include <app_failsafety.h>
#include <app_sensing.h>
#include <app_lin.h>
#include <app_motordriver.h>
#include <app_motor.h>

/* standard header */
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>

/* User activation & error typedef */
typedef enum __attribute__((packed))
{
    INIT_NOT_DONE = 0,
    INIT_ING = 1,
    INIT_DONE = 2,
    INIT_DLEAY = 3,
}INITDONE;

typedef enum __attribute__((packed))
{ 
    DISABLED = 0,
    ENABLED  = 1,
}ACTIVATION;

typedef enum __attribute__((packed))
{ 
    NORMAL_STATUS = 0,
    ERROR_STATUS  = 1,
}ERROR;

#endif