#include    "includes.h"  
#include	"Card.h"
#include	"OperateCard.h"
#include	"CardDataTypeDef.h"
#include	"IdDataTypeDef.h"
#include 	<idDataTypeDef.h>
#include	"FRAM.h"
#include	"app_ctrl.h"

#define CYCLE_TIME_TICKS            (OS_TICKS_PER_SEC / 10)
//
uint16	l_CardType;
uint8	l_FlgZoneErrEvent	= NO_OCCUR;

/***********************************************
* ������ ������ƿ飨TCB��
*/
static  OS_TCB   AppTaskCardTCB;

/***********************************************
* ������ �����ջ��STACKS��
*/
static  CPU_STK  AppTaskCardStk[ APP_TASK_CARD_STK_SIZE ];
void    AppTaskCard(void);  


/*******************************************************************************
 * ��    �ƣ� App_TaskCardCreate
 * ��    �ܣ� **���񴴽�
 * ��ڲ����� ��
 * ���ڲ����� ��
 * ���� ���ߣ� redmorningcn.
 * �������ڣ� 2017-06-01
 * ��    �ģ�
 * �޸����ڣ�
 * ��    ע�� ���񴴽�������Ҫ��app.h�ļ�������
 *******************************************************************************/
void  App_TaskCardCreate(void)
{
    OS_ERR  err;

    /***********************************************
    * ������ ���񴴽�
    */
    OSTaskCreate((OS_TCB     *)&AppTaskCardTCB,                     // ������ƿ�  ����ǰ�ļ��ж��壩
                 (CPU_CHAR   *)"App Task Card",                     // ��������
                 (OS_TASK_PTR ) AppTaskCard,                        // ������ָ�루��ǰ�ļ��ж��壩
                 (void       *) 0,                                  // ����������
                 (OS_PRIO     ) APP_TASK_CARD_PRIO,                 // �������ȼ�����ͬ�������ȼ�������ͬ��0 < ���ȼ� < OS_CFG_PRIO_MAX - 2��app_cfg.h�ж��壩
                 (CPU_STK    *)&AppTaskCardStk[0],                  // ����ջ��
                 (CPU_STK_SIZE) APP_TASK_CARD_STK_SIZE / 10,        // ����ջ�������ֵ
                 (CPU_STK_SIZE) APP_TASK_CARD_STK_SIZE,             // ����ջ��С��CPU���ݿ�� * 8 * size = 4 * 8 * size(�ֽ�)����app_cfg.h�ж��壩
                 (OS_MSG_QTY  ) 0u,                                 // ���Է��͸�����������Ϣ��������
                 (OS_TICK     ) 0u,                                 // ��ͬ���ȼ��������ѭʱ�䣨ms����0ΪĬ��
                 (void       *) 0,                                  // ��һ��ָ����������һ��TCB��չ�û��ṩ�Ĵ洢��λ��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK |               // �����ջ��������
                                OS_OPT_TASK_STK_CLR),               // ��������ʱ��ջ����
                 (OS_ERR     *)&err);                               // ָ���������ָ�룬���ڴ����������

}

/*******************************************************************************
 * ��    �ƣ� ����ĸ��д
 * ��    �ܣ� 
 * ��ڲ����� ��
 * ���ڲ����� ��
 * ��    �ߣ� ������
 * �������ڣ� 2017/12/27
 * ��    �ģ� 
 * �޸����ڣ� 
 * ��    ע�� 
 *******************************************************************************/
uint8	ReadCardFlg(void)
{	
	stcCardType		sCardType;
    
	ReadCard(CARD_FLG_ADDR+sizeof(stcCardID),(uint8 *)&sCardType,sizeof(stcCardType));		
	
	if(     (sCardType.CardType[0] == sCardType.CardType[1]) 
       && 	(sCardType.CardType[1] == sCardType.CardType[2])
           &&	(sCardType.CrcCheck == 
                 GetCrc16Check((uint8 *)&sCardType,sizeof(sCardType) -2))
               )
	{
		l_CardType 		= sCardType.CardType[0];			//IC�� ����
		
		l_FlgZoneErrEvent = NO_OCCUR;								//IC�� ��ʶҳ�����¼�
		
		//PrintfDebug("\r\n l_CardType %d",l_CardType);
		
		return 	1;
	}
	else
	{
		l_FlgZoneErrEvent = OCCUR;
		l_CardType = 0x0000;
		
		return	0;
	}
}
/*******************************************************************************
 * ��    �ƣ� GetCardStatus
 * ��    �ܣ� 
 * ��ڲ����� ��
 * ���ڲ����� ��
 * ��    �ߣ� ������
 * �������ڣ� 2017/12/27
 * ��    �ģ� 
 * �޸����ڣ� 
 * ��    ע�� 
 *******************************************************************************/
uint8 GetCardStatus(void)
{
    static  uint8   mPlugTime = 0;
    static  uint8   mNoPlugTime = 0;
    
    //�޿�״̬�£�����IC��
    //�޿�״̬�£�����IC�����������ź���
    if(ReadIC_SWT() == 1 && Ctrl.sRunPara.plugcard == 0) {
        if( mPlugTime++ > 2) {
            Ctrl.sRunPara.plugcard = 1;
            //�����ź��������� IC������  
            //BSP_OS_SemPost(&Bsp_Card_Sem);
            mNoPlugTime = 0;
            return  1;
        }
        //�п�״̬�����Ѿ��γ�IC��   
    } else if ( ReadIC_SWT() == 0 &&  Ctrl.sRunPara.plugcard == 1) {   
        if( mNoPlugTime++ > 2) {
            Ctrl.sRunPara.plugcard = 0;                
            mPlugTime = 0;
        }
    }
    return 0;
}

extern	uint8	l_CardErrorFlg;
extern  OS_SEM	Bsp_Card_Sem; 
stcCardPara		sCardPara;

void InitCardIO( void );
extern  stcModelCard            m_sModelCard;

/*******************************************************************************
* ��    �ƣ� app_task_card
* ��    �ܣ� ������
* ��ڲ����� ��
* ���ڲ����� ��
* ���� ���ߣ� redmorningcn.
* �������ڣ� 2017-05-31
* ��    �ģ�
* �޸����ڣ�
* ��    ע�� ���񴴽�������Ҫ��app.h�ļ�������
*******************************************************************************/
void    AppTaskCard(void)  
{
    OS_ERR      err;

    OS_TICK     dly     = CYCLE_TIME_TICKS;
    OS_TICK     ticks;
    
	uint16		CrcCheck;
	//ģ�Ϳ�����
	unsigned char   *p;
	uint32	writedsize = 0;	
	uint32	readsize = 0;
    uint8   carderrflg = 0;
    
    //��ʼ�� card
    InitCardIO();
    
    /***********************************************
    * ������ �ڿ��Ź���־��ע�᱾����Ŀ��Ź���־
    */
    OSRegWdtFlag(( OS_FLAGS     ) WDT_FLAG_DUMP); 
    //������
    while(1) {
        /***********************************************
        * ������ �������Ź���־��λ
        */
        OSSetWdtFlag(( OS_FLAGS     ) WDT_FLAG_DUMP);  
        
        OS_ERR      terr;
        ticks   = OSTimeGet(&terr); 
        //�ȴ��ź���,��IC�����룬�ҿ�������ȷ
        if (( 1 == GetCardStatus() ) || (1 == BSP_OS_SemWait(&Bsp_Card_Sem,dly) ) ) {
            uint8  cardflg  = 0;
            Ctrl.sRunPara.DealFlag  = 1;
            extern  uint8    InitCardPageInfo(void);
            //���ÿ������������ȷ��������¸�ֵ
            carderrflg = 1; 
            if(InitCardPageInfo()) {
                cardflg = ReadCardFlg();
            }
            //��IC����־ҳ��ȡ������
            if( cardflg ) {
                //�����ͷ�ֵ��ȫ�ֱ���
                Ctrl.sRunPara.CardType = l_CardType;
                
                extern  uint8	ReadCardPara(stcCardPara * sCardPara);
                // ����ʾ��������ȡ��ȷ��
                if(ReadCardPara((stcCardPara *)	&sCardPara)) {
                    
                    carderrflg = 0; 
                    
                    switch( l_CardType ) {
                        //���ݿ���ȡFLASH������
                    case DATA_CARD:	
                        DataCard(0);
                        
                        break;
                        //�ܶȿ�
                    case DENSITY_CARD:									
                        CrcCheck = GetCrc16Check((uint8 *)&sCardPara.sDensity,
                                                 sizeof(sCardPara.sDensity) - 2);
                        
                        Ctrl.SOilPara.Density = 820;
                        //У������	
                        if(CrcCheck == sCardPara.sDensity.CrcCheck)	 {
                            //�ܶȷ�ֵ
                            Ctrl.SOilPara.Density = sCardPara.sDensity.Density;	
                        }
                        break;
                        //ģ�Ϳ�
                    case MODEL_CARD:									
                        p = (uint8  *)&m_sModelCard;
                        writedsize = 0;
                        //�����ݶ�ֵm_sModelCard.
                        while(writedsize < sizeof(stcModelCard)) {	                
                            readsize = (sizeof(stcModelCard)- writedsize)>256?
                                256:(sizeof(stcModelCard)- writedsize);
                                
                                ReadCard(CARD_DATA_ADDR+writedsize,&p[writedsize],readsize);
                                
                                writedsize +=readsize;
                        }
                        
                        break;				
                        //�߶ȵ�����	
                    case HIGHT_CARD:
                        CrcCheck = GetCrc16Check((uint8 *)&sCardPara.sReviseHight,
                                                 sizeof(sCardPara.sReviseHight) - 2);
                        
                        Ctrl.SOilPara.Hig  = 0;
                        //У������	
                        if(CrcCheck == sCardPara.sReviseHight.CrcCheck) {
                            Ctrl.SOilPara.Hig = sCardPara.sReviseHight.ReviseHight;
                        }
                        break;		
                        //���ݿ�����//��δд������
                    case COPY_CARD:													
                        DisplaySet(100,1,"COPY");						
                        DataCard(1);
                        break;
                        //ģ��ѡ��
                    case MODEL_SELECT_CARD:	
                        Ctrl.SOilPara.ModelNum = sCardPara.SetVal;
                        break;	
                        //װ����
                    case FIX_CARD:
                        CrcCheck = GetCrc16Check((uint8 *)&sCardPara.sFixInfo,
                                                 sizeof(sCardPara.sFixInfo) - 2);
                        
                        Ctrl.sProductInfo.sLocoId.Type = 1;
                        Ctrl.sProductInfo.sLocoId.Num  = 1000;
                        //У������	
                        if(CrcCheck == sCardPara.sFixInfo.CrcCheck) {
                            Ctrl.sProductInfo.sLocoId.Num  = BUILD_INT16U(sCardPara.sFixInfo.LocoNum[0],sCardPara.sFixInfo.LocoNum[1]);
                            Ctrl.sProductInfo.sLocoId.Type = BUILD_INT16U(sCardPara.sFixInfo.LocoTyp,sCardPara.sFixInfo.E_LocoTyp); 
                        }
                        
                        break;					
                        
                    default:
                        carderrflg = 1;;
                    }
                }	
            }
            //�����ʹ��󣬷��ش�����Ϣ
            if(carderrflg == 1) {
                Ctrl.sRunPara.CardType      = DATA_CARD_ERR;    //���쳣
                Ctrl.sRunPara.CardErrData   = 0x01;             //���쳣 
            }

            Ctrl.sRunPara.DealFlag  = 0;
            OS_ERR err;

            //IC��������ɣ�֪ͨ����ͨѶ����
            OSFlagPost( ( OS_FLAG_GRP  *)&Ctrl.Os.CommEvtFlagGrp,
                ( OS_FLAGS      )COMM_EVT_FLAG_PLUG_CARD,
                ( OS_OPT        )OS_OPT_POST_FLAG_SET,
                ( OS_ERR       *)&err);
        } else {
            
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
 
    
