#include <hwProtection.h>
#include <appConfig.h>
#include <isrfuncs.h>

static hwProtIsrCallback_t ovTemperatureCallback = NULL;
static hwProtIsrCallback_t ovVoltageCallback = NULL;
static hwProtIsrCallback_t uvVoltageCallback = NULL;


void OVTEMP_Handler(void)
{
    if (ovTemperatureCallback != NULL){
        ovTemperatureCallback();
    }
    CRGA_SFRS->RESETCTRL.OVTEMPFLAGCLR = 1U;
}


void VBAT_Handler(void)
{
    if (PMUA_SFRS->PMUIRQ.STATUS.UV == 1U){
        if (uvVoltageCallback != NULL){
            uvVoltageCallback();
        }
        PMUA_SFRS->PMUIRQ.CLEAR.UV =1U;
    }
    if (PMUA_SFRS->PMUIRQ.STATUS.OV == 1U){
        if (ovVoltageCallback != NULL){
            ovVoltageCallback();
        }
        PMUA_SFRS->PMUIRQ.CLEAR.OV =1U;
    }
}


void HW_PROT_SetOverTmperature(OverTempThres_t threshold, OverTempAction_t action)
{
    CRGA_SFRS->OVTEMPCONFIG.OVTEMPLEVEL = (uint8_t)threshold;
    CRGA_SFRS->OVTEMPACTION.OVTEMP      = (uint8_t)action;
    CRGA_SFRS->OVTEMPCONFIG.DISTEMPSENS = 0U;    /* enable temperature sensor */
    CRGA_SFRS->OVTEMPCONFIG.OVTEMPENA   = 1U;    /* enable ov temperature action */ 
}

void HW_PROT_RegisterOVTemperatureIRQ(hwProtIsrCallback_t callback)
{
    ovTemperatureCallback = callback;
    CRGA_SFRS->RESETCTRL.OVTEMPFLAGCLR = 1U;
    NVIC_EnableIRQ(OVTEMP_IRQn);
}

void HW_PROT_UnRegisterOVTemperatureIRQ(void)
{
    ovTemperatureCallback = NULL;
    CRGA_SFRS->RESETCTRL.OVTEMPFLAGCLR = 1U;
    NVIC_DisableIRQ(OVTEMP_IRQn);
}


void HW_PROT_SetOverVoltage(OverVoltThres_t threshold)
{
    PMUA_SFRS->VBATTRIM.OVLEVEL    = (uint8_t)threshold;
    PMUA_SFRS->VBATCTRL.OV_MONITOR_ENA = 1U;
    PMUA_SFRS->VBATDBNC.LFENA   = 1U;
    PMUA_SFRS->VBATDBNC.OV      = 1U;
}

void HW_PROT_RegisterOverVoltageIRQ(hwProtIsrCallback_t callback)
{
    ovVoltageCallback           = callback;
    PMUA_SFRS->PMUIRQ.CLEAR.OV  = 1U;
    PMUA_SFRS->PMUIRQ.IRQ.OV    = 1U;
    NVIC_EnableIRQ(VBAT_IRQn);
}

void HW_PROT_UnRegisterOverVoltageIRQ(void)
{
    ovVoltageCallback           = NULL;
    PMUA_SFRS->PMUIRQ.CLEAR.OV  = 1U;
    PMUA_SFRS->PMUIRQ.IRQ.OV    = 0U;
    NVIC_DisableIRQ(VBAT_IRQn);
}

void HW_PROT_SetUnderVoltage(UnderVoltThres_t threshold)
{
    PMUA_SFRS->VBATTRIM.UVLEVEL         = (uint8_t)threshold;
    PMUA_SFRS->VBATCTRL.LOW_MONITOR_ENA = 1U;
    PMUA_SFRS->VBATDBNC.LFENA   = 1U;
    PMUA_SFRS->VBATDBNC.UV      = 1U;
}

void HW_PROT_RegisterUnderVoltageIRQ(hwProtIsrCallback_t callback)
{
    uvVoltageCallback           = callback;
    PMUA_SFRS->PMUIRQ.CLEAR.UV  = 1U;
    PMUA_SFRS->PMUIRQ.IRQ.UV    = 1U;
    NVIC_EnableIRQ(VBAT_IRQn);
}

void HW_PROT_UnRegisterUnderVoltageIRQ(void)
{
    ovVoltageCallback           = NULL;
    PMUA_SFRS->PMUIRQ.CLEAR.UV  = 1U;
    PMUA_SFRS->PMUIRQ.IRQ.UV    = 0U;
    NVIC_DisableIRQ(VBAT_IRQn);
}


uint8_t HW_PROT_BattVoltIsTooLow(void)
{
    uint8_t result = (uint8_t)PMUA_SFRS->VBATCTRL.LOW;
    return result;
}

uint8_t HW_PROT_BattVoltIsTooHigh(void)
{
    uint8_t result = (uint8_t)PMUA_SFRS->VBATCTRL.HIGH;
    return result;
}

uint8_t HW_PROT_ChipIsOverHeat(void)
{
    uint8_t result = (uint8_t)CRGA_SFRS->RESETCTRL.OVTEMPFLAG;
    return result;
}




