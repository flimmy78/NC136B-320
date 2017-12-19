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
* 描述： 任务控制块（TCB）
*/
static  OS_TCB   AppTaskCardTCB;

/***********************************************
* 描述： 任务堆栈（STACKS）
*/
static  CPU_STK  AppTaskCardStk[ APP_TASK_CARD_STK_SIZE ];
void    AppTaskCard(void);  


/*******************************************************************************
 * 名    称： App_TaskCardCreate
 * 功    能： **任务创建
 * 入口参数： 无
 * 出口参数： 无
 * 作　 　者： redmorningcn.
 * 创建日期： 2017-06-01
 * 修    改：
 * 修改日期：
 * 备    注： 任务创建函数需要在app.h文件中声明
 *******************************************************************************/
void  App_TaskCardCreate(void)
{
    OS_ERR  err;

    /***********************************************
    * 描述： 任务创建
    */
    OSTaskCreate((OS_TCB     *)&AppTaskCardTCB,                     // 任务控制块  （当前文件中定义）
                 (CPU_CHAR   *)"App Task Card",                     // 任务名称
                 (OS_TASK_PTR ) AppTaskCard,                        // 任务函数指针（当前文件中定义）
                 (void       *) 0,                                  // 任务函数参数
                 (OS_PRIO     ) APP_TASK_CARD_PRIO,                 // 任务优先级，不同任务优先级可以相同，0 < 优先级 < OS_CFG_PRIO_MAX - 2（app_cfg.h中定义）
                 (CPU_STK    *)&AppTaskCardStk[0],                  // 任务栈顶
                 (CPU_STK_SIZE) APP_TASK_CARD_STK_SIZE / 10,        // 任务栈溢出报警值
                 (CPU_STK_SIZE) APP_TASK_CARD_STK_SIZE,             // 任务栈大小（CPU数据宽度 * 8 * size = 4 * 8 * size(字节)）（app_cfg.h中定义）
                 (OS_MSG_QTY  ) 0u,                                 // 可以发送给任务的最大消息队列数量
                 (OS_TICK     ) 0u,                                 // 相同优先级任务的轮循时间（ms），0为默认
                 (void       *) 0,                                  // 是一个指向它被用作一个TCB扩展用户提供的存储器位置
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK |               // 允许堆栈检查该任务
                                OS_OPT_TASK_STK_CLR),               // 创建任务时堆栈清零
                 (OS_ERR     *)&err);                               // 指向错误代码的指针，用于创建结果处理

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
		l_CardType 		= sCardType.CardType[0];			//IC卡 类型
		
		l_FlgZoneErrEvent = NO_OCCUR;								//IC卡 标识页错误事件
		
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
* 名    称： app_task_card
* 功    能： 卡任务
* 入口参数： 无
* 出口参数： 无
* 作　 　者： redmorningcn.
* 创建日期： 2017-05-31
* 修    改：
* 修改日期：
* 备    注： 任务创建函数需要在app.h文件中声明
*******************************************************************************/
void    AppTaskCard(void)  
{
	uint16		CrcCheck;
	//模型卡处理
	unsigned char   *p;
	uint32	writedsize = 0;	
	uint32	readsize = 0;
    uint8   carderrflg = 0;
    
    //初始化 card
    InitCardIO();
    
    //任务体
    while(1)
    {
        //等待信号量,有IC卡插入，且卡类型正确
        BSP_OS_SemWait(&Bsp_Card_Sem,0);           
        
        uint8  cardflg  = 0;
        
        extern  uint8    InitCardPageInfo(void);

        carderrflg = 1;     //先置卡错误，如后续正确，则会重新给值
        if(InitCardPageInfo())
        {
            cardflg = ReadCardFlg();
        }
        //读IC卡标志页，取卡类型
        if( cardflg )								
        {
			sCtrl.sRunPara.CardType = l_CardType;		//卡类型幅值，全局变量
            
            extern  uint8	ReadCardPara(stcCardPara * sCardPara);

			if(ReadCardPara((stcCardPara *)	&sCardPara)){	// 卡标示参数区读取正确。
				
                carderrflg = 0; 
                
	            switch( l_CardType )
	            {
                    //数据卡，取FLASH的数据
                case DATA_CARD:	
                    DataCard(0);
                    
                    break;									
                case DENSITY_CARD:									//密度卡
                    CrcCheck = GetCrc16Check((uint8 *)&sCardPara.sDensity,
                                             sizeof(sCardPara.sDensity) - 2);
                    
                    sCtrl.SOilPara.Density = 820;
                    if(CrcCheck == sCardPara.sDensity.CrcCheck)		//校验和相等		
                    {			
                        sCtrl.SOilPara.Density = sCardPara.sDensity.Density;	//密度幅值
                    }
                	break;
            	case MODEL_CARD:									//模型卡
	                p = (uint8  *)&m_sModelCard;
	                writedsize = 0;
                    
	                while(writedsize < sizeof(stcModelCard))		//将数据读值m_sModelCard.
	                {
	                    readsize = (sizeof(stcModelCard)- writedsize)>256?
	                        256:(sizeof(stcModelCard)- writedsize);
	                        
	                        ReadCard(CARD_DATA_ADDR+writedsize,&p[writedsize],readsize);
	                        
	                        writedsize +=readsize;
	                }
                    
                    break;				
                    
                case HIGHT_CARD:												//高度调整卡				
                    
                    CrcCheck = GetCrc16Check((uint8 *)&sCardPara.sReviseHight,
                                             sizeof(sCardPara.sReviseHight) - 2);
                    
                    sCtrl.SOilPara.Hig  = 0;
                    if(CrcCheck == sCardPara.sReviseHight.CrcCheck)	//校验和相等		
                    {
                        sCtrl.SOilPara.Hig = sCardPara.sReviseHight.ReviseHight;
                    }
                    break;															
                case COPY_CARD:													//数据拷贝卡//还未写入内容
                    DisplaySet(100,1,"COPY");						
                    DataCard(1);
                    break;
                case MODEL_SELECT_CARD:											//模型选择卡
                    sCtrl.SOilPara.ModelNum = sCardPara.SetVal;
                    break;	
                    
                case FIX_CARD:													//装车卡
                    CrcCheck = GetCrc16Check((uint8 *)&sCardPara.sFixInfo,
                                             sizeof(sCardPara.sFixInfo) - 2);
                    
                    sCtrl.sProductInfo.sLocoId.Type = 1;
                    sCtrl.sProductInfo.sLocoId.Num  = 1000;
                    if(CrcCheck == sCardPara.sFixInfo.CrcCheck)		//校验和相等	
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
        
        if(carderrflg == 1)     //卡类型错误，返回错误信息
        {
            sCtrl.sRunPara.CardType = DATA_CARD_ERR;    //卡异常
            sCtrl.sRunPara.CardErrData = 0x01;          //卡异常 
        }

//        OS_ERR  os_err;
//        OSTimeDlyHMSM(0, 0, 0, 100,
//              OS_OPT_TIME_HMSM_STRICT, &os_err);
        
        OS_ERR err;

		//IC卡处理完成，通知串口通讯处理
	    OSFlagPost( ( OS_FLAG_GRP  *)&sCtrl.Os.CommEvtFlagGrp,
	        ( OS_FLAGS      )COMM_EVT_FLAG_PLUG_CARD,
	        ( OS_OPT        )OS_OPT_POST_FLAG_SET,
	        ( OS_ERR       *)&err);
    }
}
 
    
