/*******************************************************************************
 *   Filename:       app_task_led.c
 *   Revised:        All copyrights reserved to redmorningcn.
 *   Revision:       v1.0
 *   Writer:	     redmoringcn.
 *
 *   Description:    ˫��ѡ�� led �� Ctrl + H, ��ѡ Match the case, Replace with
 *                   ������Ҫ�����֣���� Replace All
 *                   ˫��ѡ�� Led �� Ctrl + H, ��ѡ Match the case, Replace with
 *                   ������Ҫ�����֣���� Replace All
 *                   ˫��ѡ�� LED �� Ctrl + H, ��ѡ Match the case, Replace with
 *                   ������Ҫ�����֣���� Replace All
 *                   �� app_cfg.h ��ָ��������� ���ȼ�  �� APP_TASK_LED_PRIO ��
 *                                            �� �����ջ�� APP_TASK_LED_STK_SIZE ����С
 *                   �� app.h �������������     ���������� void  App_TaskLedCreate(void) ��
 *                                            �� ���Ź���־λ �� WDTFLAG_Led ��
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
////LED�������� 
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
* ������ ������ƿ飨TCB��
*/
static  OS_TCB   AppTaskLedTCB;

/***********************************************
* ������ �����ջ��STACKS��
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
 * ��    �ƣ� AppTaskLed
 * ��    �ܣ� ��������
 * ��ڲ����� p_arg - �����񴴽���������
 * ���ڲ����� ��
 * ���� ���ߣ� wumingshen.
 * �������ڣ� 2015-02-05
 * ��    �ģ�
 * �޸����ڣ�
 *******************************************************************************/

osalEvt  TaskLedEvtProcess(INT8U task_id, osalEvt task_event)
{    
    /***********************************************
    * ������ �������Ź���־��λ
    */
    OSSetWdtFlag(( OS_FLAGS     ) WDT_FLAG_LED);
    
    if( task_event & OS_EVT_LED_TICKS ) {
        static  uint8   mode = 0;
        static  uint8   dlytimes = 0;
        /***********************************************
        * ������ ������ʾ
        */
        while(mode < 5) {
            mode++;
            
            BSP_LED_Toggle(7);                          //��Ӧָʾ�Ƶ���
            BSP_LED_Toggle(8);
            
            osal_start_timerEx( OS_TASK_ID_LED,
                               OS_EVT_LED_TICKS,
                               200);
            return ( task_event ^ OS_EVT_LED_TICKS );
        }
        
        /***********************************************
        * ������ ����ָʾ��
        */
        
        //if(Ctrl.sRunPara.plugcard == 1){
        //    BSP_LED_Toggle(7);  //��Ӧָʾ�Ƶ���
        //}else {
        //    BSP_LED_Off(7);
        //}
        
        if(Ctrl.Otr.ConnCtrl[0].ErrFlg == 0){  //ͨѶ����������0.1s
            BSP_LED_Toggle(8);
        } else {
            dlytimes++;
            if(dlytimes > 5){   
                BSP_LED_Toggle(8);              //ͨѶ�쳣������0.5s
                dlytimes = 0;
            }
        }        
        
        /***********************************************
        * ������ ��ʱ������
        */
        osal_start_timerEx( OS_TASK_ID_LED,
                           OS_EVT_LED_TICKS,
                           50);
        
        return ( task_event ^ OS_EVT_LED_TICKS );
    }
    
    return  0;
}


/*******************************************************************************
 * ��    �ƣ�APP_LedInit
 * ��    �ܣ������ʼ��
 * ��ڲ�������
 * ���ڲ�������
 * ���� ���ߣ�redmorningcn.
 * �������ڣ�2017-05-15
 * ��    �ģ�
 * �޸����ڣ�
 *******************************************************************************/
void TaskInitLed(void)
{    
    /***********************************************
    * ������ �ڿ��Ź���־��ע�᱾����Ŀ��Ź���־
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