#ifndef APP_MOTOR_H_
#define APP_MOTOR_H_

#define COOLANT_CV

#ifdef COOLANT_CV
#define MOTOR_INIT_STALL_CNT            76U //96 * 0.8
#define MOTOR_REQ_STALL_CNT             66U
#define MOTOR_MPMODE_STALL_CNT          35U
#endif

#define INIT_RETRY_CNT                  5U
#define REQ_RETRY_CNT                   5U

#define MOTOR_MAXPWM_VOLTAGE            16000U          // 16.0V
#define MOTOR_MINPWM_VOLTAGE            9000U           // 9.0V
#define MOTOR_INITSTOP_CNT              50U
#define MOTOR_MOVESTOP_CNT              100U
#define MOTOR_RUNERROR_TMR              240U            // 24s
#define MOTOR_TRELEASE_TMR              600U            // 1m
#define MOTOR_RELEASE_TMR               3000U           // 5m
#define MOTOR_SOFTSTOP_HALL             64U
#define MOTOR_SEAL_HALL                 72U
#define MOTOR_TARGET_HALL               81U

#define PWM_DUTY00                      0U
#define PWM_DUTY10                      10U
#define PWM_DUTY20                      20U
#define PWM_DUTY21                      21U
#define PWM_DUTY22                      22U
#define PWM_DUTY23                      23U
#define PWM_DUTY24                      24U
#define PWM_DUTY25                      25U
#define PWM_DUTY26                      26U
#define PWM_DUTY27                      27U
#define PWM_DUTY28                      28U
#define PWM_DUTY29                      29U
#define PWM_DUTY30                      30U
#define PWM_DUTY31                      31U
#define PWM_DUTY32                      32U
#define PWM_DUTY33                      33U
#define PWM_DUTY34                      34U
#define PWM_DUTY35                      35U
#define PWM_DUTY36                      36U
#define PWM_DUTY37                      37U
#define PWM_DUTY38                      38U
#define PWM_DUTY39                      39U
#define PWM_DUTY40                      40U
#define PWM_DUTY41                      41U
#define PWM_DUTY42                      42U
#define PWM_DUTY43                      43U
#define PWM_DUTY44                      44U
#define PWM_DUTY45                      45U
#define PWM_DUTY46                      46U
#define PWM_DUTY47                      47U
#define PWM_DUTY48                      48U
#define PWM_DUTY49                      49U
#define PWM_DUTY50                      50U
#define PWM_DUTY51                      51U
#define PWM_DUTY52                      52U
#define PWM_DUTY53                      53U
#define PWM_DUTY54                      54U
#define PWM_DUTY55                      55U
#define PWM_DUTY56                      56U
#define PWM_DUTY57                      57U
#define PWM_DUTY58                      58U
#define PWM_DUTY59                      59U
#define PWM_DUTY60                      60U
#define PWM_DUTY61                      61U
#define PWM_DUTY62                      62U
#define PWM_DUTY63                      63U
#define PWM_DUTY64                      64U
#define PWM_DUTY65                      65U
#define PWM_DUTY66                      66U
#define PWM_DUTY67                      67U
#define PWM_DUTY68                      68U
#define PWM_DUTY69                      69U
#define PWM_DUTY70                      70U
#define PWM_DUTY71                      71U
#define PWM_DUTY72                      72U
#define PWM_DUTY73                      73U
#define PWM_DUTY74                      74U
#define PWM_DUTY75                      75U
#define PWM_DUTY76                      76U
#define PWM_DUTY77                      77U
#define PWM_DUTY78                      78U
#define PWM_DUTY79                      79U
#define PWM_DUTY80                      80U
#define PWM_DUTY81                      81U
#define PWM_DUTY82                      82U
#define PWM_DUTY83                      83U
#define PWM_DUTY84                      84U
#define PWM_DUTY85                      85U
#define PWM_DUTY86                      86U
#define PWM_DUTY87                      87U
#define PWM_DUTY88                      88U
#define PWM_DUTY89                      89U
#define PWM_DUTY90                      90U
#define PWM_DUTY91                      91U
#define PWM_DUTY92                      92U
#define PWM_DUTY93                      93U
#define PWM_DUTY94                      94U
#define PWM_DUTY95                      95U
#define PWM_DUTY96                      96U
#define PWM_DUTY97                      97U
#define PWM_DUTY98                      98U
#define PWM_DUTY99                      99U
#define PWM_DUTY100                     100U

typedef enum eMotorRunning
{
    MOVING_DONE = 0,
    MOVING = 1,
}MOTORRUNNING;

typedef enum eMotorDirection
{
    MotorDirectionCW = 0,
    MotorDirectionCCW = 1,
}MOTORDIRECTION;

typedef struct tag_sMotorInformation
{
    uint8_t         u8MotorInitDone;
    uint8_t         u8MotorInitMode;
    uint8_t         u8MotorInitModeFirst;
    uint8_t         u8MotorInitReq;
    uint8_t         u8MotorInitReqFirst;
    uint8_t         u8MotorMoveEnable;
    uint8_t         u8MotorPositionFB;
    uint8_t         u8MotorRunning;
    uint16_t        u16MotorRunningCount;
    uint8_t         u8MotorDirection;
    uint16_t        u16MotorRpm;
    
    uint8_t         u8MotorCurrnetStatus;
    uint8_t         u8MotorStallStatus;
    
    uint8_t         u8ValveMPCount;
    uint8_t         u8ValveMPMode;
    uint8_t         u8ValveReqRetryMode;
    uint8_t         u8ValveCWRetryCnt;
    uint8_t         u8ValveCCWRetryCnt;
    uint8_t         u8ValveWakeupMode;
    uint8_t         u8ValveSoftstopMode;
    
    uint8_t         u8HallValue;
    uint8_t         u8TargetHallValue;
    uint8_t         u8SealHallValue;
    uint8_t         u8SoftHallValue;
}MOTORINFO;

typedef struct tag_sChecksumInformation
{
    uint8_t      u8SleepChecksum;
}CHECKSUMINFO;

typedef struct tag_sControlInformation
{
    FAULTFLAG       uFaultFlag;
    
    ADCINFO         sAdc;
    LININFO         sLin;
    MOTORINFO       sMotor;
    MDRIVERINFO     sMotorDriver;
    
    CHECKSUMINFO    sChecksum;
}CONTROLINFO;

extern CONTROLINFO CoolValveInfo;
void Main_Task(void);

#endif
