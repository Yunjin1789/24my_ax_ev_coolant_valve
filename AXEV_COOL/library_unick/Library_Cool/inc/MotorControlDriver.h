#ifndef MOTORCONTROLDRIVER_H_
#define MOTORCONTROLDRIVER_H_

#include <softTimerTask.h>

typedef struct {
    uint16_t Count_1ms;
    uint16_t Count_5ms;
    uint16_t Count_10ms;
    uint16_t Count_100ms;
    uint16_t Count_1000ms;
    uint8_t Flag_1ms;
    uint8_t Flag_5ms;
    uint8_t Flag_10ms;
    uint8_t Flag_100ms;
    uint8_t Flag_1000ms;
}Timer_Set;

extern Timer_Set Timer_Count_Check;

void MotorExpired(SoftTimer_t *timer);
void M_ENBL_PutVal(bool Val);
void M_DIS_PutVal(bool Val);
void M_IN1_PutVal(bool Val);
void M_FS_B_PutVal(bool Val);
uint8_t M_FS_B_GetVal(void);

#endif
