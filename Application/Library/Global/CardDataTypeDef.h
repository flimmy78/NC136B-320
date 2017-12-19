#ifndef  _CardDataTypeDef_h_
#define  _CardDataTypeDef_h_
#include <stdint.h>
#include <idDataTypeDef.h>

//时间----------------------------------------
typedef		struct	_stcTime_
{
	uint8_t	Sec;
	uint8_t	Min;
	uint8_t	Hour;
	uint8_t	Date;
	uint8_t	Month;
	uint8_t	Year;
	uint16_t	CrcCheck;
}stcTime;

typedef struct _stcCardID_						//IC 卡ID号	  32
{
	uint8_t	ProductNum[12];						//IC卡编号
	uint8_t	DriverNum[4];						//司机代码
	uint8_t	DriverUnit[8];						//司机所在单位编号
	uint8_t	Reserve0[4];						//预留0
	uint16_t	Reserve1;	 						//预留1
	uint16_t	CrcCheck;							//CRC检验
}stcCardID;

typedef struct _stcCardType_					//IC 卡类型 8
{
	uint16_t	CardType[3];						//IC卡类型
	uint16_t	CrcCheck;							//CRC检验
}stcCardType;

typedef struct _stcDensity_						//IC 卡密度 24
{
	uint8_t		LocoType[2];					//机车型号
	uint8_t		LocoNum[2];						//机车编号
	//uint8_t		DriverNum[4];					//司机编号
	uint32_t		DenChageTimes;					//密度变化号
	uint32_t		Density;						//加油密度
	uint32_t		AddOilStationNum;				//加油机编号
	uint32_t		AddOilerNum;					//加油人员编号	
	uint16_t		AddVol;							//加油油量
	uint16_t		CrcCheck;						//CRC检验
}stcDensity;

typedef struct _stcModPot_						//IC 卡cd参数
{
	uint16_t	HigVal;								//压力值
	uint16_t	DipVal;								//油量值
}stcModPot;

typedef struct _stcModel_						//油箱模型复杂数据结构
{
	stcModPot	sModPot[50];					//计算模型
	uint16_t		PotNum;							//计算模型点的个数	
	uint16_t		StartHight;						//起始高度
	uint8_t		ModelNum;						//模型编号
	uint8_t		Reserve0;						//预留    //100919   0x01表示使用，0x00表示未用
	uint16_t		CrcCheck;						//CRC校验
}stcModel;

typedef struct _stcModelSemple_					//油箱模型简化数据结构
{
	stcModPot	sModPot[10];					//计算模型
	uint16_t		PotNum;							//计算模型点的个数	
	uint16_t		StartHight;						//起始高度
	uint8_t		ModelNum;						//模型编号
	uint8_t		Reserve0;						//预留	//100919	0x01表示使用，0x00表示未用	
	uint16_t		CrcCheck;						//CRC校验
}stcModelSemple;

typedef struct _stcModelComplex_				//油箱模型简化数据结构
{
	stcModPot	    sModPot[100];					//计算模型
	uint16_t		PotNum;							//计算模型点的个数	
	uint16_t		StartHight;						//起始高度
	uint8_t		    ModelNum;						//模型编号
	uint8_t		    Reserve0;						//预留
	uint16_t		CrcCheck;						//CRC校验
}stcModelComplex;


typedef struct _stcModelCard_					//油箱模型复杂数据结构
{
	stcModel		sModel[18];					//复杂油箱模型
	stcModelSemple	sModelSemple[46];			//简单油箱模型

//	stcModel		sModel[18];					//复杂油箱模型
//	stcModelSemple	sModelSemple[18];			//简单油箱模型
//	stcModelComplex	sModelComplex;	   			
	uint16_t			Reserve0;					//预留
	uint16_t			CrcCheck;					//CRC校验	
}stcModelCard;


typedef struct _stcReviseHight_					//IC 修正高度
{
	int16_t		ReviseHight;					//修正高度起始高度
	uint16_t		CrcCheck;						//CRC校验
}stcReviseHight;

typedef struct _stcAddDipVal_					//IC 油量增加
{
	int16_t		AddDipVal;						//油量增加
	uint16_t		CrcCheck;						//CRC校验
}stcAddDipVal;

typedef struct _stcDebugCard_					//
{
	uint16_t		DebugCode;						//调试代码
	uint16_t		BufLen;							//有效数据长度
	uint8_t		Buf[100];						//数据区
	uint16_t		Reserve0;						//预留
	uint16_t		CrcCheck;						//CRC校验
}stcDebugCard;

//计算模型卡参数
typedef struct _stcCardPara_ 					//IC 卡cd参数 352
{
	stcDensity		sDensity;					//密度
	uint32_t			SetVal;						//设置值
	stcTime			sTime;						//时间
 	stcReviseHight	sReviseHight;				//修正起始高度
	stcFixInfo      sFixInfo;              	 	//装车卡
	stcDebugCard	sDebugCard;					//调试卡
	uint16_t			Reserve0;					//预留
	uint16_t			CrcCheck;					//CRC检验
}stcCardPara;

typedef struct _stcCardIndex_					//IC 卡文件索引 36
{
	uint8_t			LocoType[2];				//机车型号
	uint8_t			LocoNum[2];					//机车编号
	uint8_t			ProNum[16];					//产品编号
	uint32_t			StartAddr;					//开始地址
	uint32_t			EndAddr;					//结束地址
	uint16_t			RecLen;						//记录长度
	uint16_t			RecNum;						//记录条数	
}stcCardIndex;

typedef struct _stcCardFlg_						//IC 卡标识
{
	stcCardID		sCardID;					//IC卡 ID结构
	stcCardType		sCardType;					//IC卡 IC卡类型（数据卡，密度卡，）
	stcCardPara		sCardPara;					//IC卡 卡参数
	stcCardIndex	sCardIndex[100];			//IC卡 文件索引
	uint16_t			WriteCardTimes;				//已写卡次数
	uint16_t			CrcCheck;					//CRC检验
}stcCardFlg;


#endif
