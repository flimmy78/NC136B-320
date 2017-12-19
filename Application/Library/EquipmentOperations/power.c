//#include    "config.h"
#include    "string.h"
#include    "CrcCheck.h"
//#include    "CpuDrive.h"

typedef	struct	_stcModBusRead_
{
    uint8	DeviceAddr;
    uint8	WorkCode;
    uint16	RegStart;
    uint16	RegLenth;	
    uint16	Crc16;   	
}stcModBusRead;

typedef	struct	_stcModBusPower_
{
	uint8	Voltage[2];
	uint8	Current[2];
	uint8	Power[2];
	uint8	PosEnergy[4];
	uint8	NegEnergy[4];
//	uint8	AddNBound[2];
//	uint8	ModName[4];
}stcModBusPower;

typedef	struct	_stcPower_
{
	int16	Voltage;
	int16	Current;
	int16	Power;
//	int16	AddNBound;	
	int32	PosEnergy;
	int32	NegEnergy;
//	int32	ModName;
}stcPower;

stcPower	sPower;

#define		MODBUS_HEAD		0x01
#define		MODBUS_READ		0x03
#define		MODBUS_SET		0x10
#define		MODBUS_LEN		0x07

//------------------------------------------------------------------------
// 名称：	void	SendReadPowerCode(void)
// 功能：初始化定时器1，定时时间单位为10ms，并使能中断。
// 入口参数：无
// 出口参数：无
//从设备地址	功能码	起始寄存器地址	寄存器个数	CRC-L	CRC-H
//0x01	0x03	0x00	0x10	0x00	0x07	0x05	0xCD
//------------------------------------------------------------------------
void  SendReadPowerCode(void)
{ 
	static	stcModBusRead		sModBusRead;
    uint16	Crc16;
    
    sModBusRead.DeviceAddr 		= MODBUS_HEAD;
    sModBusRead.WorkCode		= MODBUS_READ;
   	sModBusRead.RegStart		= 0x1000;
   	sModBusRead.RegLenth		= 0x0700;
   	   	
   	Crc16 = GetModBusCrc16Up((uint8 *)&sModBusRead,sizeof(sModBusRead) - 2);
   	
   	sModBusRead.Crc16 			= Crc16; 
   	
   	SendCOM1((uint8 *)&sModBusRead,sizeof(sModBusRead));
}

//------------------------------------------------------------------------
// 名称：	void  ClearPowerVal(void)
// 功能：初始化定时器1，定时时间单位为10ms，并使能中断。
// 入口参数：无
// 出口参数：无
//从设备地址	功能码	起始寄存器地址	寄存器个数	CRC-L	CRC-H
//0x01	0x10	0x00	0xA7	0x00	0x01	0x02	0x00	0x00	0xBF	0x47
//------------------------------------------------------------------------
void  ClearPowerVal(void)
{ 
	static	stcModBusRead	sModBusRead;
    uint16			Crc16;
    
    sModBusRead.DeviceAddr 		= MODBUS_HEAD;
    sModBusRead.WorkCode		= MODBUS_READ;
   	sModBusRead.RegStart		= 0x1000;
   	sModBusRead.RegLenth		= 0x0700;
   	   	
   	Crc16 = GetModBusCrc16Up((uint8 *)&sModBusRead,sizeof(sModBusRead) - 2);
   	
   	sModBusRead.Crc16 			= Crc16; 
   	
   	SendCOM1((uint8 *)&sModBusRead,sizeof(sModBusRead));
}

stcModBusPower	sModBusPower;

//----------------------------------------------------------------------------
// 名    称：   void	ClearsModBusPower(void)
// 功    能：   清sModBusPower
// 入口参数：   无
// 出口参数：   无
//----------------------------------------------------------------------------
void	ClearsModBusPower(void)
{
	int16		i = 0;	
	static	uint8		ClearBuf[sizeof(stcModBusPower)]; 
	
	for(i = 0; i< sizeof(stcModBusPower);i++)
	{
		ClearBuf[i] = 0;
	}
	
	memcpy((uint8 *)&sModBusPower,ClearBuf,sizeof(sModBusPower));
}

//----------------------------------------------------------------------------
// 名    称：   uint8  GetDipData(void)
// 功    能：   取油量数据
// 入口参数：   无
// 出口参数：   无
//----------------------------------------------------------------------------
uint8  GetPowerVal(uint8 *Buf,uint16	Len)
{
	static	uint8		ModBusBuf[128];
	static	uint32		Times = 0;
	int16		i = 0;	
	uint16		RecCheck = 0,Crc16;
	uint16		ModBusLen;

	ModBusLen = 1 + 1 + 1 + MODBUS_LEN*2 + 2;         		//地址+操作码+数据长度字节+数据+校验

	if(Times > 10)											//长时间没有数据，清零
	{
		ClearsModBusPower();
	}
	
	for(i = 0; i < (int16)Len - ModBusLen;i++)
	{
		if(Buf[i] == MODBUS_HEAD && Buf[i+1] == MODBUS_READ 
			&& Buf[i+2] == MODBUS_LEN*2)
		{
			memcpy(ModBusBuf,&Buf[i],ModBusLen);
			
			memcpy((uint8 *)&RecCheck,&Buf[ModBusLen - 2],sizeof(RecCheck));
			
			Crc16 = GetModBusCrc16Up(ModBusBuf,ModBusLen - 2);
			
			if(Crc16 == RecCheck)							//校验正确
			{
				memcpy((uint8 *)&sModBusPower,&ModBusBuf[i+3],sizeof(sModBusPower));
				
				Times = 0;							
				
				return	1;
			}
			else
			{
				Times++;
			}
		}
	}
	
	return 0;
}

//------------------------------------------------------------------------
// 名称：	void  ClearPowerVal(void)
// 功能：初始化定时器1，定时时间单位为10ms，并使能中断。
// 入口参数：无
// 出口参数：无
//从设备地址	功能码	起始寄存器地址	寄存器个数	CRC-L	CRC-H
//0x01	0x10	0x00	0xA7	0x00	0x01	0x02	0x00	0x00	0xBF	0x47
//------------------------------------------------------------------------
void  SetPowerAddAndBound(uint8 Add)
{ 
	static	stcModBusRead	sModBusRead;
    uint16			Crc16;
    static	uint8			Buf[16];
    
    sModBusRead.DeviceAddr 		= Add;
    sModBusRead.WorkCode		= MODBUS_SET;
   	sModBusRead.RegStart		= 0x2000;
   	sModBusRead.RegLenth		= 0x0100;
   	   	
   	memcpy(Buf,(uint8 *)&sModBusRead,sizeof(sModBusRead)-2);
   	
   	Buf[sizeof(sModBusRead)-2] = 0x02;
   	
   	Buf[sizeof(sModBusRead)-1] = 0x01;			//Addr
   	
   	Buf[sizeof(sModBusRead)] = 0x06;			//Bound
   	   	
   	Crc16 = GetModBusCrc16Up(Buf,sizeof(sModBusRead)+1);
   	
   	memcpy(&Buf[sizeof(sModBusRead)+1],(uint8 *)&Crc16,sizeof(Crc16)); 
   	
   	SendCOM1(Buf,sizeof(sModBusRead)+3);
}

//------------------------------------------------------------------------
// 名称：	void	SendReadPowerCode(void)
// 功能：初始化定时器1，定时时间单位为10ms，并使能中断。
// 入口参数：无
// 出口参数：无
//从设备地址	功能码	起始寄存器地址	寄存器个数	CRC-L	CRC-H
//0x01	0x03	0x00	0x10	0x00	0x07	0x05	0xCD
//------------------------------------------------------------------------
void  ScanPowerAddr(void)
{ 
	static	stcModBusRead		sModBusRead;
	uint16	RecBufLen;
	static	uint8	Buf[128];
    uint16	Crc16;
    uint32	i;
    
    for(i = 0; i < 16;i++)
    {
	    sModBusRead.DeviceAddr 		= i;
	    sModBusRead.WorkCode		= MODBUS_READ;
	   	sModBusRead.RegStart		= 0x1000;
	   	sModBusRead.RegLenth		= 0x0700;
	   	   	
   		Crc16 = GetModBusCrc16Up((uint8 *)&sModBusRead,sizeof(sModBusRead) - 2);
   	
   		sModBusRead.Crc16 			= Crc16; 
   		
   		SendCOM1((uint8 *)&sModBusRead,sizeof(sModBusRead));
   		
   		DelayX10ms(1);
   		
		RecBufLen = ReadCOM1(Buf,256);	    
		
		if( RecBufLen )
		{				
			break;
	    }
   	}
   	
   	SetPowerAddAndBound(i);
}

//----------------------------------------------------------------------------
// 名    称：   uint8  GetDipData(void)
// 功    能：   取油量数据
// 入口参数：   无
// 出口参数：   无
//----------------------------------------------------------------------------
uint8  GetModbusPowerVal(void)
{
	static	uint8			Buf[256];									
	static	uint32			Time = 0;
	static	uint32			Times = 0;
	uint32					RecBufLen;					
	
	if(GetSysTime() - Time > 150)						
	{
		if(Times++)
		{
			RecBufLen = ReadCOM1(Buf,256);	    
			
			if( RecBufLen )
			{				
				if(!GetPowerVal(Buf,RecBufLen))						
				{
//					PrintfCOM0("\r\nGetPowerVal(Buf,RecBufLen)ERR!");
				}	
		    }
	    }
	    
	    Time = GetSysTime();
    }
    
    return	0;
}

//------------------------------------------------------------------------
// 名    称：   int16     GetRealData16(int16  Data)
// 功    能：   
// 入口参数：   
//              
// 出口参数：   
//------------------------------------------------------------------------
int16     GetRealData16(uint16  Data)
{
	int16	Tmp;
	
	Tmp = Data;
    if(Data >= 0x8000)
    {
         Tmp = -(Data - 0x8000);
    }

    return  Tmp;
}

//------------------------------------------------------------------------
// 名    称：   int16     GetRealData16(int16  Data)
// 功    能：   
// 入口参数：   
//              
// 出口参数：   
//------------------------------------------------------------------------
int32     GetRealData32(uint32  Data)
{
	int32	Tmp;
	
	Tmp = Data;
    if(Data >= 0x80000000)
    {
         Tmp = -(Data - 0x80000000);
    }

    return  Tmp;
}


//----------------------------------------------------------------------------
// 名    称：   uint8  GetPower(void)
// 功    能：   取油量数据
// 入口参数：   无
// 出口参数：   无
//----------------------------------------------------------------------------
void  MathPower(void)
{
	static	uint32	Time;
	
	if(GetSysTime() - Time > 150)
	{
		sPower.Voltage 	= GetRealData16((uint16)sModBusPower.Voltage[0]*256 + (uint16)sModBusPower.Voltage[1]);
		sPower.Current 	= GetRealData16((uint16)sModBusPower.Current[0]*256 + (uint16)sModBusPower.Current[1]);
		sPower.Power   	= GetRealData16((uint16)sModBusPower.Power[0]*256   + (uint16)sModBusPower.Power[1]);
		sPower.PosEnergy= GetRealData32(sModBusPower.PosEnergy[0]*256*256*256  + sModBusPower.PosEnergy[1]*256*256
						+ sModBusPower.PosEnergy[2]*256   + sModBusPower.PosEnergy[3]);
		sPower.NegEnergy= GetRealData32(sModBusPower.NegEnergy[0]*256*256*256  + sModBusPower.NegEnergy[1]*256*256
						+ sModBusPower.NegEnergy[2]*256   + sModBusPower.NegEnergy[3]);		
						
//		PrintfCOM0("\r\n%f,%f,%f,%f,%f",(float)(((int32)sPower.Voltage * 1000)/10000),(float)((int32)sPower.Current*5*1000)/10000
//		,((float)sPower.Power*5*1000*1000)/10000,((float)sPower.PosEnergy*5*1000*1000)/(1000*3600),((float)sPower.NegEnergy*5*1000*1000)/(1000*3600));
//				
		Time = GetSysTime();
	}
}	

//----------------------------------------------------------------------------
// 名    称：   uint8  GetPower(void)
// 功    能：   取油量数据
// 入口参数：   无
// 出口参数：   无
//----------------------------------------------------------------------------
int16  GetCur(void)
{
//	return 	((int32)sPower.Current * 5)/(10000/1000);
	return 	((int32)sPower.Current ) / 2;
}


//----------------------------------------------------------------------------
// 名    称：   uint8  GetPower(void)
// 功    能：   取油量数据
// 入口参数：   无
// 出口参数：   无
//----------------------------------------------------------------------------
int16  GetVol(void)
{
//	return 	((int32)sPower.Voltage * 1000)/10000;
	return 	((int32)sPower.Voltage )/10;	
}

//----------------------------------------------------------------------------
// 名    称：   uint8  GetPower(void)
// 功    能：   取油量数据
// 入口参数：   无
// 出口参数：   无
//----------------------------------------------------------------------------
int32  GetPow(void)
{
//	return 	((int32)sPower.Power*5*1000)/(10000/1000);
	return 	((int32)sPower.Power*5*100);
}

//----------------------------------------------------------------------------
// 名    称：   uint8  GetPower(void)
// 功    能：   取油量数据
// 入口参数：   无
// 出口参数：   无
//----------------------------------------------------------------------------
int32  GetPosEnergy(void)
{
//	return 	((int32)sPower.PosEnergy*5*1000)/(3600);
	return 	((int32)sPower.PosEnergy)*5*10/36;
}

//----------------------------------------------------------------------------
// 名    称：   uint8  GetPower(void)
// 功    能：   取油量数据
// 入口参数：   无
// 出口参数：   无
//----------------------------------------------------------------------------
int32  GetNegEnergy(void)
{
//	return 	((int32)sPower.NegEnergy*5*1000)/(3600);
	return 	((int32)sPower.NegEnergy)*5*10/36;
}

//----------------------------------------------------------------------------
// 名    称：   uint8  GetPower(void)
// 功    能：   取油量数据
// 入口参数：   无
// 出口参数：   无
//----------------------------------------------------------------------------
int32  GetInitNegEnergy(void)
{
//	return 	((int32)sPower.NegEnergy*5*1000)/(3600);
	return 	((int32)sPower.NegEnergy);
}

//----------------------------------------------------------------------------
// 名    称：   uint8  GetPower(void)
// 功    能：   取油量数据
// 入口参数：   无
// 出口参数：   无
//----------------------------------------------------------------------------
int32  GetInitEnergy(void)
{
//	return 	((int32)sPower.NegEnergy*5*1000)/(3600);
	return 	((int32)sPower.PosEnergy);
}
//----------------------------------------------------------------------------
// 名    称：   uint8  GetPower(void)
// 功    能：   取油量数据
// 入口参数：   无
// 出口参数：   无
//----------------------------------------------------------------------------
void  TestPower(void)
{
	static	uint32	Time = 0;
	
	ScanPowerAddr();
	
	GetModbusPowerVal();
	
	if(GetSysTime() - Time > 100)
	{
		MathPower();

//		PrintfCOM0("\r\n%f,%f,%f,%f,%f",(float)(((int32)sPower.Voltage * 1000)/10000),(float)((int32)sPower.Current*5*1000)/10000
//		,((float)sPower.Power*5*1000*1000)/10000,((float)sPower.PosEnergy*5*1000*1000)/(1000*3600),((float)sPower.NegEnergy*50*380)/(1000*3600));
//				
		//PrintfCOM0("\r\n%hi",-100);				
		SendReadPowerCode();
		
		DelayX10ms(1);
				
		Time = GetSysTime();
	}
	DelayX10ms(100);
}
