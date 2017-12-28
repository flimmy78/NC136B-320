/*******************************************************************************
 *   Filename:      app.c
 *   Revised:       $Date: 2013-06-30$
 *   Revision:      $
 *	 Writer:		Wuming Shen.
 *
 *   Description:
 *   Notes:
 *					QQ:276193028
 *     				E-mail:shenchangwei945@163.com
 *
 *   All copyrights reserved to Wuming Shen.
 *
 *******************************************************************************/

/*******************************************************************************
 * INCLUDES
 */
#define  SNL_APP_SOURCE
#include <app.h>
#include <app_ctrl.h>

//#include <iap.h>

#ifdef VSC_INCLUDE_SOURCE_FILE_NAMES
const  CPU_CHAR  *main__c = "$Id: $";
#endif


void    TaskInitStore(void);
osalEvt  TaskStoreEvtProcess(INT8U task_id, osalEvt task_event);

void    TaskInitCalc(void);
osalEvt  TaskCalcEvtProcess(INT8U task_id, osalEvt task_event);

void    TaskInitSpeed(void);
osalEvt  TaskSpeedEvtProcess(INT8U task_id, osalEvt task_event);

void     TaskInitLed(void);
osalEvt  TaskLedEvtProcess(INT8U task_id, osalEvt task_event);

uint8 	FRAM_StoreRunPara(stcRunPara  *sRunPara);
void	InitFlashIO(void);
void	GPIO_Fram_Init(void);
uint8 FRAM_ReadRecNumMgr(StrRecNumMgr  *SRecNumMgr);
uint8 FRAM_ReadProductInfo(StrProductInfo  *sProductInfo);
void  App_TaskCardCreate(void);
uint8 FRAM_ReadRunPara(stcRunPara  *sRunPara);


//void    TaskInitStore(void);
//osalEvt  TaskStoreEvtProcess(INT8U task_id, osalEvt task_event);
/*******************************************************************************
 * CONSTANTS
 */
#define CYCLE_TIME_TICKS            (OS_TICKS_PER_SEC * 2)

/*******************************************************************************
 * MACROS
 */

/*******************************************************************************
 * TYPEDEFS
 */

/*******************************************************************************
 * LOCAL VARIABLES
 */

/*******************************************************************************
 * GLOBAL VARIABLES
 */
/***********************************************
* 描述： 创建标志组
*/
OS_FLAG_GRP         WdtFlagGRP;                     //看门狗标志组
OS_FLAGS            WdtFlags;

stcSysCtrl          Ctrl;
/*******************************************************************************
 * LOCAL FUNCTIONS
 *
 * 返回类型         函数名称                入口参数
 */

/*******************************************************************************
 * GLOBAL FUNCTIONS
 */

/*******************************************************************************
 * EXTERN VARIABLES
 */

/*******************************************************************************
 * EXTERN FUNCTIONS
 */

/*******************************************************************************
 * 名    称： App_Main
 * 功    能： 系统初开始前调用此函数
 * 入口参数： 无
 * 出口参数： 无
 * 作　 　者： 无名沈
 * 创建日期： 2014-08-08
 * 修    改：
 * 修改日期：
 *******************************************************************************/
 void App_Main(void)
 { 
    //NVIC_SetVTOR(0x10000);
    /***********************************************
    * 描述： Disable all interrupts.
    */
    //BSP_IntDisAll();
    /***********************************************
    * 描述： 重新设置应用程序中断向量表，如果没有BOOTLOADER
    *        NVIC_VectTab_FLASH_OFFSET = 0
    */
//#if defined     (IMAGE_A)
//    /***********************************************
//    * 描述： 程序A
//    */
//    NVIC_SetVectorTable(NVIC_VectTab_FLASH, FLASH_APP_ADDR_A-FLASH_BOOT_ADDR);
//    IAP_SetStatus(IAP_STS_SUCCEED); 
//#elif defined   (IMAGE_B)
//    /***********************************************
//    * 描述： 程序B
//    */
//    NVIC_SetVectorTable(NVIC_VectTab_FLASH, FLASH_APP_ADDR_B-FLASH_BOOT_ADDR);
//    IAP_SetStatus(IAP_STS_SUCCEED); 
//#else
//    /***********************************************
//    * 描述： 程序
//    */
//    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0X0000);
//#endif
 
 }

/*******************************************************************************
 * 名    称： App_InitStartHook
 * 功    能： 系统初始化开始前调用此函数
 * 入口参数： 无
 * 出口参数： 无
 * 作　 　者： 无名沈
 * 创建日期： 2014-08-08
 * 修    改：
 * 修改日期：
 *******************************************************************************/
 void App_InitStartHook(void)
 {
 
 }
 
/*******************************************************************************
 * 名    称： App_InitEndHook
 * 功    能： 系统初始化结速后调用此函数
 * 入口参数： 无
 * 出口参数： 无
 * 作　 　者： 无名沈
 * 创建日期： 2014-08-08
 * 修    改：
 * 修改日期：
 *******************************************************************************/
 void App_InitEndHook(void)
 {
 
 }
 
 /*******************************************************************************
 * 名    称： OS_TaskCreateHook
 * 功    能： OS创建任务回调函数
 * 入口参数： 无
 * 出口参数： 无
 * 作  　者： 无名沈
 * 创建日期： 2014-08-08
 * 修    改：
 * 修改日期：
 *******************************************************************************/
 void OS_TaskCreateHook(void)
 {     
   App_TaskCommCreate();
   App_TaskCardCreate();
    //App_TaskMaterCreate();
    //App_TaskDtuCreate();
    //App_TaskKeyCreate();
    //App_TaskBeepCreate();
 }
 
 /*******************************************************************************
 * 名    称： OSAL_TaskCreateHook
 * 功    能： OSAL创建任务回调函数
 * 入口参数： 无
 * 出口参数： 无
 * 作　 　者： 无名沈
 * 创建日期： 2014-08-08
 * 修    改：
 * 修改日期：
 *******************************************************************************/
 void OSAL_TaskCreateHook(void)
 {
    //osal_add_Task(TaskInitKey,                      // 任务初始化函数指针
    //              TaskKeyEvtProcess,                // 任务事件处理函数指针
    //              OS_TASK_PRO_KEY,                  // 任务优先级
    //              OS_TASK_ID_KEY);                  // 任务ID
    
    //osal_add_Task(TaskInitCtrl,                     // 任务初始化函数指针
    //              TaskCtrlEvtProcess,               // 任务事件处理函数指针
    //              OS_TASK_PRO_CTRL,                 // 任务优先级
    //              OS_TASK_ID_CTRL);                 // 任务ID
       
    osal_add_Task(TaskInitTmr,                      // 任务初始化函数指针
                  TaskTmrEvtProcess,                // 任务事件处理函数指针
                  OS_TASK_PRO_TMR,                  // 任务优先级
                  OS_TASK_ID_TMR);                  // 任务ID
/*        

    osal_add_Task(TaskInitDisp,                     // 任务初始化函数指针
                  TaskDispEvtProcess,               // 任务事件处理函数指针
                  OS_TASK_PRO_DISP,                 // 任务优先级
                  OS_TASK_ID_DISP);                 // 任务ID 
    
    osal_add_Task(TaskInitStore,                    // 任务初始化函数指针
                  TaskStoreEvtProcess,              // 任务事件处理函数指针
                  OS_TASK_PRO_STORE,                // 任务优先级
                  OS_TASK_ID_STORE);                // 任务ID
    
    osal_add_Task(TaskInitCalc,                    // 任务初始化函数指针
                  TaskCalcEvtProcess,              // 任务事件处理函数指针
                  OS_TASK_PRO_CALC,                // 任务优先级
                  OS_TASK_ID_CALC);                // 任务ID   
   
    osal_add_Task(TaskInitSpeed,                    // 任务初始化函数指针
                  TaskSpeedEvtProcess,              // 任务事件处理函数指针
                  OS_TASK_PRO_SPEED,                // 任务优先级
                  OS_TASK_ID_SPEED);                // 任务ID       
*/     
    osal_add_Task(TaskInitLed,                      // 任务初始化函数指针
                  TaskLedEvtProcess,                // 任务事件处理函数指针
                  OS_TASK_PRO_LED,                  // 任务优先级
                  OS_TASK_ID_LED);                  // 任务ID    
 }
  
void app_init_sctrl(void)
{
    extern uint8_t BSP_FlashOsInit(void);
    //初始化fram 
    GPIO_Fram_Init();
    BSP_FramOsInit(); 
    WdtReset();
    //初始化flash
    InitFlashIO();
    BSP_FlashOsInit(); 
    WdtReset();
    
    Ctrl.Password      =   0;
    Ctrl.SoftWareID    =   1705;       //年月，版本
    
    //开始标示    //存储时间间隔
    Ctrl.sRunPara.StartFlg = 1;  
    Ctrl.sRunPara.SysSta   = 0;        
    
   	FRAM_StoreRunPara((stcRunPara *) &Ctrl.sRunPara);
    
    Ctrl.sRunPara.plugcard = 0;        //没插卡      
    Ctrl.sRunPara.StoreTime= 60;       
    
    //IC模块 数据 初始化    
    Ctrl.Otr.ConnCtrl[0].ConnFlg   = 1;
    Ctrl.Otr.ConnCtrl[0].ErrFlg    = 0;
    Ctrl.Otr.ConnCtrl[0].MasterAddr= 0x80;
    Ctrl.Otr.ConnCtrl[0].SlaveAddr = SLAVE_ADDR_OTR;
    Ctrl.Otr.ConnCtrl[0].RecvEndFlg= 0;
    Ctrl.Otr.ConnCtrl[0].SendFlg   = 0;
    Ctrl.Otr.ConnCtrl[0].SendFramNum=1;
    Ctrl.Otr.ConnCtrl[0].TimeOut   = 10;       
    
    //IC模块 参数 初始化    
    Ctrl.Otr.ConnCtrl[1].ConnFlg   = 1;
    Ctrl.Otr.ConnCtrl[1].ErrFlg    = 0;
    Ctrl.Otr.ConnCtrl[1].MasterAddr= 0x80;
    Ctrl.Otr.ConnCtrl[1].SlaveAddr = SLAVE_ADDR_SET;
    Ctrl.Otr.ConnCtrl[1].RecvEndFlg= 0;
    Ctrl.Otr.ConnCtrl[1].SendFlg   = 0;
    Ctrl.Otr.ConnCtrl[1].SendFramNum=1;
    Ctrl.Otr.ConnCtrl[1].TimeOut   = 10; 
    
    WdtReset();
    //读sCtrl 
    FRAM_ReadRecNumMgr((StrRecNumMgr  *)&Ctrl.sRecNumMgr);         //读记录号
    FRAM_ReadProductInfo((StrProductInfo  *)&Ctrl.sProductInfo);   //读产品编号
    //    FRAM_ReadCurRecord((stcFlshRec  *)&Ctrl.sRec);                 //读当前记录
    //    FRAM_ReadOilPara((StrOilPara  *)&Ctrl.SOilPara);               //读计算参数
    FRAM_ReadRunPara((stcRunPara  *)&Ctrl.sRunPara);               //读运行参数
    //    FRAM_ReadCalcModel((stcCalcModel  *)&Ctrl.sCalcModel);         //读计算模型
    
}
/*******************************************************************************
 *              end of file                                                    *
 *******************************************************************************/
