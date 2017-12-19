#include    "includes.h"  
#include	"Card.h"
#include	"OperateCard.h"
#include	"CardDataTypeDef.h"
#include	"IdDataTypeDef.h"
#include 	<idDataTypeDef.h>
#include	"FRAM.h"
#include	"app_ctrl.h"

//
uint16	l_CardType;
uint8	l_FlgZoneErrEvent	= NO_OCCUR;

extern  stcSysCtrl  sCtrl;



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
	uint16		CrcCheck;
	//ģ�Ϳ�����
	unsigned char   *p;
	uint32	writedsize = 0;	
	uint32	readsize = 0;
    uint8   carderrflg = 0;
    
    //��ʼ�� card
    InitCardIO();
    
    //������
    while(1)
    {
        //�ȴ��ź���,��IC�����룬�ҿ�������ȷ
        BSP_OS_SemWait(&Bsp_Card_Sem,0);           
        
        uint8  cardflg  = 0;
        
        extern  uint8    InitCardPageInfo(void);

        carderrflg = 1;     //���ÿ������������ȷ��������¸�ֵ
        if(InitCardPageInfo())
        {
            cardflg = ReadCardFlg();
        }
        //��IC����־ҳ��ȡ������
        if( cardflg )								
        {
			sCtrl.sRunPara.CardType = l_CardType;		//�����ͷ�ֵ��ȫ�ֱ���
            
            extern  uint8	ReadCardPara(stcCardPara * sCardPara);

			if(ReadCardPara((stcCardPara *)	&sCardPara)){	// ����ʾ��������ȡ��ȷ��
				
                carderrflg = 0; 
                
	            switch( l_CardType )
	            {
                    //���ݿ���ȡFLASH������
                case DATA_CARD:	
                    DataCard(0);
                    
                    break;									
                case DENSITY_CARD:									//�ܶȿ�
                    CrcCheck = GetCrc16Check((uint8 *)&sCardPara.sDensity,
                                             sizeof(sCardPara.sDensity) - 2);
                    
                    sCtrl.SOilPara.Density = 820;
                    if(CrcCheck == sCardPara.sDensity.CrcCheck)		//У������		
                    {			
                        sCtrl.SOilPara.Density = sCardPara.sDensity.Density;	//�ܶȷ�ֵ
                    }
                	break;
            	case MODEL_CARD:									//ģ�Ϳ�
	                p = (uint8  *)&m_sModelCard;
	                writedsize = 0;
                    
	                while(writedsize < sizeof(stcModelCard))		//�����ݶ�ֵm_sModelCard.
	                {
	                    readsize = (sizeof(stcModelCard)- writedsize)>256?
	                        256:(sizeof(stcModelCard)- writedsize);
	                        
	                        ReadCard(CARD_DATA_ADDR+writedsize,&p[writedsize],readsize);
	                        
	                        writedsize +=readsize;
	                }
                    
                    break;				
                    
                case HIGHT_CARD:												//�߶ȵ�����				
                    
                    CrcCheck = GetCrc16Check((uint8 *)&sCardPara.sReviseHight,
                                             sizeof(sCardPara.sReviseHight) - 2);
                    
                    sCtrl.SOilPara.Hig  = 0;
                    if(CrcCheck == sCardPara.sReviseHight.CrcCheck)	//У������		
                    {
                        sCtrl.SOilPara.Hig = sCardPara.sReviseHight.ReviseHight;
                    }
                    break;															
                case COPY_CARD:													//���ݿ�����//��δд������
                    DisplaySet(100,1,"COPY");						
                    DataCard(1);
                    break;
                case MODEL_SELECT_CARD:											//ģ��ѡ��
                    sCtrl.SOilPara.ModelNum = sCardPara.SetVal;
                    break;	
                    
                case FIX_CARD:													//װ����
                    CrcCheck = GetCrc16Check((uint8 *)&sCardPara.sFixInfo,
                                             sizeof(sCardPara.sFixInfo) - 2);
                    
                    sCtrl.sProductInfo.sLocoId.Type = 1;
                    sCtrl.sProductInfo.sLocoId.Num  = 1000;
                    if(CrcCheck == sCardPara.sFixInfo.CrcCheck)		//У������	
                    {
                        sCtrl.sProductInfo.sLocoId.Num = BUILD_INT16U(sCardPara.sFixInfo.LocoNum[0],sCardPara.sFixInfo.LocoNum[1]);
                        sCtrl.sProductInfo.sLocoId.Type = sCardPara.sFixInfo.LocoTyp;
//                        sCtrl.sProductInfo.sLocoId.Num  = sCardPara.sFixInfo.LocoNum[0]+
//                                                          //sCardPara.sFixInfo.LocoNum[1]<<8;
                                                           
                    }
                    
                    break;					
                    
                default:carderrflg = 1;;
                }
            }	
        }
        
        if(carderrflg == 1)     //�����ʹ��󣬷��ش�����Ϣ
        {
            sCtrl.sRunPara.CardType = DATA_CARD_ERR;    //���쳣
            sCtrl.sRunPara.CardErrData = 0x01;          //���쳣 
        }

//        OS_ERR  os_err;
//        OSTimeDlyHMSM(0, 0, 0, 100,
//              OS_OPT_TIME_HMSM_STRICT, &os_err);
        
        OS_ERR err;

		//IC��������ɣ�֪ͨ����ͨѶ����
	    OSFlagPost( ( OS_FLAG_GRP  *)&sCtrl.Os.CommEvtFlagGrp,
	        ( OS_FLAGS      )COMM_EVT_FLAG_PLUG_CARD,
	        ( OS_OPT        )OS_OPT_POST_FLAG_SET,
	        ( OS_ERR       *)&err);
    }
}
 
    
