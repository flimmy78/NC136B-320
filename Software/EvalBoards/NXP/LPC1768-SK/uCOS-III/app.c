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
    CPU_INT16U  	TimeOutCnt      = 0;                        //���Ź���ʱ������

    (void)p_arg;                                                /* See Note #1                                          */
    
    /***********************************************
    * ������ ����UCOS���Ӻ���
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
    * ������ ι��
    */
    WdtReset();
    
    app_init_sctrl();                  //��ʼ��ȫ�ֱ���
 
    App_ObjCreate();                                            /* Create Applicaiton kernel objects                    */

    App_TaskCreate();                                           /* Create Application tasks                             */

    while (DEF_TRUE) { 
        /***********************************************
        * ������ ι��
        */
        WdtReset();
            
        /***********************************************
        * ������ �õ�ϵͳ��ǰʱ��
        */
        ticks = OSTimeGet(&err);
        
        /***********************************************************************
        * ������ �������Ź���־���飬 �ж��Ƿ�����������ι��
        */
        OSFlagPend(( OS_FLAG_GRP *)&Ctrl.Os.WdtEvtFlagGRP,
                   ( OS_FLAGS     ) Ctrl.Os.WdtEvtFlags,
                   ( OS_TICK      ) 50,
                   ( OS_OPT       ) OS_OPT_PEND_FLAG_SET_ALL,                   //ȫ����һ
                   ( CPU_TS      *) NULL,
                   ( OS_ERR      *)&err);
        
        if(err == OS_ERR_NONE) {                                                //����������ι��
            TimeOutCnt = 0;                                                     //��ʱ����������
            BSP_LED_Flash( 7, 1, 50, 25);
            OSFlagPost ((OS_FLAG_GRP *)&Ctrl.Os.WdtEvtFlagGRP,                             //�������б�־
                         (OS_FLAGS     ) Ctrl.Os.WdtEvtFlags,
                         (OS_OPT       ) OS_OPT_POST_FLAG_CLR,
                         (OS_ERR      *) &err);
        } else {                                                                //������������ι��
            TimeOutCnt++;                                                       //��ʱ��������1
            if(TimeOutCnt > WdtTimeoutSec) {                                    //ι����ʱ                
                BSP_LED_Flash( 0, 5, 450, 450);
                /***********************************************
                * ������ 2017/12/1,������ϵͳ����
                */
#if defined     (RELEASE)
                SystemReset();
#else
                /***********************************************
                * ������ �������������ģʽ
                */
                while(1){
                    /*******************************************************
                    * ������ ��ʱ��Ȳ���ĳ����Ŀ��Ź���־λע�ᣬ˵�����������ˣ�
                    *        ָʾ�ƿ�����˸��������ѭ���ȴ�ϵͳ��λ
                    */
                    BSP_LED_Toggle(0);
                    BSP_OS_TimeDly(100);
                    /***********************************************
                    * ������ ι��
                    */
                    WdtReset();
                }               
#endif  
            } else {
                BSP_LED_Flash( 7, 1, 450, 450);  
            }
        }
        /***********************************************
        * ������ ȥ���������е�ʱ�䣬�ȵ�һ������������ʣ����Ҫ��ʱ��ʱ��
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
    * ������ �ڴ˴�����OSAL����
    */
    App_TaskOsalCreate();
}


/*******************************************************************************
* ��    �ƣ� OSSetWdtFlag
* ��    �ܣ� ����ι��
* ��ڲ����� ��
* ���ڲ����� ��
* ��    �ߣ� ������
* �������ڣ� 2017/11/18
* ��    �ģ� 
* �޸����ڣ� 
* ��    ע�� 
*******************************************************************************/
void OSRegWdtFlag( OS_FLAGS flag )
{
    /***********************************************
    * ������ �ڿ��Ź���־��ע�᱾����Ŀ��Ź���־
    */
    Ctrl.Os.WdtEvtFlags |= flag;
}

/*******************************************************************************
* ��    �ƣ� OSSetWdtFlag
* ��    �ܣ� ����ι��
* ��ڲ����� ��
* ���ڲ����� ��
* ��    �ߣ� ������
* �������ڣ� 2017/11/18
* ��    �ģ� 
* �޸����ڣ� 
* ��    ע�� 
*******************************************************************************/
void OSSetWdtFlag( OS_FLAGS flag )
{
    OS_ERR    err;
    
    /***********************************************
    * ������ �������Ź���־��λ
    */
    OSFlagPost(( OS_FLAG_GRP  *)&Ctrl.Os.WdtEvtFlagGRP,
                ( OS_FLAGS     ) flag,
                ( OS_OPT       ) OS_OPT_POST_FLAG_SET,
                ( OS_ERR      *) &err);
}