#ifndef  _IdDataTypeDef_h_
#define  _IdDataTypeDef_h_

//2009-02-19 15:50
//设备身份信息
//出产身份信息数据结构 //40

//调整产品编号
typedef struct _stcDeviceInfo    //设备身份信息 	//出产时设置 40
{
    uint8_t   DeviceTyp[8];      	//设备型号          8  	"NDP02-X"
//	uint16_t  DeviceNum;          //设备编号          2   "0-65535"
//	uint8_t   ProduceDate[6];     //生产日期			6	"090303"
	uint32_t  DeviceNum;          //设备编号          2   "0-65535"
	uint8_t   ProduceDate[4];     //生产日期			6	"090303"
	uint8_t   HardwareVer[10];    //硬件版本	        10  "HV2009201"   
    uint8_t   SoftwareVer[10];    //软件版本          10  "SV2009201"
    uint16_t  Reserve0;        	//预留			    2
	uint16_t  CrcCheck;           //校验和            2   
}stcDeviceInfo;

//2009-02-19 16:40
//安装设备信息		 
//初装置后,结构设置
typedef struct _stcFixInfo    //设备身份信息 	        //出产时设置 28
{
    uint8_t   LocoTyp;            //机车型号          1   
    uint8_t   E_LocoTyp;          //机车型号扩展字节  1   机车型号扩充字节,定义与LKJ2000格式相同
    uint8_t   LocoNum[2];         //机车号            2
	uint8_t   FixDate[6];         //初装日期			6	"20090303"
	uint8_t   FixUint[4];         //初装单位			4	"GTZJ"	
	uint8_t   ReFixDate[6];       //改装日期			6	"20190303"   
	uint8_t   ReFixUint[4];      	//改装单位			4	"GTGJ"	
    uint16_t 	Reserve0;   		//预留				2
	uint16_t  CrcCheck;           //校验和            2   
}stcFixInfo;

#endif
