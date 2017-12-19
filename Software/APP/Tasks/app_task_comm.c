/*******************************************************************************
 *   Filename:       app_task_comm.c
 *   Revised:        All copyrights reserved to wumingshen.
 *   Revision:       v1.0
 *   Writer:	     wumingshen.
 *
 *   Description:    ˫��ѡ�� comm �� Ctrl + H, ��ѡ Match the case, Replace with
 *                   ������Ҫ�����֣���� Replace All
 *                   ˫��ѡ�� Comm �� Ctrl + H, ��ѡ Match the case, Replace with
 *                   ������Ҫ�����֣���� Replace All
 *                   ˫��ѡ�� COMM �� Ctrl + H, ��ѡ Match the case, Replace with
 *                   ������Ҫ�����֣���� Replace All
 *                   �� app_cfg.h ��ָ��������� ���ȼ�  �� APP_TASK_COMM_PRIO     ��
 *                                            �� �����ջ�� APP_TASK_COMM_STK_SIZE ����С
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
#include <app_comm_protocol.h>
#include <app_ctrl.h>
#include <os_cfg_app.h>
#include <CrcCheck.h>

//#include <bsp_flash.h>
//#include <iap.h>

#ifdef VSC_INCLUDE_SOURCE_FILE_NAMES
const  CPU_CHAR  *app_task_comm__c = "$Id: $";
#endif

#define APP_TASK_COMM_EN     DEF_ENABLED
#if APP_TASK_COMM_EN == DEF_ENABLED
/*******************************************************************************
 * CONSTANTS
 */

/*******************************************************************************
 * MACROS
 */
#define CYCLE_TIME_TICKS            (OS_TICKS_PER_SEC * 1)

/*******************************************************************************
 * TYPEDEFS
 */



/***********************************************
* ������ ������ƿ飨TCB��
*/
static  OS_TCB   AppTaskCommTCB;

/***********************************************
* ������ �����ջ��STACKS��
*/
static  CPU_STK  AppTaskCommStk[ APP_TASK_COMM_STK_SIZE ];

//StrCtrlDCB       Ctrl;

OS_Q             MTR_RxQ;
/*******************************************************************************
 * LOCAL VARIABLES
 */

/*******************************************************************************
 * GLOBAL VARIABLES
 */

/***********************************************
* ������ ����ʱ������
*/
OS_TMR          OSTmr1;                         //��ʱ��1

/***********************************************
* ������ ����ʱ���ص�����
*/
void            OSTmr1_callback             (OS_TMR *ptmr,void *p_arg);
CPU_BOOLEAN     APP_CSNC_CommHandler        (MODBUS_CH  *pch);
CPU_BOOLEAN     APP_MaterCommHandler        (MODBUS_CH  *pch);

/*******************************************************************************
 * LOCAL FUNCTIONS
 */
static void    AppTaskComm                 (void *p_arg);
static void    APP_CommInit                (void);
        void    App_McuStatusInit           (void);
        void    ReportDevStatusHandle       (void);
        void    InformCommConfigMode        (u8 mode);
static void    App_ModbusInit              (void);

INT08U          APP_CommRxDataDealCB        (MODBUS_CH  *pch);
INT08U          IAP_CommRxDataDealCB        (MODBUS_CH  *pch);
/*******************************************************************************
 * GLOBAL FUNCTIONS
 */

/*******************************************************************************
 * EXTERN VARIABLES
 */

 /*******************************************************************************
 * EXTERN FUNCTIONS
 */
extern  void    app_comm_mtr(void);
extern  void    app_comm_dtu(void);
extern  void    app_comm_otr(void);

unsigned char GetRecSlaveAddr(void);
void    comm_para_flow(StrDevOtr * sDtu,uint8 addrnum);
void    comm_rec_read (StrDevOtr * sDtu,uint8 addrnum);


/*******************************************************************************/

/*******************************************************************************
 * ��    �ƣ� App_TaskCommCreate
 * ��    �ܣ� **���񴴽�
 * ��ڲ����� ��
 * ���ڲ����� ��
 * ���� ���ߣ� wumingshen.
 * �������ڣ� 2015-02-05
 * ��    �ģ�
 * �޸����ڣ�
 * ��    ע�� ���񴴽�������Ҫ��app.h�ļ�������
 *******************************************************************************/
void  App_TaskCommCreate(void)
{
    OS_ERR  err;

    /***********************************************
    * ������ ���񴴽�
    */
    OSTaskCreate((OS_TCB     *)&AppTaskCommTCB,                     // ������ƿ�  ����ǰ�ļ��ж��壩
                 (CPU_CHAR   *)"App Task Comm",                     // ��������
                 (OS_TASK_PTR ) AppTaskComm,                        // ������ָ�루��ǰ�ļ��ж��壩
                 (void       *) 0,                                  // ����������
                 (OS_PRIO     ) APP_TASK_COMM_PRIO,                 // �������ȼ�����ͬ�������ȼ�������ͬ��0 < ���ȼ� < OS_CFG_PRIO_MAX - 2��app_cfg.h�ж��壩
                 (CPU_STK    *)&AppTaskCommStk[0],                  // ����ջ��
                 (CPU_STK_SIZE) APP_TASK_COMM_STK_SIZE / 10,        // ����ջ�������ֵ
                 (CPU_STK_SIZE) APP_TASK_COMM_STK_SIZE,             // ����ջ��С��CPU���ݿ��� * 8 * size = 4 * 8 * size(�ֽ�)����app_cfg.h�ж��壩
                 (OS_MSG_QTY  ) 0u,                                 // ���Է��͸�����������Ϣ��������
                 (OS_TICK     ) 0u,                                 // ��ͬ���ȼ��������ѭʱ�䣨ms����0ΪĬ��
                 (void       *) 0,                                  // ��һ��ָ����������һ��TCB��չ�û��ṩ�Ĵ洢��λ��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK |               // ������ջ��������
                                OS_OPT_TASK_STK_CLR),               // ��������ʱ��ջ����
                 (OS_ERR     *)&err);                               // ָ���������ָ�룬���ڴ����������

}

extern  void    OS_ProtocolInit(void);
/*******************************************************************************
 * ��    �ƣ� AppTaskComm
 * ��    �ܣ� ��������
 * ��ڲ����� p_arg - �����񴴽���������
 * ���ڲ����� ��
 * ���� ���ߣ� wumingshen.
 * �������ڣ� 2015-02-05
 * ��    �ģ�
 * �޸����ڣ�
 *******************************************************************************/
static  void  AppTaskComm (void *p_arg)
{
    OS_ERR      err;

    OS_TICK     dly     = CYCLE_TIME_TICKS;
//    OS_TICK     ticks;
    /***********************************************
    * ������ �����ʼ��
    
    */
    APP_CommInit();   
    
    OS_ProtocolInit();
             
    /***********************************************
    * ������ Task body, always written as an infinite loop.
    */
    while (DEF_TRUE) { 
        /***********************************************
        * ������ �������Ź���־��λ
        */
        OSFlagPost(( OS_FLAG_GRP *)&WdtFlagGRP,
                    ( OS_FLAGS     ) WDT_FLAG_COMM,
                    ( OS_OPT       ) OS_OPT_POST_FLAG_SET,
                    ( OS_ERR      *) &err);
        
        
        /***********************************************
        * ������ �ȴ�COMM�ı�ʶ��
        */
        OS_FLAGS    flags = 
        OSFlagPend( ( OS_FLAG_GRP *)&sCtrl.Os.CommEvtFlagGrp,
                    ( OS_FLAGS     ) sCtrl.Os.CommEvtFlag,
                    ( OS_TICK      ) dly,
                    ( OS_OPT       ) OS_OPT_PEND_FLAG_SET_ANY,
                    ( CPU_TS      *) NULL,
                    ( OS_ERR      *)&err);
        
//        OS_ERR      terr;
//        ticks   = OSTimeGet(&terr);                        // ��ȡ��ǰOSTick
        

        /***********************************************
        * ������ û�д���,���¼�����
        */
        if ( err == OS_ERR_NONE ) {
            OS_FLAGS    flagClr = 0;

            /***********************************************
            * �ӻ�������յ�����ͨѶ���𣻻�������IC�����룬ͬ������ͨѶ
            */           
            if(    flags & COMM_EVT_FLAG_PLUG_CARD) {

					comm_para_flow((StrDevOtr *)&sCtrl.Otr,0);   	//���ø���IC�����Ͳ���
					
					flagClr |=  COMM_EVT_FLAG_PLUG_CARD;   			//����IC��
               }else if( flags & COMM_EVT_FLAG_OTR_RX) {			//���յ�����
               		//�������ݡ� 1��ѯ���Ƿ���Ҫ���ݣ�2�����ݼ�¼
                    //BSP_LED_Toggle(8);

               		if(sCtrl.Otr.RxCtrl.SourceAddr == MASTE_ADDR_HOST){	//������ַ
               		
						comm_rec_read((StrDevOtr *)&sCtrl.Otr,0);		//ȡ���ݼ�¼
					}
               			
                    sCtrl.Otr.ConnCtrl[0].TimeOut   = 0;
                    sCtrl.Otr.ConnCtrl[0].ErrFlg    = 0;
                    
                    BSP_LED_Toggle(8);

               		flagClr |=  COMM_EVT_FLAG_OTR_RX;    				//���յ�����
               	}
            
            /***********************************************
            * ������ �����־
            */
            if ( !flagClr ) {
                flagClr = flags;
            }
            
            /***********************************************
            * ������ �����־λ
            */
            OSFlagPost( ( OS_FLAG_GRP  *)&sCtrl.Os.CommEvtFlagGrp,
                        ( OS_FLAGS      )flagClr,
                        ( OS_OPT        )OS_OPT_POST_FLAG_CLR,
                        ( OS_ERR       *)&err);
        }
        
        
        if(sCtrl.Otr.ConnCtrl[0].TimeOut++ > 2)         //2��ûͨѶ
            sCtrl.Otr.ConnCtrl[0].ErrFlg = 1;
        BSP_OS_TimeDly(5);
    }
}

/*******************************************************************************
 * ��    �ƣ� APP_ModbusInit
 * ��    �ܣ� MODBUS��ʼ��
 * ��ڲ����� ��
 * ���ڲ����� ��
 * ���� ���ߣ� ������
 * �������ڣ� 2015-03-28
 * ��    �ģ�
 * �޸����ڣ�
 * ��    ע�� �ó�ʼ���ᴴ��Modbus����
 *******************************************************************************/
void App_ModbusInit(void)
{
    MODBUS_CH   *pch;
    /***********************************************
    * ������ uCModBus��ʼ����RTUʱ��Ƶ��Ϊ1000HZ
    *        ʹ���˶�ʱ��2��TIM2��
    */
    MB_Init(1000);
    
    // UART1
    /***********************************************
    * ������  UART1���ںͲ���װ�����ӡ�master
    *        ����������
    */
#if MODBUS_CFG_MASTER_EN == DEF_TRUE

#endif
    // UART1
    /***********************************************
    * ����:   UART1���ڡ�IC��ģ��
    */
    
#if MODBUS_CFG_MASTER_EN == DEF_TRUE
    pch         = MB_CfgCh( ModbusNode,             // ... Modbus Node # for this slave channel
                            MODBUS_SLAVE,           // ... This is a MASTER
                            500,                    // ... 0 when a slave
                            MODBUS_MODE_RTU,        // ... Modbus Mode (_ASCII or _RTU)
                            1,                      // ... Specify UART #2
                            57600,                  // ... Baud Rate
                            UART_DATABIT_8,         // ... Number of data bits 7 or 8
                            UART_PARITY_NONE,       // ... Parity: _NONE, _ODD or _EVEN
                            UART_STOPBIT_1,         // ... Number of stop bits 1 or 2
                            MODBUS_WR_EN);          // ... Enable (_EN) or disable (_DIS) writes
    pch->AesEn          =   DEF_DISABLED;           // ... AES���ܽ�ֹ
    pch->NonModbusEn    =   DEF_ENABLED;            // ... ֧�ַ�MODBUSͨ��
    pch->IapModbusEn    =   DEF_ENABLED;            // ... ֧��IAP MODBUSͨ��
    
    pch->RxFrameHead    = 0x1028;                   // ... ����ƥ��֡ͷ
    pch->RxFrameTail    = 0x102C;                   // ... ����ƥ��֡β
    
    sCtrl.Otr.pch       = pch;                      // ... modbus���ƿ��ȫ�ֽṹ�彨������
#endif
    
    // UART3
    /***********************************************
    * ������  UART3���ں�IC��ģ�����ӡ�master
    *        
    */
#if MODBUS_CFG_MASTER_EN == DEF_TRUE


#endif
   
}


/*******************************************************************************
 * ��    �ƣ� APP_CommInit
 * ��    �ܣ� �����ʼ��
 * ��ڲ����� ��
 * ���ڲ����� ��
 * ���� ���ߣ� wumingshen.
 * �������ڣ� 2015-03-28
 * ��    �ģ�
 * �޸����ڣ�
 *******************************************************************************/
 void APP_CommInit(void)
{
    OS_ERR err;
    
    /***********************************************
    * ������ �����¼���־��,Э��comm�շ�
    */
    OSFlagCreate(( OS_FLAG_GRP  *)&sCtrl.Os.CommEvtFlagGrp,
                 ( CPU_CHAR     *)"App_CommFlag",
                 ( OS_FLAGS      )0,
                 ( OS_ERR       *)&err);
    
    sCtrl.Os.CommEvtFlag= COMM_EVT_FLAG_OTR_RX        // OTR �����¼�
                        + COMM_EVT_FLAG_OTR_TIMEOUT  // OTR ������ʱ����ʱ����ʹ��
                         + COMM_EVT_FLAG_PLUG_CARD;  // IC������

    
    /***********************************************
    * ������ ��ʼ��MODBUSͨ��
    */        
    App_ModbusInit();
    
    /***********************************************
    * ������ �ڿ��Ź���־��ע�᱾����Ŀ��Ź���־
    */
    WdtFlags |= WDT_FLAG_COMM;
}



/*******************************************************************************
* ��    �ƣ� APP_CommRxDataDealCB
* ��    �ܣ� �������ݴ����ص���������MB_DATA.C����
* ��ڲ����� ��
* ���ڲ����� ��
* ��    �ߣ� ������
* �������ڣ� 2016-01-04
* ��    �ģ�
* �޸����ڣ�
* ��    ע�� 
*******************************************************************************/
INT08U APP_CommRxDataDealCB(MODBUS_CH  *pch)
{
    /***********************************************
    * ������ 2016/01/08���ӣ����ڷ�MODBBUS IAP����ͨ��
    */
#if MB_IAPMODBUS_EN == DEF_ENABLED
    //    /***********************************************
    //    * ������ �������������ģʽ
    //    */
    //    if ( ( Iap.Status != IAP_STS_DEF ) && 
    //         ( Iap.Status != IAP_STS_SUCCEED ) &&
    //         ( Iap.Status != IAP_STS_FAILED ) ) {
    //        return IAP_CommRxDataDealCB(pch);
    //    }
#endif
    /***********************************************
    * ������ ��ȡ֡ͷ
    */
//    CPU_SR_ALLOC();
    
    //redmorningcn ���ݸ��ƹ�����MBS_RTU_Task �Ѿ�ִ�С��˴������ظ�������
    
    //    CPU_CRITICAL_ENTER();
    //    //u8  Len     = pch->RxBufByteCtr;
    u8  Len     = pch->RxBufByteCnt;
    //    memcpy( (INT08U *)&pch->RxFrameData, (INT08U *)pch->RxBuf, Len );
    //    CPU_CRITICAL_EXIT();
    
    OS_ERR      err;
    
    //DataBuf = pch->RxFrameData;
    
    //INT08U  *DataBuf = 0 ;
    /***********************************************
    * ������ �������ݴ���,
    * ����Э�飬����������            Դ��ַ
    *           Ŀ���ַ��           GetRecSlaveAddr ������á�
    *           ������������          Len
    *           ������                 DataBuf
    *           �������            GetRecvFrameNbr����
    */  
    uint8   DataPos;
    u8  SourceAddr = 
        CSNC_GetData(	(uint8 *)pch->RxFrameData,          //RecBuf,
                        (uint16 )Len,                       //RecLen, 
                        (uint8 *)&DataPos,                   //DataBuf,
                        (uint16 *)&Len);                    //InfoLen)
    
    /***********************************************
    * ������ ������յ�ַ��װ�õ�ַ�������򲻴������������ݴ���
    */
    if ( SLAVE_ADDR_OTR != GetRecSlaveAddr() ) 
        return FALSE;
    
    /***********************************************
    * ������ ���� ��ַ  pch��Դ  �ж������� �� ���� ��
    *       ����ֵ��
    */
    //switch  (pch->Ch )
    switch  (pch->PortNbr )     //���ݶ˿ںŶ���
    {
        /***********************************************
        * ������ UART0���յ�����
        */
    case 0:
        break;
        
        /***********************************************
        * ������ UART1���յ����ݡ����ݷ��͵�ַ��A1��A2
        *                         ���ݽ��յ�ַ��80
        */        
    case 1:
           if(         MASTE_ADDR_HOST == SourceAddr 
               )   
            {
                sCtrl.Otr.RxCtrl.SourceAddr = SourceAddr;           //Դ��ַ
                sCtrl.Otr.RxCtrl.DestAddr   = GetRecSlaveAddr();    //Ŀ���ַ
                sCtrl.Otr.RxCtrl.FramNum    = GetRecvFrameNbr();    //���յ���֡��
                
               /***********************************************
                * ����������ͨѶ��IC��ͨѶ��������������ǰ 4 ���ֽ�Ϊ�����֣�   
                *                           ��4�ֽں�������������ݡ�                        
                */                   
                if(Len >=4)                                        
                {
//                    sCtrl.Otr.RxCtrl.Len        = Len - 4;                  //����������
//                    //ȡ���ݼ�¼�������ݼ�¼���浽������
//                    OS_CRITICAL_ENTER();
//                        memcpy( (INT08U *)&sCtrl.Otr.RxCtrl.Code, (INT08U *)&pch->RxFrameData[DataPos], 4);        //������
//                        memcpy( (INT08U *)&sCtrl.Otr.Rd.Buf[0],   (INT08U *)&pch->RxFrameData[DataPos+4], Len-4 );   //������
//                    OS_CRITICAL_EXIT();                    
                    sCtrl.Otr.RxCtrl.Len        = Len;                  //����������    ֻ�������ݼ�¼����ѯ
                    //ȡ���ݼ�¼�������ݼ�¼���浽������
                    //OS_CRITICAL_ENTER();
                        memcpy( (INT08U *)&sCtrl.Otr.Rd.Buf[0],   (INT08U *)&pch->RxFrameData[DataPos], Len );   //������
                    //OS_CRITICAL_EXIT(); 
                    
                }
                /***********************************************
                * ������ UART2����״̬����
                */       
                for(uint8 i = 0;i< COMM_DEV_OTR_CONN_NUM;i++)
                {
                    if( sCtrl.Otr.ConnCtrl[i].SlaveAddr == SourceAddr){
                        sCtrl.Otr.ConnCtrl[i].RecvEndFlg  = 1;      //���յ����ݣ���1�����ݴ�������0
                        sCtrl.Otr.ConnCtrl[i].TimeOut     = 0;      //��ʱ���������㡣
                    }
                } 
                
                OSFlagPost(( OS_FLAG_GRP *)&sCtrl.Os.CommEvtFlagGrp,
                ( OS_FLAGS     ) COMM_EVT_FLAG_OTR_RX,
                ( OS_OPT       ) OS_OPT_POST_FLAG_SET,
                ( OS_ERR      *) &err);       
            }
                
            sCtrl.Otr.ConnectFlag       = 1;
            sCtrl.Otr.ConnectTimeOut    = 0;   
            
            break;
        
        /***********************************************
        * ������ UART3���յ����ݡ����ݷ��͵�ַ��CA��C2
        *                         ���ݽ��յ�ַ:80
        */   
    case 3:

        
        break;
        /***********************************************
        * ������ UART2���յ�����
        */   
    case 2:

        
        break;
    }
    
    return TRUE;
}

/***********************************************
* ������ 2016/01/08���ӣ����ڷ�MODBBUS IAP����ͨ��
*/
#if MB_IAPMODBUS_EN == DEF_ENABLED
/*******************************************************************************
 * ��    �ƣ� APP_CommRxDataDealCB
 * ��    �ܣ� �������ݴ����ص���������MB_DATA.C����
 * ��ڲ����� ��
 * ���ڲ����� ��
 * ��    �ߣ� ������
 * �������ڣ� 2016-01-04
 * ��    �ģ�
 * �޸����ڣ�
 * ��    ע�� 
 *******************************************************************************/
INT08U IAP_CommRxDataDealCB(MODBUS_CH  *pch)
{
//    CPU_SR_ALLOC();
//    CPU_CRITICAL_ENTER();
//    u8  Len     = pch->RxBufByteCtr;
//    memcpy( (INT08U *)&pch->RxFrameData, (INT08U *)pch->RxBuf, Len );
//    CPU_CRITICAL_EXIT();
//    
//    /***********************************************
//    * ������ ��ȡ֡ͷ
//    */    
//    INT08U  *DataBuf    = pch->RxFrameData;
//    
//    /***********************************************
//    * ������ �������ݴ���
//    */
//    u8  DstAddr = 
//    CSNC_GetData(	(unsigned char *)pch->RxFrameData,          //RecBuf,
//                    (unsigned char	 )Len,                       //RecLen, 
//                    (unsigned char *)DataBuf,                   //DataBuf,
//                    (unsigned short *)&Len);                    //InfoLen)
//    /***********************************************
//    * ������ �����ս���
//    */
//    if ( ( Len == 16) && 
//         ( 0 >= memcmp((const char *)"IAP_pragram end!",(const char *)&pch->RxFrameData[8], 16) ) ) {
//        /***********************************************
//        * ������ �������ݴ���
//        */
//        CSNC_SendData(	(MODBUS_CH      *)sCtrl.Com.pch,
//                        (unsigned char  ) sCtrl.Com.SlaveAddr,          // SourceAddr,
//                        (unsigned char  ) sCtrl.Com.Rd.Head.SrcAddr,    // DistAddr,
//                        (unsigned char *)&pch->RxFrameData[8],          // DataBuf,
//                        (unsigned short ) Len); 
//        /***********************************************
//        * ������ ��λIAP������־λ
//        */
//        OS_ERR err;
//        OS_FlagPost(( OS_FLAG_GRP *)&sCtrl.Os.CommEvtFlagGrp,
//                    ( OS_FLAGS     ) COMM_EVT_FLAG_IAP_END,
//                    ( OS_OPT       ) OS_OPT_POST_FLAG_SET,
//                    ( CPU_TS       ) 0,
//                    ( OS_ERR      *) &err);
//        
//        return TRUE;
//    }
//    
//    if ( ( Len == 18) && 
//         ( 0 >= memcmp((const char *)"IAP_pragram start!",(const char *)&pch->RxFrameData[8], 18) ) ) {
//        
//#if defined     (IMAGE_A) || defined   (IMAGE_B)
//#else
//        /***********************************************
//        * ������ �������ݴ���
//        */
//        CSNC_SendData(	(MODBUS_CH      *)sCtrl.Com.pch,
//                        (unsigned char  ) sCtrl.Com.SlaveAddr,          // SourceAddr,
//                        (unsigned char  ) sCtrl.Com.Rd.Head.SrcAddr,    // DistAddr,
//                        (unsigned char *)&pch->RxFrameData[8],         // DataBuf,
//                        (unsigned int	 ) Len);                        // DataLen 
//#endif
//        IAP_Restart();
//        Iap.FrameIdx    = 0;
//        return TRUE;
//    }
//    
//    if ( Iap.Status < IAP_STS_START )
//        return TRUE;
//    /***********************************************
//    * ������ д����
//    */
//    /***********************************************
//    * ������ �����ݴ�������ͽṹ
//    */
//    sCtrl.Com.Wr.Head.DataLen       = 0;
//    /***********************************************
//    * ������ �༭Ӧ������
//    */
//    char str[20];
//    usprintf(str,"\n%d",Iap.FrameIdx);
//    str[19]  = 0;
//    /***********************************************
//    * ������ д���ݵ�Flash
//    */
//    IAP_Program((StrIapState *)&Iap, (INT16U *)&pch->RxFrameData[8], Len, (INT16U )GetRecvFrameNbr() );
//    /***********************************************
//    * ������ �������ݴ���
//    */
//    CSNC_SendData(	(MODBUS_CH     *) sCtrl.Com.pch,
//                    (unsigned char  ) sCtrl.Com.SlaveAddr,                    // SourceAddr,
//                    (unsigned char  ) sCtrl.Com.Rd.Head.SrcAddr,              // DistAddr,
//                    (unsigned char *) str,                                    // DataBuf,
//                    (unsigned short ) strlen(str));                           // DataLen 
//    if ( Len < 128 ) {
//        /***********************************************
//        * ������ ��λIAP������־λ
//        */
//        OS_ERR err;
//        OS_FlagPost(( OS_FLAG_GRP *)&sCtrl.Os.CommEvtFlagGrp,
//                    ( OS_FLAGS     ) COMM_EVT_FLAG_IAP_END,
//                    ( OS_OPT       ) OS_OPT_POST_FLAG_SET,
//                    ( CPU_TS       ) 0,
//                    ( OS_ERR      *) &err);
//    } else {
//        IAP_Programing();                               // ��λ��ʱ������
//    }
//        
//    /***********************************************
//    * ������ ���ڽ���COMMģ�����Ϣ������
//    */
//    sCtrl.Com.ConnectTimeOut    = 0;                // ��ʱ����������
//    sCtrl.Com.ConnectFlag       = TRUE;             // ת���ӳɹ���־
//    
    return TRUE;
}
#endif

/*******************************************************************************
 * ��    �ƣ� NMBS_FCxx_Handler
 * ��    �ܣ� ��MODBUS�������ݴ����ص���������mbs_core.d���á����������ж�Ӧ��
 * ��ڲ����� ��
 * ���ڲ����� ��
 * ��    �ߣ� ������
 * �������ڣ� 2017-02-03
 * ��    �ģ�
 * �޸����ڣ�
 * ��    ע�� 
 *******************************************************************************/
extern OS_Q                MTR_RxQ;
extern OS_Q                COM_RxQ;
extern OS_Q                DTU_RxQ;
//CPU_BOOLEAN  NMBS_FCxx_Handler (MODBUS_CH  *pch)
//{
//    OS_ERR      err;
//    CPU_INT16U  head    = BUILD_INT16U(pch->RxBuf[1], pch->RxBuf[0]);
//    CPU_INT16U  tail    = BUILD_INT16U(pch->RxBuf[pch->RxBufByteCtr-1],
//                                       pch->RxBuf[pch->RxBufByteCtr-2]);
//    /***********************************************
//    * ������ ��ɳ�ϳ�DTUЭ�鴦��
//    */
//    
//    if ( ( pch->RxFrameHead == head ) &&              
//         ( pch->RxFrameTail == tail ) ) {
//        APP_CommRxDataDealCB(pch);
//        /***********************************************
//        * ������ �������Э��Э��,���Э��Э�鴦������
//        */
//    } else  if ( ( 0xAA55 == head ) || ( 0xAAAA == head ) ) {
//        OS_ERR  err;
//        
//        CPU_SR_ALLOC();
//        OS_CRITICAL_ENTER();
//        memcpy( (INT08U *)&pch->RxFrameData, (INT08U *)pch->RxBuf, pch->RxBufByteCnt );
//        OS_CRITICAL_EXIT();
//        
//        (void)OSQPost((OS_Q         *)&MTR_RxQ,
//                      (void         *) pch,
//                      (OS_MSG_SIZE   ) pch->RxBufByteCtr,
//                      (OS_OPT        ) OS_OPT_POST_FIFO,
//                      (OS_ERR       *)&err);
//         //BSP_OS_TimeDly(5);
//         /***********************************************
//         * ������ ���Э��Э�鴦��
//         */  
//    } else {
//        return DEF_FALSE;
//    }
//    return DEF_TRUE;
//}

CPU_BOOLEAN  NMBS_FCxx_Handler (MODBUS_CH  *pch)
{
//    OS_ERR      err;
    //   CPU_INT16U  head    = BUILD_INT16U(pch->RxBuf[1], pch->RxBuf[0]);
    //    CPU_INT16U  tail    = BUILD_INT16U(pch->RxBuf[pch->RxBufByteCtr-1],
    //                                       pch->RxBuf[pch->RxBufByteCtr-2]);
    CPU_INT16U  head    = BUILD_INT16U( pch->RxFrameData[1], pch->RxFrameData[0]);
    CPU_INT16U  tail    = BUILD_INT16U( pch->RxFrameData[pch->RxBufByteCnt-1],
                                       pch->RxFrameData[pch->RxBufByteCnt-2]);           //20170526 RxBufByteCtr �жϺ����ж�̬���¡�
    
    /***********************************************
    * ������ ��ɳ�ϳ�DTUЭ�鴦��
    */
    
    if ( ( pch->RxFrameHead == head ) &&              
        ( pch->RxFrameTail == tail ) ) {
            APP_CommRxDataDealCB(pch);
            /***********************************************
            * ������ �������Э��Э��,���Э��Э�鴦������
            */
        } else  if ( ( 0xAA55 == head ) || ( 0xAAAA == head ) ) {
            OS_ERR  err;
            
            //CPU_SR_ALLOC();
            //OS_CRITICAL_ENTER();
            memcpy( (INT08U *)&pch->RxFrameData, (INT08U *)pch->RxBuf, pch->RxBufByteCnt );
            //OS_CRITICAL_EXIT();
            
            (void)OSQPost((OS_Q         *)&MTR_RxQ,
                          (void         *) pch,
                          //(OS_MSG_SIZE   ) pch->RxBufByteCtr,
                          (OS_MSG_SIZE   ) pch->RxBufByteCnt,
                          (OS_OPT        ) OS_OPT_POST_FIFO,
                          (OS_ERR       *)&err);
             //BSP_OS_TimeDly(5);
             /***********************************************
             * ������ ���Э��Э�鴦��
             */  
        } else {
            return DEF_FALSE;
        }
    return DEF_TRUE;
}


/*******************************************************************************
 * ��    �ƣ� APP_MaterCommHandler
 * ��    �ܣ� ����������ݴ�������
 * ��ڲ����� ��
 * ���ڲ����� ��
 * �������ߣ� wumingshen.
 * �������ڣ� 2015-03-28
 * ��    �ģ�
 * �޸����ڣ�
 *******************************************************************************/
/***********************************************
* ������ 2015/12/07���ӣ����ڷ�MODBBUSͨ��
*        ��MODBUSͨ�ţ���֡ͷ֡β��ͨ�����ݴ���
*/
#if MB_NONMODBUS_EN == DEF_ENABLED
CPU_BOOLEAN APP_MaterCommHandler (MODBUS_CH  *pch)
{    
//    uint8_t     fram_clr[4]     = {0};
//	uint8_t     IDBuf[4]        = {0};
//	uint8_t     clear_buf[24]   = {0};
//	uint32_t    ID              = 0;
//	uint32_t    utemp;
    	
	//TIME        system; 
    
    /***********************************************
    * ������ �������ݵ�������
    */
    //CPU_SR_ALLOC();
    //CPU_CRITICAL_ENTER();
    //uint32_t  Len     = pch->RxBufByteCnt;
    //memcpy( (INT08U *)&pch->RxFrameData, (INT08U *)pch->RxBuf, Len );
    //CPU_CRITICAL_EXIT();
    uint32_t    Len       = pch->RxBufByteCnt;
    INT08U      *DataBuf  = pch->RxFrameData;
    
    
    CPU_INT16U  head    = BUILD_INT16U(pch->RxBuf[1], pch->RxBuf[0]);
    CPU_INT16U  tail    = BUILD_INT16U(pch->RxBuf[pch->RxBufByteCnt-1],
                                       pch->RxBuf[pch->RxBufByteCnt-2]);
    /***********************************************
    * ������ ��ɳ�ϳ�DTUЭ�鴦��
    */
    if ( ( pch->RxFrameHead == head ) &&
         ( pch->RxFrameTail == tail ) ) {
        DataBuf = &pch->RxFrameData[8];
//        Len     = sCtrl.Com.Rd.Head.DataLen;        
        head    = BUILD_INT16U(DataBuf[1], DataBuf[0]);
    }
    
    /***********************************************
    * ������ 
    ���    ����       �ֽ��� ����˵��
    1       ֡ͷ       2      0xAA 0xAA
    2       ֡����     1      0x08 ֡���ȹ̶�Ϊ8�ֽ�
    3       ��������   2      0x00 0x01
    4       Ԥ��       2      0x00 0x00
    5       �ۼӺ�     1      ǰ7���ֽڵ��ۼӺ�
    */ 
    if ( 0xAAAA == head ) {
        Len            -= 1;
        int8_t chkSum   = GetCheckSum(DataBuf,Len);
        
        if ( chkSum ==  DataBuf[Len] ) {
//            pch->TxFrameData[0]     = 0xAA;
//            pch->TxFrameData[1]     = 0xAA;
//            pch->TxFrameData[2]     = 0x30;
//            pch->TxFrameData[3]     = 0x00;
//            pch->TxFrameData[4]     = 0x71;
//            
//            CPU_SR  cpu_sr;
//            OS_CRITICAL_ENTER();
//            memcpy(&pch->TxFrameData[5],(uint8_t *)&Mater.Energy, sizeof(StrEnergy));
//            OS_CRITICAL_EXIT();
//            
//            pch->TxFrameData[45]    = 0x00;
//            pch->TxFrameData[46]    = 0x00;
//            
//            pch->TxFrameData[47]    = GetCheckSum(pch->TxFrameData,47);
//            
//            NMB_Tx((MODBUS_CH   *)pch,
//                   (CPU_INT08U  *)pch->TxFrameData,
//                   (CPU_INT16U   )48);
            return TRUE;
        }
    /***********************************************
    * ������ ���Բ���
    */
    } else if ( ( 0xAA55 == head ) && ( Len > 10 )) {
        /***********************************************
        * ������ ����Ƿ���֡β��û��֡β�����CRCУ��
        */
        if ( tail != 0x55AA  ) {
            //uint16_t crc16_Cal = crc16((uint8_t *)&DataBuf[0],9);
            //uint16_t crc16_Rec = BUILD_INT16U(DataBuf[9],DataBuf[10]);
            //if(crc16_Cal != crc16_Rec) {                
            //    return TRUE;
            //}
        }
        switch(DataBuf[2])
        {
        /***********************************************
        * ������ �������к�
        */
        case 0xA1:
//            ID = BUILD_INT32U(DataBuf[3], DataBuf[4], DataBuf[5], DataBuf[6]);
            //WriteFM24CL64(64 , (uint8_t *)&ID , 4);
            //ReadFM24CL64(64 , (uint8_t *)&ID , 4);
//            uartprintf(pch,"\r\n ��װ��ID����Ϊ��%d\r\n" , ID);
            break;            
        /***********************************************
        * ������ ���û����ͺ�/������
        */
        case 0xA2:	
            //Mater.LocoTyp   = BUILD_INT16U(DataBuf[3],DataBuf[4]);
            //Mater.LocoNbr   = BUILD_INT16U(DataBuf[5],DataBuf[6]);
            //WriteFM24CL64(68 , (uint8_t *)&Mater.LocoTyp, 2);
            //WriteFM24CL64(70 , (uint8_t *)&Mater.LocoNbr, 2);
            //ReadFM24CL64(68 ,  (uint8_t *)&Mater.LocoTyp, 2);
            //ReadFM24CL64(70 ,  (uint8_t *)&Mater.LocoNbr, 2);
            //uartprintf(pch,"\r\n ��ǰ���ͣ�%d����ǰ���ţ�%d\r\n", Mater.LocoTyp,Mater.LocoNbr);
            break; 
        /***********************************************
        * ������ ����ʱ��
        */
        case 0xA3:
//            system.Year     =DataBuf[3];
//            system.Month    =DataBuf[4];
//            system.Day      =DataBuf[5];
//            system.Hour     =DataBuf[6];
//            system.Min      =DataBuf[7];
//            system.Sec      =DataBuf[8];
//            WriteTime(system);
//            GetTime((TIME *)&recordsfr.Time[0]);
//            uartprintf(pch,"\r\n ��ǰʱ��Ϊ��20%02d-%02d-%02d  %02d:%02d:%02d", 
//                       recordsfr.Time[0], recordsfr.Time[1], recordsfr.Time[2],
//                       recordsfr.Time[3], recordsfr.Time[4], recordsfr.Time[5]);
            break;
            
        default:
            break;
        }
    }
    /***********************************************
    * ������ ���ô������ݴ����ص�����
    */ 
    return TRUE;
}

/*******************************************************************************
 * ��    �ƣ� APP_CSNC_CommHandler
 * ��    �ܣ� �������ݴ�������
 * ��ڲ����� ��
 * ���ڲ����� ��
 * �������ߣ� wumingshen.
 * �������ڣ� 2015-03-28
 * ��    �ģ�
 * �޸����ڣ�
 *******************************************************************************/
CPU_BOOLEAN APP_CSNC_CommHandler (MODBUS_CH  *pch)
{   
//    if ( pch->PortNbr == sCtrl.Dtu.pch->PortNbr ) {
//        if ( GetRecvFrameNbr() == sCtrl.Com.Rd.Head.PacketSn ) {
//            /***********************************************
//            * ������ �����ʱ�ϴ���־
//            */
//            //SetSendFrameNbr();
//            //extern void UpdateRecordPoint(uint8_t storeflag);
//            //UpdateRecordPoint(1);
//            //pch->StatNoRespCtr  = 0;
//            
//            //osal_set_event( OS_TASK_ID_TMR, OS_EVT_TMR_MIN);
//        }
//    } else {
//        /***********************************************
//        * ������ �����ݴ�������ͽṹ
//        */ 
//        APP_MaterCommHandler(pch);
//    }
//    
//    /***********************************************
//    * ������ ���ڽ���COMMģ�����Ϣ������
//    */
//    sCtrl.Com.ConnectTimeOut    = 0;                // ��ʱ����������
//    sCtrl.Com.ConnectFlag       = TRUE;             // ת���ӳɹ���־
//    
    return TRUE;
}
#endif
/*******************************************************************************
 * 				end of file
 *******************************************************************************/
#endif