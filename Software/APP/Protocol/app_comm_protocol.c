/*******************************************************************************
*   Filename:       app_lib.c
*   Revised:        All copyrights reserved to Wuming Shen.
*   Date:           2014-07-05
*   Revision:       v1.0
*   Writer:	     Wuming Shen.
*
*   Description:    双击选中 wdt 按 Ctrl + H, 钩选 Match the case, Replace with
*                   输入您要的名字，点击 Replace All
*                   双击选中 Wdt 按 Ctrl + H, 钩选 Match the case, Replace with
*                   输入您要的名字，点击 Replace All
*                   双击选中 WDT 按 Ctrl + H, 钩选 Match the case, Replace with
*                   输入您要的名字，点击 Replace All
*                   在 app_cfg.h 中指定任务的优先级 和 任务堆栈大小
*
*   Notes:
*					QQ:276193028
*     				E-mail:shenchangwei945@163.com
*
*******************************************************************************/

/*******************************************************************************
* INCLUDES
*/
#define  SNL_APP_SOURCE
#include <app.h>
#include <CrcCheck.h>
#include <BSP_OS.h>

//#include <bsp_usart.h>

#define BSP_COMM_PROTOCOL_MODULE_EN 1
#if BSP_COMM_PROTOCOL_MODULE_EN > 0

#ifdef VSC_INCLUDE_SOURCE_FILE_NAMES
const  CPU_CHAR  *app_comm_protocol__c = "$Id: $";
#endif

/*******************************************************************************
* CONSTANTS
*/
#define         FRAM_HERD0     		0x10         	//报文头
#define         FRAM_HERD1     		0x28       		//报文头				
#define     	HOST_ID      		10				//上位机地址	  		   
#define   		DEVICE_ID0      	15				//本机地址0	
#define   		DEVICE_ID1      	15				//本机地址1	

#define			RS485_CHK_SUM		0x02			//b0001：CRC方式；b0010：累加和方式； b0011;累加和二进制补码方式 
#define			RS485_CHK_CRC		0x01			//b0001：CRC方式；b0010：累加和方式； b0011;累加和二进制补码方式 
#define			RS485_CHK_RESUM		0x03			//b0001：CRC方式；b0010：累加和方式； b0011;累加和二进制补码方式 

#define         FRAM_END0     		0x10         	//报文尾
#define         FRAM_END1     		0x2C       		//报文尾	

/*******************************************************************************
* MACROS
*/

/*******************************************************************************
* TYPEDEFS
*/

/*******************************************************************************
* LOCAL VARIABLES
*/

/*******************************************************************************
* GLOBAL VARIABLES
*/

/*******************************************************************************
* LOCAL FUNCTIONS
*/

/*******************************************************************************
* GLOBAL FUNCTIONS
*/

/*******************************************************************************
* EXTERN VARIABLES
*/

/*******************************************************************************
* EXTERN FUNCTIONS
*/

static unsigned char   l_recslaveaddr = 0;
static unsigned char   l_RecvFrameNbr = 0;
/***********************************************
* 描述： 
*/
unsigned char GetRecSlaveAddr(void)
{
	return	l_recslaveaddr;
}
/***********************************************
* 描述：接收序号 
*/
unsigned char GetRecvFrameNbr(void)
{
	return	l_RecvFrameNbr;
}

//static uint8	RecvFrameNbr = 0;
///***********************************************
//* 描述： 
//*/
//uint8	GetRecvFrameNbr(void)
//{
//	return 	RecvFrameNbr;
//}

static  OS_SEM			Bsp_Fram_Sem;    	//信号量

void    OS_ProtocolInit(void)
{
    BSP_OS_SemCreate(&Bsp_Fram_Sem, 1, "Protocol Sem");
}


/***********************************************
* 描述： 
*/
void	SetRecvFrameNbr(void)
{
	l_RecvFrameNbr++;		
}

/*******************************************************************************
* 名    称： GetCheckSumNR
* 功    能： 
* 入口参数： 无
* 出口参数： 无
* 作　 　者： wumingshen.
* 创建日期： 2015-12-07
* 修    改：
* 修改日期：
* 备    注：
*******************************************************************************/
static uint8 GetCheckSumNR(uint8  Buf[], uint32 Len)
{
    int32  i;
    int8   CheckSum = 0;
    
    for(i = 0;i < Len; i++) {
        CheckSum += Buf[i];                         //求和
    }
    
    if((unsigned char)CheckSum >= 0x80) {
    	CheckSum = ~CheckSum + 1 +0x80;             //取反+1
    } else {
    	CheckSum = CheckSum;                       	//取反+1
    }
    return  CheckSum;
}

/*******************************************************************************
* 名    称： CSNC_GetData
* 功    能： 
* 入口参数： 无
* 出口参数： 无
* 作　 　者： wumingshen.
* 创建日期： 2015-12-07
* 修    改：
* 修改日期：
* 备    注：
*******************************************************************************/
unsigned char  CSNC_GetData(	INT08U *RecBuf,
                            INT16U	RecLen,
                            INT08U *Datapos,
                            INT16U *InfoLen)
{
    unsigned short		i,j,k;
    unsigned char		    SumCheck8;
    unsigned char		    Tmp;
    unsigned short		RecCRC16,Check16;
    
	//unsigned char 		DstAddr;	
    unsigned char 		SourceAddr;
	unsigned short		DataLen;  
    unsigned char         *pDat         = RecBuf;
    
    unsigned short        MaxDataLen    = RecLen - 10;
    
    if(RecLen < 10)         //长度小于最小长度，直接退出。
        return 0;   
    
    *InfoLen            = 0;
    //StrCommRecvPack *Rd = (StrCommRecvPack *)DataBuf;
    //DataBuf = DataBuf;
    /***********************************************
    * 描述： 识别帧头
    */
    for(i = 0; i < MaxDataLen -1; i++)	{		        //循环遍历 (2017 redmorningcn循环中有加1的，运算值需减1)
		if(	( (RecBuf[i + 0] ) == FRAM_HERD0 ) && 
           ( (RecBuf[i + 1] ) == FRAM_HERD1 ) ) {
               pDat    = &RecBuf[i];
               goto next;
           }
    }
    return 0;
next:
    /***********************************************
    * 描述： 接收数据去掉帧头识别字0X10的重复部分
    */
	for(j = 0;j < RecLen - 1 ;j++ )	{				    //去重复通讯首字节   (2017 redmorningcn循环中有加1的，运算值需减1)
        if( ( pDat[j]   == FRAM_HERD0 ) &&
           ( pDat[j+1] == FRAM_HERD0 ) ) {
               for(k = j; k < RecLen; k++) {
                   Tmp      = pDat[k+1];
                   pDat[k]  = Tmp;
               }
           }
	}
    pDat    += 6;
    DataLen  = *pDat;
    // 长度超范围
    if ( DataLen > MaxDataLen )
        return 0;
    
    pDat    += DataLen + 4;
    /***********************************************
    * 描述： 检查帧尾
    */
    if( ( pDat[0] 	!= FRAM_END0)	||
       ( pDat[1] 	!= FRAM_END1) ) {
           return 0;
       }
    
    pDat            = &RecBuf[i];
    l_recslaveaddr 	= pDat[3];
    
    //DstAddr         = l_recslaveaddr;
    SourceAddr      = pDat[2];
    
    l_RecvFrameNbr  = pDat[4];
    *InfoLen 		= DataLen; 
    
    SumCheck8 		= GetCheckSumNR((unsigned char *)&pDat[2],5);	//索引区校验
    
    RecCRC16 		= ((unsigned short)pDat[8+DataLen])
        * 256 + pDat[9+DataLen];	        //校验	
    
    switch(pDat[5]>>4) {							    //校验和算法判断
        case	RS485_CHK_SUM:
            break;
            case	RS485_CHK_CRC:
                Check16 = GetCrc16Check(&pDat[2],6+DataLen);;
                break;
                case	RS485_CHK_RESUM:
                    break;
    }
    
    if(	(SumCheck8 == 	pDat[7] ) && 
       (RecCRC16  ==	Check16 ) ) {
           /***********************************************
           * 描述： 获取帧头
           */
           //memcpy( (INT08U *)&Rd.Head, (INT08U *)&pDat[0], 8 );
           
           /***********************************************
           * 描述： 获取数据
           */       
           //memcpy( (INT08U *)DataBuf,  (INT08U *)&pDat[8], DataLen);					//数据拷贝
           
           
           //DataBuf     = (INT08U *)&pDat[8];
           *Datapos   = 8;
           //return		DstAddr;//SourceAddr;
           return  SourceAddr;
       }
    
    return	0;
}

static uint8	SendFrameNbr = 0;

/***********************************************
* 描述： 
*/
uint8	GetSendFrameNbr(void)
{
	return SendFrameNbr;		
}

/***********************************************
* 描述： 
*/
void	SetSendFrameNbr(void)
{
	SendFrameNbr++;		
}

static  unsigned char	SndBuf[250] = {0};
static  unsigned char	SndBufTmp[250] = {0};
/*******************************************************************************
* 名    称： CSNC_SendData
* 功    能： 
* 入口参数： 无
* 出口参数： 无
* 作  　者： wumingshen.
* 创建日期： 2015-12-07
* 修    改：
* 修改日期：
* 备    注：
*******************************************************************************/
void	CSNC_SendData(	MODBUS_CH *pch,
                      INT08U  SourceAddr,
                      INT08U  DistAddr,
                      INT08U  SendFrameNum,
                      INT08U  SendCode,                       
                      INT08U *DataBuf,
                      INT16U  DataLen)
{
	unsigned char	    SumCheck8;
	unsigned short	Crc16;
	unsigned short	AddHeadNum;
	unsigned short	i;
	unsigned char	    ByteStation;	
    
    BSP_OS_SemWait(&Bsp_Fram_Sem,0);
    
	ByteStation = 0;
	SndBuf[ByteStation++] = FRAM_HERD0;
	SndBuf[ByteStation++] = FRAM_HERD1;
	SndBuf[ByteStation++] = SourceAddr;	 
	SndBuf[ByteStation++] = DistAddr;				    // 地址
	SndBuf[ByteStation++] = SendFrameNum;               // 发送帧序号
    
	SndBuf[ByteStation++] = RS485_CHK_CRC<<4 + SendCode;// 发送控制字	
	SndBuf[ByteStation++] = DataLen;
	
	SumCheck8 = GetCheckSumNR(&SndBuf[2],5);		   	// 索引区校验
    
	SndBuf[ByteStation++] = SumCheck8;
    
	memcpy(&SndBuf[8],DataBuf,DataLen);
    
	Crc16 = GetCrc16Check(&SndBuf[2],6+DataLen);
    
	SndBuf[2+6+DataLen+1] = Crc16;
	SndBuf[2+6+DataLen]   = Crc16>>8;
    
	SndBufTmp[0] = SndBuf[0];						    // 准备数据发送
	SndBufTmp[1] = SndBuf[1];
	
	AddHeadNum = 0;
	for(i = 2; i< 2+6+DataLen + 2;i++ )	{				// 数据，补移位 FRAM_HERD0
        
		SndBufTmp[i+AddHeadNum] = SndBuf[i];
		
		if(SndBuf[i] == FRAM_HERD0) {
			SndBufTmp[i+AddHeadNum+1] = FRAM_HERD0;
			AddHeadNum++;
		}
	}
    
	SndBufTmp[2+6+DataLen + 2 +AddHeadNum] = FRAM_END0;
	SndBufTmp[2+6+DataLen + 2 +AddHeadNum+1] = FRAM_END1;      
    
    CPU_INT16U nbr_bytes= (2+6+DataLen + 2 +AddHeadNum +2);
    if ( nbr_bytes < MB_DATA_NBR_REGS * 2 ) {  
        //串口在非接收状态，发送数据。
        //        if (pch->RxBufByteCtr == 0) 
        uint8   times = 0;
         while(pch->RxBufByteCtr != 0)
         {
            BSP_OS_TimeDly(5);
            times++;
             if(pch->RxBufByteCtr == 0 || times > 10){
                 break;
             }
         }
             
        {                
            NMB_Tx(pch,
                   (CPU_INT08U  *)SndBufTmp,
                   (CPU_INT16U   )nbr_bytes);
        }
    }
    //BSP_OS_TimeDlyMs( nbr_bytes * 1000 * 10 / pch->BaudRate + 2 );
    BSP_OS_SemPost(&Bsp_Fram_Sem);                        	// 发送信号量
    
}

/*******************************************************************************
* 				                    end of file                                *
*******************************************************************************/
#endif