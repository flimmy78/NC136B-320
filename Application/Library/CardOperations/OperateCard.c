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

/*******************************************************************************
 * 名    称： 首字母大写
 * 功    能： 
 * 入口参数： 无
 * 出口参数： 无
 * 作    者： 无名沈
 * 创建日期： 2017/12/27
 * 修    改： 
 * 修改日期： 
 * 备    注： 
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
/*******************************************************************************
 * 名    称： GetCardStatus
 * 功    能： 
 * 入口参数： 无
 * 出口参数： 无
 * 作    者： 无名沈
 * 创建日期： 2017/12/27
 * 修    改： 
 * 修改日期： 
 * 备    注： 
 *******************************************************************************/
uint8 GetCardStatus(void)
{
    static  uint8   mPlugTime = 0;
    static  uint8   mNoPlugTime = 0;
    
    //无卡状态下，插入IC卡
    //无卡状态下，插入IC卡卡，发送信号量
    if(ReadIC_SWT() == 1 && Ctrl.sRunPara.plugcard == 0) {
        if( mPlugTime++ > 2) {
            Ctrl.sRunPara.plugcard = 1;
            //发送信号量，启动 IC卡任务  
            //BSP_OS_SemPost(&Bsp_Card_Sem);
            mNoPlugTime = 0;
            return  1;
        }
        //有卡状态，且已经拔出IC卡   
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
    OS_ERR      err;

    OS_TICK     dly     = CYCLE_TIME_TICKS;
    OS_TICK     ticks;
    
	uint16		CrcCheck;
	//模型卡处理
	unsigned char   *p;
	uint32	writedsize = 0;	
	uint32	readsize = 0;
    uint8   carderrflg = 0;
    
    //初始化 card
    InitCardIO();
    
    /***********************************************
    * 描述： 在看门狗标志组注册本任务的看门狗标志
    */
    OSRegWdtFlag(( OS_FLAGS     ) WDT_FLAG_DUMP); 
    //任务体
    while(1) {
        /***********************************************
        * 描述： 本任务看门狗标志置位
        */
        OSSetWdtFlag(( OS_FLAGS     ) WDT_FLAG_DUMP);  
        
        OS_ERR      terr;
        ticks   = OSTimeGet(&terr); 
        //等待信号量,有IC卡插入，且卡类型正确
        if (( 1 == GetCardStatus() ) || (1 == BSP_OS_SemWait(&Bsp_Card_Sem,dly) ) ) {
            uint8  cardflg  = 0;
            Ctrl.sRunPara.DealFlag  = 1;
            extern  uint8    InitCardPageInfo(void);
            //先置卡错误，如后续正确，则会重新给值
            carderrflg = 1; 
            if(InitCardPageInfo()) {
                cardflg = ReadCardFlg();
            }
            //读IC卡标志页，取卡类型
            if( cardflg ) {
                //卡类型幅值，全局变量
                Ctrl.sRunPara.CardType = l_CardType;
                
                extern  uint8	ReadCardPara(stcCardPara * sCardPara);
                // 卡标示参数区读取正确。
                if(ReadCardPara((stcCardPara *)	&sCardPara)) {
                    
                    carderrflg = 0; 
                    
                    switch( l_CardType ) {
                        //数据卡，取FLASH的数据
                    case DATA_CARD:	
                        DataCard(0);
                        
                        break;
                        //密度卡
                    case DENSITY_CARD:									
                        CrcCheck = GetCrc16Check((uint8 *)&sCardPara.sDensity,
                                                 sizeof(sCardPara.sDensity) - 2);
                        
                        Ctrl.SOilPara.Density = 820;
                        //校验和相等	
                        if(CrcCheck == sCardPara.sDensity.CrcCheck)	 {
                            //密度幅值
                            Ctrl.SOilPara.Density = sCardPara.sDensity.Density;	
                        }
                        break;
                        //模型卡
                    case MODEL_CARD:									
                        p = (uint8  *)&m_sModelCard;
                        writedsize = 0;
                        //将数据读值m_sModelCard.
                        while(writedsize < sizeof(stcModelCard)) {	                
                            readsize = (sizeof(stcModelCard)- writedsize)>256?
                                256:(sizeof(stcModelCard)- writedsize);
                                
                                ReadCard(CARD_DATA_ADDR+writedsize,&p[writedsize],readsize);
                                
                                writedsize +=readsize;
                        }
                        
                        break;				
                        //高度调整卡	
                    case HIGHT_CARD:
                        CrcCheck = GetCrc16Check((uint8 *)&sCardPara.sReviseHight,
                                                 sizeof(sCardPara.sReviseHight) - 2);
                        
                        Ctrl.SOilPara.Hig  = 0;
                        //校验和相等	
                        if(CrcCheck == sCardPara.sReviseHight.CrcCheck) {
                            Ctrl.SOilPara.Hig = sCardPara.sReviseHight.ReviseHight;
                        }
                        break;		
                        //数据拷贝卡//还未写入内容
                    case COPY_CARD:													
                        DisplaySet(100,1,"COPY");						
                        DataCard(1);
                        break;
                        //模型选择卡
                    case MODEL_SELECT_CARD:	
                        Ctrl.SOilPara.ModelNum = sCardPara.SetVal;
                        break;	
                        //装车卡
                    case FIX_CARD:
                        CrcCheck = GetCrc16Check((uint8 *)&sCardPara.sFixInfo,
                                                 sizeof(sCardPara.sFixInfo) - 2);
                        
                        Ctrl.sProductInfo.sLocoId.Type = 1;
                        Ctrl.sProductInfo.sLocoId.Num  = 1000;
                        //校验和相等	
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
            //卡类型错误，返回错误信息
            if(carderrflg == 1) {
                Ctrl.sRunPara.CardType      = DATA_CARD_ERR;    //卡异常
                Ctrl.sRunPara.CardErrData   = 0x01;             //卡异常 
            }

            Ctrl.sRunPara.DealFlag  = 0;
            OS_ERR err;

            //IC卡处理完成，通知串口通讯处理
            OSFlagPost( ( OS_FLAG_GRP  *)&Ctrl.Os.CommEvtFlagGrp,
                ( OS_FLAGS      )COMM_EVT_FLAG_PLUG_CARD,
                ( OS_OPT        )OS_OPT_POST_FLAG_SET,
                ( OS_ERR       *)&err);
        } else {
            
        }
        /***********************************************
        * 描述： 去除任务运行的时间，等到一个控制周期里剩余需要延时的时间
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
 
    
