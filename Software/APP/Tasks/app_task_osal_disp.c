/*******************************************************************************
 *   Filename:       app_task_disp.c
 *   Revised:        All copyrights reserved to wumingshen.
 *   Revision:       v1.0
 *   Writer:	     wumingshen.
 *
 *   Description:    双击选中 disp 按 Ctrl + H, 钩选 Match the case, Replace with
 *                   输入您要的名字，点击 Replace All
 *                   双击选中 Disp 按 Ctrl + H, 钩选 Match the case, Replace with
 *                   输入您要的名字，点击 Replace All
 *                   双击选中 DISP 按 Ctrl + H, 钩选 Match the case, Replace with
 *                   输入您要的名字，点击 Replace All
 *                   在 app_cfg.h 中指定本任务的 优先级  （ APP_TASK_DISP_PRIO ）
 *                                            和 任务堆栈（ APP_TASK_DISP_STK_SIZE ）大小
 *                   在 app.h 中声明本任务的     创建函数（ void  App_TaskDispCreate(void) ）
 *                                            和 看门狗标志位 （ WDTFLAG_Disp ）
 *
 *   Notes:
 *     				E-mail: shenchangwei945@163.com
 *
 *******************************************************************************/

/*******************************************************************************
 * INCLUDES
 */
#define  SNL_APP_SOURCE
#include <includes.h>
#include <global.h>
#include <app_ctrl.h>
#include <bsp_max7219.h>
//#include <iap.h>

#ifdef VSC_INCLUDE_SOURCE_FILE_NAMES
const  CPU_CHAR  *app_task_disp__c = "$Id: $";
#endif

#define APP_TASK_DISP_EN     DEF_ENABLED
#if APP_TASK_DISP_EN == DEF_ENABLED
/*******************************************************************************
 * CONSTANTS
 */
#define CYCLE_TIME_TICKS     (OS_CFG_TICK_RATE_HZ * 2u)

/*******************************************************************************
 * MACROS
 */

/*******************************************************************************
 * TYPEDEFS
 */

/*******************************************************************************
 * LOCAL VARIABLES
 */

#if ( OSAL_EN == DEF_ENABLED )
#else
/***********************************************
* 描述： 任务控制块（TCB）
*/
static  OS_TCB   AppTaskDispTCB;

/***********************************************
* 描述： 任务堆栈（STACKS）
*/
//static  CPU_STK  AppTaskDispStk[ APP_TASK_DISP_STK_SIZE ];

#endif
/*******************************************************************************
 * GLOBAL VARIABLES
 */

/*******************************************************************************
 * LOCAL FUNCTIONS
 */
void    comm_mtr_err_judye(void);

     
#if ( OSAL_EN == DEF_ENABLED )
#else
static  void    AppTaskDisp           (void *p_arg);
#endif

/*******************************************************************************
 * GLOBAL FUNCTIONS
 */
uint8 JudgeFlashIDErrFlg(void) ;  

/*******************************************************************************
 * EXTERN VARIABLES
 */
extern  stcSysCtrl  sCtrl;


/*******************************************************************************/

/*******************************************************************************
 * 名    称： AppTaskDisp
 * 功    能： 控制任务
 * 入口参数： p_arg - 由任务创建函数传入
 * 出口参数： 无
 * 作　 　者： wumingshen.
 * 创建日期： 2015-02-05
 * 修    改：
 * 修改日期：
 *******************************************************************************/
extern  MODBUS_CH   *g_pch; 

//#if ( OSAL_EN == DEF_ENABLED )
osalEvt  TaskDispEvtProcess(INT8U task_id, osalEvt task_event)
//#else
//static  void  AppTaskDisp (void *p_arg)
//#endif
{
//    OS_ERR      err;
//    INT32U      ticks;
//    INT32S      dly;
//    CPU_SR_ALLOC();
//    
//    /***********************************************
//    * 描述：Task body, always written as an infinite loop.
//    */
//#if ( OSAL_EN == DEF_ENABLED )
//#else
//    TaskInitDisp();
//    
//    while (DEF_TRUE) {
//#endif
//        /***********************************************
//        * 描述： 本任务看门狗标志置位
//        */
//        OS_FlagPost ((OS_FLAG_GRP *)&WdtFlagGRP,
//                     (OS_FLAGS     ) WDT_FLAG_DISP,
//                     (OS_OPT       ) OS_OPT_POST_FLAG_SET,
//                     (CPU_TS       ) 0,
//                     (OS_ERR      *) &err);
//        /***********************************************
//        * 描述： 得到系统当前时间
//        */
//        ticks = OSTimeGet(&err);
//        
//#if ( OSAL_EN == DEF_ENABLED )

        if( task_event & OS_EVT_DISP_TICKS ) {
//#else
//#endif
            static  uint8   mode    = 0;
            static  uint8   times   = 0;
            static  uint8   disflg  = 0;
            uint8            delay  = 4;      //油量显示最大时间间隔

            stcTime         sTime;

            /***********************************************
            * 描述： 根据显示模式显示
            */
            switch(mode/2) {
            /*******************************************************************
            * 描述： 显示装置时间
            */
            case 0: 
                    mode++;
                    ReadTime((stcTime * )& sTime);
                    uprintf("%02d-%02d-%02d",sTime.Year
                                            ,sTime.Month
                                            ,sTime.Date
                            );     
                    break;
            case 1: 
                    mode++;
                    ReadTime((stcTime * )& sTime);     
                    uprintf("%02d-%02d-%02d",sTime.Hour
                                            ,sTime.Min
                                            ,sTime.Sec
                            );    
                    break;

            /*******************************************************************
            * 描述： 显示软件版本
            *        时间：开机显示一次，显示2s；
            */
            case 2: 
                mode++;
                uprintf("SV_%4d",sCtrl.SoftWareID);
                
                break;
            /*******************************************************************
            * 描述： 显示硬件版本
            *        第三版硬件    
            */
            case 3: 
                mode++;
                uprintf("SV31");
                
                break;
            /*******************************************************************
            * 描述： 显示油箱模型编号
            */
            case 4: 
                mode++;
                
                uprintf("C_%d",sCtrl.SOilPara.ModelNum);                
                break;
            /*******************************************************************
            * 描述： 高度
            */
            case 5:  
                mode++;
                uprintf("H_%d",sCtrl.SOilPara.Hig);                
                break;  
            /*******************************************************************
            * 描述： 线性
            */
            case 6:  
                mode++;
                uprintf("L_%d",sCtrl.SOilPara.Hig);                
                break; 
            /*******************************************************************
            * 描述： 车型车号
            */
            case 7:  
                mode++;
                uprintf("%4d.%4d",   sCtrl.sProductInfo.sLocoId.Type,
                                    sCtrl.sProductInfo.sLocoId.Num);                
                break; 
            /*******************************************************************
            * 描述： 全灭
            */
            case 8:  
                mode++;
//                mode++;
                uprintf("      ");                
                break;   
            /*******************************************************************
            * 描述： 高度值
            */
            case 9:  
                mode++;
                uprintf("%4d.%4d",sCtrl.sRec.Myhig1,sCtrl.sRec.Myhig2);                
                break;  
            /*******************************************************************
            * 描述： 速度1
            */
            case 10:  
                mode++;
                uprintf("S1_%d",sCtrl.sRec.Myspeed1);                
                break;   
                
            /*******************************************************************
            * 描述： 速度1
            */
            case 11:  
                mode++;
                uprintf("CS_%d",sCtrl.sRec.Myspeed1);                
                break;   
            /*******************************************************************
            * 描述： 故障判断。---铁电，flash。如果异常。显示异常代码。
            */
            case 12:  
                mode++;
                //uprintf("CS_%d",sCtrl.sRec.Myspeed1);   
                uint8   ErrFlg;
                //FLASH故障判断。
                ErrFlg  = JudgeFlashIDErrFlg();  
                if(ErrFlg != 0xff)
                    uprintf("E-%d",ErrFlg); 
                else
                    uprintf("88888888"); 

                //铁电故障判断。参数读取错误，装置异常。
                break;   
                
            /*******************************************************************
            * 描述： 开机后，显示油量，通讯故障判断及故障代码
            */
            default:
                times++;
                if(disflg == 0 || (times % delay == 0))  //如果无内容显示，显示油量值
                {
                    uprintf("%8d",sCtrl.sRec.MstDip);   //显示油量
                    
                    if( sCtrl.sRunPara.SysSta && times % delay == 0)
                        uprintf("E-%d",sCtrl.sRunPara.SysSta);   //显示故障代码
                }
                
                comm_mtr_err_judye();           //油尺通讯故障判断
                break;
            }
            
            disflg = BSP_DispEvtProcess();  //执行显示操作，有内容显示，不赋值。否者油量赋值
            
            osal_start_timerEx( OS_TASK_ID_DISP,
                                OS_EVT_DISP_TICKS,
                                500);
            return ( task_event ^ OS_EVT_DISP_TICKS );
        }
    return  task_event;
}


/*******************************************************************************
 * 名    称： APP_DispInit
 * 功    能： 任务初始化
 * 入口参数： 无
 * 出口参数： 无
 * 作　 　者： wumingshen.
 * 创建日期： 2015-12-08
 * 修    改：
 * 修改日期：
 *******************************************************************************/
void TaskInitDisp(void)
{    
//    /***********************************************
//    * 描述： 初始化本任务用到的相关硬件
//    */
    BSP_DispInit();

    BSP_DispOff();
    
    BSP_DispSetBrightness(10);
    BSP_DispClrAll();
    uprintf("88888888");
    BSP_DispEvtProcess();
    

    /***********************************************
    * 描述： 在看门狗标志组注册本任务的看门狗标志
    */
    WdtFlags |= WDT_FLAG_DISP;
    /*************************************************
    * 描述：启动事件查询
    */
    osal_start_timerEx( OS_TASK_ID_DISP,
                        OS_EVT_DISP_TICKS,
                        100);
}

void    comm_mtr_err_judye(void)
{
    /*******************************************************************
    * 描述：油尺串口通讯故障判断
    */
    static  uint32  comm_mtr_err_times = 0;
    
    if(         sCtrl.Mtr.ConnCtrl[0].ErrFlg == 1 
       &&       sCtrl.Mtr.ConnCtrl[1].ErrFlg == 1){

        comm_mtr_err_times++;                   //错误次数
        
        if(comm_mtr_err_times > 4 && sCtrl.sRunPara.SysSta == 0 ){
            sCtrl.sRunPara.SysSta   = 83;       //测量装置1、测量装置2均有故障
            
            sCtrl.sRec.ErrorCode    = sCtrl.sRunPara.SysSta;    //错误代码赋值
        }
    }else if(   sCtrl.Mtr.ConnCtrl[0].ErrFlg == 1){
        comm_mtr_err_times++;                   //错误次数
        
        if(comm_mtr_err_times > 8 && sCtrl.sRunPara.SysSta == 0 ){
            sCtrl.sRunPara.SysSta   = 71;       //测量装置1故障
            
            sCtrl.sRec.ErrorCode    = sCtrl.sRunPara.SysSta;    //错误代码赋值
        }
        
    }else if(   sCtrl.Mtr.ConnCtrl[1].ErrFlg == 1){
        comm_mtr_err_times++;                   //错误次数
        
        if(comm_mtr_err_times > 8 && sCtrl.sRunPara.SysSta == 0 )
            sCtrl.sRunPara.SysSta   = 81;       //测量装置2故障
        
            sCtrl.sRec.ErrorCode    = sCtrl.sRunPara.SysSta;    //错误代码赋值
    }else {
        comm_mtr_err_times = 0;                 //错误次数

        if(     sCtrl.sRunPara.SysSta == 81
            ||  sCtrl.sRunPara.SysSta == 71
            ||  sCtrl.sRunPara.SysSta == 83){
            uprintf("E-%d",sCtrl.sRunPara.SysSta+100);  //显示故障代码+100
            
            sCtrl.sRec.ErrorCode    = sCtrl.sRunPara.SysSta+100;//错误代码赋值
            sCtrl.sRunPara.SysSta   = 0;        //清除故障代码
        }
    }
}
/*******************************************************************************
 * 				                    end of file                                *
 *******************************************************************************/
#endif