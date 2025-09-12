/******************************************************************************
*
* @file      lin_cfg.h
*
* @author    FPT Software
*
* @version   1.0
*
* @date      Sun Jan 08 23:03:34 KST 2023
*
* @brief     Hardware configuration file
*
******************************************************************************/
#ifndef    _LIN_CFG_H_
#define    _LIN_CFG_H_
#include "lin_hw_cfg.h"
#include "linstackTask.h"

/* Define operating mode */
#define _MASTER_MODE_     0
#define _SLAVE_MODE_      1
#define LIN_MODE   _SLAVE_MODE_
/* Define protocol version */
#define PROTOCOL_21       0
#define PROTOCOL_J2602    1
#define PROTOCOL_20       2
#define LIN_PROTOCOL    PROTOCOL_21

#define UART_ADDR        UART0_ADDR    /* For slave */


#define LIN_BAUD_RATE    19200    	 /*For slave*/
/**********************************************************************/
/***************          Diagnostic class selection  *****************/
/**********************************************************************/
#define _DIAG_CLASS_I_          0
#define _DIAG_CLASS_II_         1
#define _DIAG_CLASS_III_        2

#define _DIAG_CLASS_SUPPORT_    _DIAG_CLASS_I_

#define MAX_LENGTH_SERVICE 6

#define MAX_QUEUE_SIZE 1


#define _DIAG_NUMBER_OF_SERVICES_    2

#define DIAGSRV_READ_BY_IDENTIFIER_ORDER    0

#define DIAGSRV_ASSIGN_FRAME_ID_RANGE_ORDER    1


/**************** FRAME SUPPORT DEFINITION ******************/
#define _TL_SINGLE_FRAME_       0
#define _TL_MULTI_FRAME_        1

#define _TL_FRAME_SUPPORT_      _TL_SINGLE_FRAME_

/* frame buffer size */
#define LIN_FRAME_BUF_SIZE			16
#define LIN_FLAG_BUF_SIZE			3

/**********************************************************************/
/***************               Interfaces           *******************/
/**********************************************************************/
//typedef enum {
//  LI0
//}l_ifc_handle;

/**********************************************************************/
/***************               Signals              *******************/
/**********************************************************************/
/* Number of signals */
#define LIN_NUM_OF_SIGS  13
/* List of signals */
typedef enum {

   /* Interface_name = LI0 */

   LI0_C_3way2_C_LinError

   , LI0_C_3way2_C_InitialSta
  
   , LI0_C_3way2_C_Running
  
   , LI0_C_3way2_C_Fault
  
   , LI0_C_3way2_C_Voltage
  
   , LI0_C_3way2_C_OverTemp
  
   , LI0_C_3way2_C_PositionFB
  
   , LI0_C_3way_PosReq
  
   , LI0_C_3way2_MoveEnable
  
   , LI0_C_3way_Initial
  
   , LI0_C_3way2_LinError
  
   , LI0_C_3way2_C_SW_Ver
  
   , LI0_C_3way2_C_HW_Ver
  
} l_signal_handle;
/**********************************************************************/
/*****************               Frame             ********************/
/**********************************************************************/
/* Number of frames */
#define LIN_NUM_OF_FRMS  4
/* List of frames */
typedef enum {
/* All frames for master node */

   /* Interface_name = LI0 */

   LI0_C_3way2_Datc

   , LI0_Datc_3way2
  
   , LI0_MasterReq
  
   , LI0_SlaveResp
  
} l_frame_handle;
/**********************************************************************/
/***************             Configuration          *******************/
/**********************************************************************/
/* Size of configuration in ROM and RAM used for interface: LI1 */
#define LIN_SIZE_OF_CFG  6
#define LIN_CFG_FRAME_NUM  2
/*********************************************************************
 * global macros
 *********************************************************************/
#define l_bool_rd(SIGNAL) l_bool_rd_##SIGNAL()
#define l_bool_wr(SIGNAL, A) l_bool_wr_##SIGNAL(A)
#define l_u8_rd(SIGNAL) l_u8_rd_##SIGNAL()
#define l_u8_wr(SIGNAL, A) l_u8_wr_##SIGNAL(A)
#define l_u16_rd(SIGNAL) l_u16_rd_##SIGNAL()
#define l_u16_wr(SIGNAL, A) l_u16_wr_##SIGNAL(A)
#define l_bytes_rd(SIGNAL, start, count, data)  l_bytes_rd_##SIGNAL(start, count, data)
#define l_bytes_wr(SIGNAL, start, count, data) l_bytes_wr_##SIGNAL(start, count, data)
#define l_flg_tst(FLAG) l_flg_tst_##FLAG()
#define l_flg_clr(FLAG) l_flg_clr_##FLAG()
#define LIN_TEST_BIT(A,B) ((l_bool)((((A) & (1U << (B))) != 0U) ? 1U : 0U))
#define LIN_SET_BIT(A,B)                      ((A) |= (l_u8) (1U << (B)))
#define LIN_CLEAR_BIT(A,B)               ((A) &= ((l_u8) (~(1U << (B)))))
#define LIN_BYTE_MASK  ((l_u16)(((l_u16)((l_u16)1 << CHAR_BIT)) - (l_u16)1))
#define LIN_FRAME_LEN_MAX                                             10U

/* Returns the low byte of the 32-bit value    */
#define BYTE_0(n)                              ((l_u8)((n) & (l_u8)0xFF))
/* Returns the second byte of the 32-bit value */
#define BYTE_1(n)                        ((l_u8)(BYTE_0((n) >> (l_u8)8)))
/* Returns the third byte of the 32-bit value  */
#define BYTE_2(n)                       ((l_u8)(BYTE_0((n) >> (l_u8)16)))
/* Returns high byte of the 32-bit value       */
#define BYTE_3(n)                       ((l_u8)(BYTE_0((n) >> (l_u8)24)))

/*
 * defines for signal access
 */



#define LIN_BYTE_OFFSET_LI0_C_3way2_C_LinError    0U
#define LIN_BIT_OFFSET_LI0_C_3way2_C_LinError    0U
#define LIN_SIGNAL_SIZE_LI0_C_3way2_C_LinError    1U
#define LIN_FLAG_BYTE_OFFSET_LI0_C_3way2_C_LinError    0U
#define LIN_FLAG_BIT_OFFSET_LI0_C_3way2_C_LinError    0U


#define LIN_BYTE_OFFSET_LI0_C_3way2_C_InitialSta    0U
#define LIN_BIT_OFFSET_LI0_C_3way2_C_InitialSta    2U
#define LIN_SIGNAL_SIZE_LI0_C_3way2_C_InitialSta    2U
#define LIN_FLAG_BYTE_OFFSET_LI0_C_3way2_C_InitialSta    0U
#define LIN_FLAG_BIT_OFFSET_LI0_C_3way2_C_InitialSta    1U


#define LIN_BYTE_OFFSET_LI0_C_3way2_C_Running    0U
#define LIN_BIT_OFFSET_LI0_C_3way2_C_Running    4U
#define LIN_SIGNAL_SIZE_LI0_C_3way2_C_Running    1U
#define LIN_FLAG_BYTE_OFFSET_LI0_C_3way2_C_Running    0U
#define LIN_FLAG_BIT_OFFSET_LI0_C_3way2_C_Running    2U


#define LIN_BYTE_OFFSET_LI0_C_3way2_C_Fault    1U
#define LIN_BIT_OFFSET_LI0_C_3way2_C_Fault    0U
#define LIN_SIGNAL_SIZE_LI0_C_3way2_C_Fault    4U
#define LIN_FLAG_BYTE_OFFSET_LI0_C_3way2_C_Fault    0U
#define LIN_FLAG_BIT_OFFSET_LI0_C_3way2_C_Fault    3U


#define LIN_BYTE_OFFSET_LI0_C_3way2_C_Voltage    1U
#define LIN_BIT_OFFSET_LI0_C_3way2_C_Voltage    4U
#define LIN_SIGNAL_SIZE_LI0_C_3way2_C_Voltage    2U
#define LIN_FLAG_BYTE_OFFSET_LI0_C_3way2_C_Voltage    0U
#define LIN_FLAG_BIT_OFFSET_LI0_C_3way2_C_Voltage    4U


#define LIN_BYTE_OFFSET_LI0_C_3way2_C_OverTemp    1U
#define LIN_BIT_OFFSET_LI0_C_3way2_C_OverTemp    6U
#define LIN_SIGNAL_SIZE_LI0_C_3way2_C_OverTemp    2U
#define LIN_FLAG_BYTE_OFFSET_LI0_C_3way2_C_OverTemp    0U
#define LIN_FLAG_BIT_OFFSET_LI0_C_3way2_C_OverTemp    5U


#define LIN_BYTE_OFFSET_LI0_C_3way2_C_PositionFB    2U
#define LIN_BIT_OFFSET_LI0_C_3way2_C_PositionFB    0U
#define LIN_SIGNAL_SIZE_LI0_C_3way2_C_PositionFB    2U
#define LIN_FLAG_BYTE_OFFSET_LI0_C_3way2_C_PositionFB    0U
#define LIN_FLAG_BIT_OFFSET_LI0_C_3way2_C_PositionFB    6U


#define LIN_BYTE_OFFSET_LI0_C_3way_PosReq    8U
#define LIN_BIT_OFFSET_LI0_C_3way_PosReq    0U
#define LIN_SIGNAL_SIZE_LI0_C_3way_PosReq    2U
#define LIN_FLAG_BYTE_OFFSET_LI0_C_3way_PosReq    2U
#define LIN_FLAG_BIT_OFFSET_LI0_C_3way_PosReq    0U


#define LIN_BYTE_OFFSET_LI0_C_3way2_MoveEnable    10U
#define LIN_BIT_OFFSET_LI0_C_3way2_MoveEnable    0U
#define LIN_SIGNAL_SIZE_LI0_C_3way2_MoveEnable    1U
#define LIN_FLAG_BYTE_OFFSET_LI0_C_3way2_MoveEnable    2U
#define LIN_FLAG_BIT_OFFSET_LI0_C_3way2_MoveEnable    1U


#define LIN_BYTE_OFFSET_LI0_C_3way_Initial    11U
#define LIN_BIT_OFFSET_LI0_C_3way_Initial    0U
#define LIN_SIGNAL_SIZE_LI0_C_3way_Initial    2U
#define LIN_FLAG_BYTE_OFFSET_LI0_C_3way_Initial    2U
#define LIN_FLAG_BIT_OFFSET_LI0_C_3way_Initial    2U


#define LIN_BYTE_OFFSET_LI0_C_3way2_LinError    12U
#define LIN_BIT_OFFSET_LI0_C_3way2_LinError    0U
#define LIN_SIGNAL_SIZE_LI0_C_3way2_LinError    1U
#define LIN_FLAG_BYTE_OFFSET_LI0_C_3way2_LinError    2U
#define LIN_FLAG_BIT_OFFSET_LI0_C_3way2_LinError    3U


#define LIN_BYTE_OFFSET_LI0_C_3way2_C_SW_Ver    7U
#define LIN_BIT_OFFSET_LI0_C_3way2_C_SW_Ver    0U
#define LIN_SIGNAL_SIZE_LI0_C_3way2_C_SW_Ver    8U
#define LIN_FLAG_BYTE_OFFSET_LI0_C_3way2_C_SW_Ver    1U
#define LIN_FLAG_BIT_OFFSET_LI0_C_3way2_C_SW_Ver    0U


#define LIN_BYTE_OFFSET_LI0_C_3way2_C_HW_Ver    6U
#define LIN_BIT_OFFSET_LI0_C_3way2_C_HW_Ver    0U
#define LIN_SIGNAL_SIZE_LI0_C_3way2_C_HW_Ver    8U
#define LIN_FLAG_BYTE_OFFSET_LI0_C_3way2_C_HW_Ver    0U
#define LIN_FLAG_BIT_OFFSET_LI0_C_3way2_C_HW_Ver    7U




#define LIN_FLAG_BYTE_OFFSET_LI0_C_3way2_Datc             0
#define LIN_FLAG_BIT_OFFSET_LI0_C_3way2_Datc              0

#define LIN_FLAG_BYTE_OFFSET_LI0_Datc_3way2             2
#define LIN_FLAG_BIT_OFFSET_LI0_Datc_3way2              0


/**********************************************************************/
/***************        Static API Functions        *******************/
/**********************************************************************/
/*
 * the static signal access macros
 */


/* static access macros for signal LI0_C_3way2_C_LinError */

 
#define l_bool_rd_LI0_C_3way2_C_LinError() \
    (LIN_TEST_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_C_3way2_C_LinError], \
    LIN_BIT_OFFSET_LI0_C_3way2_C_LinError))

#define l_bool_wr_LI0_C_3way2_C_LinError(A) \
    {(A) ? \
    (LIN_SET_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_C_3way2_C_LinError], \
    LIN_BIT_OFFSET_LI0_C_3way2_C_LinError)):\
    (LIN_CLEAR_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_C_3way2_C_LinError], \
    LIN_BIT_OFFSET_LI0_C_3way2_C_LinError));\
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_C_3way2_C_LinError],\
         LIN_FLAG_BIT_OFFSET_LI0_C_3way2_C_LinError);}
 
/* static access macros for signal LI0_C_3way2_C_InitialSta */
 
#define l_u8_rd_LI0_C_3way2_C_InitialSta() \
    ((l_u8)  (((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_C_3way2_C_InitialSta]) >> 2U) & 0x03U))


#define l_u8_wr_LI0_C_3way2_C_InitialSta(A) \
    { \
    lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_C_3way2_C_InitialSta] = \
    (l_u8)((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_C_3way2_C_InitialSta] & 0xf3U) | \
    (((A) << 2U) & 0x0cU)); \
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_C_3way2_C_InitialSta],\
         LIN_FLAG_BIT_OFFSET_LI0_C_3way2_C_InitialSta); \
    }


/* static access macros for signal LI0_C_3way2_C_Running */

 
#define l_bool_rd_LI0_C_3way2_C_Running() \
    (LIN_TEST_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_C_3way2_C_Running], \
    LIN_BIT_OFFSET_LI0_C_3way2_C_Running))

#define l_bool_wr_LI0_C_3way2_C_Running(A) \
    {(A) ? \
    (LIN_SET_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_C_3way2_C_Running], \
    LIN_BIT_OFFSET_LI0_C_3way2_C_Running)):\
    (LIN_CLEAR_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_C_3way2_C_Running], \
    LIN_BIT_OFFSET_LI0_C_3way2_C_Running));\
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_C_3way2_C_Running],\
         LIN_FLAG_BIT_OFFSET_LI0_C_3way2_C_Running);}
 
/* static access macros for signal LI0_C_3way2_C_Fault */
 
#define l_u8_rd_LI0_C_3way2_C_Fault() \
    ((l_u8)  (((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_C_3way2_C_Fault]) >> 0U) & 0x0fU))


#define l_u8_wr_LI0_C_3way2_C_Fault(A) \
    { \
    lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_C_3way2_C_Fault] = \
    (l_u8)((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_C_3way2_C_Fault] & 0xf0U) | \
    (((A) << 0U) & 0x0fU)); \
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_C_3way2_C_Fault],\
         LIN_FLAG_BIT_OFFSET_LI0_C_3way2_C_Fault); \
    }


 
/* static access macros for signal LI0_C_3way2_C_Voltage */
 
#define l_u8_rd_LI0_C_3way2_C_Voltage() \
    ((l_u8)  (((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_C_3way2_C_Voltage]) >> 4U) & 0x03U))


#define l_u8_wr_LI0_C_3way2_C_Voltage(A) \
    { \
    lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_C_3way2_C_Voltage] = \
    (l_u8)((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_C_3way2_C_Voltage] & 0xcfU) | \
    (((A) << 4U) & 0x30U)); \
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_C_3way2_C_Voltage],\
         LIN_FLAG_BIT_OFFSET_LI0_C_3way2_C_Voltage); \
    }


 
/* static access macros for signal LI0_C_3way2_C_OverTemp */
 
#define l_u8_rd_LI0_C_3way2_C_OverTemp() \
    ((l_u8)  (((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_C_3way2_C_OverTemp]) >> 6U) & 0x03U))


#define l_u8_wr_LI0_C_3way2_C_OverTemp(A) \
    { \
    lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_C_3way2_C_OverTemp] = \
    (l_u8)((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_C_3way2_C_OverTemp] & 0x3fU) | \
    (((A) << 6U) & 0xc0U)); \
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_C_3way2_C_OverTemp],\
         LIN_FLAG_BIT_OFFSET_LI0_C_3way2_C_OverTemp); \
    }


 
/* static access macros for signal LI0_C_3way2_C_PositionFB */
 
#define l_u8_rd_LI0_C_3way2_C_PositionFB() \
    ((l_u8)  (((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_C_3way2_C_PositionFB]) >> 0U) & 0x03U))


#define l_u8_wr_LI0_C_3way2_C_PositionFB(A) \
    { \
    lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_C_3way2_C_PositionFB] = \
    (l_u8)((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_C_3way2_C_PositionFB] & 0xfcU) | \
    (((A) << 0U) & 0x03U)); \
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_C_3way2_C_PositionFB],\
         LIN_FLAG_BIT_OFFSET_LI0_C_3way2_C_PositionFB); \
    }


 
/* static access macros for signal LI0_C_3way_PosReq */
 
#define l_u8_rd_LI0_C_3way_PosReq() \
    ((l_u8)  (((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_C_3way_PosReq]) >> 0U) & 0x03U))


#define l_u8_wr_LI0_C_3way_PosReq(A) \
    { \
    lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_C_3way_PosReq] = \
    (l_u8)((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_C_3way_PosReq] & 0xfcU) | \
    (((A) << 0U) & 0x03U)); \
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_C_3way_PosReq],\
         LIN_FLAG_BIT_OFFSET_LI0_C_3way_PosReq); \
    }


/* static access macros for signal LI0_C_3way2_MoveEnable */

 
#define l_bool_rd_LI0_C_3way2_MoveEnable() \
    (LIN_TEST_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_C_3way2_MoveEnable], \
    LIN_BIT_OFFSET_LI0_C_3way2_MoveEnable))

#define l_bool_wr_LI0_C_3way2_MoveEnable(A) \
    {(A) ? \
    (LIN_SET_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_C_3way2_MoveEnable], \
    LIN_BIT_OFFSET_LI0_C_3way2_MoveEnable)):\
    (LIN_CLEAR_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_C_3way2_MoveEnable], \
    LIN_BIT_OFFSET_LI0_C_3way2_MoveEnable));\
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_C_3way2_MoveEnable],\
         LIN_FLAG_BIT_OFFSET_LI0_C_3way2_MoveEnable);}
 
/* static access macros for signal LI0_C_3way_Initial */
 
#define l_u8_rd_LI0_C_3way_Initial() \
    ((l_u8)  (((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_C_3way_Initial]) >> 0U) & 0x03U))


#define l_u8_wr_LI0_C_3way_Initial(A) \
    { \
    lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_C_3way_Initial] = \
    (l_u8)((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_C_3way_Initial] & 0xfcU) | \
    (((A) << 0U) & 0x03U)); \
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_C_3way_Initial],\
         LIN_FLAG_BIT_OFFSET_LI0_C_3way_Initial); \
    }


/* static access macros for signal LI0_C_3way2_LinError */

 
#define l_bool_rd_LI0_C_3way2_LinError() \
    (LIN_TEST_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_C_3way2_LinError], \
    LIN_BIT_OFFSET_LI0_C_3way2_LinError))

#define l_bool_wr_LI0_C_3way2_LinError(A) \
    {(A) ? \
    (LIN_SET_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_C_3way2_LinError], \
    LIN_BIT_OFFSET_LI0_C_3way2_LinError)):\
    (LIN_CLEAR_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_C_3way2_LinError], \
    LIN_BIT_OFFSET_LI0_C_3way2_LinError));\
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_C_3way2_LinError],\
         LIN_FLAG_BIT_OFFSET_LI0_C_3way2_LinError);}
 
/* static access macros for signal LI0_C_3way2_C_SW_Ver */
 
#define l_u8_rd_LI0_C_3way2_C_SW_Ver() \
    ((l_u8)  (((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_C_3way2_C_SW_Ver]) >> 0U) & 0xffU))


#define l_u8_wr_LI0_C_3way2_C_SW_Ver(A) \
    { \
    lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_C_3way2_C_SW_Ver] = \
    (l_u8)((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_C_3way2_C_SW_Ver] & 0x00U) | \
    (((A) << 0U) & 0xffU)); \
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_C_3way2_C_SW_Ver],\
         LIN_FLAG_BIT_OFFSET_LI0_C_3way2_C_SW_Ver); \
    }


 
/* static access macros for signal LI0_C_3way2_C_HW_Ver */
 
#define l_u8_rd_LI0_C_3way2_C_HW_Ver() \
    ((l_u8)  (((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_C_3way2_C_HW_Ver]) >> 0U) & 0xffU))


#define l_u8_wr_LI0_C_3way2_C_HW_Ver(A) \
    { \
    lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_C_3way2_C_HW_Ver] = \
    (l_u8)((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_C_3way2_C_HW_Ver] & 0x00U) | \
    (((A) << 0U) & 0xffU)); \
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_C_3way2_C_HW_Ver],\
         LIN_FLAG_BIT_OFFSET_LI0_C_3way2_C_HW_Ver); \
    }




/* Signal flag APIs */

#define l_flg_tst_LI0_C_3way2_C_LinError_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_C_3way2_C_LinError],\
         LIN_FLAG_BIT_OFFSET_LI0_C_3way2_C_LinError)
#define l_flg_clr_LI0_C_3way2_C_LinError_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_C_3way2_C_LinError],\
         LIN_FLAG_BIT_OFFSET_LI0_C_3way2_C_LinError)

#define l_flg_tst_LI0_C_3way2_C_InitialSta_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_C_3way2_C_InitialSta],\
         LIN_FLAG_BIT_OFFSET_LI0_C_3way2_C_InitialSta)
#define l_flg_clr_LI0_C_3way2_C_InitialSta_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_C_3way2_C_InitialSta],\
         LIN_FLAG_BIT_OFFSET_LI0_C_3way2_C_InitialSta)

#define l_flg_tst_LI0_C_3way2_C_Running_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_C_3way2_C_Running],\
         LIN_FLAG_BIT_OFFSET_LI0_C_3way2_C_Running)
#define l_flg_clr_LI0_C_3way2_C_Running_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_C_3way2_C_Running],\
         LIN_FLAG_BIT_OFFSET_LI0_C_3way2_C_Running)

#define l_flg_tst_LI0_C_3way2_C_Fault_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_C_3way2_C_Fault],\
         LIN_FLAG_BIT_OFFSET_LI0_C_3way2_C_Fault)
#define l_flg_clr_LI0_C_3way2_C_Fault_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_C_3way2_C_Fault],\
         LIN_FLAG_BIT_OFFSET_LI0_C_3way2_C_Fault)

#define l_flg_tst_LI0_C_3way2_C_Voltage_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_C_3way2_C_Voltage],\
         LIN_FLAG_BIT_OFFSET_LI0_C_3way2_C_Voltage)
#define l_flg_clr_LI0_C_3way2_C_Voltage_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_C_3way2_C_Voltage],\
         LIN_FLAG_BIT_OFFSET_LI0_C_3way2_C_Voltage)

#define l_flg_tst_LI0_C_3way2_C_OverTemp_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_C_3way2_C_OverTemp],\
         LIN_FLAG_BIT_OFFSET_LI0_C_3way2_C_OverTemp)
#define l_flg_clr_LI0_C_3way2_C_OverTemp_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_C_3way2_C_OverTemp],\
         LIN_FLAG_BIT_OFFSET_LI0_C_3way2_C_OverTemp)

#define l_flg_tst_LI0_C_3way2_C_PositionFB_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_C_3way2_C_PositionFB],\
         LIN_FLAG_BIT_OFFSET_LI0_C_3way2_C_PositionFB)
#define l_flg_clr_LI0_C_3way2_C_PositionFB_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_C_3way2_C_PositionFB],\
         LIN_FLAG_BIT_OFFSET_LI0_C_3way2_C_PositionFB)

#define l_flg_tst_LI0_C_3way_PosReq_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_C_3way_PosReq],\
         LIN_FLAG_BIT_OFFSET_LI0_C_3way_PosReq)
#define l_flg_clr_LI0_C_3way_PosReq_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_C_3way_PosReq],\
         LIN_FLAG_BIT_OFFSET_LI0_C_3way_PosReq)

#define l_flg_tst_LI0_C_3way2_MoveEnable_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_C_3way2_MoveEnable],\
         LIN_FLAG_BIT_OFFSET_LI0_C_3way2_MoveEnable)
#define l_flg_clr_LI0_C_3way2_MoveEnable_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_C_3way2_MoveEnable],\
         LIN_FLAG_BIT_OFFSET_LI0_C_3way2_MoveEnable)

#define l_flg_tst_LI0_C_3way_Initial_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_C_3way_Initial],\
         LIN_FLAG_BIT_OFFSET_LI0_C_3way_Initial)
#define l_flg_clr_LI0_C_3way_Initial_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_C_3way_Initial],\
         LIN_FLAG_BIT_OFFSET_LI0_C_3way_Initial)

#define l_flg_tst_LI0_C_3way2_LinError_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_C_3way2_LinError],\
         LIN_FLAG_BIT_OFFSET_LI0_C_3way2_LinError)
#define l_flg_clr_LI0_C_3way2_LinError_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_C_3way2_LinError],\
         LIN_FLAG_BIT_OFFSET_LI0_C_3way2_LinError)

#define l_flg_tst_LI0_C_3way2_C_SW_Ver_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_C_3way2_C_SW_Ver],\
         LIN_FLAG_BIT_OFFSET_LI0_C_3way2_C_SW_Ver)
#define l_flg_clr_LI0_C_3way2_C_SW_Ver_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_C_3way2_C_SW_Ver],\
         LIN_FLAG_BIT_OFFSET_LI0_C_3way2_C_SW_Ver)

#define l_flg_tst_LI0_C_3way2_C_HW_Ver_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_C_3way2_C_HW_Ver],\
         LIN_FLAG_BIT_OFFSET_LI0_C_3way2_C_HW_Ver)
#define l_flg_clr_LI0_C_3way2_C_HW_Ver_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_C_3way2_C_HW_Ver],\
         LIN_FLAG_BIT_OFFSET_LI0_C_3way2_C_HW_Ver)



/* Frame flag APIs */

   /* Interface_name = LI0 */

 #define l_flg_tst_LI0_C_3way2_Datc_flag() \
          lin_frame_flag_tbl[LI0_C_3way2_Datc]
 #define l_flg_clr_LI0_C_3way2_Datc_flag() \
          lin_frame_flag_tbl[LI0_C_3way2_Datc] = 0

 #define l_flg_tst_LI0_Datc_3way2_flag() \
          lin_frame_flag_tbl[LI0_Datc_3way2]
 #define l_flg_clr_LI0_Datc_3way2_flag() \
          lin_frame_flag_tbl[LI0_Datc_3way2] = 0

 #define l_flg_tst_LI0_MasterReq_flag() \
          lin_frame_flag_tbl[LI0_MasterReq]
 #define l_flg_clr_LI0_MasterReq_flag() \
          lin_frame_flag_tbl[LI0_MasterReq] = 0

 #define l_flg_tst_LI0_SlaveResp_flag() \
          lin_frame_flag_tbl[LI0_SlaveResp]
 #define l_flg_clr_LI0_SlaveResp_flag() \
          lin_frame_flag_tbl[LI0_SlaveResp] = 0



/* INTERFACE MANAGEMENT */

#define l_ifc_init_LI0() l_ifc_init(LI0)



#define l_ifc_wake_up_LI0() l_ifc_wake_up(LI0)



#define l_ifc_rx_LI0() l_ifc_rx(LI0)



#define l_ifc_tx_LI0() l_ifc_tx(LI0)



#define l_ifc_aux_LI0() l_ifc_aux(LI0)



#define l_ifc_read_status_LI0() l_ifc_read_status(LI0)


#endif    /* _LIN_CFG_H_ */