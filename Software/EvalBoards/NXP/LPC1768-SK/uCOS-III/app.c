/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2012; Micrium, Inc.; Weston, FL
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
*                                            EXAMPLE CODE
*
*                                             NXP LPC1768
*                                                on the
*                                     IAR LPC1768-SK Kickstart Kit
*
* Filename      : app.c
* Version       : V1.00
* Programmer(s) : FT
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <includes.h>
#include <os_app_hooks.h>
#include <app.h>

#include <lpc17xx_nvic.h>

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

                                                                /* ----------------- APPLICATION GLOBALS ------------------ */
                                                                /* -------- uC/OS-III APPLICATION TASK STACKS ------------- */
static  OS_TCB        App_TaskStartTCB;
static  CPU_STK_SIZE  App_TaskStartStk[APP_CFG_TASK_START_STK_SIZE];


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void    App_ObjCreate           (void);
static  void    App_TaskCreate          (void);
static  void    App_TaskStart           (void       *p_arg);
extern  void    App_Main                (void);


/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This the main standard entry point.
*
* Note(s)     : none.
*********************************************************************************************************
*/

int  main (void)
{
    OS_ERR  err;
    
    App_Main();

    CPU_Init();
     
    BSP_Init();                                                 /* Initialize BSP functions                             */
    
    Mem_Init();

    OSInit(&err);                                               /* Initialize "uC/OS-III, The Real-Time Kernel"         */

    OSTaskCreate((OS_TCB     *)&App_TaskStartTCB,               /* Create the start task                                */
                 (CPU_CHAR   *)"App Task Start",
                 (OS_TASK_PTR ) App_TaskStart,
                 (void       *) 0,
                 (OS_PRIO     ) APP_CFG_TASK_START_PRIO,
                 (CPU_STK    *)&App_TaskStartStk[0],
                 (CPU_STK     )(APP_CFG_TASK_START_STK_SIZE / 10u),
                 (CPU_STK_SIZE) APP_CFG_TASK_START_STK_SIZE,
                 (OS_MSG_QTY  ) 0,
                 (OS_TICK     ) 0,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
#if (OS_TASK_NAME_EN > 0u)
    OSTaskNameSet(APP_CFG_TASK_START_PRIO, "Start", &err);
#endif

    OSStart(&err);                                              /* Start multitasking (i.e. give control to uC/OS-III). */

    while(DEF_ON){												/* Should Never Get Here							    */
    };
}

/*
*********************************************************************************************************
*                                          App_TaskStart()
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Argument(s) : p_arg   is the argument passed to 'App_TaskStart()' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Notes       : (1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                   used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static  void  App_TaskStart (void *p_arg)
{
     OS_ERR  os_err;

    (void)p_arg;                                                /* See Note #1                                          */
    
    /***********************************************
    * 描述： 设置UCOS钩子函数
    */
    App_OS_SetAllHooks();
    
    BSP_OSTickInit();                                                /* Start BSP and tick initialization                    */

#if (OS_TASK_STAT_EN > 0)
    OSStatInit();                                               /* Determine CPU capacity                               */
#endif

    App_ObjCreate();                                            /* Create Applicaiton kernel objects                    */

    App_TaskCreate();                                           /* Create Application tasks                             */

    BSP_LED_Off(0);

    while (DEF_TRUE) {    
                                                                /* Task body, always written as an infinite loop.       */
        //BSP_LED_Toggle(8);
        
        OSTimeDlyHMSM(0, 0, 0, 100,
                      OS_OPT_TIME_HMSM_STRICT, &os_err);
    }
}


/*
*********************************************************************************************************
*                                      App_ObjCreate()
*
* Description:  Creates the application kernel objects.
*
* Argument(s) :  none.
*
* Return(s)   :  none.
*
* Caller(s)   :  App_TaskStart().
*
* Note(s)     :  none.
*********************************************************************************************************
*/

static  void  App_ObjCreate (void)
{
}


/*
*********************************************************************************************************
*                                      App_TaskCreate()
*
* Description :  This function creates the application tasks.
*
* Argument(s) :  none.
*
* Return(s)   :  none.
*
* Caller(s)   :  App_TaskStart().
*
* Note(s)     :  none.
*********************************************************************************************************
*/
extern void OS_TaskCreateHook(void);
static  void  App_TaskCreate (void)
{
  
    OS_TaskCreateHook();
    /***********************************************
    * 描述： 在此处创建OSAL任务
    */
    App_TaskOsalCreate();
}

