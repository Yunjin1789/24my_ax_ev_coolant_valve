/**
 * @copyright 2015 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file flash_device.c
 */

#include <stdint.h>
#include <string.h>
#include <flash_device.h>
#include <flash_sfrs.h>

void Flash_EraseSector(uint32_t sectorAddress)
{
    /* Erase sector data */
    f_FLASH_EraseSector(sectorAddress);
}

void Flash_WriteWord(uint32_t address,uint32_t data)
{
    f_FLASH_WriteWord(address, data);
}