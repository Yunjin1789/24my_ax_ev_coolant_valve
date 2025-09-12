/**
 * @copyright 2017 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file gpio_device.h
 */

#ifndef GPIO_DEVICE_H__
#define GPIO_DEVICE_H__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <rugbyPro.h>
#include "errno.h"


typedef enum{
  GPIO_PORT_1 = 0U,
  GPIO_PORT_2,
  GPIO_PORT_3,
  GPIO_PORT_4,
  GPIO_PORT_5,
  GPIO_PORT_6,
  GPIO_PORT_7,
  GPIO_PORT_8,
  GPIO_PORT_9,
  /* special for Debug below*/
  GPIO_PORT_LINS,
  GPIO_PORT_LINM,
}GpioPort_t;


typedef enum{
  GPIO_DIR_INPUT  = 0U,
  GPIO_DIR_OUTPUT,
}GpioDir_t;

/* 01: pull none, 00: pull up, 11: pull down, 10: pull up and down*/
typedef enum{
  GPIO_PULL_UP          = 0U,
  GPIO_PULL_NONE        = 1U,
  GPIO_PULL_UP_DOWN     = 2U,
  GPIO_PULL_DOWN        = 3U,
}GpioPullMode_t;

typedef enum{
  GPIO_LOW = 0U,
  GPIO_HIGH,
}GpioSetState_t;

typedef enum{
  GPIO_MUX_GPIO = 0U,
  GPIO_MUX_PWM,
  GPIO_MUX_UART,
}GpioMuxMode_t;


typedef enum{
  GPIO_PWM_CH1 = 0U,
  GPIO_PWM_CH2,
  GPIO_PWM_CH3,
  GPIO_PWM_NONE,
}GpioPWMchannel_t;

/* 00: disable, 01: rising, 10: falling, 11: both */
typedef enum{
  GPIO_EDGE_NODE = 0U,
  GPIO_EDGE_RISING,
  GPIO_EDGE_FALLING,
  GPIO_EDGE_BOTH,
}GpioISREdge_t;


#ifdef _UNICK_SOL_
typedef enum{
	GPIO1_M_SEN = 0,
	GPIO2_DUMMY,
	GPIO3_DUMMY,
	GPIO4_DUMMY,
	GPIO5_SOL_PWM,
	GPIO6_DUMMY,
	GPIO7_DUMMY,
	GPIO8_DUMMY,
}GpioUnick_User_t;

#define GPIO2_SPI_CS_M_OUT 1
#else
typedef enum{
	GPIO1_HALL_OUT = 0,
	GPIO2_SPI_CS_M_OUT,
	GPIO3_SPI_CLK_OUT,
	GPIO4_SPI_MOSI_OUT,
	GPIO5_SPI_MIS0_IN,
	GPIO6_MOTOR_CFB_OUT,
	GPIO7_MOTOR_FSB_OUT,
	GPIO8_LDO_ON_OFF_OUT,
}GpioUnick_User_t;
#endif

#define MOTOR_CS_L GPIO_Set((GpioPort_t)GPIO2_SPI_CS_M_OUT,GPIO_LOW) 
#define MOTOR_CS_H GPIO_Set((GpioPort_t)GPIO2_SPI_CS_M_OUT,GPIO_HIGH)

typedef void (*gpio_cb_func_t)(void);


void GPIOs_Init(void);
void GPIO_Init(GpioPort_t port,GpioDir_t dir,GpioPullMode_t pullMode);
void GPIO_RegisterIRQ(GpioPort_t port, GpioISREdge_t edge, gpio_cb_func_t callback);
void GPIO_UnRegisterIRQ(GpioPort_t port);
void GPIO_Set(GpioPort_t port, GpioSetState_t state);
void GPIO_SetGPIOs(uint8_t portsMask, uint8_t value);
uint8_t GPIO_Read(GpioPort_t port);

//#define PPSADL_PPSADL0              GPIO_Read(GPIO_PORT_1)
//#define PPSADL_PPSADL1              GPIO_Read(GPIO_PORT_1)
//#define PPSADL_PPSADL2              GPIO_Read(GPIO_PORT_1)
//#define PPSADL_PPSADL3              GPIO_Read(GPIO_PORT_1)
//#define PPSADL_PPSADL4              GPIO_Read(GPIO_PORT_1)
//#define PPSADL_PPSADL5              GPIO_Read(GPIO_PORT_1)
#define PPSADL_PPSADL6              GPIO_Read(GPIO_PORT_1)
//#define PPSADL_PPSADL7              GPIO_Read(GPIO_PORT_1)
#endif /* __GPIO_DEVICE_H__ */