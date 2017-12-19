#include <includes.h>
#include <FRAM.h>
#include <osal.h>
#include <bsp_os.h>

//ÿȦ���峣��
#define		PPR		200
//pi = 3.14 * 100
#define		PI		314
//�ֶ�ֱ�� 1.1m * 10
//#define		DIM		11
#define		DIM		10.5
//��̳��� PPR *10*100*1000  
#define		PI_DIM_MUL		(10*100)

#if (UCOS_EN == DEF_ENABLED ) && ( OS_VERSION > 30000U )
extern  OS_SEM           time0_over_Sem;               // �ź���
#endif


uint32	mPluseSumCnt0 = 0;
//-------------------------------------------------------------------------------
//��������:         GetFrqCnt()
//��������:         ȡƵ��ֵ
//��ڲ���:         ��		 
//���ڲ���:         
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

		if(m_AddPluse0 == MATH_PLUSE_NUM || m_Time0Sum > MATH_PLUSE_TIME) 		//500�������1��ʱ�䵽
		{
//			m_sSpeed1Pluse.TimeCnt 	= 	m_Time0Sum;
//			m_sSpeed1Pluse.PluseCnt	=   m_AddPluse0;
//				
			m_AddPluse0 = 0;
			m_Time0Sum = 0;
			m_Pluse0ChgFlg = 1;
            
            #if (OS_VERSION > 30000U)
            BSP_OS_SemPost(&time0_over_Sem);                        	// �����ź���
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
//	if(m_Pluse0ChgFlg == 1)							//�����ۼӴ���,
//	{
//		m_Pluse0ChgFlg = 0;
//	}
//
//}
//
////���㹫ʽ��V = ��PI * DIM * n * 3600��/ (( DPR *1000*10*100 )*(t/12000000);
////------------------------------------------------------------------------------
////��    �ܣ�    ��������̣��ֶ���̣��Զ������
////��ڲ�����    
////���ڲ�����    
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
//    err = BSP_OS_SemWait(&time0_over_Sem,10);                        	// �����ź���
//    if(!err)
//    {
//            // �����ź���,�ٶ�������Ҫ3�����һ�Σ�������׼ȷ
//        if(times++ > 5)
//            l_speed1 = 0;
//    }
//        
//    if(err){
//        times = 0;
//#endif
//        
//        if( TimeCnt > 2     && 
//            m_Pluse0ChgFlg      )                   //����������
//        {
//            m_Pluse0ChgFlg = 0;
//            
//            DistInSec = PluseCnt*PI*DIM;			//��λʱ�������о���  ��δ��ϵ����	 
//                                                    // V = s/t <==> ((n/N)*100pi*10d)/t  (m/us)    ��PI��DIM�ֱ�Ŵ�100��10�� PI_DIM_MUL = 100*100 ��
//                                                    //<==> ((n*pi*d)/N/t)*(1000*1000*3600��/(1000*PI_DIM_MUL))  (km/h)	 
//                                                    //<==>  ((n*pi*d)*(36/2))/t
//
//            DistInSec =  (DistInSec*(36/2));
//            
//            if((DistInSec % TimeCnt) > TimeCnt/2 )   //4��5��
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
////��    �ܣ�    ��������̣��ֶ���̣��Զ������
////��ڲ�����    
////���ڲ�����    
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
//	PluseCnt   = plussumcnt - LsFrqSumCnt;					//��������������
////	printfcom0("\r\n Dist Pplussumcnt is %d�� %d",plussumcnt,LsFrqSumCnt);
//
//	LsFrqSumCnt = plussumcnt;
//
//	ReadDist((stcDist *)&g_sDist);
//	if(PluseCnt > 0)											//������Ž��м���
//	{
////		printfcom0("\r\n11 Dist %d, ModDPR %d",g_sDist.A_sDist.Dist,g_sDist.A_sDist.ModDPR);
//
//		DistInSec = PluseCnt*PI*DIM;							//��λʱ�������о��� D = ((n/N)*pi*d0/PI_DIM_MUL 	��PI��DIM�ֱ�Ŵ�100��10�� PI_DIM_MUL = 100*100 ��
//																// <==> (n*pi*d)/(N *PI_DIM_MUL)	(m)
//		 														// <==>	(n*pi*d)/(N *PI_DIM_MUL)/1000(Km)
//		divtmp = 200*10*100;									//N*PI_DIM_MUL*1000
////		printfcom0("\r\n DistInSec %d,",DistInSec);
//		moddprtmp = g_sDist.A_sDist.ModDPR;
////		printfcom0("\r\n ModDPR %d",g_sDist.A_sDist.ModDPR);
//		disttmp = g_sDist.A_sDist.Dist;
//		A_DistTmp = moddprtmp + DistInSec;
////		printfcom0("\r\n A_DistTmp %d,moddprtmp %d,DistInSec %d",A_DistTmp,moddprtmp,DistInSec);
//		//A_DistTmp = g_sDist.A_sDist.ModDPR + DistInSec; 		//���й��� = �ϴ�����+�˴���������δ��ϵ��DPR��
//		
//		//����̼���
//		//if(A_DistTmp > DPR)									//�������������
//	
//		if(A_DistTmp > divtmp*1000)								//�������������
//		{
//			//g_sDist.A_sDist.Dist	+= (A_DistTmp / DPR);		//����ۼ�
//			disttmp	+= (((A_DistTmp / (divtmp)))/1000);	//����ۼ�
////			printfcom0("\r\n\r\n\r\n A_DistTmp > divtmp %d,%d,%d,%d,disttmp %d",A_DistTmp > divtmp,A_DistTmp,divtmp*1000,((A_DistTmp / (divtmp)))/1000,disttmp);
//		}
//
//		moddprtmp  	=  A_DistTmp % (divtmp*1000);		//ȡ����
//
//	    g_sDist.A_sDist.ModDPR =moddprtmp ;
//		g_sDist.A_sDist.Dist =disttmp;
//		
//		l_kilemeter = g_sDist.A_sDist.Dist;
////	   	printfcom0("\r\n22 Dist %d, ModDPR %d",g_sDist.A_sDist.Dist,g_sDist.A_sDist.ModDPR,A_DistTmp);		
//		StoresDist((stcDist *)&g_sDist);											//���������
//	}
//}
//
//
////------------------------------------------------------------------------------
////��    �ܣ�    ��������̣��ֶ���̣��Զ������
////��ڲ�����    
////���ڲ�����    
////------------------------------------------------------------------------------
//void  SpeedAndDistTast( void )
//{
//
//		CalcSpeed1();                       //�����ٶ�
//		CalcSpeed2();                       //�����ٶ�
//		CalcSpeedcs();                      //�����ٶ�
//        
//		//CalcDist();                         //�������
//        CheckLocoStaTask();                 //��������
//
//}