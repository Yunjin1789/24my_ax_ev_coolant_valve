/**
 * @copyright 2015 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file adc_device.c

 * @Author: Jack.Pan
 * @E-mail:jack.pan@indiemicro.com
 * @Date: 2020/03/18
 */

#include <adc_device.h>
#include <isrfuncs.h>
#include <appConfig.h>
static ADCMeasureParam_t adcMeasParamm = {ADC_MEASURE_ITEM_NONE, 0};
static adc_cb_func_t adcCallback = NULL;
static uint16_t mAdcResult[3];
//static uint16_t adcResult[3];

void ADC_Handler(void)
{  

    if (adcCallback !=NULL){        
        mAdcResult[1] = (uint16_t)(ADC_SFRS->DATA1 & 0x0FFFU);           /*Buck volt, Vtemp*/
#if 1        
        switch(adcMeasParamm.item)
        {
            case ADC_MEASURE_ITEM_GPIO:
            mAdcResult[2] = (uint16_t)((ADC_SFRS->DATA0345 & 0x0FFFU));        /*Vtemp */
            break;
            default:
            mAdcResult[0] = (uint16_t)(ADC_SFRS->DATA0345 & 0x0FFFU);        /*Vtemp */
            break;
        }      
#else
        mAdcResult[0] = (uint16_t)(ADC_SFRS->DATA0345 & 0x0FFFU);        /*Battery volt, GPIOs */
        mAdcResult[2] = (uint16_t)(ADC_SFRS->DATA2 & 0x0FFFU);           /*Led PN */        
#endif        
        adcCallback(adcMeasParamm, mAdcResult);

    }
    ADC_SFRS->CNTRL.IRQCLR = 1U;
}

void ADC_GeneralInit(void)
{
    ADC_SFRS->CONF.AUTOEN       = 0U; 
    ADC_SFRS->CONF.SAMPCYC      = 10U;
    ADC_SFRS->CONF.MODE         = (uint8_t)ADC_MODE_SINGLE_END;
    ADC_SFRS->CNTRL.STUPDLY     = (uint8_t)ADC_STARTUP_DELAY_1US;
    ADC_SFRS->CONF.SYNCENA      = (uint8_t)SYNC_MODE_NONE; /* disable sync convert mode */
    ADC_SFRS->CNTRL.CONT        = 0U; /* singel convert mode */
    ADC_REG_TSET.WORD           = (uint32_t)(((uint32_t)ADC_WAIT_TIME_1500NS << TSET_TCURR_POS) | ((uint32_t)ADC_WAIT_TIME_1000NS << TSET_TCHNL_POS) | ((uint32_t)ADC_WAIT_TIME_1000NS << TSET_TGUARD_POS));
}

void ADC_Init(AdcMeasureItem_t item, uint8_t channel, LedType_t type, AdcSyncMode_t syncMode)
{
    adcMeasParamm.item       = item;
    adcMeasParamm.channel    = channel;
    
    if(item == ADC_MEASURE_ITEM_VTEMP){
        ADC_SFRS->CONF.SAMPCYC      = 44U;
    }else{
        ADC_SFRS->CONF.SAMPCYC      = 10U;
    }
    ADC_SFRS->CONF.ATTEN = 0U;
    
    switch(item){
    case ADC_MEASURE_ITEM_VTEMP:/* 1x */
      ADC_SFRS->CNTRL.CH1SEL = (uint8_t)ADC_CH1_CH3_SEL_TEMP_SENSOR;
      ADC_SFRS->CNTRL.CHSEQ  = (uint8_t)ADC_CH1;
      ADC_SFRS->CONF.SYNCENA = (uint8_t)syncMode;
      break;

    case ADC_MEASURE_ITEM_LED_BUCK_BAT:            /*LED: 1/5.5x, BUCK: 1/8x, VBAT:1/32x*/
      IOCTRLA_SFRS->LEDPIN.VFW_ENA = 1U;        /* Enable 2mA led current source */
      IOCTRLA_SFRS->LEDPIN.SENSE_ENA = 1U;      /* LED Forward Voltage Sense Enable. Set to enable LED forward voltage sense module.*/
      ADC_SFRS->CNTRL.CH1SEL = (uint8_t)ADC_CH1_VH3_SEL_VBUCK;
      if (type == LED_RED){
          ADC_SFRS->CNTRL.CH2SEL = (uint8_t)PHY_CHANNEL_R(channel);/*  measure red PN volt */
      }else if (type == LED_GREEN){
          ADC_SFRS->CNTRL.CH2SEL = (uint8_t)PHY_CHANNEL_G(channel);/*  measure green PN volt */
      }else{
          ADC_SFRS->CNTRL.CH2SEL = (uint8_t)PHY_CHANNEL_B(channel);/*  measure blue PN volt */
      }
      ADC_SFRS->CNTRL.CH3SEL = (uint8_t)ADC_CH1_CH3_SEL_VBAT;
      ADC_SFRS->CNTRL.CHSEQ  = (uint8_t)ADC_CH2_THEN_CH1_THEN_CH3;
      ADC_SFRS->CONF.SYNCENA = (uint8_t)syncMode;
      break;
    case ADC_MEASURE_ITEM_BUCK_VBAT:
      ADC_SFRS->CNTRL.CH1SEL = (uint8_t)ADC_CH1_VH3_SEL_VBUCK;
      ADC_SFRS->CNTRL.CH3SEL = (uint8_t)ADC_CH1_CH3_SEL_VBAT;
      ADC_SFRS->CNTRL.CHSEQ  = (uint8_t)ADC_CH1_THEN_CH3;
      ADC_SFRS->CONF.SYNCENA = (uint8_t)syncMode;
      break;
    case ADC_MEASURE_ITEM_LED:
      IOCTRLA_SFRS->LEDPIN.VFW_ENA = 1U;        /* Enable 2mA led current source */
      IOCTRLA_SFRS->LEDPIN.SENSE_ENA = 1U;      /* LED Forward Voltage Sense Enable. Set to enable LED forward voltage sense module.*/
      if (type == LED_RED){
          ADC_SFRS->CNTRL.CH2SEL = (uint8_t)PHY_CHANNEL_R(channel);/*  measure red PN volt */
      }else if (type == LED_GREEN){
          ADC_SFRS->CNTRL.CH2SEL = (uint8_t)PHY_CHANNEL_G(channel);/*  measure green PN volt */
      }else{
          ADC_SFRS->CNTRL.CH2SEL = (uint8_t)PHY_CHANNEL_B(channel);/*  measure blue PN volt */
      }
      ADC_SFRS->CNTRL.CHSEQ  = (uint8_t)ADC_CH2;
      ADC_SFRS->CONF.SYNCENA = (uint8_t)syncMode;
      break;
    case ADC_MEASURE_ITEM_VBG:  /*1x */
      ADC_SFRS->CNTRL.CHSEQ = (uint8_t)ADC_CH0;
      ADC_SFRS->CONF.SYNCENA = (uint8_t)syncMode;
      break; 
      
    case ADC_MEASURE_ITEM_VBAT: /*1/32x */
      ADC_SFRS->CNTRL.CH1SEL = (uint8_t)ADC_CH1_CH3_SEL_VBAT;
      ADC_SFRS->CNTRL.CHSEQ = (uint8_t)ADC_CH1;
      ADC_SFRS->CONF.SYNCENA = (uint8_t)syncMode;
      break;
      
    case ADC_MEASURE_ITEM_BUCK: /*1/8x */
      ADC_SFRS->CNTRL.CH1SEL = (uint8_t)ADC_CH1_VH3_SEL_VBUCK;
      ADC_SFRS->CNTRL.CHSEQ = (uint8_t)ADC_CH1;
      ADC_SFRS->CONF.SYNCENA = (uint8_t)syncMode;
      break;
      
    case ADC_MEASURE_ITEM_VDD1V5:/* 1/3x */
      ADC_SFRS->CONF.ATTEN = 1U;
      ADC_SFRS->CNTRL.CHSEQ  = (uint8_t)ADC_CH5;
      ADC_SFRS->CONF.SYNCENA = (uint8_t)syncMode;
      break;
      
    case ADC_MEASURE_ITEM_GPIO:/*1/3x,*/
      if (type == LED_RED){
          //ADC_SFRS->CNTRL.CH2SEL = (uint8_t)PHY_CHANNEL_R(channel);/*  measure red PN volt */
          ADC_SFRS->CNTRL.CH4SEL = 0;
      }else if (type == LED_GREEN){
          ADC_SFRS->CNTRL.CH4SEL = 1;
          //ADC_SFRS->CNTRL.CH2SEL = (uint8_t)PHY_CHANNEL_G(channel);/*  measure green PN volt */
      }else{
          ADC_SFRS->CNTRL.CH4SEL = 5;
          //ADC_SFRS->CNTRL.CH2SEL = (uint8_t)PHY_CHANNEL_B(channel);/*  measure blue PN volt */
      }
      ADC_SFRS->CONF.ATTEN = 1U;
      ADC_SFRS->CNTRL.CHSEQ  = (uint8_t)ADC_CH4;
      ADC_SFRS->CONF.SYNCENA = (uint8_t)syncMode;
      break;

    default:
      break;
    }
}

void ADC_UnInit(AdcMeasureItem_t item)
{
    
  
}

/**
 * @brief Register a callback function to ADC interrupt handler.
 *
 * @param dev Pointer to the ADC device.
 * @param cb The callback function to be registered.
 * @return 0 for success or error code upon a failure.
 */
void ADC_RegisterIRQ(adc_cb_func_t callback)
{
    adcCallback = callback;
    ADC_SFRS->CNTRL.IRQCLR = 1U;
    ADC_SFRS->CNTRL.IRQENA = 1U;
    NVIC_EnableIRQ(ADC_IRQn);   
}

/**
 * @brief Unregister a callback function to ADC interrupt handler.
 *
 * @param dev Pointer to the ADC device.
 * @param cb The callback function to be unregistered.
 * @return 0 for success or error code upon a failure.
 */
void ADC_UnregisterIRQ(void)
{
    adcCallback = NULL;
    ADC_SFRS->CNTRL.IRQCLR = 1U;
    ADC_SFRS->CNTRL.IRQENA = 0U;
    NVIC_DisableIRQ(ADC_IRQn);
}

void ADC_Start(void)
{
    ADC_SFRS->CNTRL.IRQCLR = 1U;
    ADC_SFRS->CNTRL.CONVERT = 1U;
}


/*
** ===================================================================
**     Method      :  AD1_MeasureChan (component ADC)
*/
/*!
**     @brief
**         This method performs one measurement on all channels that
**         are set in the component inspector. (Note: If the [number of
**         conversions] is more than one the conversion of A/D channels
**         is performed specified number of times.)
**     @param
**         WaitForResult   - Wait for a result of a
**                           conversion. If [interrupt service] is
**                           disabled, A/D peripheral doesn't support
**                           measuring all channels at once or Autoscan
**                           mode property isn't enabled and at the same
**                           time the [number of channels] is greater
**                           than 1, then the WaitForResult parameter is
**                           ignored and the method waits for each
**                           result every time. If the [interrupt
**                           service] is disabled and a [number of
**                           conversions] is greater than 1, the
**                           parameter is ignored and the method also
**                           waits for each result every time.
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_DISABLED - Device is disabled
**                           ERR_BUSY - A conversion is already running
*/
/* ===================================================================*/
byte AD1_MeasureChan(bool WaitForResult)
{
  //if (!EnUser) 
  {                       /* Is the device disabled by user? */
//    return ERR_DISABLED;               /* If yes then error */
  }
//  if (ModeFlg != IDLE) 
  {               /* Is the device in running mode? */
    //return ERR_BUSY;                   /* If yes then error */
  }
  //ModeFlg = MEASURE;                   /* Set state of device to the MEASURE mode */
//  AD_Monitor_HWEnDi();                 /* Start measurement */
//  if (WaitForResult) 
  {                 /* Is WaitForResult TRUE? */
//    while (ModeFlg != IDLE) {}         /* If yes then wait for end of measurement */
  }
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  AD1_GetChanValue16 (component ADC)
*/
/*!
**     @brief
**         Returns the last measured value of the required channel.
**         Format and width of the value is a native format of the A/D
**         converter.
**     @param
**         Channel         - Channel number. If only one
**                           channel in the component is set then this
**                           parameter is ignored.
**     @param
**         Value           - Pointer to the measured value. Data
**                           type is a byte, a word or an int. It
**                           depends on the supported modes, resolution,
**                           etc. of the AD converter. See the Version
**                           specific information for the current CPU in
**                           [General Info].
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_NOTAVAIL - Requested value not
**                           available
**                           ERR_RANGE - Parameter "Channel" out of
**                           range
**                           ERR_OVERRUN - External trigger overrun flag
**                           was detected after the last value(s) was
**                           obtained (for example by GetValue). This
**                           error may not be supported on some CPUs
**                           (see generated code).
*/
/* ===================================================================*/
byte AD1_GetChanValue16(byte Channel, uint16_t* Value)
{
  uint16_t *pnVoltDummy, *pnVoltDummy1;

  if (Channel > 3U) {                 /* Is the sample number out of range? */
    return ERR_RANGE;                  /* If yes then error */
  }

  switch(Channel)
  {
    case 0:      
      MES_GetRunTimeLedPNVolt((LedNum_t)0U, Value, &pnVoltDummy, &pnVoltDummy1);
      break;
    case 1:
      MES_GetRunTimeLedPNVolt((LedNum_t)0U, &pnVoltDummy, Value, &pnVoltDummy1);
      break;
    case 2:
      MES_GetRunTimeLedPNVolt((LedNum_t)0U, &pnVoltDummy, &pnVoltDummy1,  Value);
      break;  
    case 3:
      MES_GetBatteryVolt(Value);
      break;
    default:
      return ERR_RANGE;     
  }
  
  return ERR_OK;                       /* OK */
}
