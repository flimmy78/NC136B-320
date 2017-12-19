//#include 	"config.h"
//#include 	"String.h"
//#include	"Globe.h"
//#include	"HardDrive.h"
//#include	"StoreRec.h"
//#include	"CpuDrive.h"
//#include	"FramStore.h"
//#include	"OperateCard.h"
//#include	"stdlib.h" 

#include    "includes.h"  
#include	"Card.h"
#include	"OperateCard.h"
#include	"app_ctrl.h"

//#include	"flash.h"

#define		CARD_BUF_LEN		1024			//IC������������С	
uint8 FRAM_StoreRecNumMgr(StrRecNumMgr  *SRecNumMgr);

extern  stcSysCtrl  sCtrl;


//----------------------------------------------------------------------
//��������:  	uint32_t  GetWritePageLen(uint32_t Start, uint32_t WriteLen,uint32_t MaxWriteLen)
//��������:  	��д��Ҫ�����ҳд����
//��ڲ���:   	StartAddr:IC����ʼд��ַ;FinishLen:��д��ַ,MaxWriteLen:Ҫд��ַ
//���ڲ���:   	
//˵��:                 
//-----------------------------------------------------------------------
uint32_t  GetWritePageLen(uint32_t StartAddr, uint32_t FinishLen,uint32_t MaxWriteLen)
{
    uint32_t  	PageSize;
    uint32_t  	PageSegLen;
    uint32_t  	NoWriteLen;
	
	PageSize = GetCardPageSize();
	
    PageSegLen = PageSize - ((StartAddr + FinishLen) % PageSize);		//������βд
    
    NoWriteLen = MaxWriteLen - FinishLen;
    
    if( PageSegLen < NoWriteLen)
    {
        return PageSegLen;
    }
    else
    {
        return  NoWriteLen;
    }
}

//----------------------------------------------------------------------
//��������:  	stcFlshRec    FlshRecToCardRec( stcFlshRec   sFlshRec)
//��������:  	���ͳ߼�¼FLASHת��ΪIC����¼
//��ڲ���:   	FLASH��¼
//���ڲ���:   	IC����¼
//˵��:                 
//-----------------------------------------------------------------------
stcFlshRec    FlshRecToCardRec( stcFlshRec   sFlshRec)
{
	stcFlshRec  sCardRec;		
	memcpy((uint8_t *)&sCardRec,(uint8_t *)&sFlshRec,sizeof(sCardRec));
	
	return	sCardRec;
}

//extern	void	SysHoldTast(void);

//----------------------------------------------------------------------
//��������:  	void    ReadCardDisplay(uint8_t	 Sct)
//��������:  	���ͳ߼�¼FLASHת��ΪIC����¼
//��ڲ���:   	FLASH��¼
//���ڲ���:   	IC����¼
//˵��:                 
//-----------------------------------------------------------------------
void    ReadCardDisplay(uint8_t	 Sct)
{
	uint8_t	i = 0;
	
    //	if(GetSysTime() % 100 < 12)
	{
		i = 0;
		while(i < 10 )
		{
			DisplaySet(10,2,"%u  %u",Sct,i);
			i++;
		}
		
		//OnLed(WRITE_CARD_LED,10,5,5);
	}													//xitong 
}

//----------------------------------------------------------------------
//��������:  	uint8_t  StoreCardRec(uint32_t   FlshStartRecNum,uint32_t	FlshRecCnt,uint32_t	CardStartAddr )
//��������:  	��IC����¼
//��ڲ���:   	FlshStartRecNum:��ʼFlsh��¼��,FlshRecCnt:��¼����,StartCardRecAddr:IC����ʼ��ַ
//���ڲ���:   	
//˵��:                 
//-----------------------------------------------------------------------
uint8_t  StoreCardRec(uint32_t   FlshStartRecNum,uint32_t	FlshRecCnt,uint32_t	CardStartAddr,uint32_t	WriteTimes )
{
	uint8_t				CardBuf[1024];
	uint32_t			FlshRecNum;											//Flsh��¼��ˮ��
	uint32_t			MaxWriteLen;										//���Ҫд������
	uint32_t			FinishLen = 0;										//��д����
	uint32_t			WritePageLen;	
	uint32_t			NextRecCardAddr;
	uint32_t			BufWriteLen = 0;
	
	stcFlshRec		sFlshRec;
	
	FlshRecNum	= FlshStartRecNum;
	MaxWriteLen = FlshRecCnt * sizeof(sFlshRec);					
    
	BufWriteLen = 0;
	while( FinishLen < MaxWriteLen)
	{
		ReadFlshRec((stcFlshRec *)&sFlshRec,FlshRecNum++);
        
		WritePageLen = GetWritePageLen(CardStartAddr,FinishLen,MaxWriteLen);
        
		memcpy(&CardBuf[BufWriteLen],(uint8_t *)&sFlshRec,sizeof(sFlshRec));
		BufWriteLen += sizeof(sFlshRec);
        
		if(WritePageLen == BufWriteLen || WritePageLen < BufWriteLen)
		{
			NextRecCardAddr = CardStartAddr + FinishLen;				//IC����һ��ַ
            
   			WriteCard(NextRecCardAddr,CardBuf,WritePageLen);      		//д��
     		
     		FinishLen += WritePageLen;									//��Ҫ���д����
            
     		ReadCardDisplay(WriteTimes);								//ָʾ���ڶ���
            
			BufWriteLen = 0;
		}
		if(GetPlugFlg() == NO_PLUG_IN_CARD)								//�����˳�
		{
			return FALSE;
		}
	}
    
	return TRUE;
}

//-------------------------------------------------------------------------------------------------
//��������:             uint32_t	GetReadFlshRecCnt(uint32_t ZoneMaxRecNum)	   
//��������:             ȡ����Ӧ��ȡ�ļ�¼��
//��ڲ���:             ��������¼��
//���ڲ���:             ��¼��
//--------------------------------------------------------------------------------------------------
uint32_t	GetReadFlshRecCnt(uint32_t NoReadRecNum,uint32_t ZoneMaxRecNum)
{	
	uint32_t	FlshMaxRec;
	
	FlshMaxRec = (FLSH_MAX_SIZE)/(sizeof(stcFlshRec));
	
	////printfcom0("\r\n FlshMaxRec %d",FlshMaxRec);
	if(NoReadRecNum >= ZoneMaxRecNum)
	{
		if(ZoneMaxRecNum > FlshMaxRec)
		{
			return	FlshMaxRec;
		}
		
		return	ZoneMaxRecNum;
	}
	else
	{
		if(NoReadRecNum > FlshMaxRec)
		{
			return	FlshMaxRec;
		}
        
		return	NoReadRecNum;
	}
}

uint8_t	l_UnFihCardEvtFlg = NO_ERROR;
//uint8_t	l_WriteFlgErrCardEvtFlg = NO_ERROR;

void	DataDensityCard(stcCardFlg *	sCardFlg);

stcDeviceInfo 	sDeviceInfo;
stcFixInfo		sFixInfo;						
stcCardIndex	sCardIndex;
uint8			CardBuf[1024];

void	DataCard(uint8_t copyflg)
{
	uint32		FlshRecCnt;	
	uint32		FlshStartRecNum;
//	uint32		CardEndAddr;
	uint32		CardStartAddr;
	uint32		CardMaxRecNum;
	uint32		NoReadRecNum;
	uint16		WriteCardTimes;
	uint32 		CardAddrTmp;
    uint32      storeCardRead;

	//�����ַ
	CardAddrTmp = 	CARD_FLG_ADDR+
                    sizeof(stcCardID)+
                    sizeof(stcCardType)+
                    sizeof(stcCardPara)+
					sizeof(stcCardIndex)*100;
	
	//��ȡ����IC���ļ���
	ReadCard(CardAddrTmp,(uint8 *)&WriteCardTimes,sizeof(WriteCardTimes));	
    
	//���������ļ���ʼ��ַ��
	if(WriteCardTimes == 0 || WriteCardTimes >= 100 )
	{
		CardStartAddr = CARD_DATA_ADDR;
		if(WriteCardTimes > 100)
			WriteCardTimes = 0;
	}
	else
	{
		CardAddrTmp = 	CARD_FLG_ADDR+
                        sizeof(stcCardID)+
                        sizeof(stcCardType)+
                        sizeof(stcCardPara)+
						sizeof(stcCardIndex)*(WriteCardTimes-1);
		
		ReadCard(CardAddrTmp,(uint8 *)&sCardIndex,sizeof(stcCardIndex));
        
		CardStartAddr 	= sCardIndex.EndAddr;
		
		if(CardStartAddr < CARD_DATA_ADDR)
		{
			CardStartAddr = CARD_DATA_ADDR;
		}
	}
    
	//����д��������ַ
	CardAddrTmp = 	CARD_FLG_ADDR+
                    sizeof(stcCardID)+
                    sizeof(stcCardType)+
                    sizeof(stcCardPara)+
					sizeof(stcCardIndex)*WriteCardTimes;
    
	ReadCard(CardAddrTmp,(uint8 *)&sCardIndex,sizeof(stcCardIndex));
    
    
	//�����¼����
	if(sCtrl.sRecNumMgr.IcRead >= sCtrl.sRecNumMgr.CardRead)	
	{
		NoReadRecNum	= 		sCtrl.sRecNumMgr.IcRead -
                             	sCtrl.sRecNumMgr.CardRead;  //Flsh��δ����¼����
        
		if(copyflg == 1)
			NoReadRecNum = sCtrl.sRecNumMgr.IcRead;			//copy��
        
        if(NoReadRecNum < 2)                                //û�������ݼ�¼
        {
            sCtrl.sRunPara.CardType     = DATA_CARD_ERR;    //ָʾ������
            sCtrl.sRunPara.CardErrData  = 0x04;             //ģ���������ݼ�¼
            return;
        }
	}
	else
	{
		sCtrl.sRecNumMgr.CardRead = sCtrl.sRecNumMgr.IcRead;	//����쳣�����·�ֵ
		NoReadRecNum = 0;
        
        sCtrl.sRunPara.CardType     = DATA_CARD_ERR;    //ָʾ������
        sCtrl.sRunPara.CardErrData  = 0x02;             //��¼��Ŵ����������´洢
		return;
	}
    
	//ȡIC��δд�ռ䳤��
	CardMaxRecNum	= GetToEndLen(CardStartAddr) / sizeof(stcFlshRec);	
    
    //IC����д���ݳ���С��5������ΧIC��������ʶ���˳�������
    if(CardMaxRecNum < 5){
        sCtrl.sRunPara.CardType     = DATA_CARD_ERR;    //ָʾ������
        sCtrl.sRunPara.CardErrData  = 0x03;             //IC������ 
        return;
    }
	//Ҫд���ݳ��ȣ����ݿռ��С��ʣ���¼�ţ�IC���������㡣	
	FlshRecCnt 		= GetReadFlshRecCnt(NoReadRecNum,CardMaxRecNum);	
    
	//���㿪ʼ��¼��
	FlshStartRecNum = sCtrl.sRecNumMgr.IcRead  - FlshRecCnt ;
    storeCardRead   = sCtrl.sRecNumMgr.IcRead;
    /********************************************
    *	��ʼд����
    */
	uint32      MaxWriteLen;							//���Ҫд������
	uint32      FinishLen = 0;							//��д����
	uint32      WritePageLen;	
	uint32      NextRecCardAddr;
	uint32      BufWriteLen = 0;
    
	MaxWriteLen = FlshRecCnt * sizeof(stcFlshRec);				//�����ݳ���			
    
	BufWriteLen = 0;
    int writepagetimes = 0;
	while( FinishLen < MaxWriteLen)								//д���˳�
	{
		ReadFlshRec((stcFlshRec *)&sCtrl.sRec,FlshStartRecNum++);//��flash�ж�������
        
        if(sCtrl.sRec.StoreCnt == 0xffffffff)
           sCtrl.sRec.StoreCnt  =  FlshStartRecNum;
        
		WritePageLen = GetWritePageLen(	CardStartAddr,			//��ʼ��ַ
                                        FinishLen,				//����ɳ���
                                        MaxWriteLen);			//������ݳ���
        
		memcpy(&CardBuf[BufWriteLen],(uint8_t *)&sCtrl.sRec,sizeof(stcFlshRec));
		BufWriteLen += sizeof(stcFlshRec);
        
		if(WritePageLen == BufWriteLen || WritePageLen < BufWriteLen)
		{
			NextRecCardAddr = CardStartAddr + FinishLen;        //IC����һ��ַ
            
   			WriteCard(NextRecCardAddr,CardBuf,WritePageLen);    //д��
     		
     		FinishLen += WritePageLen;                          //��Ҫ���д����
            
			BufWriteLen = 0;
            
			sCtrl.sRunPara.CardType = DATA_CARD_DIS;            //IC��ָʾ
            
            writepagetimes++;
            if(writepagetimes %20 == 0){
            //
                OS_ERR  os_err;
                BSP_LED_Toggle(7);
                BSP_LED_Toggle(8);

                OSFlagPost( ( OS_FLAG_GRP  *)&sCtrl.Os.CommEvtFlagGrp,
                           ( OS_FLAGS      )COMM_EVT_FLAG_PLUG_CARD,
                           ( OS_OPT        )OS_OPT_POST_FLAG_SET,
                           ( OS_ERR       *)&os_err);

            }
            //û��ic��ҲҪ�˳�
            //if(sCtrl.sRunPara.plugcard == 0)                    //
            if(ReadIC_SWT() == 0)
                return;
		}
	}
    
	/********************************************
	*	 IC����ɣ��޸�����ҳ
	*/
    //sCardIndex.LocoNum      = sCtrl.sRec.LocoNum;
    memcpy((uint8 *)&sCardIndex.LocoNum,(uint8 *)&sCtrl.sRec.LocoNum,sizeof(sCardIndex.LocoNum));
    //sCardIndex.LocoType     = sCtrl.sRec.LocoTyp;
    memcpy((uint8 *)&sCardIndex.LocoType,(uint8 *)&sCtrl.sRec.LocoTyp,sizeof(sCtrl.sRec.LocoTyp));

	sCardIndex.StartAddr	    = CardStartAddr;
	sCardIndex.EndAddr 		    = CardStartAddr + MaxWriteLen;
	sCardIndex.RecLen		    = MaxWriteLen;
	sCardIndex.RecNum		    = MaxWriteLen/(sizeof(stcFlshRec));
	WriteCardTimes++;									
        
	CardAddrTmp =	CARD_FLG_ADDR+
                    sizeof(stcCardID)+
                    sizeof(stcCardType)+
                    sizeof(stcCardPara)+
					sizeof(stcCardIndex)*100; 
	
	WriteCard(CardAddrTmp,(uint8 *)&WriteCardTimes,sizeof(WriteCardTimes));	
    
	CardAddrTmp = 	CARD_FLG_ADDR+
                    sizeof(stcCardID)+
                    sizeof(stcCardType)+
                    sizeof(stcCardPara)+
					sizeof(stcCardIndex)*(WriteCardTimes-1);
	
	WriteCard(CardAddrTmp,(uint8_t *)&sCardIndex,sizeof(stcCardIndex));
    
	if(copyflg == 0)
	{
        //sCtrl.sRecNumMgr.CardRead   = sCtrl.sRecNumMgr.GrsRead; //�Ѷ���¼��
        sCtrl.sRecNumMgr.CardRead   = storeCardRead; //�Ѷ���¼��
        
		FRAM_StoreRecNumMgr((StrRecNumMgr * )&sCtrl.sRecNumMgr);
	}
	
	sCtrl.sRunPara.CardType = DATA_CARD_FIN;					//IC������
} 
uint32  GetRecNumAddr(uint32 FlshRecNum);
uint8 ReadFlsh(uint32 Addr,uint8 *buf,uint32 Len);


////-------------------------------------------------------------------------------
////o����y??3?:        	stcFlshRec	ReadFlshRec(uint32	FlshRecNum)
////o����y1|?��:       	?��???������??o?��?FLSH????
////��??��2?��y:        	?T
////3??��2?��y:      	???������??o?��?Flsh????
////-------------------------------------------------------------------------------
uint8	ReadFlshRec(stcFlshRec * sFlshRec,uint32	FlshRecNum)
{
	uint32		FlshAddr;
	
    if(FlshRecNum==0xffffffff)      //��¼���쳣���˳�
        return 0;
    
	FlshAddr = GetRecNumAddr( FlshRecNum );						
    
	if(ReadFlsh(FlshAddr,(uint8 *)sFlshRec,sizeof(stcFlshRec)))
	{
		return 1;
	}
	
	return	0;
}


extern  stcCardFlg	l_sCardFlg;
extern	uint16	l_CardType;

extern	uint32_t		l_CardPageSize;	
extern	uint32_t		l_CardPageNum;
//-------------------------------------------------------------------------------------------------
//��������:             uint32_t	GetCardMaxRecNum(void)
//��������:             ȡһ��IC������ܴ��������ݼ�¼��
//��ڲ���:             �������ṹ��
//���ڲ���:             �Ƿ�ɹ�
//--------------------------------------------------------------------------------------------------
uint32_t	GetCardMaxRecNum(void)
{
	uint32_t	RecNum;
	
	RecNum = (l_CardPageNum*l_CardPageSize - CARD_DATA_ADDR)/ sizeof(stcFlshRec);
	
	return	RecNum;
}
