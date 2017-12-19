#include <includes.h>
#include <FRAM.h>
#include <osal.h>
#include <bsp_os.h>

//每圈脉冲常量
#define		PPR		200
//pi = 3.14 * 100
#define		PI		314
//轮对直径 1.1m * 10
//#define		DIM		11
#define		DIM		10.5
//里程常数 PPR *10*100*1000  
#define		PI_DIM_MUL		(10*100)

#if (UCOS_EN == DEF_ENABLED ) && ( OS_VERSION > 30000U )
extern  OS_SEM           time0_over_Sem;               // 信号量
#endif


uint32	mPluseSumCnt0 = 0;
//-------------------------------------------------------------------------------
//函数名称:         GetFrqCnt()
//函数功能:         取频率值
//入口参数:         无		 
//出口参数:         
//-------------------------------------------------------------------------------
uint32  GetFrq1SumCnt(void)
{
	return	mPluseSumCnt0;
}

uint32	m_Time0Sum = 0;
uint32	m_AddPluse0 = 0;
uint32	m_Pluse0ChgFlg = 0;

//static	stcSpeedPluse	m_sSpeed1Pluse;


#define	MATH_PLUSE_TIME	 (1*1000*1000)
#define	MATH_PLUSE_NUM 	 500

#define		CAP00	126

void TIMER0_IRQHandler(void)
{
    uint8  tmp = 0;

	if (TIM_GetIntCaptureStatus(LPC_TIM0,TIM_MR0_INT))
	{
		TIM_ClearIntCapturePending(LPC_TIM0,TIM_MR0_INT);

        nop();
//    uint8  tmp = 0;
   
        tmp = GPIO_ReadIoVal(CAP00);
        
        if(tmp == 1)
            return;

		m_Time0Sum += TIM_GetCaptureValue(LPC_TIM0,TIM_COUNTER_INCAP0);
		TIM_ResetCounter(LPC_TIM0);

		m_AddPluse0++;
		mPluseSumCnt0++;

		if(m_AddPluse0 == MATH_PLUSE_NUM || m_Time0Sum > MATH_PLUSE_TIME) 		//500个脉冲或1秒时间到
		{
//			m_sSpeed1Pluse.TimeCnt 	= 	m_Time0Sum;
//			m_sSpeed1Pluse.PluseCnt	=   m_AddPluse0;
//				
			m_AddPluse0 = 0;
			m_Time0Sum = 0;
			m_Pluse0ChgFlg = 1;
            
            #if (OS_VERSION > 30000U)
            BSP_OS_SemPost(&time0_over_Sem);                        	// 发送信号量
            #endif
		}
	}
}


uint16  l_speed1;
int32  	l_addspeed1;
uint16	GetMySpeed1(void)
{
	return	l_speed1;
}

int32	GetMyAddSpeed1(void)
{
	return	l_addspeed1;
}

//void GetSpeedPluse1(stcSpeedPluse * sSpeedPluse)
//{
//	static	uint32	Time;
//	
//	sSpeedPluse->PluseCnt = m_sSpeed1Pluse.PluseCnt;
//	sSpeedPluse->TimeCnt  = m_sSpeed1Pluse.TimeCnt;
//
//	if(m_Pluse0ChgFlg == 1)							//脉冲累加处理,
//	{
//		m_Pluse0ChgFlg = 0;
//	}
//
//}
//
////计算公式：V = （PI * DIM * n * 3600）/ (( DPR *1000*10*100 )*(t/12000000);
////------------------------------------------------------------------------------
////功    能：    计算总里程，分段里程，自定义里程
////入口参数：    
////出口参数：    
////------------------------------------------------------------------------------
//void  CalcSpeed1( void )
//{
//	uint32	PluseCnt;
//	uint32	TimeCnt;
//	uint32	DistInSec;
//	uint16	speed;
//	static	int32	lstaddspeed = 0;
//	
//	PluseCnt = m_sSpeed1Pluse.PluseCnt;
//	TimeCnt	 = m_sSpeed1Pluse.TimeCnt;
//
//
//#if (OS_VERSION > 30000U)
//    CPU_BOOLEAN err;   
//    static  uint8 times = 0;
//
//    err = BSP_OS_SemWait(&time0_over_Sem,10);                        	// 发送信号量
//    if(!err)
//    {
//            // 发送信号量,速度慢，需要3秒测试一次，测量才准确
//        if(times++ > 5)
//            l_speed1 = 0;
//    }
//        
//    if(err){
//        times = 0;
//#endif
//        
//        if( TimeCnt > 2     && 
//            m_Pluse0ChgFlg      )                   //产生新脉冲
//        {
//            m_Pluse0ChgFlg = 0;
//            
//            DistInSec = PluseCnt*PI*DIM;			//单位时间内运行距离  （未除系数）	 
//                                                    // V = s/t <==> ((n/N)*100pi*10d)/t  (m/us)    （PI和DIM分别放大100和10倍 PI_DIM_MUL = 100*100 ）
//                                                    //<==> ((n*pi*d)/N/t)*(1000*1000*3600）/(1000*PI_DIM_MUL))  (km/h)	 
//                                                    //<==>  ((n*pi*d)*(36/2))/t
//
//            DistInSec =  (DistInSec*(36/2));
//            
//            if((DistInSec % TimeCnt) > TimeCnt/2 )   //4舍5入
//            {
//                speed = DistInSec / TimeCnt	+1;		
//            }
//            else
//            {
//                speed = DistInSec / TimeCnt;
//            }
//
//    //		if(PluseCnt > 5)
//    //		{
//    //			if(lstaddspeed)
//    //			{
//    //				l_addspeed1 = TimeCnt/PluseCnt - lstaddspeed;		
//    //			}
//    //			lstaddspeed = 	TimeCnt/PluseCnt;
//    //		}
//        }	
//        else
//        {
//            speed = 0;
//        }	
//
//        l_speed1 = speed;
//    }
////	printfcom0("\r\n \r\n Speed1 %d \r\n ",speed);
//}

//uint32	l_kilemeter;
//uint32	 GetMyKileMeter(void)
//{
//	return	l_kilemeter;
////}
////------------------------------------------------------------------------------
////功    能：    计算总里程，分段里程，自定义里程
////入口参数：    
////出口参数：    
////------------------------------------------------------------------------------
//void  CalcDist( void )
//{
//	uint32	A_DistTmp;
//	uint32	PluseCnt;
//	uint32	DistInSec;
//	uint32	moddprtmp;
//	uint32	disttmp;
//	static	uint32	LsFrqSumCnt = 0;
//	uint32	plussumcnt;
//	uint32	divtmp;
//	stcDist g_sDist;
//	
//	if(GetMySpeed1())
//	{	
//		plussumcnt = GetFrq1SumCnt();
//	}
//	else
//	{
//		plussumcnt = GetFrq2SumCnt();
//	}
//	PluseCnt   = plussumcnt - LsFrqSumCnt;					//计算新增脉冲数
////	printfcom0("\r\n Dist Pplussumcnt is %d， %d",plussumcnt,LsFrqSumCnt);
//
//	LsFrqSumCnt = plussumcnt;
//
//	ReadDist((stcDist *)&g_sDist);
//	if(PluseCnt > 0)											//有脉冲才进行计算
//	{
////		printfcom0("\r\n11 Dist %d, ModDPR %d",g_sDist.A_sDist.Dist,g_sDist.A_sDist.ModDPR);
//
//		DistInSec = PluseCnt*PI*DIM;							//单位时间内运行距离 D = ((n/N)*pi*d0/PI_DIM_MUL 	（PI和DIM分别放大100和10倍 PI_DIM_MUL = 100*100 ）
//																// <==> (n*pi*d)/(N *PI_DIM_MUL)	(m)
//		 														// <==>	(n*pi*d)/(N *PI_DIM_MUL)/1000(Km)
//		divtmp = 200*10*100;									//N*PI_DIM_MUL*1000
////		printfcom0("\r\n DistInSec %d,",DistInSec);
//		moddprtmp = g_sDist.A_sDist.ModDPR;
////		printfcom0("\r\n ModDPR %d",g_sDist.A_sDist.ModDPR);
//		disttmp = g_sDist.A_sDist.Dist;
//		A_DistTmp = moddprtmp + DistInSec;
////		printfcom0("\r\n A_DistTmp %d,moddprtmp %d,DistInSec %d",A_DistTmp,moddprtmp,DistInSec);
//		//A_DistTmp = g_sDist.A_sDist.ModDPR + DistInSec; 		//运行公里 = 上次余数+此次运行数（未除系数DPR）
//		
//		//总里程计算
//		//if(A_DistTmp > DPR)									//满脉冲计数处理
//	
//		if(A_DistTmp > divtmp*1000)								//满脉冲计数处理
//		{
//			//g_sDist.A_sDist.Dist	+= (A_DistTmp / DPR);		//里程累加
//			disttmp	+= (((A_DistTmp / (divtmp)))/1000);	//里程累加
////			printfcom0("\r\n\r\n\r\n A_DistTmp > divtmp %d,%d,%d,%d,disttmp %d",A_DistTmp > divtmp,A_DistTmp,divtmp*1000,((A_DistTmp / (divtmp)))/1000,disttmp);
//		}
//
//		moddprtmp  	=  A_DistTmp % (divtmp*1000);		//取余量
//
//	    g_sDist.A_sDist.ModDPR =moddprtmp ;
//		g_sDist.A_sDist.Dist =disttmp;
//		
//		l_kilemeter = g_sDist.A_sDist.Dist;
////	   	printfcom0("\r\n22 Dist %d, ModDPR %d",g_sDist.A_sDist.Dist,g_sDist.A_sDist.ModDPR,A_DistTmp);		
//		StoresDist((stcDist *)&g_sDist);											//存里程数据
//	}
//}
//
//
////------------------------------------------------------------------------------
////功    能：    清零总里程，分段里程，自定义里程
////入口参数：    
////出口参数：    
////------------------------------------------------------------------------------
//void  SpeedAndDistTast( void )
//{
//
//		CalcSpeed1();                       //计算速度
//		CalcSpeed2();                       //计算速度
//		CalcSpeedcs();                      //计算速度
//        
//		//CalcDist();                         //计算里程
//        CheckLocoStaTask();                 //机车工况
//
//}
