/*
*********************************************************************************************************
*                                     MICRIUM BOARD SUPPORT SUPPORT
*
*                          (c) Copyright 2003-2009; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                    MICRIUM BOARD SUPPORT PACKAGE
*                                             OS LAYER
*
* Filename      : bsp_os.c
* Version       : V1.00
* Programmer(s) : FT
*                 EHS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define  BSP_OS_MODULE
#include <bsp_os.h>
#include <bsp.h>

#if (APP_CFG_PROBE_COM_EN == DEF_ENABLED)
#include <app_probe.h>
#endif


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*********************************************************************************************************
**                                         GLOBAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*********************************************************************************************************
**                                     BSP OS LOCKS FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*********************************************************************************************************
**                                     BSP OS LOCKS FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                      BSP_OS_SemCreate()
*
* Description : Creates a sempahore to lock/unlock
*
* Argument(s) : p_sem        Pointer to a BSP_OS_SEM structure
*
*               sem_val      Initial value of the semaphore.
*
*               p_sem_name   Pointer to the semaphore name.
*
* Return(s)   : DEF_OK       if the semaphore was created.
*               DEF_FAIL     if the sempahore could not be created.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

CPU_BOOLEAN  BSP_OS_SemCreate (BSP_OS_SEM       *p_sem,
                               BSP_OS_SEM_VAL    sem_val,
                               CPU_CHAR         *p_sem_name)
{
    OS_ERR     err;


    OSSemCreate((OS_SEM    *)p_sem,
                (CPU_CHAR  *)p_sem_name,
                (OS_SEM_CTR )sem_val,
                (OS_ERR    *)&err);

    if (err != OS_ERR_NONE) {
        return (DEF_FAIL);
    }

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                     BSP_OS_SemWait()
*
* Description : Wait on a semaphore to become available
*
* Argument(s) : sem          sempahore handler
*
*               dly_ms       delay in miliseconds to wait on the semaphore
*
* Return(s)   : DEF_OK       if the semaphore was acquire
*               DEF_FAIL     if the sempahore could not be acquire
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_BOOLEAN  BSP_OS_SemWait (BSP_OS_SEM  *p_sem,
                             CPU_INT32U   dly_ms)
{
    OS_ERR      err;
    CPU_INT32U  dly_ticks;


    dly_ticks  = ((dly_ms * DEF_TIME_NBR_mS_PER_SEC) / OSCfg_TickRate_Hz);

    OSSemPend((OS_SEM *)p_sem,
              (OS_TICK )dly_ticks,
              (OS_OPT  )OS_OPT_PEND_BLOCKING,
              (CPU_TS  )0,
              (OS_ERR *)&err);

    if (err != OS_ERR_NONE) {
       return (DEF_FAIL);
    }

    return (DEF_OK);
}

/*
*********************************************************************************************************
*                                      BSP_OS_SemPost()
*
* Description : Post a semaphore
*
* Argument(s) : sem          Semaphore handler
*
* Return(s)   : DEF_OK     if the semaphore was posted.
*               DEF_FAIL      if the sempahore could not be posted.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_BOOLEAN  BSP_OS_SemPost (BSP_OS_SEM *p_sem)
{
    OS_ERR  err;


    OSSemPost((OS_SEM *)p_sem,
              (OS_OPT  )OS_OPT_POST_1,
              (OS_ERR *)&err);

    if (err != OS_ERR_NONE) {
        return (DEF_FAIL);
    }

    return (DEF_OK);
}


/*
*********************************************************************************************************
*********************************************************************************************************
**                                     uC/OS-II TIMER FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                    BSP_TimeDlyMs()
*
* Description : This function delay the exceution for specifi amount of miliseconds
*
* Argument(s) : dly_ms       Delay in miliseconds
*
* Return(s)   : none.
*
* Caller(s)   : Application
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

void   BSP_OS_TimeDly (CPU_INT32U  dly_ms)
{
    CPU_INT16U  ms;
    CPU_INT16U  sec;
    OS_ERR      err;


    if (dly_ms > 10000u) {                                       /* Limit delays to 10 seconds.                        */
        dly_ms = 10000u;
    }

    if (dly_ms >= 1000u) {
        ms  = dly_ms % 1000u;
        sec = dly_ms / 1000u;
    } else {
        ms  = dly_ms;
        sec = 0u;
    }


    OSTimeDlyHMSM((CPU_INT16U) 0u,
                  (CPU_INT16U) 0u,
                  (CPU_INT16U) sec,
                  (CPU_INT32U) ms,
                  (OS_OPT    ) OS_OPT_TIME_HMSM_STRICT,
                  (OS_ERR   *)&err);
}

/*
*********************************************************************************************************
*                                    BSP_TimeDlyMs()
*
* Description : This function delay the exceution for specifi amount of miliseconds
*
* Argument(s) : dly_ms       Delay in miliseconds
*
* Return(s)   : none.
*
* Caller(s)   : Application
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

void  BSP_OS_TimeDlyHMSM(  CPU_INT16U   hours,
                           CPU_INT16U   minutes,
                           CPU_INT16U   seconds,
                           CPU_INT32U   milli)
{
    OS_ERR      err;
    OSTimeDlyHMSM((CPU_INT16U) hours,
                  (CPU_INT16U) minutes,
                  (CPU_INT16U) seconds,
                  (CPU_INT32U) milli,
                  (OS_OPT    ) OS_OPT_TIME_HMSM_STRICT,
                  (OS_ERR   *)&err);
}

/*
*********************************************************************************************************
*                                            BSP_OS_TickGet()
*
* Description : Get the OS Tick Counter as if it was running continuously.
*
* Argument(s) : none.
*
* Return(s)   : The effective OS Tick Counter.
*
* Caller(s)   : OS_TaskBlock, OS_TickListInsertDly and OSTimeGet.
*
*               This function is an INTERNAL uC/OS-III function & MUST NOT be called by application
*               function(s).
*
* Note(s)     : none.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             EXAMPLE CODE
*********************************************************************************************************
* Licensing terms:
*   This file is provided as an example on how to use Micrium products. It has not necessarily been
*   tested under every possible condition and is only offered as a reference, without any guarantee.
*
*   Please feel free to use any application code labeled as 'EXAMPLE CODE' in your application products.
*   Example code may be used as is, in whole or in part, or may be used as a reference only. This file
*   can be modified as required.
*
*   You can find user manuals, API references, release notes and more at: https://doc.micrium.com
*
*   You can contact us at: http://www.micrium.com
*
*   Please help us continue to provide the Embedded community with the finest software available.
*
*   Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                             OS TICK BSP
*
* File : bsp_os.c
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*********************************************************************************************************
*/

#include  <cpu.h>
#include  <lib_def.h>
#include  <os.h>
#include  <bsp_os.h>

//#include "em_cmu.h"
//#include "em_rtc.h"


/*
*********************************************************************************************************
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*********************************************************************************************************
*/

#ifndef  OS_CFG_DYN_TICK_EN                                     /* Dynamic tick only available for uCOS-III             */
#define  OS_CFG_DYN_TICK_EN      DEF_DISABLED
#endif

#if (OS_CFG_DYN_TICK_EN == DEF_ENABLED)
#define  BSP_OS_LPTIMER_TICK_RATE_HZ          (16384u)
#define  BSP_OS_LPTICK_TO_OSTICK(lptick)    (((lptick) * OSCfg_TickRate_Hz)/BSP_OS_LPTIMER_TICK_RATE_HZ)
#define  BSP_OS_OSTICK_TO_LPTICK(ostick)    (((ostick) * BSP_OS_LPTIMER_TICK_RATE_HZ)/OSCfg_TickRate_Hz)
#endif


/*
*********************************************************************************************************
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*********************************************************************************************************
*/

#if (OS_CFG_DYN_TICK_EN == DEF_ENABLED)
static  OS_TICK  BSP_OS_TicksToGo = 0u;                         /* Stored in OS Tick units.                             */
static  OS_TICK  BSP_OS_LastTick  = 0u;                         /* Stored in OS Tick units.                             */
static  OS_TICK  BSP_OS_UnTick    = 0u;                         /* Stored in OS Tick units.                             */
#endif


/*
*********************************************************************************************************
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*********************************************************************************************************
*/

#if (OS_CFG_DYN_TICK_EN == DEF_ENABLED)
static  void     BSP_OS_LP_TickInit(void);
static  OS_TICK  BSP_OS_LP_TickGet (void);
#endif


/*
*********************************************************************************************************
*********************************************************************************************************
*                                           GLOBAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                    INITIALIZE OS TICK INTERRUPT
*
* Description : Initialize the tick interrupt.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none
*********************************************************************************************************
*/

void  BSP_OSTickInit (void)
{
#if (OS_CFG_DYN_TICK_EN != DEF_ENABLED)
//    CPU_INT32U  cpu_freq;
#endif

#if (OS_CFG_DYN_TICK_EN == DEF_ENABLED)
    BSP_OS_LP_TickInit();
#else
    extern void BSP_Start(void);
    BSP_Start();
    //cpu_freq = SystemCoreClockGet();                            /* Determine SysTick reference freq.                    */
    //OS_CPU_SysTickInitFreq(cpu_freq);                           /* Init uC/OS periodic time src (SysTick).              */
#endif
}


/*
*********************************************************************************************************
*                                     DISABLE OS TICK INTERRUPT
*
* Description : Disable the tick interrupt.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none
*********************************************************************************************************
*/

void  BSP_OSTickDisable (void)
{
    //SysTick->CTRL = 0x0000000;
}


/*
*********************************************************************************************************
*********************************************************************************************************
**                                       uC/OS-III Low Power Tick
*********************************************************************************************************
*********************************************************************************************************
*/

#if (OS_CFG_DYN_TICK_EN == DEF_ENABLED)

/*
*********************************************************************************************************
*                                          BSP_OS_LP_TickInit()
*
* Description : Initialize the RTC timer to enable dynamic ticking.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_Tick_Init.
*
*               This function is an INTERNAL uC/OS-III function & MUST NOT be called by application
*               function(s).
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_OS_LP_TickInit (void)
{
//    RTC_Init_TypeDef  rtc_init_cfg;
//
//
//    BSP_OS_TicksToGo = 0u;
//    BSP_OS_LastTick  = 0u;
//
//
//    CMU_ClockEnable(cmuClock_CORELE, true);                     /* Enable and select RTC oscillator.                    */
//
//    CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);           /* Select the low frequency crystal oscillator.         */
//
//    CMU_ClockDivSet(cmuClock_RTC, cmuClkDiv_2);                 /* Set 2 times divider to reduce energy.                */
//
//    CMU_ClockEnable(cmuClock_RTC, true);                        /* Enable clock to RTC module.                          */
//
//    rtc_init_cfg.enable   = false;                              /* Specify the RTC initial configuration.               */
//    rtc_init_cfg.debugRun = false;
//    rtc_init_cfg.comp0Top = true;
//
//    RTC_Init(&rtc_init_cfg);                                    /* Initialization of RTC.                               */
//
//    RTC_IntDisable(RTC_IFC_COMP0);                              /* Disable interrupt generation from RTC0.              */
//
//    NVIC_SetPriority(RTC_IRQn, 255);                            /* Tick interrupt MUST execute at the lowest int prio.  */
//
//    NVIC_ClearPendingIRQ(RTC_IRQn);                             /* Enable interrupts.                                   */
//    NVIC_EnableIRQ(RTC_IRQn);
//
//    RTC_CompareSet(0, 128);                                     /* Set compare register.                                */
//
//    RTC_IntClear(RTC_IFC_COMP0);                                /* Get the RTC started.                                 */
//    RTC_IntEnable(RTC_IF_COMP0);
//    RTC_Enable(true);
}


/*
*********************************************************************************************************
*                                        RTC_IRQHandler()
*
* Description : Handle the RTC interrupt for the OS LP mode.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  RTC_IRQHandler (void)
{
    CPU_SR_ALLOC();


    CPU_CRITICAL_ENTER();
    OSIntEnter();
    CPU_CRITICAL_EXIT();

    RTC_IntClear(_RTC_IFC_MASK);                                /* Clear the RTC tick interrupt.                        */

    if (BSP_OS_TicksToGo != 0) {
    	RTC_CounterSet(0);
        OSTimeDynTick(BSP_OS_TicksToGo + BSP_OS_UnTick + BSP_OS_LPTICK_TO_OSTICK(RTC_CounterGet()));
        BSP_OS_LastTick += BSP_OS_TicksToGo + BSP_OS_LPTICK_TO_OSTICK(RTC_CounterGet());

        BSP_OS_TicksToGo = 0;
        BSP_OS_UnTick    = 0;
    } else {
                                                                /* No tasks need to be awakened only keep track of time */
        BSP_OS_LastTick += BSP_OS_LPTICK_TO_OSTICK(RTC_CompareGet(0));
        BSP_OS_UnTick   += BSP_OS_LPTICK_TO_OSTICK(RTC_CompareGet(0));
    }

    OSIntExit();
}


/*
*********************************************************************************************************
*                                            BSP_OS_TickGet()
*
* Description : Get the OS Tick Counter as if it was running continuously.
*
* Argument(s) : none.
*
* Return(s)   : The effective OS Tick Counter.
*
* Caller(s)   : OS_TaskBlock, OS_TickListInsertDly and OSTimeGet.
*
*               This function is an INTERNAL uC/OS-III function & MUST NOT be called by application
*               function(s).
*
* Note(s)     : none.
*********************************************************************************************************
*/

OS_TICK  BSP_OS_TickGet (void)
{
    OS_TICK  tick;


    tick = BSP_OS_LastTick + BSP_OS_LPTICK_TO_OSTICK(BSP_OS_LP_TickGet());

    return (tick);
}


/*
*********************************************************************************************************
*                                          BSP_OS_TickNextSet()
*
* Description : Set the number of OS Ticks to wait before calling OSTimeTick.
*
* Argument(s) : ticks       number of OS Ticks to wait.
*
* Return(s)   : Number of effective OS Ticks until next OSTimeTick.
*
* Caller(s)   : OS_TickTask and OS_TickListInsert.
*
*               This function is an INTERNAL uC/OS-III function & MUST NOT be called by application
*               function(s).
*
* Note(s)     : none.
*********************************************************************************************************
*/

OS_TICK  BSP_OS_TickNextSet (OS_TICK  ticks)
{
    OS_TICK     lpticks;

                                                                /* Check if OSTimeTick needs to be called.              */
    if (ticks != (OS_TICK)-1) {
                                                                /* Get tick count in LPTMR units.                       */
        lpticks = BSP_OS_OSTICK_TO_LPTICK(ticks);
                                                                /* Adjust for maximum value.                            */
        if (lpticks > DEF_INT_16U_MAX_VAL) {
            BSP_OS_TicksToGo = BSP_OS_LPTICK_TO_OSTICK(DEF_INT_16U_MAX_VAL);
            lpticks          = DEF_INT_16U_MAX_VAL;
        } else {
            BSP_OS_TicksToGo = ticks;
        }
                                                                /* Adjust for invalid value.                            */
        if (lpticks & 0xFFFF == 0) {
            lpticks          = DEF_INT_16U_MAX_VAL;
            BSP_OS_TicksToGo = BSP_OS_LPTICK_TO_OSTICK(DEF_INT_16U_MAX_VAL);
        }
    } else if (ticks == 0) {
        lpticks          = 1;
        BSP_OS_TicksToGo = 1;
    } else {
        lpticks          = DEF_INT_16U_MAX_VAL;
        BSP_OS_TicksToGo = 0;
    }


    RTC_Enable(false);                                          /* Stop timer.                                          */

                                                                /* Re-configure timer.                                  */

    RTC_CompareSet(0, lpticks);                                 /* Reset the RTC compare value to trigger at the        */
                                                                /* configured tick rate.                                */

    RTC_Enable(true);                                           /* Restart timer.                                       */

                                                                /* Return the the number of effective OS ticks that     */
    return (BSP_OS_TicksToGo);                                  /* will elapse before the next interrupt.               */
}


/*
*********************************************************************************************************
*                                          BSP_OS_LP_TickGet()
*
* Description : Gets the RTC timer state.
*
* Argument(s) : none.
*
* Return(s)   : RTC state.
*
* Caller(s)   : BSP_OS_TickNextSet and BSP_OS_TickGet.
*
*               This function is an INTERNAL uC/OS-III function & MUST NOT be called by application
*               function(s).
*
* Note(s)     : none.
*********************************************************************************************************
*/

OS_TICK  BSP_OS_LP_TickGet(void)
{
    OS_TICK  ticks;


    //ticks = RTC_CounterGet();

    return (ticks);
}
#endif
