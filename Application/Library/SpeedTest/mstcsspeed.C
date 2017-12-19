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
extern  OS_SEM           time2_over_Sem;               // 信号量
#endif

uint32	mPluseSumCntcs = 0;
//-------------------------------------------------------------------------------
//函数名称:         GetFrqCnt()
//函数功能:         取频率值
//入口参数:         无		 
//出口参数:         
//-------------------------------------------------------------------------------
uint32  GetFrq3SumCnt(void)
{
	return	mPluseSumCntcs;
}

uint32	m_TimecsSum = 0;
uint32	m_AddPlusecs = 0;
uint32	m_PlusecsChgFlg = 0;
stcSpeedPluse	m_sSpeedcsPluse;


#define	MATH_PLUSE_TIME	 (2*1000*1000)
#define	MATH_PLUSE_NUM 	 100

#define		CAP00	126
#define		CAP10	118
#define		CAP20	4

void TIMER2_IRQHandler(void)
{        
    uint8  tmp = 0;

	if (TIM_GetIntCaptureStatus(LPC_TIM2,TIM_MR0_INT))
	{
		TIM_ClearIntCapturePending(LPC_TIM2,TIM_MR0_INT);

        nop();
   
        tmp = GPIO_ReadIoVal(CAP20);  
        
        if(tmp == 1)
            return;
        
		m_TimecsSum += TIM_GetCaptureValue(LPC_TIM2,TIM_COUNTER_INCAP0);
		TIM_ResetCounter(LPC_TIM2);

		m_AddPlusecs++;
		mPluseSumCntcs++;

		if(m_AddPlusecs == MATH_PLUSE_NUM || m_TimecsSum > MATH_PLUSE_TIME) 		//500个脉冲或1秒时间到
		{
			m_sSpeedcsPluse.TimeCnt 	= 	m_TimecsSum;
			m_sSpeedcsPluse.PluseCnt	=   m_AddPlusecs;
				
			m_AddPlusecs = 0;
			m_TimecsSum = 0;
			m_PlusecsChgFlg = 1;
            
            #if (OS_VERSION > 30000U)
                BSP_OS_SemPost(&time2_over_Sem);                        	// 发送信号量
            #endif
		}
	}
}


uint16  l_speedcs = 0;
uint16	GetMyEngRotSpd(void)
{
	return	l_speedcs;
}

//void GetSpeedPlusecs(stcSpeedPluse * sSpeedPluse)
//{
//	static	uint32	Time;
//	
//	sSpeedPluse->PluseCnt = m_sSpeedcsPluse.PluseCnt;
//	sSpeedPluse->TimeCnt  = m_sSpeedcsPluse.TimeCnt;
//
//	if(m_PlusecsChgFlg == 1)							//脉冲累加处理,
//	{
//		m_PlusecsChgFlg = 0;
////		Time = GetSysTime();
//	}
//		
////	if(GetSysTime() - Time > 250)					//超过2秒没更新，清零    ---  最后的刷新时间2秒
//	{	
////		printfcom0("\r\n outtime");
//		sSpeedPluse->PluseCnt = 0;
//		sSpeedPluse->TimeCnt = 0;
//	}	
//}
//
////计算公式：V = （PI * DIM * n * 3600）/ (( DPR *1000*10*100 )*(t/12000000);
////------------------------------------------------------------------------------
////功    能：    计算总里程，分段里程，自定义里程
////入口参数：    
////出口参数：    
////------------------------------------------------------------------------------
//void  CalcSpeedcs( void )
//{
////	uint32	PluseCnt = 0;
////	uint32	TimeCnt;
////	uint32	DistInSec;
////	uint16	speed;
////	uint32	cstime     = 0;
////	uint32	avg;
////
////    
////    PluseCnt = m_sSpeedcsPluse.PluseCnt;
////	cstime	 = m_sSpeedcsPluse.TimeCnt;
//    	
//
//    
//    
////	if(sSpeedPluse.TimeCnt)
////	{
////		if(times < 3)
////		{		
////			PluseCnt 		= sSpeedPluse.PluseCnt;
////			cstime	 		= sSpeedPluse.TimeCnt;
////		}
////		else
////		{
////			avg =  PluseCnt/3;
////			PluseCnt = PluseCnt - avg + sSpeedPluse.PluseCnt;
////			avg =  cstime/3;
////			cstime   = cstime - sSpeedPluse.PluseCnt;
////		}
////
//#if (OS_VERSION > 30000U)
//    CPU_BOOLEAN err;   
//    static  uint8 times = 0;
//
//    err = BSP_OS_SemWait(&time2_over_Sem,10);                        	// 发送信号量
//    if(!err)
//    {
//            // 发送信号量,速度慢，需要3秒测试一次，测量才准确
//        if(times++ > 5)
//            l_speedcs = 0;  
//    }
//    
//    if(err){
//        times = 0;
//#endif    
//        if( cstime >1000    && 
//        m_PlusecsChgFlg      )                   //产生新脉冲
//    //	if(cstime >1000)
//        {
//            m_PlusecsChgFlg = 0;            
//            l_speedcs = ((PluseCnt * 60 * 1000) / (cstime/1000));
//        }
//    }
//}


