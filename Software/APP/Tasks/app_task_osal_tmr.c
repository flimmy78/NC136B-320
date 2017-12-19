/*******************************************************************************
 *   Filename:       app_task_tmr.c
 *   Revised:        All copyrights reserved to wumingshen.
 *   Revision:       v1.0
 *   Writer:	     wumingshen.
 *
 *   Description:    ˫��ѡ�� tmr �� Ctrl + H, ��ѡ Match the case, Replace with
 *                   ������Ҫ�����֣���� Replace All
 *                   ˫��ѡ�� Tmr �� Ctrl + H, ��ѡ Match the case, Replace with
 *                   ������Ҫ�����֣���� Replace All
 *                   ˫��ѡ�� TMR �� Ctrl + H, ��ѡ Match the case, Replace with
 *                   ������Ҫ�����֣���� Replace All
 *                   �� app_cfg.h ��ָ��������� ���ȼ�  �� APP_TASK_TMR_PRIO ��
 *                                            �� �����ջ�� APP_TASK_TMR_STK_SIZE ����С
 *                   �� app.h �������������     ���������� void  App_TaskTmrCreate(void) ��
 *                                            �� ���Ź���־λ �� WDTFLAG_Tmr ��
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
#include "DS3231.h"
#include "app.h"
#include "bsp_timer.h"
#include "os_cfg_app.h"

#ifdef VSC_INCLUDE_SOURCE_FILE_NAMES
const  CPU_CHAR  *app_task_tmr__c = "$Id: $";
#endif

#define APP_TASK_TMR_EN     DEF_ENABLED
#if APP_TASK_TMR_EN == DEF_ENABLED
/*******************************************************************************
 * CONSTANTS
 */
#define CYCLE_TIME_TICKS            (OS_CFG_TICK_RATE_HZ * 1u)
#define CYCLE_SAMPLE_MSEC_TICKS     (OS_CFG_TICK_RATE_HZ / 5)
#define CYCLE_SAMPLE_SEC_TICKS      (OS_CFG_TICK_RATE_HZ * 5u)
#define CYCLE_SAMPLE_MIN_TICKS      (OS_CFG_TICK_RATE_HZ * 60u)
#define CYCLE_SAMPLE_TICKS          (OS_CFG_TICK_RATE_HZ * 2u)
     
/*******************************************************************************
 * MACROS
 */

/*******************************************************************************
 * TYPEDEFS
 */
extern     stcSysCtrl   sCtrl;

/*******************************************************************************
 * LOCAL VARIABLES
 */

#if ( OSAL_EN == DEF_ENABLED )
#else
/***********************************************
* ������ ������ƿ飨TCB��
*/
static  OS_TCB   AppTaskTmrTCB;

/***********************************************
* ������ �����ջ��STACKS��
*/
static  CPU_STK  AppTaskTmrStk[ APP_TASK_TMR_STK_SIZE ];

#endif
/*******************************************************************************
 * GLOBAL VARIABLES
 */

/*******************************************************************************
 * LOCAL FUNCTIONS
 */

/*******************************************************************************
 * GLOBAL FUNCTIONS
 */

/*******************************************************************************
 * EXTERN VARIABLES
 */

/*******************************************************************************/
extern  OS_SEM	Bsp_Card_Sem; 
uint8_t ReadIC_SWT(void);


/*******************************************************************************
 * ��    �ƣ� AppTaskTmr
 * ��    �ܣ� ��������
 * ��ڲ����� p_arg - �����񴴽���������
 * ���ڲ����� ��
 * ���� ���ߣ� wumingshen.
 * �������ڣ� 2015-02-05
 * ��    �ģ�
 * �޸����ڣ�
 *******************************************************************************/
osalEvt  TaskTmrEvtProcess(INT8U task_id, osalEvt task_event)
{
    OS_ERR      err;
//    static BOOL     chang_flag = FALSE;
    
    /***********************************************
    * ������ �������Ź���־��λ
    */
    OSFlagPost ((OS_FLAG_GRP *)&WdtFlagGRP,
                 (OS_FLAGS     ) WDT_FLAG_TMR,
                 (OS_OPT       ) OS_OPT_POST_FLAG_SET,
                 (OS_ERR      *) &err);    
  
    /***************************************************************************
    * ������ ͳ��ģ������߷���ģ��ͨѶ��ʱ����
    COMM_EVT_FLAG_DTU_TIMEOUT ��ʾ��
    */
    
    static  uint8   mPlugTime = 0;
    static  uint8   mNoPlugTime = 0;

    if( task_event & OS_EVT_TMR_SEC ) {        

	   //�޿�״̬�£�����IC��
	   if(ReadIC_SWT() == 1 && sCtrl.sRunPara.plugcard == 0)  //�޿�״̬�£�����IC�����������ź���
		{
	        if( mPlugTime++ > 2)                          	
	        {
	            sCtrl.sRunPara.plugcard = 1;
				
				BSP_OS_SemPost(&Bsp_Card_Sem);              //�����ź��������� IC������       

	            mNoPlugTime = 0;
	        }
		//�п�״̬��
		} else if( ReadIC_SWT() == 0 &&  sCtrl.sRunPara.plugcard == 1) {//�п�״̬�����Ѿ��γ�IC��      
	        if( mNoPlugTime++ > 2)                          	
	        {
	            sCtrl.sRunPara.plugcard = 0;

	            mPlugTime = 0;
	        }
	    }

        return ( task_event ^ OS_EVT_TMR_SEC );
    }
    return 0;
}

/*******************************************************************************
 * ��    �ƣ� APP_TmrInit
 * ��    �ܣ� �����ʼ��
 * ��ڲ����� ��
 * ���ڲ����� ��
 * �������ߣ� wumingshen.
 * �������ڣ� 2015-03-28
 * ��    �ģ�
 * �޸����ڣ�
 *******************************************************************************/
void TaskInitTmr(void)
{
    /***********************************************
    * ������ ��ʼ��
    */    
       
    /***********************************************
    * ������ �ڿ��Ź���־��ע�᱾����Ŀ��Ź���־
    */
    WdtFlags |= WDT_FLAG_TMR;
    /*************************************************
    * �����������¼���ѯ
    */
    osal_start_timerRl( OS_TASK_ID_TMR, OS_EVT_TMR_SEC, OS_TICKS_PER_SEC/10);
}

/*******************************************************************************
 * 				                    end of file                                *
 *******************************************************************************/
#endif
