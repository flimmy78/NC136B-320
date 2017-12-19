#include <includes.h>
#include  "csnr_package_deal.h"


stcCsnrProtocolPara l_sendsprotocolpara;	
stcCsnrProtocolPara l_recsprotocolpara;	

uint8	l_gprsrecflg = 0;
uint8	GetGprsRecFlg(void)
{
	return	l_gprsrecflg;
}

void	SetGprsRecFlg(void)
{
	l_gprsrecflg = 1;
}
void	ClearGprsRecFlg(void)
{
	l_gprsrecflg = 0;
}


void	 GprsRecOperate(stcCsnrProtocolPara   recsprotocolpara)
{
//		printfcom0("\r\n接收内容：\r\n\r\n");
		
//		if(DataUnpack_CSNR((stcCsnrProtocolPara *)&l_recsprotocolpara,RecBuf,RecLen))	//接收数据
		{
				SetGprsRecFlg();
				memcpy((uint8 *)&l_recsprotocolpara,(uint8 *)&recsprotocolpara,sizeof(stcCsnrProtocolPara));	
		}
}

//
//
//void	Gprs_Send_Ctrl(void)
//{
//	static	uint32	time = 0;
//	static	uint32	framnum = 1;
//	uint32	currecnum;
//	uint32	gprsrecnum;
//	uint8		sendbuf[256];
//	uint8		sendlen;
//	uint8		sFlshRecbuf[128];	
//	uint32	i;
//	stcFlshRec	 sFlshRec;
//				
//	l_sendsprotocolpara.sourceaddr = 0x80;
//	l_sendsprotocolpara.destaddr   = 0xca;
//	l_sendsprotocolpara.framcode   = 0x00;
//	l_sendsprotocolpara.framnum    = framnum;
//
//	if(
//            GetSysTime() - time > 100*10 
//        || 	(GetGprsRecFlg()  && (GetSysTime() - time > 100))
//        )	
//	{
//		time = GetSysTime();
//		ClearGprsRecFlg();	
//		
//		currecnum 	= GetCurFlshRecNum();									//当前流水号
//		gprsrecnum 	= GetLstFlshRecNum_gprs();
//		
//		if(gprsrecnum > currecnum)												//出错处理
//		{
//				gprsrecnum = currecnum;
//				StoreLstRedFlshRecNum_gprs(gprsrecnum);
//		}
//	
//		if(gprsrecnum < currecnum)
//		{
//			ReadFlshRec((stcFlshRec *)&sFlshRec,gprsrecnum);
//			//ReadFileData(gprsrecnum*128,(uint8 *)&sFlshRec,128);
//
//			if(l_sendsprotocolpara.framnum == l_recsprotocolpara.framnum) //接收正确
//			{
//				framnum++;
//				gprsrecnum++;
//			}
//			
//			l_sendsprotocolpara.framnum    = framnum;
//			l_sendsprotocolpara.databuf    = (uint8 *)&sFlshRec;
//			l_sendsprotocolpara.datalen    = sizeof(sFlshRec);
//			
//			DataPackage_CSNR(l_sendsprotocolpara,sendbuf,&sendlen);
//			
//			//SendCom0(sendbuf,sendlen);	
//			//地址判断
//			if((l_sendsprotocolpara.destaddr & 0xA0) == 0xA0)
//			{
//				SendCOM1(sendbuf,sendlen) ;
//			}
//			else if((l_sendsprotocolpara.destaddr & 0xC0) == 0xC0)
//			{
////				if(	GetPrintfFlg()== 0)
////				{
////					SendCOM2(sendbuf,sendlen) ; 
////				}
//				SendCOM3(sendbuf,sendlen) ; 	
//			}
//			
//			printfcom0("\r\n\r\n GPRS发送\r\n\r\n");
//            
////			printftextbox("\r\n发送数据：",framnum); 
////			for(i=0;i<sendlen;i++)
////				printftextbox("%02x ",sendbuf[i]);	
////			
//			StoreLstRedFlshRecNum_gprs(gprsrecnum);
////			printftextbox("\r\n数据发送：记录号：%d",framnum);  
//		}
//	}
//}
//
//
//void	Gprs_Send_liveinfo(void)
//{
//	static	uint32	time = 0;
//	uint8		buf[8];
//	uint16	locotype,loconum;
//	uint8		len;
//	uint8		sendbuf[256];
//	uint8		sendlen;	
//	stcCsnrProtocolPara sendsprotocolpara;	
//
//	if(GetSysTime() - time > 100*60)
//	{
//		time = GetSysTime();
//		
//		locotype = 	104;
//		loconum = 	234;
//	
////		printftextbox("\r\n生命信号：");  
//		
//		sendsprotocolpara.sourceaddr = 0x80;
//		sendsprotocolpara.destaddr   = 0xca;
//		sendsprotocolpara.framcode   = 0x02;
//		sendsprotocolpara.framnum    = 0;	
//	
//		len = 0;
//		memcpy(&buf[len],(uint8 *)&locotype,sizeof(locotype));
//		len += sizeof(locotype);
//		memcpy(&buf[len],(uint8 *)&loconum,sizeof(loconum));
//		len += sizeof(loconum);
//		
//		sendsprotocolpara.databuf    = buf;
//		sendsprotocolpara.datalen    = sizeof(buf);	
//		
//		DataPackage_CSNR(sendsprotocolpara,sendbuf,&sendlen);
//		
////		SendCom0(sendbuf,sendlen);	
//		if((sendsprotocolpara.destaddr & 0xA0) == 0xA0)
//		{
//			SendCOM1(sendbuf,sendlen) ;
//		}
//		else if((sendsprotocolpara.destaddr & 0xC0) == 0xC0)
//		{
//			if(	GetPrintfFlg()== 0)
//			{
//				SendCOM2(sendbuf,sendlen) ; 
//			}
//			SendCOM3(sendbuf,sendlen) ; 	
//		}
//	}
//}

//void  GprsDataComDeal(void)
//{
////	Gprs_Rec_Ctrl();
//	Gprs_Send_Ctrl();
////	Gprs_Send_liveinfo();
//}
