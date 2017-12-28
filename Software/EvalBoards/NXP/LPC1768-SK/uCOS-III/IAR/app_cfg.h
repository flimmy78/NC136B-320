/*
*********************************************************************************************************
*                                              uC/OS-III
*                                        The Real-Time Kernel
*
*                          (c) Copyright 2004-2012; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*
*               uC/OS-III is provided in source form for FREE evaluation, for educational
*               use or peaceful research.  If you plan on using uC/OS-III in a commercial
*               product you need to contact Micrium to properly license its use in your
*               product.  We provide ALL the source code for your convenience and to
*               help you experience uC/OS-III.  The fact that the source code is provided
*               does NOT mean that you can use it without paying a licensing fee.
*
*               Knowledge of the source code may NOT be used to develop a similar product.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                       APPLICATION CONFIGURATION
*
*                                             NXP LPC1768
*                                                on the
*                                     IAR LPC1768-SK Kickstart Kit
*
*
* Filename      : app_cfg.h
* Version       : V1.00
* Programmer(s) : FT
*                 DC
*********************************************************************************************************
*/

#ifndef  APP_CFG_MODULE_PRESENT
#define  APP_CFG_MODULE_PRESENT


/*
*********************************************************************************************************
*                                       ADDITIONAL uC/MODULE ENABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       MODULE ENABLE / DISABLE
*********************************************************************************************************
*/

#define  APP_CFG_SERIAL_EN                          DEF_DISABLED


/****************************************************
* 描述： 系统使用到的ModBus从机节点站号
*/
#define ModbusNode                                  2u

/*
*********************************************************************************************************
*                                           配置任务的优先级
*                                            TASK PRIORITIES
*                设置任务优先级，uCOS-III中优先级可以相同，使用时间片转轮询完成相同优先级任务的执行
*                           !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*                        !!!任务优先级分配很重要，优先级会影响整个系统的运行!!!
*                           !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*********************************************************************************************************
*/

#define  APP_TASK_START_PRIO        (OS_CFG_PRIO_MAX - 14)
#define  MB_OS_CFG_RX_TASK_PRIO     (OS_CFG_PRIO_MAX - 13) 
#define  APP_TASK_DTU_PRIO          (OS_CFG_PRIO_MAX - 12) 
#define  APP_TASK_SENSOR_PRIO       (OS_CFG_PRIO_MAX - 11)  
#define  APP_TASK_MATER_PRIO        (OS_CFG_PRIO_MAX - 11) 
#define  APP_TASK_BEEP_PRIO         (OS_CFG_PRIO_MAX - 10)  
#define  APP_TASK_KEY_PRIO          (OS_CFG_PRIO_MAX -  9)  
#define  APP_TASK_CONTROL_PRIO      (OS_CFG_PRIO_MAX -  8)  

#define  APP_TASK_CARD_PRIO         (OS_CFG_PRIO_MAX -  7)
#define  APP_TASK_COMM_PRIO         (OS_CFG_PRIO_MAX - 10) 
#define  APP_TASK_OSAL_PRIO         (OS_CFG_PRIO_MAX -  2) 

#define  APP_TASK_WIFI_PRIO         (OS_CFG_PRIO_MAX - 12) 
//#define  APP_TASK_OSAL_PRIO         (OS_CFG_PRIO_MAX -  3)  // 例程任务优先级，可以随意修改。
  // 例程任务优先级，可以随意修改。

#define  APP_TASK_SAMPLE_PRIO       (OS_CFG_PRIO_MAX -  2)  // 例程任务优先级，可以随意修改。
/*
*********************************************************************************************************
*                                           配置任务堆栈的大小
*                                            TASK STACK SIZES
*                             Size of the task stacks (# of OS_STK entries)
*                      堆栈的字节大小 = CPU数据宽度 * 8 * size = 4 * 8 * size(字节)
*                           如：APP_TASK_SAMPLE_STK_SIZE = 128u，实际为 128u * 4 = 512（字节）
*                             可以在IAR仿真中查看堆栈大小分配是否充足。
*********************************************************************************************************
*/

#define  MB_OS_CFG_RX_TASK_STK_SIZE                 256u        // ModBus任务堆栈的大小
#define  APP_TASK_START_STK_SIZE                    128u        // 给起始任务分配堆栈
#define  APP_TASK_SAMPLE_STK_SIZE                    60u        // 例程任务堆栈大小，用户更换成自己相应的名称
#define  APP_TASK_WIFI_STK_SIZE                     128u
#define  APP_TASK_COMM_STK_SIZE                     512u
#define  APP_TASK_CARD_STK_SIZE                    	512u
#define  APP_TASK_DTU_STK_SIZE                      256u
#define  APP_TASK_SENSOR_STK_SIZE                   256u
#define  APP_TASK_MATER_STK_SIZE                    256u
#define  APP_TASK_KEY_STK_SIZE                      128u
#define  APP_TASK_CONTROL_STK_SIZE                  128u
#define  APP_TASK_BEEP_STK_SIZE                     128u
#define  APP_TASK_OSAL_STK_SIZE                     256u


/*
*********************************************************************************************************
*                                    BSP CONFIGURATION: RS-232
*********************************************************************************************************
*/

#define  BSP_CFG_SER_COMM_SEL             			BSP_SER_COMM_UART_02
#define  BSP_CFG_TS_TMR_SEL                         2

/*
*********************************************************************************************************
*                                            TASK PRIORITIES
*********************************************************************************************************
*/

#define  APP_CFG_TASK_START_PRIO                           2u
#define  APP_CFG_TASK_JOY_PRIO                             3u
#define  APP_CFG_TASK_LCD_PRIO                             4u

#define  OS_TASK_TMR_PRIO                         (OS_LOWEST_PRIO - 2)


/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*********************************************************************************************************
*/

#define  APP_CFG_TASK_START_STK_SIZE                     64u
#define  APP_CFG_TASK_JOY_STK_SIZE                       128u
#define  APP_CFG_TASK_LCD_STK_SIZE                       512u


/*
*********************************************************************************************************
*                                             uC/LIB CONFIGURATION
*********************************************************************************************************
*/

//#define  LIB_MEM_CFG_OPTIMIZE_ASM_EN            DEF_ENABLED
//#define  LIB_MEM_CFG_ARG_CHK_EXT_EN             DEF_ENABLED
//#define  LIB_MEM_CFG_ALLOC_EN                   DEF_ENABLED
//#define  LIB_MEM_CFG_HEAP_SIZE                  15*1024u


/*
*********************************************************************************************************
*                                     TRACE / DEBUG CONFIGURATION
*********************************************************************************************************
*/

#define  TRACE_LEVEL_OFF                                  0u
#define  TRACE_LEVEL_INFO                                 1u
#define  TRACE_LEVEL_DBG                                  2u

#define  APP_CFG_TRACE_LEVEL                    TRACE_LEVEL_DBG
#define  APP_CFG_TRACE                          printf

#define  APP_TRACE_INFO(x)                     ((APP_CFG_TRACE_LEVEL >= TRACE_LEVEL_INFO)  ? (void)(APP_CFG_TRACE x) : (void)0)
#define  APP_TRACE_DBG(x)                      ((APP_CFG_TRACE_LEVEL >= TRACE_LEVEL_DBG)   ? (void)(APP_CFG_TRACE x) : (void)0)

#endif
