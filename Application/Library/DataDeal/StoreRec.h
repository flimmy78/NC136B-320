#ifndef  _StoreData_h_
#define  _StoreData_h_

//-------------------------------------------------------------------------------
//函数名称:        void  StoreRecTask(void)
//函数功能:        存数据记录任务
//入口参数:        无
//出口参数:         
//函数调用:			
//-------------------------------------------------------------------------------
extern	void	StoreRecTask(void);

//-------------------------------------------------------------------------------
//函数名称:        	stcFlshRec	ReadFlshRec(uint32	FlshRecNum)
//函数功能:       	读指定流水号的FLSH记录
//入口参数:        	无
//出口参数:      	指定流水号的Flsh记录
//-------------------------------------------------------------------------------
extern	uint8 	ReadFlshRec(stcFlshRec	* sFlshRec,uint32	FlshRecNum);

//-------------------------------------------------------------------------------------------------
//函数名称:             void  ShowFlshRec(stcFlshRec sDip) 
//函数功能:             将记录转变为比较容易读的格式
//入口参数:             无
//出口参数:             无
//说明:                 
//--------------------------------------------------------------------------------------------------
extern	void  ShowFlshRec(stcFlshRec sDip) ;

//-------------------------------------------------------------------------------
//函数名称:        	uint8	JudgeFlshEvt(void)
//函数功能:       	判断Flsh事件
//入口参数:        	无
//出口参数:      	无
//-------------------------------------------------------------------------------
extern	uint8	JudgeFlshEvt(void);

//-------------------------------------------------------------------------------------------------
//函数名称:             void  PrintfAllFlshRec(void) 
//函数功能:             打印记录
//入口参数:             无
//出口参数:             无
//说明:                 
//--------------------------------------------------------------------------------------------------
extern	void  PrintfAllFlshRec(void) ;

#endif