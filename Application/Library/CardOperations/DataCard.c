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

#define		CARD_BUF_LEN		1024			//IC卡处理缓冲区大小	
uint8 FRAM_StoreRecNumMgr(StrRecNumMgr  *SRecNumMgr);

extern  stcSysCtrl  sCtrl;


//----------------------------------------------------------------------
//函数名称:  	uint32_t  GetWritePageLen(uint32_t Start, uint32_t WriteLen,uint32_t MaxWriteLen)
//函数功能:  	求写将要完成下页写长度
//入口参数:   	StartAddr:IC卡起始写地址;FinishLen:已写地址,MaxWriteLen:要写地址
//出口参数:   	
//说明:                 
//-----------------------------------------------------------------------
uint32_t  GetWritePageLen(uint32_t StartAddr, uint32_t FinishLen,uint32_t MaxWriteLen)
{
    uint32_t  	PageSize;
    uint32_t  	PageSegLen;
    uint32_t  	NoWriteLen;
	
	PageSize = GetCardPageSize();
	
    PageSegLen = PageSize - ((StartAddr + FinishLen) % PageSize);		//从首往尾写
    
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
//函数名称:  	stcFlshRec    FlshRecToCardRec( stcFlshRec   sFlshRec)
//函数功能:  	将油尺记录FLASH转化为IC卡记录
//入口参数:   	FLASH记录
//出口参数:   	IC卡记录
//说明:                 
//-----------------------------------------------------------------------
stcFlshRec    FlshRecToCardRec( stcFlshRec   sFlshRec)
{
	stcFlshRec  sCardRec;		
	memcpy((uint8_t *)&sCardRec,(uint8_t *)&sFlshRec,sizeof(sCardRec));
	
	return	sCardRec;
}

//extern	void	SysHoldTast(void);

//----------------------------------------------------------------------
//函数名称:  	void    ReadCardDisplay(uint8_t	 Sct)
//函数功能:  	将油尺记录FLASH转化为IC卡记录
//入口参数:   	FLASH记录
//出口参数:   	IC卡记录
//说明:                 
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
//函数名称:  	uint8_t  StoreCardRec(uint32_t   FlshStartRecNum,uint32_t	FlshRecCnt,uint32_t	CardStartAddr )
//函数功能:  	存IC卡记录
//入口参数:   	FlshStartRecNum:起始Flsh记录号,FlshRecCnt:记录个数,StartCardRecAddr:IC卡起始地址
//出口参数:   	
//说明:                 
//-----------------------------------------------------------------------
uint8_t  StoreCardRec(uint32_t   FlshStartRecNum,uint32_t	FlshRecCnt,uint32_t	CardStartAddr,uint32_t	WriteTimes )
{
	uint8_t				CardBuf[1024];
	uint32_t			FlshRecNum;											//Flsh记录流水号
	uint32_t			MaxWriteLen;										//最大要写卡长度
	uint32_t			FinishLen = 0;										//已写长度
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
			NextRecCardAddr = CardStartAddr + FinishLen;				//IC卡下一地址
            
   			WriteCard(NextRecCardAddr,CardBuf,WritePageLen);      		//写卡
     		
     		FinishLen += WritePageLen;									//将要完成写长度
            
     		ReadCardDisplay(WriteTimes);								//指示正在读卡
            
			BufWriteLen = 0;
		}
		if(GetPlugFlg() == NO_PLUG_IN_CARD)								//拨卡退出
		{
			return FALSE;
		}
	}
    
	return TRUE;
}

//-------------------------------------------------------------------------------------------------
//函数名称:             uint32_t	GetReadFlshRecCnt(uint32_t ZoneMaxRecNum)	   
//函数功能:             取分区应读取的记录数
//入口参数:             分区最大记录数
//出口参数:             记录数
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

	//计算地址
	CardAddrTmp = 	CARD_FLG_ADDR+
                    sizeof(stcCardID)+
                    sizeof(stcCardType)+
                    sizeof(stcCardPara)+
					sizeof(stcCardIndex)*100;
	
	//读取已有IC卡文件数
	ReadCard(CardAddrTmp,(uint8 *)&WriteCardTimes,sizeof(WriteCardTimes));	
    
	//计算数据文件开始地址。
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
    
	//计算写卡索引地址
	CardAddrTmp = 	CARD_FLG_ADDR+
                    sizeof(stcCardID)+
                    sizeof(stcCardType)+
                    sizeof(stcCardPara)+
					sizeof(stcCardIndex)*WriteCardTimes;
    
	ReadCard(CardAddrTmp,(uint8 *)&sCardIndex,sizeof(stcCardIndex));
    
    
	//计算记录条数
	if(sCtrl.sRecNumMgr.IcRead >= sCtrl.sRecNumMgr.CardRead)	
	{
		NoReadRecNum	= 		sCtrl.sRecNumMgr.IcRead -
                             	sCtrl.sRecNumMgr.CardRead;  //Flsh的未读记录长度
        
		if(copyflg == 1)
			NoReadRecNum = sCtrl.sRecNumMgr.IcRead;			//copy卡
        
        if(NoReadRecNum < 2)                                //没有新数据记录
        {
            sCtrl.sRunPara.CardType     = DATA_CARD_ERR;    //指示卡错误
            sCtrl.sRunPara.CardErrData  = 0x04;             //模块无新数据记录
            return;
        }
	}
	else
	{
		sCtrl.sRecNumMgr.CardRead = sCtrl.sRecNumMgr.IcRead;	//序号异常，重新幅值
		NoReadRecNum = 0;
        
        sCtrl.sRunPara.CardType     = DATA_CARD_ERR;    //指示卡错误
        sCtrl.sRunPara.CardErrData  = 0x02;             //记录序号错误。数据重新存储
		return;
	}
    
	//取IC卡未写空间长度
	CardMaxRecNum	= GetToEndLen(CardStartAddr) / sizeof(stcFlshRec);	
    
    //IC卡可写数据长度小于5条，则范围IC卡已满标识，退出卡操作
    if(CardMaxRecNum < 5){
        sCtrl.sRunPara.CardType     = DATA_CARD_ERR;    //指示卡错误
        sCtrl.sRunPara.CardErrData  = 0x03;             //IC卡已满 
        return;
    }
	//要写数据长度，根据空检大小，剩余记录号，IC卡余量计算。	
	FlshRecCnt 		= GetReadFlshRecCnt(NoReadRecNum,CardMaxRecNum);	
    
	//计算开始记录号
	FlshStartRecNum = sCtrl.sRecNumMgr.IcRead  - FlshRecCnt ;
    storeCardRead   = sCtrl.sRecNumMgr.IcRead;
    /********************************************
    *	开始写数据
    */
	uint32      MaxWriteLen;							//最大要写卡长度
	uint32      FinishLen = 0;							//已写长度
	uint32      WritePageLen;	
	uint32      NextRecCardAddr;
	uint32      BufWriteLen = 0;
    
	MaxWriteLen = FlshRecCnt * sizeof(stcFlshRec);				//总数据长度			
    
	BufWriteLen = 0;
    int writepagetimes = 0;
	while( FinishLen < MaxWriteLen)								//写完退出
	{
		ReadFlshRec((stcFlshRec *)&sCtrl.sRec,FlshStartRecNum++);//从flash中读出数据
        
        if(sCtrl.sRec.StoreCnt == 0xffffffff)
           sCtrl.sRec.StoreCnt  =  FlshStartRecNum;
        
		WritePageLen = GetWritePageLen(	CardStartAddr,			//开始地址
                                        FinishLen,				//已完成长度
                                        MaxWriteLen);			//最大数据长度
        
		memcpy(&CardBuf[BufWriteLen],(uint8_t *)&sCtrl.sRec,sizeof(stcFlshRec));
		BufWriteLen += sizeof(stcFlshRec);
        
		if(WritePageLen == BufWriteLen || WritePageLen < BufWriteLen)
		{
			NextRecCardAddr = CardStartAddr + FinishLen;        //IC卡下一地址
            
   			WriteCard(NextRecCardAddr,CardBuf,WritePageLen);    //写卡
     		
     		FinishLen += WritePageLen;                          //将要完成写长度
            
			BufWriteLen = 0;
            
			sCtrl.sRunPara.CardType = DATA_CARD_DIS;            //IC卡指示
            
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
            //没有ic卡也要退出
            //if(sCtrl.sRunPara.plugcard == 0)                    //
            if(ReadIC_SWT() == 0)
                return;
		}
	}
    
	/********************************************
	*	 IC卡完成，修改索引页
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
        //sCtrl.sRecNumMgr.CardRead   = sCtrl.sRecNumMgr.GrsRead; //已读记录号
        sCtrl.sRecNumMgr.CardRead   = storeCardRead; //已读记录号
        
		FRAM_StoreRecNumMgr((StrRecNumMgr * )&sCtrl.sRecNumMgr);
	}
	
	sCtrl.sRunPara.CardType = DATA_CARD_FIN;					//IC卡结束
} 
uint32  GetRecNumAddr(uint32 FlshRecNum);
uint8 ReadFlsh(uint32 Addr,uint8 *buf,uint32 Len);


////-------------------------------------------------------------------------------
////oˉêy??3?:        	stcFlshRec	ReadFlshRec(uint32	FlshRecNum)
////oˉêy1|?ü:       	?á???¨á÷??o?μ?FLSH????
////è??ú2?êy:        	?T
////3??ú2?êy:      	???¨á÷??o?μ?Flsh????
////-------------------------------------------------------------------------------
uint8	ReadFlshRec(stcFlshRec * sFlshRec,uint32	FlshRecNum)
{
	uint32		FlshAddr;
	
    if(FlshRecNum==0xffffffff)      //记录号异常，退出
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
//函数名称:             uint32_t	GetCardMaxRecNum(void)
//函数功能:             取一张IC卡最大能存最大的数据记录数
//入口参数:             整个卡结构体
//出口参数:             是否成功
//--------------------------------------------------------------------------------------------------
uint32_t	GetCardMaxRecNum(void)
{
	uint32_t	RecNum;
	
	RecNum = (l_CardPageNum*l_CardPageSize - CARD_DATA_ADDR)/ sizeof(stcFlshRec);
	
	return	RecNum;
}
