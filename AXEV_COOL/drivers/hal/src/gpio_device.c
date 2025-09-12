/**
 * @copyright 2017 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file gpio_device.c
 * @Author: Jack.Pan
 * @E-mail:jack.pan@indiemicro.com
 * @Date: 2020/03/18
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <gpio_device.h>
#include <isrfuncs.h>
#include <appConfig.h>

#ifdef GPIO_SFRS
static gpio_cb_func_t gpioCallback[9]= {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
extern void Hall_Count_Int_OnInterrupt(void);

void GPIO_Handler( void )
{
    for (uint8_t i = (uint8_t)GPIO_PORT_1; i<= (uint8_t)GPIO_PORT_9; i++ ){
        if (gpioCallback[i] !=NULL){
            if (GPIO_SFRS->GPIO_CFG[i].ACTDET == 1U){
                GPIO_SFRS->GPIO_CFG[i].CLR = 1U;
                gpioCallback[i]();
            }
        }
    }
}


void GPIO_Init(GpioPort_t port,GpioDir_t dir,GpioPullMode_t pullMode)
{
    IOCTRLA_SFRS->GPIO[(uint8_t)port].HWMODE   = (uint8_t)GPIO_MUX_GPIO;
    IOCTRLA_SFRS->GPIO[(uint8_t)port].PULLMODE = (uint8_t)pullMode;
    GPIO_SFRS->GPIO_CFG[(uint8_t)port].DIR     = (uint8_t)dir;
    if (dir == GPIO_DIR_INPUT){
        IOCTRLA_SFRS->GPIO[(uint8_t)port].RDENA = 1U;
    }
}



void GPIO_RegisterIRQ(GpioPort_t port, GpioISREdge_t edge, gpio_cb_func_t callback)
{
    GPIO_SFRS->GPIO_CFG[(uint8_t)port].CLR = 1U;
    GPIO_SFRS->GPIO_CFG[(uint8_t)port].IE = 1U;
    GPIO_SFRS->GPIO_CFG[(uint8_t)port].EDGE = (uint8_t)edge;
    gpioCallback[(uint8_t)port] = callback;
    NVIC_EnableIRQ(GPIO_IRQn);
}

void GPIO_UnRegisterIRQ(GpioPort_t port)
{
    gpioCallback[(uint8_t)port] = NULL;
}

void GPIO_Set(GpioPort_t port, GpioSetState_t state)
{
    if(port <= GPIO_PORT_8){
        GPIO_SFRS->GPADATA[1U << (uint8_t)port] =  (uint8_t)state << (uint8_t)port;
    }else{
        GPIO_SFRS->GPBDATA[1U << ((uint8_t)port - (uint8_t)GPIO_PORT_9)]=  (uint8_t)state << ((uint8_t)port - (uint8_t)GPIO_PORT_9);
    }
}

void GPIO_SetGPIOs(uint8_t portsMask, uint8_t value)
{
    GPIO_SFRS->GPADATA[portsMask] = value;/* value */
}


uint8_t GPIO_Read(GpioPort_t port)
{
    uint8_t data;
    if(port <= GPIO_PORT_8){
        data = GPIO_SFRS->GPADATA[1U << (uint8_t)port];
    }else{
        data = GPIO_SFRS->GPBDATA[1U << ((uint8_t)port - (uint8_t)GPIO_PORT_9)];
    }
    return (uint8_t)(data != 0U);
}

void GPIO_Init_PWM(GpioPort_t port,GpioDir_t dir,GpioPullMode_t pullMode)
{
//    SYSCTRLA_REG_DEBUG_ACCESS_KEY.DEBUG_ACCESS_KEY        = 0x05U;
//    SYSCTRLA_REG_TRIM_ACCESS_KEY.KEY   = 0x0EU;
  
    IOCTRLA_SFRS->GPIO[(uint8_t)port].HWMODE  = GPIO_MUX_PWM;       /* PWM */
    //IOCTRLA_SFRS->GPIO[(uint8_t)port].PWM_SEL = port;
    IOCTRLA_SFRS->GPIO[(uint8_t)port].PULLMODE = (uint8_t)pullMode;
    IOCTRLA_SFRS->GPIO[(uint8_t)port].RDENA = 1U;
    GPIO_SFRS->GPIO_CFG[(uint8_t)port].DIR     = (uint8_t)dir;        
}

void GPIO_Init_ADC(GpioPort_t port,GpioDir_t dir,GpioPullMode_t pullMode)
{
    SYSCTRLA_REG_DEBUG_ACCESS_KEY.DEBUG_ACCESS_KEY        = 0x05U;
    SYSCTRLA_REG_TRIM_ACCESS_KEY.KEY   = 0x0EU;
  
    IOCTRLA_SFRS->GPIO[(uint8_t)port].HWMODE  = GPIO_MUX_GPIO;       /* PWM */
    //IOCTRLA_SFRS->GPIO[(uint8_t)port].PWM_SEL = port;
    IOCTRLA_SFRS->GPIO[(uint8_t)port].PULLMODE = (uint8_t)pullMode;
    GPIO_SFRS->GPIO_CFG[(uint8_t)port].DIR     = (uint8_t)dir;
    if (dir == GPIO_DIR_INPUT){
        IOCTRLA_SFRS->GPIO[(uint8_t)port].RDENA = 1U;
    }
}
SYSCTRLA_SFRS_t sysCtrl;
void GPIOs_Init(void)
{
#if _UNICK_SOL_  
    GPIO_Set(GPIO2_DUMMY, GPIO_LOW);  
    GPIO_Set(GPIO3_DUMMY, GPIO_LOW);  
    GPIO_Set(GPIO4_DUMMY, GPIO_LOW);      
    GPIO_Set(GPIO6_DUMMY, GPIO_LOW);  
    GPIO_Set(GPIO7_DUMMY, GPIO_LOW);  
    GPIO_Set(GPIO8_DUMMY, GPIO_LOW); 

    GPIO_Init(GPIO1_M_SEN, GPIO_DIR_OUTPUT,GPIO_PULL_NONE);
    GPIO_Init_ADC(GPIO2_DUMMY, GPIO_DIR_INPUT,GPIO_PULL_UP);   
    GPIO_Init_ADC(GPIO3_DUMMY, GPIO_DIR_INPUT,GPIO_PULL_UP);
    GPIO_Init(GPIO4_DUMMY, GPIO_DIR_INPUT,GPIO_PULL_UP);

#if PWM_TO_GPIO == 0U
    GPIO_Init(GPIO5_SOL_PWM, GPIO_DIR_OUTPUT,GPIO_PULL_NONE);    
    GPIO_Set(GPIO5_SOL_PWM, GPIO_LOW);         
#endif
    GPIO_Init_PWM(GPIO5_SOL_PWM, GPIO_DIR_OUTPUT,GPIO_PULL_NONE); 

    GPIO_Init_ADC(GPIO6_DUMMY, GPIO_DIR_INPUT,GPIO_PULL_NONE);
    GPIO_Init(GPIO7_DUMMY, GPIO_DIR_INPUT,GPIO_PULL_NONE);
    GPIO_Init(GPIO8_DUMMY, GPIO_DIR_INPUT,GPIO_PULL_NONE);

    GPIO_Set(GPIO1_M_SEN, GPIO_HIGH); 
#else
    GPIO_Init((GpioPort_t)GPIO1_HALL_OUT, GPIO_DIR_INPUT,GPIO_PULL_NONE);
    GPIO_Init((GpioPort_t)GPIO2_SPI_CS_M_OUT, GPIO_DIR_OUTPUT,GPIO_PULL_UP);   
    GPIO_Init((GpioPort_t)GPIO3_SPI_CLK_OUT, GPIO_DIR_OUTPUT,GPIO_PULL_UP);
    GPIO_Init((GpioPort_t)GPIO4_SPI_MOSI_OUT, GPIO_DIR_OUTPUT,GPIO_PULL_UP);
    GPIO_Init((GpioPort_t)GPIO5_SPI_MIS0_IN, GPIO_DIR_INPUT,GPIO_PULL_DOWN);     
    GPIO_Init((GpioPort_t)GPIO6_MOTOR_CFB_OUT, GPIO_DIR_OUTPUT,GPIO_PULL_NONE);
    GPIO_Init((GpioPort_t)GPIO7_MOTOR_FSB_OUT, GPIO_DIR_INPUT,GPIO_PULL_NONE);
    GPIO_Init((GpioPort_t)GPIO8_LDO_ON_OFF_OUT, GPIO_DIR_OUTPUT,GPIO_PULL_NONE);

    IOCTRLA_SFRS->GPIO[(uint8_t)GPIO3_SPI_CLK_OUT].HWMODE = 5U;/* GPIO_PORT_3: SPI Master Clock Out.*/
    IOCTRLA_SFRS->GPIO[(uint8_t)GPIO4_SPI_MOSI_OUT].HWMODE = 4U;/* GPIO_PORT_4: SPI Master MOSI*/
    IOCTRLA_SFRS->GPIO[(uint8_t)GPIO5_SPI_MIS0_IN].HWMODE = 4U;/* GPIO_PORT_5: SPI Master MISO.*/

    GPIO_Set((GpioPort_t)GPIO2_SPI_CS_M_OUT, GPIO_HIGH);
    GPIO_Set((GpioPort_t)GPIO8_LDO_ON_OFF_OUT, GPIO_LOW);  

    GPIO_SFRS->GPIO_CFG[(uint8_t)GPIO3_SPI_CLK_OUT].DIR   = (uint8_t)GPIO_DIR_OUTPUT;
    IOCTRLA_SFRS->GPIO[(uint8_t)GPIO3_SPI_CLK_OUT].PULLMODE = (uint8_t)GPIO_PULL_UP;

    GPIO_SFRS->GPIO_CFG[(uint8_t)GPIO4_SPI_MOSI_OUT].DIR   = (uint8_t)GPIO_DIR_OUTPUT;
    IOCTRLA_SFRS->GPIO[(uint8_t)GPIO4_SPI_MOSI_OUT].PULLMODE = (uint8_t)GPIO_PULL_UP;

    GPIO_SFRS->GPIO_CFG[(uint8_t)GPIO5_SPI_MIS0_IN].DIR   = (uint8_t)GPIO_DIR_INPUT;
    IOCTRLA_SFRS->GPIO[(uint8_t)GPIO5_SPI_MIS0_IN].PULLMODE = (uint8_t)GPIO_PULL_DOWN;

    GPIO_Init_ADC((GpioPort_t)GPIO6_MOTOR_CFB_OUT, GPIO_DIR_INPUT,GPIO_PULL_NONE);

    GPIO_Set((GpioPort_t)GPIO8_LDO_ON_OFF_OUT, GPIO_HIGH);  
#endif    
}
#endif