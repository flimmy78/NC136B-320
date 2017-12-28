/*******************************************************************************
 *   Filename:      bsp_FRAM.c
 *   Revised:       $Date: 2017-05-15$
 *   Revision:      $
 *   Writer:	    redmorningcn.
 *
 *   Description:
 *   Notes:
 *     				E-mail:redmorningcn@qq.com
 *
 *   All copyrights reserved to redmorningcn.
 *
 *******************************************************************************/

/*******************************************************************************
* INCLUDES
*/
#include <includes.h>
#include "bsp_FRAM.h"
#include "App_ctrl.h"
					  
/***********************************************
* 描述: OS接口
*/
#if UCOS_EN     == DEF_ENABLED
#if OS_VERSION > 30000U
static  OS_SEM			Bsp_FramSem;    	//信号量
#else
static  OS_EVENT		*Bsp_FramSem;       //信号量
#endif
#endif


#if (UCOS_EN     == DEF_ENABLED)
/*******************************************************************************
* 名    称： 		FRAM_WaitEvent
* 功    能： 		等待信号量
* 入口参数： 	无
* 出口参数：  	0（操作有误），1（操作成功）
* 作    者： 	redmorningcn
* 创建日期： 	2017-05-15
* 修    改：
* 修改日期：
* 备    注： 仅在使用UCOS操作系统时使用
*******************************************************************************/
static uint8_t FRAM_WaitEvent(void)
{
    /***********************************************
    * 描述： OS接口
    */
    //return TRUE;
    return BSP_OS_SemWait(&Bsp_FramSem,0);           	// 等待信号量
}

/*******************************************************************************
* 名    称：	 	FRAM_SendEvent
* 功    能： 		释放信号量
* 入口参数： 	无
* 出口参数： 	无
* 作    者： 	redmorningcn
* 创建日期： 	2017-05-15
* 修    改：
* 修改日期：
* 备    注： 仅在使用UCOS操作系统时使用
*******************************************************************************/
static void FRAM_SendEvent(void)
{
    BSP_OS_SemPost(&Bsp_FramSem);                        	// 发送信号量
}

/*******************************************************************************
* 名    称： FRAM_SendEvent
* 功    能： 释放信号量
* 入口参数： 无
* 出口参数： 无
* 作    者： redmorningcn
* 创建日期： 2017-05-15
* 修    改：
* 修改日期：
* 备    注： 仅在使用UCOS操作系统时使用
*******************************************************************************/
void BSP_FramOsInit(void)
{
    /***********************************************
    * 描述： OS接口
    */
#if (UCOS_EN     == DEF_ENABLED)
#if OS_VERSION > 30000U
    BSP_OS_SemCreate(&Bsp_FramSem,1, "Bsp EepSem");      // 创建信号量
#else
    Bsp_FramSem     = OSSemCreate(1);                    // 创建信号量
#endif
#endif
}
#endif
//-------------------------------------------------------------------------------------------------------
//函数名称: 		WriteFM24CL16()
//功    能:			对FM24CL16指定地址进行写数据
//入口参数:         ADDR:       操作地址    地址：0 ~ sizeof(FM24CL16)
//                  		*DataBuf:   数据缓冲
//                  Len: 	数据长度
//作　 　者： 	redmorningcn
//创建日期： 	2017-05-15
//出口参数:    	无
//说明：            
//--------------------------------------------------------------------------------------------------------
uint8 WriteFM24CL64(uint16  Addr, uint8 *DataBuf, uint32 DataBufLen)            
{
    uint32  i = 0;
    uint8   SlaveAddr;                                      //从机地址
    uint16  AddrTemp = 0;                                   //地址缓存
    
    AddrTemp = Addr;
    AddrTemp += FRAM_START_ADDR;                            //计算地址

    if( (uint16)AddrTemp + DataBufLen > FRAM_END_ADDR ) 
    {
        return  FALSE;                                      			//地址未超阶
    }
        
    SlaveAddr = FRAM_DIVICE_ADDR & (~(0x01));               //求FM24CL64设备写地址
    
    StartI2C();                                             		//启动IIC总线
    
    WriteByteWithI2C(SlaveAddr);                            //写FM24CL64地址
    CheckSlaveAckI2C();
 
    WriteByteWithI2C(Addr >> 8);                            //写FM24CL64高位地址
    CheckSlaveAckI2C();
    
    WriteByteWithI2C( Addr );                               //写FM24CL64低位地址
    CheckSlaveAckI2C();     
  
    for(i = 0; i < DataBufLen; i++)                         //发送数据
    {
        WriteByteWithI2C(DataBuf[i]);                       //写数据
        CheckSlaveAckI2C();                                 //读从机应答信号
    }
    
    StopI2C();                                              //停止IIC操作   
    
    return  TRUE;                                           //正确
}

//-------------------------------------------------------------------------------------------------------
//函数名称:         ReadFM24CL16()
//功    能:         读取FM24CL16指定地址进行数据
//入口参数:         ADDR:       操作地址   地址：0 ~ sizeof(FM24CL16)
//                  *DataBuf:   数据缓冲 
//                  Len:        数据长度
//出口参数:         无
//作　 　者： 	redmorningcn
//创建日期： 	2017-05-15
//说明：            
//--------------------------------------------------------------------------------------------------------
uint8 ReadFM24CL64(uint16  Addr, uint8 *DataBuf, uint32 DataBufLen)            
{
    uint32  i = 0;
    uint8   SlaveAddr;                                      //从机地址
    uint16  AddrTemp;                                       //地址缓存
    
    AddrTemp = Addr;
    
    AddrTemp += FRAM_START_ADDR;                            //计算地址
    
    if( (uint16)AddrTemp + DataBufLen > FRAM_END_ADDR ) 
    {
        return  FALSE;                                      //地址未超阶
    }
    
    SlaveAddr = FRAM_DIVICE_ADDR & (~(0x01));               //求FM24CL64设备写地址
    
    StartI2C();                                             //启动IIC总线
    
    WriteByteWithI2C(SlaveAddr);                            //写FM24CL64设备写地址
    CheckSlaveAckI2C();                                     //读从机应答信号
    
    WriteByteWithI2C(Addr >> 8 );                           //写数据高位地址
    CheckSlaveAckI2C();                                     //读从机应答信号
    
    WriteByteWithI2C( Addr );                               //写数据低位地址
    CheckSlaveAckI2C();                                     //读从机应答信号
        
    SlaveAddr = (FRAM_DIVICE_ADDR)|(0x01);                  //求FM24CL64设备读地址
    
    StartI2C();                                             //启动IIC总线
    
    WriteByteWithI2C(SlaveAddr);                            //写FM24CL16地址
    CheckSlaveAckI2C();                                     //读从机应答信号
   
    for(i = 0; i < (DataBufLen -1); i++)                    //发送数据
    {
        DataBuf[i] = ReadByteWithI2C();                     //写数据
        MasterAckI2C();                                     //主机应答从机
    }
    DataBuf[i] = ReadByteWithI2C();                         //写数据
    
    MasterNoAckI2C();                                       //主机应答从机
    
    StopI2C();
    
    return  TRUE;                                           //正确
}


//  RecNumMgr
//
//-------------------------------------------------------------------------------------------------------
//函数名称	:       FRAM_StoreRecNumMgr()
//功    		能	:       存储数据记录管理地址
//入口参数	:       待存储的数据记录指针
//出口参数  :       成功TURE，失败FALSE
//作　 　者： 	redmorningcn
//创建日期： 	2017-05-15
//说		 明：            
//--------------------------------------------------------------------------------------------------------
uint8 FRAM_StoreRecNumMgr(StrRecNumMgr  *SRecNumMgr)
{
	uint8	flg = TRUE;
	
	FRAM_WaitEvent();
	if(!WriteFM24CL64(FRAM_RECNUM_MGR_ADDR, (uint8 *)SRecNumMgr, sizeof(StrRecNumMgr)))		//存数据管理
		flg = FALSE;
	
	FRAM_SendEvent();
	return	flg;
}


//-------------------------------------------------------------------------------------------------------
//函数名称	:      FRAM_ReadRecNumMgr()
//功    		能	:      读数据记录管理指令
//入口参数	:      待读取的数据记录的储存位置
//出口参数  :      成功TURE，失败FALSE
//作　 　者 ： 	redmorningcn
//创建日期  ： 	2017-05-15
//说		 明：  
//--------------------------------------------------------------------------------------------------------
uint8 FRAM_ReadRecNumMgr(StrRecNumMgr  *SRecNumMgr)
{
	uint8	flg = TRUE;
	
	FRAM_WaitEvent();
	if(!ReadFM24CL64(FRAM_RECNUM_MGR_ADDR, (uint8 *)SRecNumMgr, sizeof(StrRecNumMgr)))		//读数据管理
		flg = FALSE;
	
	FRAM_SendEvent();
	return	flg;
}

//  ProductInfo
//
//-------------------------------------------------------------------------------------------------------
//函数名称:       FRAM_StoreProductInfo()
//功    能:       存产品信息
//入口参数:      
//出口参数:       
//作　 　者： 	    redmorningcn
//创建日期： 	    2017-05-15
//说		 明：            
//--------------------------------------------------------------------------------------------------------
uint8 FRAM_StoreProductInfo(StrProductInfo  *sProductInfo)
{
	uint8	flg = TRUE;
	
	FRAM_WaitEvent();
	if(!WriteFM24CL64(FRAM_PRODUCT_INFO_ADDR, (uint8 *)sProductInfo, sizeof(StrProductInfo)))		
		flg = FALSE;
	
	FRAM_SendEvent();
	return	flg;
}

//-------------------------------------------------------------------------------------------------------
//函数名称	:      FRAM_ReadProductInfo()
//功    		能	:      读产品信息
//入口参数	:      产品的
//出口参数  :       
//作　 　者 ： 	redmorningcn
//创建日期  ： 	2017-05-15
//说		 明：  
//--------------------------------------------------------------------------------------------------------
uint8 FRAM_ReadProductInfo(StrProductInfo  *sProductInfo)
{
	uint8	flg = TRUE;
	
	FRAM_WaitEvent();
	if(!ReadFM24CL64(FRAM_PRODUCT_INFO_ADDR, (uint8 *)sProductInfo, sizeof(StrProductInfo)))		
		flg = FALSE;
	
	FRAM_SendEvent();
	return	flg;
}



//  currecord
//
//-------------------------------------------------------------------------------------------------------
//函数名称	:      FRAM_StoreCurRecord()
//功    		能	:      存当前记录的 
//入口参数	:      
//出口参数  :       
//作　 　者 ： 	redmorningcn
//创建日期  ： 	2017-05-15
//说		 明：            
//--------------------------------------------------------------------------------------------------------
uint8 FRAM_StoreCurRecord(stcFlshRec  *sFlshRec)
{
	uint8	flg = TRUE;
	
	FRAM_WaitEvent();
	if(!WriteFM24CL64(FRAM_REC_ADDR, (uint8 *)sFlshRec, sizeof(stcFlshRec)))		
		flg = FALSE;
	
	FRAM_SendEvent();
	return	flg;
}

//-------------------------------------------------------------------------------------------------------
//函数名称	:      FRAM_ReadProductInfo()
//功    		能	:      读当前记录的 
//入口参数	:      产品的
//出口参数  :       
//作　 　者 ： 	redmorningcn
//创建日期  ： 	2017-05-15
//说		 明：  
//--------------------------------------------------------------------------------------------------------
uint8 FRAM_ReadCurRecord(stcFlshRec  *sFlshRec)
{
	uint8	flg = TRUE;
	
	FRAM_WaitEvent();
	if(!ReadFM24CL64(FRAM_REC_ADDR, (uint8 *)sFlshRec, sizeof(stcFlshRec)))		
		flg = FALSE;
	
	FRAM_SendEvent();
	return	flg;
}

//  oilpara
//
//-------------------------------------------------------------------------------------------------------
//函数名称	:      FRAM_StoreOilPara()
//功    		能	:      
//入口参数	:      
//出口参数  :       
//作　 　者 ： 	redmorningcn
//创建日期  ： 	2017-05-15
//说		 明：            
//--------------------------------------------------------------------------------------------------------
uint8 FRAM_StoreOilPara(StrOilPara  *SOilPara)
{
	uint8	flg = TRUE;
	
	FRAM_WaitEvent();
	if(!WriteFM24CL64(FRAM_OIL_PARA_ADDR, (uint8 *)SOilPara, sizeof(StrOilPara)))		
		flg = FALSE;
	
	FRAM_SendEvent();
	return	flg;
}

//-------------------------------------------------------------------------------------------------------
//函数名称	:      FRAM_ReadOilPara()
//功    		能	:      
//入口参数	:     
//出口参数  :       
//作　 　者 ： 	redmorningcn
//创建日期  ： 	2017-05-15
//说		 明：  
//--------------------------------------------------------------------------------------------------------
uint8 FRAM_ReadOilPara(StrOilPara  *SOilPara)
{
	uint8	flg = TRUE;
	
	FRAM_WaitEvent();
	if(!ReadFM24CL64(FRAM_OIL_PARA_ADDR, (uint8 *)SOilPara, sizeof(StrOilPara)))		
		flg = FALSE;
	
	FRAM_SendEvent();
	return	flg;
}


//  runpara
//
//-------------------------------------------------------------------------------------------------------
//函数名称	:      FRAM_StoreRunPara()
//功    		能	:      
//入口参数	:      
//出口参数  :       
//作　 　者 ： 	redmorningcn
//创建日期  ： 	2017-05-15
//说		 明：            
//--------------------------------------------------------------------------------------------------------
uint8 FRAM_StoreRunPara(stcRunPara  *sRunPara)
{
	uint8	flg = TRUE;
	
	FRAM_WaitEvent();
	
	if(!WriteFM24CL64(FRAM_RUN_PARA_ADDR, (uint8 *)sRunPara, sizeof(stcRunPara)))		
		flg = FALSE;
	
	FRAM_SendEvent();
	return	flg;
}

//-------------------------------------------------------------------------------------------------------
//函数名称	:      FRAM_ReadRunPara()
//功    		能	:      
//入口参数	:     
//出口参数  :       
//作　 　者 ： 	redmorningcn
//创建日期  ： 	2017-05-15
//说		 明：  
//--------------------------------------------------------------------------------------------------------
uint8 FRAM_ReadRunPara(stcRunPara  *sRunPara)
{
	uint8	flg = TRUE;
	
	FRAM_WaitEvent();
	
	if(!ReadFM24CL64(FRAM_RUN_PARA_ADDR, (uint8 *)sRunPara, sizeof(stcRunPara)))		
		flg = FALSE;
	
	FRAM_SendEvent();
	return	flg;
}

//  calcmodel
//
//-------------------------------------------------------------------------------------------------------
//函数名称	:      FRAM_StoreCalcModel()
//功    		能	:      
//入口参数	:      
//出口参数  :       
//作　 　者 ： 	redmorningcn
//创建日期  ： 	2017-05-15
//说		 明：            
//--------------------------------------------------------------------------------------------------------
uint8 FRAM_StoreCalcModel(stcCalcModel  *sCalcModel)
{
	uint8	flg = TRUE;
	
	FRAM_WaitEvent();
	
	if(!WriteFM24CL64(FRAM_CALC_MODEL_ADDR, (uint8 *)sCalcModel, sizeof(stcCalcModel)))		
		flg = FALSE;
	
	FRAM_SendEvent();
	return	flg;
}

//-------------------------------------------------------------------------------------------------------
//函数名称	:      FRAM_ReadRunPara()
//功    		能	:      
//入口参数	:     
//出口参数  :       
//作　 　者 ： 	redmorningcn
//创建日期  ： 	2017-05-15
//说		 明：  
//--------------------------------------------------------------------------------------------------------
uint8 FRAM_ReadCalcModel(stcCalcModel  *sCalcModel)
{
	uint8	flg = TRUE;
	
	FRAM_WaitEvent();
	
	if(!ReadFM24CL64(FRAM_CALC_MODEL_ADDR, (uint8 *)sCalcModel, sizeof(stcCalcModel)))		
		flg = FALSE;
	
	FRAM_SendEvent();
	return	flg;
}

//-------------------------------------------------------------------------------------------------------
//函数名称	:      GetOilBoxModelsModel()
//功         能	:  查找邮箱模型区域，寻找有效的油箱模型。 
//入口参数	:     
//出口参数  :       
//作　 　者 ： 	redmorningcn
//创建日期  ： 	2017-05-15
//说		 明：  
//--------------------------------------------------------------------------------------------------------
uint8	GetOilBoxModelsModel(stcModel * sModel)
{
	  uint32	i;
	  uint32	ModelNumAdd;
	  uint32	ModelAdd;
	  uint8		ModelNum;
	  uint16	CrcCheck;
	  uint8		OilBoxTypCod;

	  OilBoxTypCod	= GetOilBoxCod(); 

	  for(i = 0; i < 18;i++)
	  {
	      ModelNumAdd =  OIL_BOX_MODEL_ADDR + sizeof(stcModel)*i + sizeof(stcModPot)*50 + 2 + 2;

		  ReadFM24CL64(ModelNumAdd,(uint8 *)&ModelNum, sizeof(ModelNum));

//		  printfcom0("\r\n 11 OilBoxTypCod == ModelNum %d ,i = %d",ModelNum,i);

		  if(OilBoxTypCod == ModelNum)
		  {
//		  	  printfcom0("\r\n 2OilBoxTypCod == ModelNum %d,i = %d",ModelNum,i);
			  
			  ModelAdd = OIL_BOX_MODEL_ADDR + sizeof(stcModel)*i;		  		
		      ReadFM24CL64(ModelAdd,(uint8 *)sModel,sizeof(stcModel));
//			  printfcom0("\r\n ModelAdd == ModelNum %d,i = %d",ModelAdd);
//			  for(i = 0;i < sModel->PotNum;i++)
//			  {
//			  	printfcom0("\r\n  gaodu %d,youliang %d",sModel->sModPot[i].HigVal,sModel->sModPot[i].DipVal);
//			  }

			  CrcCheck = GetCrc16Check((uint8 *)sModel,sizeof(stcModel) - 2);

			  if(CrcCheck == sModel->CrcCheck)
			  {
			  	 //printfcom0("\r\n 3PotNum %d",sModel->PotNum);	
			  	 return  1;
			  }
		  }
	  }

	  return  0;
}

//-------------------------------------------------------------------------------------------------------
//函数名称	:      GetOilBoxModelsModelSemple()
//功    		能	:     查找邮箱模型区域，寻找有效的油箱模型。  
//入口参数	:     
//出口参数  :       
//作　 　者 ： 	redmorningcn
//创建日期  ： 	2017-05-15
//说		 明：  
//--------------------------------------------------------------------------------------------------------
uint8	GetOilBoxModelsModelSemple(stcModelSemple * sModelSemple)
{
	  uint32	i;
	  uint32	ModelNumAdd;
	  uint32	ModelAdd;
	  uint8		ModelNum;
	  uint16	CrcCheck;
	  uint8		OilBoxTypCod;

	  OilBoxTypCod	= GetOilBoxCod(); 

	  for(i = 0; i < 46;i++)
	  {
		  ModelNumAdd =  OIL_BOX_MODEL_ADDR + sizeof(stcModel)*18 
		  				+ sizeof(stcModelSemple)*i + sizeof(stcModPot)*10 + 2 + 2;

		  ReadFM24CL64(ModelNumAdd,(uint8 *)&ModelNum, sizeof(ModelNum));

		  //printfcom0("\r\n 22 OilBoxTypCod == ModelNum %d, %d",ModelNum,OilBoxTypCod);

		  if(OilBoxTypCod == ModelNum)
		  {
			  ModelAdd = OIL_BOX_MODEL_ADDR + sizeof(stcModel)*18  
			  			+ sizeof(stcModelSemple)*i ;
		
		      ReadFM24CL64(ModelAdd,(uint8 *)sModelSemple,sizeof(stcModelSemple));

			  CrcCheck = GetCrc16Check((uint8 *)sModelSemple,sizeof(stcModelSemple) - 2);

//			  printfcom0("\r\n 22 CrcCheck == CrcCheck %d, %d",CrcCheck,sModelSemple->CrcCheck);
			  if(CrcCheck == sModelSemple->CrcCheck)
			  {
			  	 return  1;
			  }
		  }
	  }

	  return  0;
}

//#endif
    
