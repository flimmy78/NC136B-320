#include <includes.h>
#include <FRAM.h>
#include <osal.h>
#include <bsp_os.h>


//每圈脉冲常量
#define		PPR		200
//pi = 3.14 * 100
#define		PI		314
//轮对直径 1.1m * 10
#define		DIM		10.5
//里程常数 PPR *10*100*1000  
#define		PI_DIM_MUL		(10*100)

#if (UCOS_EN == DEF_ENABLED ) && ( OS_VERSION > 30000U )
extern  OS_SEM           time1_over_Sem;               // 信号量
#endif

uint32	mPluseSumCnt1 = 0;
//-------------------------------------------------------------------------------
//函数名称:         GetFrqCnt()
//函数功能:         取频率值
//入口参数:         无		 
//出口参数:         
//-------------------------------------------------------------------------------
uint32  GetFrq2SumCnt(void)
{
	return	mPluseSumCnt1;
}

uint32	m_Time1Sum = 0;
uint32	m_AddPluse1 = 0;
uint32	m_Pluse1ChgFlg = 0;
//static	stcSpeedPluse	m_sSpeed2Pluse;


#define	MATH_PLUSE_TIME2	 (3*1000*1000)
#define	MATH_PLUSE_NUM2 	 50


#define		CAP00	126
#define		CAP10	118
#define		CAP20	4

void TIMER1_IRQHandler(void)
{        
    uint8  tmp = 0;

	if (TIM_GetIntCaptureStatus(LPC_TIM1,TIM_MR0_INT))
	{
		TIM_ClearIntCapturePending(LPC_TIM1,TIM_MR0_INT);

        
        nop();
   
        tmp = GPIO_ReadIoVal(CAP10);  
        
        if(tmp == 1)
            return;
        
		m_Time1Sum += TIM_GetCaptureValue(LPC_TIM1,TIM_COUNTER_INCAP0);
		TIM_ResetCounter(LPC_TIM1);

		m_AddPluse1++;
		mPluseSumCnt1++;

		if(m_AddPluse1 == MATH_PLUSE_NUM2 || m_Time1Sum > MATH_PLUSE_TIME2) 		//500个脉冲或1秒时间到
		{
//			m_sSpeed2Pluse.TimeCnt 	= 	m_Time1Sum;
//			m_sSpeed2Pluse.PluseCnt	=   m_AddPluse1;
//				
			m_AddPluse1 = 0;
			m_Time1Sum = 0;
			m_Pluse1ChgFlg = 1;
            
            #if (OS_VERSION > 30000U)
                BSP_OS_SemPost(&time1_over_Sem);                        	// 发送信号量
            #endif
		}
	}
}


uint16  l_speed2 = 0;
int32	l_addspeed2 = 0;
uint16	GetMySpeed2(void)
{
	return	l_speed2;
}

int32	GetMyAddSpeed2(void)
{
	return	l_addspeed2;
}
//
//void GetSpeedPluse2(stcSpeedPluse * sSpeedPluse)
//{
////	static	uint32	Time;
//	
//	sSpeedPluse->PluseCnt = m_sSpeed2Pluse.PluseCnt;
//	sSpeedPluse->TimeCnt  = m_sSpeed2Pluse.TimeCnt;
//
//	if(m_Pluse1ChgFlg == 1)							//脉冲累加处理,
//	{
//		m_Pluse1ChgFlg = 0;
////		Time = GetSysTime();
//	}
//	//printfcom2("\r\n m_sSpeed2Pluse.PluseCnt %d,%d \r\n \r\n ",m_sSpeed2Pluse.PluseCnt,m_sSpeed2Pluse.TimeCnt);
//
//		
////	if(GetSysTime() - Time > 550)					//超过2秒没更新，清零    ---  最后的刷新时间2秒
//	{	
//		//printfcom2("\r\n outtime");
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
//void  CalcSpeed2( void )
//{
//	uint32	PluseCnt;
//	uint32	TimeCnt;
//	uint32	DistInSec;
//	uint16	speed;
//	static	int32	lstaddspeed;
//    
//
//	PluseCnt = m_sSpeed2Pluse.PluseCnt;
//	TimeCnt	 = m_sSpeed2Pluse.TimeCnt;
//    	
//#if (OS_VERSION > 30000U)
//    CPU_BOOLEAN err;   
//    static  uint8 times = 0;
//
//    err = BSP_OS_SemWait(&time1_over_Sem,10);                        	// 发送信号量
//
//    if(!err)
//    {
//        if(times++ > 5)        //无信号次数
//            l_speed2 = 0;        
//    }
//    
//    if(err){
//        times = 0;
//#endif
//        if( TimeCnt > 2     && 
//            m_Pluse1ChgFlg      )                   //产生新脉冲
//        {
//            m_Pluse1ChgFlg = 0;
//            
//            DistInSec = PluseCnt*PI*DIM;			//单位时间内运行距离  （未除系数）	 
//                                                    // V = s/t <==> ((n/N)*100pi*10d)/t  (m/us)    （PI和DIM分别放大100和10倍 PI_DIM_MUL = 100*100 ）
//                                                    //<==> ((n*pi*d)/N/t)*(1000*1000*3600）/(1000*PI_DIM_MUL))  (km/h)	 
//                                                    //<==>  ((n*pi*d)*(36/2))/t
//    //
//            DistInSec =  (200*DistInSec*(36/2));
//            
//            //DistInSec /= 10;
//            //DistInSec =  (DistInSec*(36/2));
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
//    //				l_addspeed2 = TimeCnt/PluseCnt - lstaddspeed;		
//    //			}
//    //			lstaddspeed = 	TimeCnt/PluseCnt;
//    //		}
//                speed /= 10;
//                l_speed2 = speed;
//        }	
//    }
//    
//
////	printfcom0("\r\n \r\n Speed2 %d \r\n ",speed);
//	//printfcom2("\r\n \r\n Speed2 %d, %d, %d \r\n ",speed,sSpeedPluse.PluseCnt,sSpeedPluse.TimeCnt);
//}


