/*******************************************************************************
 *   Filename:       app_task_led.c
 *   Revised:        All copyrights reserved to redmorningcn.
 *   Revision:       v1.0
 *   Writer:	     redmoringcn.
 *
 *   Description:    双击选中 led 按 Ctrl + H, 钩选 Match the case, Replace with
 *                   输入您要的名字，点击 Replace All
 *                   双击选中 Led 按 Ctrl + H, 钩选 Match the case, Replace with
 *                   输入您要的名字，点击 Replace All
 *                   双击选中 LED 按 Ctrl + H, 钩选 Match the case, Replace with
 *                   输入您要的名字，点击 Replace All
 *                   在 app_cfg.h 中指定本任务的 优先级  （ APP_TASK_LED_PRIO ）
 *                                            和 任务堆栈（ APP_TASK_LED_STK_SIZE ）大小
 *                   在 app.h 中声明本任务的     创建函数（ void  App_TaskLedCreate(void) ）
 *                                            和 看门狗标志位 （ WDTFLAG_Led ）
 *
 *   Notes:
 *     				E-mail: redmorningcn@qq.com
 *
 *******************************************************************************/

/*******************************************************************************
 * INCLUDES
 */
#define  SNL_APP_SOURCE
#include <includes.h>
#include <global.h>
#include <app_ctrl.h>
//#include <iap.h>

#ifdef VSC_INCLUDE_SOURCE_FILE_NAMES
const  CPU_CHAR  *app_task_led__c = "$Id: $";
#endif

#define APP_TASK_LED_EN     DEF_ENABLED
#if APP_TASK_LED_EN == DEF_ENABLED
/*******************************************************************************
 * CONSTANTS
 */
#define CYCLE_TIME_TICKS     (OS_CFG_TICK_RATE_HZ * 2u)

/*******************************************************************************
 * MACROS
 */
//#define     BSP_LED_NUM         8
////LED点亮次数 
//#define     LED_TOGGLE_CNT      15


/*******************************************************************************
 * TYPEDEFS
 */

/*******************************************************************************
 * LOCAL VARIABLES
 */
//
//uint8   LedWorkTimes[BSP_LED_NUM + 1] = 0;

#if ( OSAL_EN == DEF_ENABLED )
#else
/***********************************************
* 描述： 任务控制块（TCB）
*/
static  OS_TCB   AppTaskLedTCB;

/***********************************************
* 描述： 任务堆栈（STACKS）
*/
//static  CPU_STK  AppTaskLedStk[ APP_TASK_LED_STK_SIZE ];

#endif
/*******************************************************************************
 * GLOBAL VARIABLES
 */

/*******************************************************************************
 * LOCAL FUNCTIONS
 */
#if ( OSAL_EN == DEF_ENABLED )
#else
static  void    AppTaskLed           (void *p_arg);
#endif

/*******************************************************************************
 * GLOBAL FUNCTIONS
 */

/*******************************************************************************
 * EXTERN VARIABLES
 */

/*******************************************************************************/


/*******************************************************************************
 * 名    称： AppTaskLed
 * 功    能： 控制任务
 * 入口参数： p_arg - 由任务创建函数传入
 * 出口参数： 无
 * 作　 　者： wumingshen.
 * 创建日期： 2015-02-05
 * 修    改：
 * 修改日期：
 *******************************************************************************/

osalEvt  TaskLedEvtProcess(INT8U task_id, osalEvt task_event)
{    
    /***********************************************
    * 描述： 本任务看门狗标志置位
    */
    OSSetWdtFlag(( OS_FLAGS     ) WDT_FLAG_LED);
    
    if( task_event & OS_EVT_LED_TICKS ) {
        static  uint8   mode = 0;
        static  uint8   dlytimes = 0;
        /***********************************************
        * 描述： 开机显示
        */
        while(mode < 5) {
            mode++;
            
            BSP_LED_Toggle(7);                          //对应指示灯点亮
            BSP_LED_Toggle(8);
            
            osal_start_timerEx( OS_TASK_ID_LED,
                               OS_EVT_LED_TICKS,
                               200);
            return ( task_event ^ OS_EVT_LED_TICKS );
        }
        
        /***********************************************
        * 描述： 运行指示灯
        */
        
        //if(Ctrl.sRunPara.plugcard == 1){
        //    BSP_LED_Toggle(7);  //对应指示灯点亮
        //}else {
        //    BSP_LED_Off(7);
        //}
        
        if(Ctrl.Otr.ConnCtrl[0].ErrFlg == 0){  //通讯正常，快闪0.1s
            BSP_LED_Toggle(8);
        } else {
            dlytimes++;
            if(dlytimes > 5){   
                BSP_LED_Toggle(8);              //通讯异常，慢闪0.5s
                dlytimes = 0;
            }
        }        
        
        /***********************************************
        * 描述： 定时器重启
        */
        osal_start_timerEx( OS_TASK_ID_LED,
                           OS_EVT_LED_TICKS,
                           50);
        
        return ( task_event ^ OS_EVT_LED_TICKS );
    }
    
    return  0;
}


/*******************************************************************************
 * 名    称：APP_LedInit
 * 功    能：任务初始化
 * 入口参数：无
 * 出口参数：无
 * 作　 　者：redmorningcn.
 * 创建日期：2017-05-15
 * 修    改：
 * 修改日期：
 *******************************************************************************/
void TaskInitLed(void)
{    
    /***********************************************
    * 描述： 在看门狗标志组注册本任务的看门狗标志
    */
    OSRegWdtFlag(( OS_FLAGS     )WDT_FLAG_LED );
    
    osal_start_timerEx( OS_TASK_ID_LED,
                        OS_EVT_LED_TICKS,
                        1000);
}

/*******************************************************************************
 * 				                    end of file                                *
 *******************************************************************************/
#endif