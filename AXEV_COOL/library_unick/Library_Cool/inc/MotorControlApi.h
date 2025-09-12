#ifndef MOTORCONTROLAPI_H_
#define MOTORCONTROLAPI_H_

#define PWM_DUTY00      0
#define PWM_DUTY10      10
#define PWM_DUTY20	20
#define PWM_DUTY21	21
#define PWM_DUTY22	22
#define PWM_DUTY23	23
#define PWM_DUTY24	24
#define PWM_DUTY25	25
#define PWM_DUTY26	26
#define PWM_DUTY27	27
#define PWM_DUTY28	28
#define PWM_DUTY29	29
#define PWM_DUTY30	30
#define PWM_DUTY31	31
#define PWM_DUTY32	32
#define PWM_DUTY33	33
#define PWM_DUTY34	34
#define PWM_DUTY35	35
#define PWM_DUTY36	36
#define PWM_DUTY37	37
#define PWM_DUTY38	38
#define PWM_DUTY39	39
#define PWM_DUTY40	40
#define PWM_DUTY41	41
#define PWM_DUTY42	42
#define PWM_DUTY43	43
#define PWM_DUTY44	44
#define PWM_DUTY45	45
#define PWM_DUTY46	46
#define PWM_DUTY47	47
#define PWM_DUTY48	48
#define PWM_DUTY49	49
#define PWM_DUTY50	50
#define PWM_DUTY51	51
#define PWM_DUTY52	52
#define PWM_DUTY53	53
#define PWM_DUTY54	54
#define PWM_DUTY55	55
#define PWM_DUTY56	56
#define PWM_DUTY57	57
#define PWM_DUTY58	58
#define PWM_DUTY59	59
#define PWM_DUTY60	60
#define PWM_DUTY61	61
#define PWM_DUTY62	62
#define PWM_DUTY63	63
#define PWM_DUTY64	64
#define PWM_DUTY65	65
#define PWM_DUTY66	66
#define PWM_DUTY67	67
#define PWM_DUTY68	68
#define PWM_DUTY69	69
#define PWM_DUTY70	70
#define PWM_DUTY71	71
#define PWM_DUTY72	72
#define PWM_DUTY73	73
#define PWM_DUTY74	74
#define PWM_DUTY75	75
#define PWM_DUTY76	76
#define PWM_DUTY77	77
#define PWM_DUTY78	78
#define PWM_DUTY79	79
#define PWM_DUTY80	80
#define PWM_DUTY81	81
#define PWM_DUTY82	82
#define PWM_DUTY83	83
#define PWM_DUTY84	84
#define PWM_DUTY85	85
#define PWM_DUTY86	86
#define PWM_DUTY87	87
#define PWM_DUTY88	88
#define PWM_DUTY89	89
#define PWM_DUTY90	90
#define PWM_DUTY91	91
#define PWM_DUTY92	92
#define PWM_DUTY93	93
#define PWM_DUTY94	94
#define PWM_DUTY95	95
#define PWM_DUTY96	96
#define PWM_DUTY97	97
#define PWM_DUTY98	98
#define PWM_DUTY99	99
#define PWM_DUTY100	100

extern uint8_t Pre_LIN_Order;
extern uint8_t Now_LIN_Order;
extern uint8_t Stall_Current_Status;

typedef enum tag_MotorDirection
{
    MotorDirectionCW = 0,
    MotorDirectionCCW = 1,
}MotorDirection;

typedef struct{

    uint16_t Status_ERR_OK;
    uint16_t Motor_Short_circuit;
    uint16_t Motor_OPEN_Load;
    uint16_t Thermal_warning;
    uint16_t OVER_Temperature_Shutdown;
    uint16_t Open_Load_Check_Count;
    uint16_t Motor_Short_circuit_Count;
    uint16_t Thermal_warning_Count;
    uint16_t OVER_Temperature_Shutdown_Count;
    uint16_t Motor_Shorted; 
    uint16_t Motor_OT;
    uint16_t Motor_TW;
    uint16_t Motor_OL;
    uint16_t Motor_OV;
    uint16_t Motor_UV;
} Motor_Status_ERR_Check;

typedef struct{

    uint8_t             C_3way_PosReq;
    uint8_t             C_3way_MoveEnalbe;
    uint8_t             C_3way_Initial;
    uint8_t             C_3way_LinError;
    
} LIN_Master;

typedef struct{
	uint16_t High_Volatage_Error;
	uint16_t Low_Volatage_Error;
	uint16_t Stall_ERR_flag;
	uint16_t Dust_ERR_flag;
	uint16_t Hall_ERR_flag;
	uint16_t Motor_Open_ERR_flag;
	uint16_t Motor_Shorted_ERR_flag;
	uint16_t	OVER_Temperature_Shutdown;
	uint16_t Thermal_warning;
	uint16_t Thermal_warning_Set;	
}Status_Erorr;

extern Motor_Status_ERR_Check Motor_Status_ERR;
extern Status_Erorr Status_ERR;
extern LIN_Master Datc;

uint8_t Hall_Sampling(void);
void Motor_Direction(MotorDirection num);
void Motor_Stop(void);
void Main_Task(void);

void MotorStallCheck(void);
void MotorControlInit(void);
void MotorControlTask(void);

#endif
