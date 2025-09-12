#ifndef APP_LIN_H_
#define APP_LIN_H_

#define MPMODE_CNT                              50U
#define MPMODE_RESET_CNT                        3000U

#define UNLOCK_PASSWARD_1                       0xd2cb9dd9f7973e3e
#define UNLOCK_PASSWARD_2                       0xabe3008dac64b685
#define UNLOCK_PASSWARD_3                       0x4137391c10803f2c
#define UNLOCK_PASSWARD_4                       0xcce295eb96a241f8

typedef struct tag_sLinReceiveData
{
    uint8_t             C_3way_PosReq;
    uint8_t             C_3way_MoveEnable;
    uint8_t             C_3way_Initial;
    uint8_t             C_3way_LinError;
    uint64_t            C_3way_DataFrame;
}RECEIVE;

typedef struct tag_sLinInformation
{
    RECEIVE             sReceive;
}LININFO;

void Lin_Task(void);

#endif
