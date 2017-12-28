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
#define WdtTimeoutSec               120
#define CYCLE_TIME_TICKS            (OS_TICKS_PER_SEC * 1)

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
    OS_ERR          err;
    OS_TICK         dly             = CYCLE_TIME_TICKS;
    OS_TICK         ticks;
    CPU_INT16U  	TimeOutCnt      = 0;                        //看门狗超时计数器

    (void)p_arg;                                                /* See Note #1                                          */
    
    /***********************************************
    * 描述： 设置UCOS钩子函数
    */
    App_OS_SetAllHooks();
    
    CPU_Init();
     
    BSP_Init();                                                 /* Initialize BSP functions                             */
    
    Mem_Init();

    BSP_OSTickInit();                                                /* Start BSP and tick initialization                    */

#if (OS_TASK_STAT_EN > 0)
    OSStatInit();                                               /* Determine CPU capacity                               */
#endif

    /***********************************************
    * 描述： 喂狗
    */
    WdtReset();
    
    app_init_sctrl();                  //初始化全局变量
 
    App_ObjCreate();                                            /* Create Applicaiton kernel objects                    */

    App_TaskCreate();                                           /* Create Application tasks                             */

    while (DEF_TRUE) { 
        /***********************************************
        * 描述： 喂狗
        */
        WdtReset();
            
        /***********************************************
        * 描述： 得到系统当前时间
        */
        ticks = OSTimeGet(&err);
        
        /***********************************************************************
        * 描述： 独立看门狗标志组检查， 判断是否所有任务已喂狗
        */
        OSFlagPend(( OS_FLAG_GRP *)&Ctrl.Os.WdtEvtFlagGRP,
                   ( OS_FLAGS     ) Ctrl.Os.WdtEvtFlags,
                   ( OS_TICK      ) 50,
                   ( OS_OPT       ) OS_OPT_PEND_FLAG_SET_ALL,                   //全部置一
                   ( CPU_TS      *) NULL,
                   ( OS_ERR      *)&err);
        
        if(err == OS_ERR_NONE) {                                                //所有任务已喂狗
            TimeOutCnt = 0;                                                     //超时计数器清零
            BSP_LED_Flash( 7, 1, 50, 25);
            OSFlagPost ((OS_FLAG_GRP *)&Ctrl.Os.WdtEvtFlagGRP,                             //清零所有标志
                         (OS_FLAGS     ) Ctrl.Os.WdtEvtFlags,
                         (OS_OPT       ) OS_OPT_POST_FLAG_CLR,
                         (OS_ERR      *) &err);
        } else {                                                                //不是所有任务都喂狗
            TimeOutCnt++;                                                       //超时计数器加1
            if(TimeOutCnt > WdtTimeoutSec) {                                    //喂狗超时                
                BSP_LED_Flash( 0, 5, 450, 450);
                /***********************************************
                * 描述： 2017/12/1,无名沈：系统重启
                */
#if defined     (RELEASE)
                SystemReset();
#else
                /***********************************************
                * 描述： 如果程序处在升级模式
                */
                while(1){
                    /*******************************************************
                    * 描述： 长时间等不到某任务的看门狗标志位注册，说明有任务死了，
                    *        指示灯快速闪烁，进入死循环等待系统复位
                    */
                    BSP_LED_Toggle(0);
                    BSP_OS_TimeDly(100);
                    /***********************************************
                    * 描述： 喂狗
                    */
                    WdtReset();
                }               
#endif  
            } else {
                BSP_LED_Flash( 7, 1, 450, 450);  
            }
        }
        /***********************************************
        * 描述： 去除任务运行的时间，等到一个控制周期里剩余需要延时的时间
        */
        dly   = CYCLE_TIME_TICKS - ( OSTimeGet(&err) - ticks );
        if ( dly  < 1 ) {
            dly = 1;
        } else if ( dly > CYCLE_TIME_TICKS ) {
            dly = CYCLE_TIME_TICKS;
        }
        BSP_OS_TimeDly(dly);        
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


/*******************************************************************************
* 名    称： OSSetWdtFlag
* 功    能： 任务喂狗
* 入口参数： 无
* 出口参数： 无
* 作    者： 无名沈
* 创建日期： 2017/11/18
* 修    改： 
* 修改日期： 
* 备    注： 
*******************************************************************************/
void OSRegWdtFlag( OS_FLAGS flag )
{
    /***********************************************
    * 描述： 在看门狗标志组注册本任务的看门狗标志
    */
    Ctrl.Os.WdtEvtFlags |= flag;
}

/*******************************************************************************
* 名    称： OSSetWdtFlag
* 功    能： 任务喂狗
* 入口参数： 无
* 出口参数： 无
* 作    者： 无名沈
* 创建日期： 2017/11/18
* 修    改： 
* 修改日期： 
* 备    注： 
*******************************************************************************/
void OSSetWdtFlag( OS_FLAGS flag )
{
    OS_ERR    err;
    
    /***********************************************
    * 描述： 本任务看门狗标志置位
    */
    OSFlagPost(( OS_FLAG_GRP  *)&Ctrl.Os.WdtEvtFlagGRP,
                ( OS_FLAGS     ) flag,
                ( OS_OPT       ) OS_OPT_POST_FLAG_SET,
                ( OS_ERR      *) &err);
}