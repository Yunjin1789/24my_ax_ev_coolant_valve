/**
 * @copyright 2020 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 * @file measureTask.c
 * @Author: Jack.Pan
 * @E-mail:jack.pan@indiemicro.com
 * @Date: 2020/07/18
 */

#include <crc32.h>
#include <measureTask.h>
#include <gpio_device.h>

#define AVERAGE_MEASURE_POS     (4U)
#define AVERAGE_MEASURE_GAIN    (1U << AVERAGE_MEASURE_POS)

#define MEASURE_GAIN_POS         12U
#define MEASURE_GAIN             4096 /*(1UL << MEASURE_GAIN_POS)*/

#define MEASURE_TEMP_GAIN_POS    14U
#define MEASURE_TEMP_GAIN        7447 /*16384*0.4545*/

/*  Please don't do any change for it would be used in Color mixing task */
#define TEMP_CMP_NUM_MAX          (61U)

#define MINIMUM_TEMP              (0)
#define MAXIMUM_TEMP              (120)
#define DELTA_TEMP_POS            (1U)
#define DELTA_TEMP                (1U << DELTA_TEMP_POS)/*must be 2^DELTA_TEMP_POS*/

typedef struct{
  int32_t coefficient;
  int32_t offset;
}coeffParam_t;

typedef struct{
  uint32_t currCode;
  uint16_t buff[AVERAGE_MEASURE_GAIN];
  uint16_t count;
  coeffParam_t coeff;
  int16_t target;
}AdcDatabuff_t;

typedef struct{
  AdcDatabuff_t vBatt;
  AdcDatabuff_t vBuck;
  AdcDatabuff_t vLedPN[LED_NUM][3];
  AdcDatabuff_t vChipTemp;
  coeffParam_t  tChip;
  int16_t       chipTemperature;
  int16_t       ledPNTemperature[LED_NUM][3];
}AdcResult_t;

typedef struct{
    uint16_t intensityEfficiency; /*efficiency:100.0% means 1024*/
    int16_t deltaVoltPN;          /*mV*/
    uint16_t x;                   /*float rate: 100.0% means 1024*/
    uint16_t y;                   /*float rate: 100.0% means 1024*/
}TempParams_t;

static uint8_t dataIntegrityError = FALSE;

void add_data_to_buff(uint32_t *original, uint16_t newData, uint16_t *bufferIndex,uint16_t *buff);
static TaskState_t adcTaskState = TASK_STATE_INIT;
static AdcResult_t adcResult;

static uint8_t measLedNo = 0U;
static uint8_t measLedColorNo = 0U;
static AdcSyncMode_t measSyncMode = SYNC_MODE_NONE;
static AdcMeasureItem_t  measItem = ADC_MEASURE_ITEM_NONE;
static volatile uint32_t measStart = 0U;
static volatile uint16_t measGeneralAdcCode[3];
void measureDoneISR(ADCMeasureParam_t item, uint16_t *const result);
void measureParamStart(void);
void measureParamInit(void);

static uint32_t updateParamIndex = 0U;
static uint32_t updateParamStart = 0U;
void updateTimerExpired(SoftTimer_t *timer);

int16_t get_volt(AdcDatabuff_t *const param);
int16_t get_chip_temperature(int16_t vTemp);
//int8_t get_led_temperature(TempParams_t const table[],int16_t deltaPNVolt);
void updateSystemInfo(void);
void updateLedTemperature(LedNum_t index);
void updateParams(void);

static volatile uint32_t adcConvertDone = 0U;

static SoftTimer_t updateTimer = {
    .mode     = TIMER_PERIODIC_MODE,
    .interval = 1U,
    .handler  = updateTimerExpired
};

void updateTimerExpired(SoftTimer_t *timer)
{
    static uint8_t measWaitCount = 3U;
    /*  update then start next measurement  */
    if (++measWaitCount >= 3U){
        measWaitCount = 0U;
        measStart = 1U; 
    }
    updateParamStart = 1U;
    TM_PostTask(TASK_ID_ADC_MEASURE);
}

void add_data_to_buff(uint32_t *original, uint16_t newData, uint16_t *bufferIndex,uint16_t *buff)
{
    if (*original == INVALID_PARAM){
        *original = newData;
        *bufferIndex = 0;
        for (uint8_t i = 0U; i < AVERAGE_MEASURE_GAIN; i++){
            buff[i] = newData;
        }
    }else{
        buff[*bufferIndex] = newData;
    }
    (*bufferIndex) ++;
    if ((*bufferIndex) >= AVERAGE_MEASURE_GAIN){
        (*bufferIndex) = 0U;
    }
}

void measureDoneISR(ADCMeasureParam_t param, uint16_t *const result)
{
    measGeneralAdcCode[0] = result[0];
    measGeneralAdcCode[1] = result[1];
    measGeneralAdcCode[2] = result[2];
    measStart = 1U;
    adcConvertDone = 1U;
    TM_PostTask(TASK_ID_ADC_MEASURE);
}

void measureParamStart(void)
{
    switch(measItem){
    case ADC_MEASURE_ITEM_NONE:
      measSyncMode = SYNC_MODE_NONE;
      measItem = ADC_MEASURE_ITEM_VTEMP;
      break;
    case ADC_MEASURE_ITEM_VTEMP:
      add_data_to_buff(&adcResult.vChipTemp.currCode, measGeneralAdcCode[1], &adcResult.vChipTemp.count,adcResult.vChipTemp.buff);
      measSyncMode = SYNC_MODE_NONE;
      measItem  = ADC_MEASURE_ITEM_BUCK_VBAT;
      break;
    case ADC_MEASURE_ITEM_BUCK_VBAT:
      add_data_to_buff(&adcResult.vBatt.currCode,measGeneralAdcCode[0], &adcResult.vBatt.count,adcResult.vBatt.buff);
      add_data_to_buff(&adcResult.vBuck.currCode,measGeneralAdcCode[1], &adcResult.vBuck.count,adcResult.vBuck.buff);
      for (measLedNo = 0U; measLedNo < LED_NUM; measLedNo++){
          if (PWM_LedIsOn(measLedNo) != 0U){
              break;
          }else{
              adcResult.vLedPN[measLedNo][0].currCode = INVALID_PARAM;
              adcResult.vLedPN[measLedNo][1].currCode = INVALID_PARAM;
              adcResult.vLedPN[measLedNo][2].currCode = INVALID_PARAM;
          }
      }
      if (measLedNo < LED_NUM){
          measSyncMode   =   SYNC_MODE_PWM_PERIOD;
          //measItem    = ADC_MEASURE_ITEM_LED_BUCK_BAT;
          measItem    = ADC_MEASURE_ITEM_GPIO;                    
          measLedColorNo = 0U;
      }else{
          measSyncMode = SYNC_MODE_NONE;
          measItem  = ADC_MEASURE_ITEM_NONE;
      }
      break;
    case ADC_MEASURE_ITEM_LED_BUCK_BAT:
      add_data_to_buff(&adcResult.vBatt.currCode,measGeneralAdcCode[0], &adcResult.vBatt.count,adcResult.vBatt.buff);
      add_data_to_buff(&adcResult.vLedPN[measLedNo][measLedColorNo].currCode, measGeneralAdcCode[2], &adcResult.vLedPN[measLedNo][measLedColorNo].count,adcResult.vLedPN[measLedNo][measLedColorNo].buff);
      measLedColorNo++;  

      if (measLedColorNo >= 3U){
          measLedColorNo = 0U;
          measLedNo++;
          for (; measLedNo < LED_NUM; measLedNo++){
              if (PWM_LedIsOn(measLedNo) != 0U){
                  break;
              }else{
                  adcResult.vLedPN[measLedNo][0].currCode = INVALID_PARAM;
                  adcResult.vLedPN[measLedNo][1].currCode = INVALID_PARAM;
                  adcResult.vLedPN[measLedNo][2].currCode = INVALID_PARAM;
              }
          }
          if (measLedNo >= LED_NUM){
              measSyncMode = SYNC_MODE_NONE;
              measItem  = ADC_MEASURE_ITEM_NONE;
          }
      }
      break;
    case ADC_MEASURE_ITEM_GPIO: 
      add_data_to_buff(&adcResult.vBatt.currCode,measGeneralAdcCode[0], &adcResult.vBatt.count,adcResult.vBatt.buff);
      add_data_to_buff(&adcResult.vLedPN[measLedNo][measLedColorNo].currCode, measGeneralAdcCode[2], &adcResult.vLedPN[measLedNo][measLedColorNo].count,adcResult.vLedPN[measLedNo][measLedColorNo].buff);
      measLedColorNo++;  

      if (measLedColorNo >= 3U){
          measLedColorNo = 0U;
          measLedNo++;
          for (; measLedNo < LED_NUM; measLedNo++){
              if (PWM_LedIsOn(measLedNo) != 0U){
                  break;
              }else{
                  adcResult.vLedPN[measLedNo][0].currCode = INVALID_PARAM;
                  adcResult.vLedPN[measLedNo][1].currCode = INVALID_PARAM;
                  adcResult.vLedPN[measLedNo][2].currCode = INVALID_PARAM;
              }
          }
          if (measLedNo >= LED_NUM){
              measSyncMode = SYNC_MODE_NONE;
              measItem  = ADC_MEASURE_ITEM_NONE;
          }
      }     
    default:
      break;
    }
    if (measItem != ADC_MEASURE_ITEM_NONE){
        ADC_Init(measItem,measLedNo,(LedType_t)measLedColorNo,measSyncMode);
        ADC_Start();
    }
}


int16_t get_volt(AdcDatabuff_t *const param)
{
    int16_t volt = (int16_t)INVALID_VOLT;
    int32_t sVolt;
    uint32_t uVolt;
    uint32_t sum = 0;
    if (param->currCode != INVALID_PARAM){
        for (uint8_t i = 0U; i < AVERAGE_MEASURE_GAIN; i++){
            sum += param->buff[i];
        }
        sum  = sum >> AVERAGE_MEASURE_POS;
        sVolt = param->coeff.coefficient * (int32_t)sum + param->coeff.offset;
        uVolt = (uint32_t)sVolt;
        uVolt = uVolt >> MEASURE_GAIN_POS;
        volt = (int16_t)uVolt;
    }
    return volt;
}

int16_t get_chip_temperature(int16_t vTemp)
{
    int32_t  stemp;
    uint32_t uTemp;
    uint8_t  negative = 0U;
    stemp = adcResult.tChip.coefficient*vTemp + adcResult.tChip.offset;
    if (stemp < 0){
        stemp = -stemp;
        negative = 1U;
    }
    uTemp = (uint32_t)stemp;
    uTemp = uTemp >> MEASURE_TEMP_GAIN_POS;
    stemp = (int32_t)uTemp;
    if (negative != 0U){
        stemp = -stemp;
    }
    return (int16_t)stemp;
}

void updateSystemInfo(void)
{
    /* calculate chip temperature */
    adcResult.vChipTemp.target = get_volt(&adcResult.vChipTemp);
    adcResult.chipTemperature = get_chip_temperature(adcResult.vChipTemp.target);
#if CODE_DEBUG_EN == 1
    (void)DEBUG_OUT("VTEMP:%d  ",adcResult.chipTemperature);
#endif
    /* calculate battery volt */
    adcResult.vBatt.target = get_volt(&adcResult.vBatt);
#if CODE_DEBUG_EN == 1
    (void)DEBUG_OUT("VBATT:%4d  ",adcResult.vBatt.target);
#endif
    /* calculate buck volt */
    adcResult.vBuck.target = get_volt(&adcResult.vBuck);
#if CODE_DEBUG_EN == 1
    (void)DEBUG_OUT("VBUCK:%4d  ",adcResult.vBuck.target);
#endif
}

#if 1
int8_t get_led_temperature(TempParams_t const table[],int16_t deltaPNVolt)
{
    int8_t temperature;
    uint8_t minPos = 0U;
    uint8_t maxPos = TEMP_CMP_NUM_MAX-1U;
    uint8_t tempPos = ((TEMP_CMP_NUM_MAX-1U)>> 1U);
    for(;;){
        if (deltaPNVolt > table[tempPos].deltaVoltPN){
            maxPos = tempPos;
        }else if (deltaPNVolt < table[tempPos].deltaVoltPN){
            minPos = tempPos;
        }else{
            break;
        }
        tempPos = (minPos + maxPos) >> 1U;
        if ((minPos + 1U)>= maxPos){
            break;
        }
    }
    if (deltaPNVolt <= table[TEMP_CMP_NUM_MAX-1U].deltaVoltPN){
        tempPos = TEMP_CMP_NUM_MAX-1U;
    }
    tempPos = tempPos << DELTA_TEMP_POS;
    temperature = (int8_t)tempPos;
    temperature = temperature + MINIMUM_TEMP;
    return temperature;
}

uint32_t volt_temp_test = 0;
extern uint32_t PWM_Duty_Set;

int16_t get_gpiovolt(AdcDatabuff_t *const param)
{
    //int16_t volt = (int16_t)INVALID_VOLT;
    //int32_t sVolt;
    //uint32_t uVolt;
    uint32_t sum = 0;
    //if (param->currCode != INVALID_PARAM)
    {
        for (uint8_t i = 0U; i < AVERAGE_MEASURE_POS; i++){
            sum += param->buff[i];
        }
        //sum  = sum >> AVERAGE_MEASURE_POS;
        sum = (uint32_t)sum/(AVERAGE_MEASURE_POS);
        //sVolt = param->coeff.coefficient * (int32_t)sum + param->coeff.offset;
        //sVolt = sum;
        //uVolt = (uint32_t)sVolt;
        //uVolt = uVolt >> MEASURE_GAIN_POS;
        //volt_temp_test = volt = (int16_t)uVolt;
    }

    if(PWM_Duty_Set>=2000) sum = sum - 300;
    else if(PWM_Duty_Set<2000) sum = sum - 200;

    return sum;
}


void updateLedTemperature(LedNum_t ledIndex)
{
    //int16_t typicalVolt[3];
    //ColorTemperature_t temperature;
    //temperature.value[0] = ROOM_TEMPERATURE;
    //temperature.value[1] = ROOM_TEMPERATURE;
    //temperature.value[2] = ROOM_TEMPERATURE;
    
    //(void)CLM_GetLedPNVolts(ledIndex,temperature, &typicalVolt[0], &typicalVolt[1], &typicalVolt[2]);
    for (uint8_t i = 0U; i < 3U; i++){
        adcResult.vLedPN[ledIndex][i].target = get_gpiovolt(&adcResult.vLedPN[ledIndex][i]);
        //adcResult.vLedPN[ledIndex][i].target = get_volt(&adcResult.vLedPN[ledIndex][i]);        
#if 0        
        if (adcResult.vLedPN[ledIndex][i].target != (int16_t)INVALID_VOLT){
            if (i == (uint8_t)LED_RED){
                adcResult.ledPNTemperature[ledIndex][i] = get_led_temperature(LP_GetTempTableRed(),  (adcResult.vLedPN[ledIndex][i].target - typicalVolt[i]) );
            }else if (i == (uint8_t)LED_GREEN){
                adcResult.ledPNTemperature[ledIndex][i] = get_led_temperature(LP_GetTempTableGreen(),(adcResult.vLedPN[ledIndex][i].target - typicalVolt[i]) );
            }else{
                adcResult.ledPNTemperature[ledIndex][i] = get_led_temperature(LP_GetTempTableBlue(), (adcResult.vLedPN[ledIndex][i].target - typicalVolt[i]) );
            }
        }else{
            adcResult.ledPNTemperature[ledIndex][i] = ROOM_TEMPERATURE;
        }
#endif        
#if CODE_DEBUG_EN == 1
        (void)DEBUG_OUT("TLED[%d][%d]:%3d  ",ledIndex,i,adcResult.vLedPN[ledIndex][i].target);
#endif  
    }
#if CODE_DEBUG_EN == 1
    (void)DEBUG_OUT("\r\n");
#endif
}
#endif
void updateParams(void)
{
    if( updateParamIndex == 0U){
        /* update system information */
        updateSystemInfo();
    }else if( updateParamIndex == 1U){
        /* update led temperature information LED0-LED3*/
#if LED_NUM > 1U
        for (int8_t i = (int8_t)LED0; i <(LED_NUM/2); i++){
            //updateLedTemperature((LedNum_t)i);
        }
#endif
    }else{
        /* update led temperature information LED4-LED7*/
        for (uint8_t i = (LED_NUM/2); i < LED_NUM; i++){
            updateLedTemperature((LedNum_t)i);
        }
#if CODE_DEBUG_EN == 1
        (void)DEBUG_OUT("\n");
#endif
    }
    
    if (++updateParamIndex > 2U){
        updateParamIndex = 0U;
    }
}


void MES_TaskHandler(void)
{
    switch(adcTaskState){
    case TASK_STATE_INIT:
      measureParamInit();
      
      ADC_GeneralInit();/*  init ADC */
      ADC_RegisterIRQ(measureDoneISR);
      /* start updating timer */
      SoftTimer_Start(&updateTimer);
      adcTaskState = TASK_STATE_ACTIVE;
      break;
    case TASK_STATE_ACTIVE:
      /* start parameters updating  */
      if (updateParamStart != 0U){
          updateParamStart = 0U;
          updateParams();
      }
      /*  start parameters measuring */
      if (measStart != 0U){
          measStart = 0U;
          measureParamStart();
      }
      break;
    default:
      break;
    }
}

#define CODEH_VOLT_DEF 3300U //4000U
#define CODEL_VOLT_DEF 0U //1500U

#define CODEH_VOLT_SET 4000U
#define CODEL_VOLT_SET 1500U

void measureParamInit(void)
{
    uint32_t crc = CRC32_GetRunTimeCRC32((uint8_t *)HWCFG_SFRS->payload+4U ,508U);
    if (crc != HWCFG_SFRS->crc32){
        dataIntegrityError = TRUE;
    }
    /*  Vbap:1.188V  */
    uint16_t adc0V5 = HWCFG_SFRS->ADC_0V5;
    uint16_t adc1V0 = HWCFG_SFRS->ADC_1V0;
    adcResult.vChipTemp.coeff.coefficient = (1000 -500)*MEASURE_GAIN/((int32_t)adc1V0 - (int32_t)adc0V5);
    adcResult.vChipTemp.coeff.offset =  500*MEASURE_GAIN - adcResult.vChipTemp.coeff.coefficient*(int32_t)adc0V5;
    adcResult.vChipTemp.currCode = INVALID_PARAM;
    /* v = a*t + b;  a = 2.2)  t = v/a - b/a;   gain = 1/a, offset = - b/a*/
    adcResult.tChip.coefficient = MEASURE_TEMP_GAIN;
    adcResult.tChip.offset      = 25*(int32_t)(1UL << MEASURE_TEMP_GAIN_POS) - MEASURE_TEMP_GAIN*(int32_t)HWCFG_SFRS->TSENSOR_mV_25C;
    
    /* battery volt*/
    uint16_t batt13V5 = HWCFG_SFRS->VBAT_CODE_13V5;
    uint16_t batt8V0 = HWCFG_SFRS->VBAT_CODE_8V0;
    
    if (batt8V0 == 0xFFFFU){
        adcResult.vBatt.coeff.coefficient = (13500 - 0)*MEASURE_GAIN/((int32_t)batt13V5 - 0);
        adcResult.vBatt.coeff.offset      = 0;
    }else{
        adcResult.vBatt.coeff.coefficient = (13500 - 8000)*MEASURE_GAIN/((int32_t)batt13V5 - (int32_t)batt8V0);
        adcResult.vBatt.coeff.offset      = 8000*MEASURE_GAIN - adcResult.vBatt.coeff.coefficient*(int32_t)batt8V0;
    }
    
    adcResult.vBatt.currCode    = INVALID_PARAM;
    /* buck volt*/
    uint16_t buck5V0 = HWCFG_SFRS->BUCK_5V_ADC_CODE;
    adcResult.vBuck.coeff.coefficient = (5000- 0)*MEASURE_GAIN/((int32_t)buck5V0 - 0);
    adcResult.vBuck.coeff.offset = 0;
    adcResult.vBuck.currCode = INVALID_PARAM;
    #if 1
    /* led parameters init */
    uint16_t codeH,codeL;
    for (uint8_t i = 0; i < LED_NUM; i++){
        codeH = HWCFG_SFRS->LED[PHY_CHANNEL_R(i)].PN_CODE_VOLT_4000mV;
        codeL = HWCFG_SFRS->LED[PHY_CHANNEL_R(i)].PN_CODE_VOLT_1500mV;
        
        //codeH = CODEH_VOLT_DEF;
        //codeL = CODEL_VOLT_DEF;        

        adcResult.vLedPN[i][0].coeff.coefficient = (CODEH_VOLT_SET - CODEL_VOLT_SET)*MEASURE_GAIN/((int32_t)codeH - (int32_t)codeL);
        adcResult.vLedPN[i][0].coeff.offset      = CODEL_VOLT_SET*MEASURE_GAIN - adcResult.vLedPN[i][0].coeff.coefficient*(int32_t)codeL;
        adcResult.vLedPN[i][0].currCode = INVALID_PARAM;
        
        codeH = HWCFG_SFRS->LED[PHY_CHANNEL_G(i)].PN_CODE_VOLT_4000mV;
        codeL = HWCFG_SFRS->LED[PHY_CHANNEL_G(i)].PN_CODE_VOLT_1500mV;

        //codeH = CODEH_VOLT_DEF;
        //codeL = CODEL_VOLT_DEF;        

        adcResult.vLedPN[i][1].coeff.coefficient = (CODEH_VOLT_SET - CODEL_VOLT_DEF)*MEASURE_GAIN/((int32_t)codeH - (int32_t)codeL);
        adcResult.vLedPN[i][1].coeff.offset      = CODEL_VOLT_DEF*MEASURE_GAIN - adcResult.vLedPN[i][1].coeff.coefficient*(int32_t)codeL;
        adcResult.vLedPN[i][1].currCode = INVALID_PARAM;

        codeH = HWCFG_SFRS->LED[PHY_CHANNEL_B(i)].PN_CODE_VOLT_4000mV;
        codeL = HWCFG_SFRS->LED[PHY_CHANNEL_B(i)].PN_CODE_VOLT_1500mV;


        //codeH = CODEH_VOLT_DEF;
        //codeL = CODEL_VOLT_DEF;        

        adcResult.vLedPN[i][2].coeff.coefficient = (CODEH_VOLT_SET - CODEL_VOLT_SET)*MEASURE_GAIN/((int32_t)codeH - (int32_t)codeL);
        adcResult.vLedPN[i][2].coeff.offset      = CODEL_VOLT_SET*MEASURE_GAIN - adcResult.vLedPN[i][2].coeff.coefficient*(int32_t)codeL;
        adcResult.vLedPN[i][2].currCode = INVALID_PARAM;
        
        for (uint8_t channel  = 0U; channel < 3U; channel++){
            adcResult.vLedPN[i][channel].target    = (int16_t)INVALID_VOLT;
            adcResult.ledPNTemperature[i][channel] = ROOM_TEMPERATURE;
        }
    }

    // adcResult.vLedPN[0][1].coeff.coefficient = 3300U; //24000;    
    // adcResult.vLedPN[0][1].coeff.offset = 0U;

    // adcResult.vLedPN[0][2].coeff.coefficient = 3300U; //800;    
    // adcResult.vLedPN[0][2].coeff.offset = 0U;
    #endif
    
}

/*
 *  Get factory calibration data state
 *  @return 0: calibration data OK, 1 : calibration data error
 */
uint8_t MES_DataIntegrityError(void)
{
    return dataIntegrityError;
  
}

/*
 *  Get battery voltage in mV
 *  @param [out] battVolt: 0-65535
 *  @return 0: get voltage success, other : failed
 */
int8_t MES_GetBatteryVolt(uint16_t *battVolt)
{
    int8_t result = -1;
    if (adcResult.vBatt.currCode != INVALID_PARAM){
        *battVolt = (uint16_t)adcResult.vBatt.target;
        result = 0;
    }
    return result;
}

/*
 *  Get buck voltage in mV
 *  @param [out] buckVolt: 0-65535
 *  @return 0: get voltage success, other : failed
 */
int8_t MES_GetBuckVolt(uint16_t *buckVolt)
{
    int8_t result = -1;
    if (adcResult.vBuck.currCode != INVALID_PARAM){
        *buckVolt = (uint16_t)adcResult.vBuck.target;
        result = 0;
    }
    return result;
}


int16_t MES_GetChipTemperature(int16_t *chipTemperature)
{
    int8_t result = -1;
    if (adcResult.vChipTemp.currCode != INVALID_PARAM){
        *chipTemperature = adcResult.chipTemperature;
        result = 0;
    }
    return result;
}

int16_t MES_GetChipTempVolt(uint16_t *chipTempVolt)
{
    int8_t result = -1;
    if (adcResult.vChipTemp.currCode != INVALID_PARAM){
        *chipTempVolt = adcResult.vChipTemp.target;
        result = 0;
    }
    return result;
}


#if 0
ColorTemperature_t MES_GetLedTemperature(LedNum_t ledNo)
{
    ColorTemperature_t temperature;
    temperature.value[0] = (int8_t)adcResult.ledPNTemperature[ledNo][0];
    temperature.value[1] = (int8_t)adcResult.ledPNTemperature[ledNo][1];
    temperature.value[2] = (int8_t)adcResult.ledPNTemperature[ledNo][2];
    return temperature;
}
#endif

int8_t MES_GetLedRGBTemperature(LedNum_t index, int8_t *rTemp,int8_t *gTemp,int8_t *bTemp)
{
    *rTemp = (int8_t)adcResult.ledPNTemperature[index][0];
    *gTemp = (int8_t)adcResult.ledPNTemperature[index][1];
    *bTemp = (int8_t)adcResult.ledPNTemperature[index][2];
    return 0;
}

/*
 *  Get Runtime Led PN voltage in mV
 *  @param [out] pnVoltR/pnVoltG/pnVoltB: 0-4000mV
 *  @return :none
 */
void MES_GetRunTimeLedPNVolt(LedNum_t ledIndex,uint16_t *pnVoltR, uint16_t *pnVoltG,uint16_t *pnVoltB)
{
    *pnVoltR = (uint16_t)adcResult.vLedPN[ledIndex][0].target;
    *pnVoltG = (uint16_t)adcResult.vLedPN[ledIndex][1].target;
    *pnVoltB = (uint16_t)adcResult.vLedPN[ledIndex][2].target;
}


void MES_MnftGetLedInfo(LedNum_t ledNo, uint16_t *pnRVolt,uint16_t *pnGVolt,uint16_t *pnBVolt)
{
    int32_t sLedCode = 0;
    uint32_t uLedCode[3];
    for(uint8_t i = 0U; i < LED_NUM; i++){
        //(void)CLM_SetPWMs((LedNum_t)i,0U, 0U, 0U, 0U);  /* turn off all of channels */
    }
    while(PWM_UpdateFinished() != TRUE){}
    
    for (uint8_t led = 0U; led < 3U; led++){
        uLedCode[led] = 0U;
        ADC_Init(ADC_MEASURE_ITEM_LED,(uint8_t)ledNo,(LedType_t)led,SYNC_MODE_SOFT_INPUT);
        for (uint16_t i = 0U; i < 16U; i++){
            ADC_Start();
            adcConvertDone = 0U;
            ADC_SFRS->CONF.SWSYNCIN = 1; /* soft trigger for adc conversion   */
            ADC_SFRS->CONF.SWSYNCIN = 0;
            while(adcConvertDone == 0U){}
            uLedCode[led] += measGeneralAdcCode[2];
        }
        uLedCode[led] = (uLedCode[led] >> 4);
        sLedCode = (int32_t)uLedCode[led];
        sLedCode = sLedCode* adcResult.vLedPN[ledNo][led].coeff.coefficient + adcResult.vLedPN[ledNo][led].coeff.offset;
        uLedCode[led] = (uint32_t)sLedCode;
    }
    *pnRVolt = (uint16_t)(uLedCode[0] >> MEASURE_GAIN_POS);
    *pnGVolt = (uint16_t)(uLedCode[1] >> MEASURE_GAIN_POS);
    *pnBVolt = (uint16_t)(uLedCode[2] >> MEASURE_GAIN_POS);
    (void)sLedCode;
}

















