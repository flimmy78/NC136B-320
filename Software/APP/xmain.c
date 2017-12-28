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
* ������ ������־��
*/
OS_FLAG_GRP         WdtFlagGRP;                     //���Ź���־��
OS_FLAGS            WdtFlags;

stcSysCtrl          Ctrl;
/*******************************************************************************
 * LOCAL FUNCTIONS
 *
 * ��������         ��������                ��ڲ���
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
 * ��    �ƣ� App_Main
 * ��    �ܣ� ϵͳ����ʼǰ���ô˺���
 * ��ڲ����� ��
 * ���ڲ����� ��
 * ���� ���ߣ� ������
 * �������ڣ� 2014-08-08
 * ��    �ģ�
 * �޸����ڣ�
 *******************************************************************************/
 void App_Main(void)
 { 
    //NVIC_SetVTOR(0x10000);
    /***********************************************
    * ������ Disable all interrupts.
    */
    //BSP_IntDisAll();
    /***********************************************
    * ������ ��������Ӧ�ó����ж����������û��BOOTLOADER
    *        NVIC_VectTab_FLASH_OFFSET = 0
    */
//#if defined     (IMAGE_A)
//    /***********************************************
//    * ������ ����A
//    */
//    NVIC_SetVectorTable(NVIC_VectTab_FLASH, FLASH_APP_ADDR_A-FLASH_BOOT_ADDR);
//    IAP_SetStatus(IAP_STS_SUCCEED); 
//#elif defined   (IMAGE_B)
//    /***********************************************
//    * ������ ����B
//    */
//    NVIC_SetVectorTable(NVIC_VectTab_FLASH, FLASH_APP_ADDR_B-FLASH_BOOT_ADDR);
//    IAP_SetStatus(IAP_STS_SUCCEED); 
//#else
//    /***********************************************
//    * ������ ����
//    */
//    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0X0000);
//#endif
 
 }

/*******************************************************************************
 * ��    �ƣ� App_InitStartHook
 * ��    �ܣ� ϵͳ��ʼ����ʼǰ���ô˺���
 * ��ڲ����� ��
 * ���ڲ����� ��
 * ���� ���ߣ� ������
 * �������ڣ� 2014-08-08
 * ��    �ģ�
 * �޸����ڣ�
 *******************************************************************************/
 void App_InitStartHook(void)
 {
 
 }
 
/*******************************************************************************
 * ��    �ƣ� App_InitEndHook
 * ��    �ܣ� ϵͳ��ʼ�����ٺ���ô˺���
 * ��ڲ����� ��
 * ���ڲ����� ��
 * ���� ���ߣ� ������
 * �������ڣ� 2014-08-08
 * ��    �ģ�
 * �޸����ڣ�
 *******************************************************************************/
 void App_InitEndHook(void)
 {
 
 }
 
 /*******************************************************************************
 * ��    �ƣ� OS_TaskCreateHook
 * ��    �ܣ� OS��������ص�����
 * ��ڲ����� ��
 * ���ڲ����� ��
 * ��  ���ߣ� ������
 * �������ڣ� 2014-08-08
 * ��    �ģ�
 * �޸����ڣ�
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
 * ��    �ƣ� OSAL_TaskCreateHook
 * ��    �ܣ� OSAL��������ص�����
 * ��ڲ����� ��
 * ���ڲ����� ��
 * ���� ���ߣ� ������
 * �������ڣ� 2014-08-08
 * ��    �ģ�
 * �޸����ڣ�
 *******************************************************************************/
 void OSAL_TaskCreateHook(void)
 {
    //osal_add_Task(TaskInitKey,                      // �����ʼ������ָ��
    //              TaskKeyEvtProcess,                // �����¼�������ָ��
    //              OS_TASK_PRO_KEY,                  // �������ȼ�
    //              OS_TASK_ID_KEY);                  // ����ID
    
    //osal_add_Task(TaskInitCtrl,                     // �����ʼ������ָ��
    //              TaskCtrlEvtProcess,               // �����¼�������ָ��
    //              OS_TASK_PRO_CTRL,                 // �������ȼ�
    //              OS_TASK_ID_CTRL);                 // ����ID
       
    osal_add_Task(TaskInitTmr,                      // �����ʼ������ָ��
                  TaskTmrEvtProcess,                // �����¼�������ָ��
                  OS_TASK_PRO_TMR,                  // �������ȼ�
                  OS_TASK_ID_TMR);                  // ����ID
/*        

    osal_add_Task(TaskInitDisp,                     // �����ʼ������ָ��
                  TaskDispEvtProcess,               // �����¼�������ָ��
                  OS_TASK_PRO_DISP,                 // �������ȼ�
                  OS_TASK_ID_DISP);                 // ����ID 
    
    osal_add_Task(TaskInitStore,                    // �����ʼ������ָ��
                  TaskStoreEvtProcess,              // �����¼�������ָ��
                  OS_TASK_PRO_STORE,                // �������ȼ�
                  OS_TASK_ID_STORE);                // ����ID
    
    osal_add_Task(TaskInitCalc,                    // �����ʼ������ָ��
                  TaskCalcEvtProcess,              // �����¼�������ָ��
                  OS_TASK_PRO_CALC,                // �������ȼ�
                  OS_TASK_ID_CALC);                // ����ID   
   
    osal_add_Task(TaskInitSpeed,                    // �����ʼ������ָ��
                  TaskSpeedEvtProcess,              // �����¼�������ָ��
                  OS_TASK_PRO_SPEED,                // �������ȼ�
                  OS_TASK_ID_SPEED);                // ����ID       
*/     
    osal_add_Task(TaskInitLed,                      // �����ʼ������ָ��
                  TaskLedEvtProcess,                // �����¼�������ָ��
                  OS_TASK_PRO_LED,                  // �������ȼ�
                  OS_TASK_ID_LED);                  // ����ID    
 }
  
void app_init_sctrl(void)
{
    extern uint8_t BSP_FlashOsInit(void);
    //��ʼ��fram 
    GPIO_Fram_Init();
    BSP_FramOsInit(); 
    WdtReset();
    //��ʼ��flash
    InitFlashIO();
    BSP_FlashOsInit(); 
    WdtReset();
    
    Ctrl.Password      =   0;
    Ctrl.SoftWareID    =   1705;       //���£��汾
    
    //��ʼ��ʾ    //�洢ʱ����
    Ctrl.sRunPara.StartFlg = 1;  
    Ctrl.sRunPara.SysSta   = 0;        
    
   	FRAM_StoreRunPara((stcRunPara *) &Ctrl.sRunPara);
    
    Ctrl.sRunPara.plugcard = 0;        //û�忨      
    Ctrl.sRunPara.StoreTime= 60;       
    
    //ICģ�� ���� ��ʼ��    
    Ctrl.Otr.ConnCtrl[0].ConnFlg   = 1;
    Ctrl.Otr.ConnCtrl[0].ErrFlg    = 0;
    Ctrl.Otr.ConnCtrl[0].MasterAddr= 0x80;
    Ctrl.Otr.ConnCtrl[0].SlaveAddr = SLAVE_ADDR_OTR;
    Ctrl.Otr.ConnCtrl[0].RecvEndFlg= 0;
    Ctrl.Otr.ConnCtrl[0].SendFlg   = 0;
    Ctrl.Otr.ConnCtrl[0].SendFramNum=1;
    Ctrl.Otr.ConnCtrl[0].TimeOut   = 10;       
    
    //ICģ�� ���� ��ʼ��    
    Ctrl.Otr.ConnCtrl[1].ConnFlg   = 1;
    Ctrl.Otr.ConnCtrl[1].ErrFlg    = 0;
    Ctrl.Otr.ConnCtrl[1].MasterAddr= 0x80;
    Ctrl.Otr.ConnCtrl[1].SlaveAddr = SLAVE_ADDR_SET;
    Ctrl.Otr.ConnCtrl[1].RecvEndFlg= 0;
    Ctrl.Otr.ConnCtrl[1].SendFlg   = 0;
    Ctrl.Otr.ConnCtrl[1].SendFramNum=1;
    Ctrl.Otr.ConnCtrl[1].TimeOut   = 10; 
    
    WdtReset();
    //��sCtrl 
    FRAM_ReadRecNumMgr((StrRecNumMgr  *)&Ctrl.sRecNumMgr);         //����¼��
    FRAM_ReadProductInfo((StrProductInfo  *)&Ctrl.sProductInfo);   //����Ʒ���
    //    FRAM_ReadCurRecord((stcFlshRec  *)&Ctrl.sRec);                 //����ǰ��¼
    //    FRAM_ReadOilPara((StrOilPara  *)&Ctrl.SOilPara);               //���������
    FRAM_ReadRunPara((stcRunPara  *)&Ctrl.sRunPara);               //�����в���
    //    FRAM_ReadCalcModel((stcCalcModel  *)&Ctrl.sCalcModel);         //������ģ��
    
}
/*******************************************************************************
 *              end of file                                                    *
 *******************************************************************************/
