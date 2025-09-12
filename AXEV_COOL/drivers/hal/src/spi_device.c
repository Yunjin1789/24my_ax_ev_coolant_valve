/**
 * @copyright 2015 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file spi_device.c
 * @Author: Jack.Pan
 * @E-mail:jack.pan@indiemicro.com
 * @Date: 2020/03/18
 */

#include <appConfig.h>
#include <gpio_device.h>
#include <spi_device.h>
#include <atomic.h>
#include <isrfuncs.h>

void delay_ms(uint32_t time) {
    while (time--)
    {
        //__NOP;
    };
}


int8_t SPI_Init(SpiClkDevider_t devider)
{
    // GPIO_Init(GPIO_PORT_2, GPIO_DIR_OUTPUT,GPIO_PULL_UP);
    // GPIO_Set(GPIO_PORT_2, 1);
    // GPIO_SFRS->GPIO_CFG[(uint8_t)GPIO_PORT_3].DIR   = (uint8_t)GPIO_DIR_OUTPUT;
    // IOCTRLA_SFRS->GPIO[(uint8_t)GPIO_PORT_3].PULLMODE = (uint8_t)GPIO_PULL_UP;

    // GPIO_SFRS->GPIO_CFG[(uint8_t)GPIO_PORT_4].DIR   = (uint8_t)GPIO_DIR_OUTPUT;
    // IOCTRLA_SFRS->GPIO[(uint8_t)GPIO_PORT_4].PULLMODE = (uint8_t)GPIO_PULL_UP;

    // GPIO_SFRS->GPIO_CFG[(uint8_t)GPIO_PORT_5].DIR   = (uint8_t)GPIO_DIR_INPUT;
    // IOCTRLA_SFRS->GPIO[(uint8_t)GPIO_PORT_5].PULLMODE = (uint8_t)GPIO_PULL_DOWN;

    // IOCTRLA_SFRS->GPIO[(uint8_t)GPIO_PORT_3].HWMODE = 5U;/* GPIO_PORT_3: SPI Master Clock Out.*/
    // IOCTRLA_SFRS->GPIO[(uint8_t)GPIO_PORT_4].HWMODE = 4U;/* GPIO_PORT_4: SPI Master MOSI*/
    // IOCTRLA_SFRS->GPIO[(uint8_t)GPIO_PORT_5].HWMODE = 4U;/* GPIO_PORT_5: SPI Master MISO.*/

    SPI_REG_CTRL.FIFOSOFTRESET = 1U;
    SPI_REG_CTRL.LPBK = 0U;
    SPI_REG_CTRL.SCKEXT = ((uint8_t)devider >> 2);
    SPI_REG_CTRL.SPR    = (uint8_t)devider &0x03U;
    SPI_REG_CTRL.ENA_REQ = 1U;
    return 0;
}

/*#pragma optimize=none*/
int8_t SPI_SendBuff(uint8_t * const buff, uint16_t length)
{
    uint8_t data = 0U;
    SPI_REG_CTRL.FIFOSOFTRESET = 1U;
    for (uint16_t i = 0U; i < length; i++){
        SPI_REG_TXDATA = buff[i];
        while(SPI_REG_STATUS.XFERCNT == 0U){} /* waiting for tx completed */
        data = SPI_REG_RXDATA;
        
    }
    return (int8_t)data;
}

int8_t SPI_ReadBuff(uint8_t * const buff, uint16_t length)
{
    MOTOR_CS_L; 
    SPI_REG_CTRL.FIFOSOFTRESET = 1U;
    for (uint16_t i = 0; i < length; i++){
        SPI_REG_TXDATA = 0xFFU;
        while(SPI_REG_STATUS.XFERCNT == 0U){}/* waiting for tx completed */
        buff[i] = SPI_REG_RXDATA;
    }

    while(SPI_REG_STATUS.XFERCNT == 0U){} /* waiting for tx completed */
    delay_ms(4);
    MOTOR_CS_H; 
    return 0;
}

byte SM1_RecvChar(SM1_TComData *Chr)
{
  uint16_t length = 2;
  MOTOR_CS_L;
    SPI_REG_CTRL.FIFOSOFTRESET = 1U;
    for (uint16_t i = 0; i < length; i++){
        SPI_REG_TXDATA = 0xFFU;
        while(SPI_REG_STATUS.XFERCNT == 0U){}/* waiting for tx completed */
        //Chr = Chr | (SPI_REG_RXDATA <<(8*i));
    }
    
   while(SPI_REG_STATUS.XFERCNT == 0U){} /* waiting for tx completed */
  //return ERR_RXEMPTY;                /* If yes then error is returned */
  delay_ms(1000);
  MOTOR_CS_H;
  return ERR_OK;
}

/*
** ===================================================================
**     Method      :  SM1_SendChar (component SynchroMaster)
**     Description :
**         Sends one character to the channel.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Chr             - Character to send
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_DISABLED - Device is disabled (only if
**                           output DMA is supported and enabled)
**                           ERR_TXFULL - Transmitter is full
** ===================================================================
*/
byte SM1_SendChar(SM1_TComData Chr)
{
   uint16_t length = 2;
   uint8_t buff[2];
   MOTOR_CS_L; 

   SPI_REG_CTRL.FIFOSOFTRESET = 1U;
   for (uint16_t i = 0U; i < length; i++){
       SPI_REG_TXDATA = Chr>>(8*(1-i)) & 0xFF;
       while(SPI_REG_STATUS.XFERCNT == 0U){} /* waiting for tx completed */
       switch(i)
       {
            case 0:
                buff[i] = SPI_REG_RXDATA & 0x0F;
                break;
            case 1:
            default:    
                buff[i] = SPI_REG_RXDATA;
                break;
       }             
    }

  while(SPI_REG_STATUS.XFERCNT == 0U){} /* waiting for tx completed */
  delay_ms(2);


  MOTOR_CS_H;
  return ERR_OK;                       /* OK */
}

void SPI_Handler(void)
{
  
  
}