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

void SPI_Init(SpiClkDevider_t devider)
{
    SPI_REG_CTRL.FIFOSOFTRESET = 1U;
    SPI_REG_CTRL.LPBK = 0U;
    SPI_REG_CTRL.CPOL = 0U;
    SPI_REG_CTRL.CPHA = 0U;
    SPI_REG_CTRL.SCKEXT = ((uint8_t)devider >> 2);
    SPI_REG_CTRL.SPR = (uint8_t)devider & 0x03U;
    SPI_REG_CTRL.ENA_REQ = 1U;
}

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

void SPI_WriteReadByte(uint8_t *const buff, uint16_t length)
{
    MOTOR_CS_L;

    SPI_REG_CTRL.FIFOSOFTRESET = 1U;
    for (uint16_t i = 0; i < length; i++)
    {
        SPI_REG_TXDATA = buff[i];
        while (SPI_REG_STATUS.XFERCNT == 0U)
        {
        }
        buff[i] = SPI_REG_RXDATA;
    }
    MOTOR_CS_H;
}

void SPI_Handler(void)
{
  
  
}