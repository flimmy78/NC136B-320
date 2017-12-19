#ifndef	APP_CTRL_H
#define	APP_CTRL_H
/*******************************************************************************
 *   Filename:       app_ctrl.h
 *   Revised:        $Date: 2009-01-08	20:15 (Fri) $
 *   Revision:       $
 *	 Writer:		 Wuming Shen.
 *
 *   Description:
 *
 *   Notes:
 *					QQ:276193028
 *     				E-mail:shenchangwei945@163.com
 *
 *   All copyrights reserved to Wuming Shen.  现代虚拟仪器仪表研究所
 *
 *******************************************************************************/

/*******************************************************************************
 * INCLUDES
 */
#include <global.h>
#include <app.h>
//#include <bsp_key.h>
//#include <bsp_max7219.h>
//#include <protocol.h>
#include <os.h>
#include <osal.h>
#include <mb.h>
#include <RecDataTypeDef.h>
#include <CardDataTypeDef.h>


#ifdef   APP_GLOBALS
#define  APP_EXT
#else
#define  APP_EXT  extern
#endif
/*******************************************************************************
 * CONSTANTS
 */

 /*******************************************************************************
 * 				            系统参数相关宏定义                                  *
 *******************************************************************************/

/***************************************************
* 描述： RF应用数据帧队列缓冲
*/
#define AppMsgQ                            10
#define AppMsgQSize                        10
 
/***************************************************
* 描述： 定义存储设备类型
*/
#define INTNER_FLASH                        0
#define INTNER_EEPROM                       1
#define EXTERN_EEPROM                       2
#define STORAGE_TYPE                        EXTERN_EEPROM


//#define COMM_EVT_FLAG_HEART               (1 << 0)// 心跳包发送
//#define COMM_EVT_FLAG_RESET               (1 << 1)// WIFI复位
//#define COMM_EVT_FLAG_CONNECT             (1 << 2)// WIFI连接
//#define COMM_EVT_FLAG_RECV                (1 << 3)// 串口接收
//#define COMM_EVT_FLAG_REPORT              (1 << 4)// 上报消息
//#define COMM_EVT_FLAG_CLOSE               (1 << 5)// 断开
//#define COMM_EVT_FLAG_TIMEOUT             (1 << 6)// 超时
//#define COMM_EVT_FLAG_CONFIG              (1 << 7)// 配置
//#define COMM_EVT_FLAG_IAP_START           (1 << 8)// IAP开始
//#define COMM_EVT_FLAG_IAP_END             (1 << 9)// IAP结束

#define COMM_EVT_FLAG_OTR_RX                (1 << 2)	// OTR 接收事件
#define COMM_EVT_FLAG_OTR_TIMEOUT           (1 << 5)	// OTR 操作超时
#define COMM_EVT_FLAG_PLUG_CARD           	(1 << 6)	// OTR 插入IC卡


#define OS_EVT_FLAG_00                    (1 << 0)// 
#define OS_EVT_FLAG_01                    (1 << 1)// 
#define OS_EVT_FLAG_02                    (1 << 2)// 
#define OS_EVT_FLAG_03                    (1 << 3)// 
#define OS_EVT_FLAG_04                    (1 << 4)// 
#define OS_EVT_FLAG_05                    (1 << 5)// 
#define OS_EVT_FLAG_06                    (1 << 6)// 
#define OS_EVT_FLAG_07                    (1 << 7)// 
#define OS_EVT_FLAG_08                    (1 << 8)// 
 
/*******************************************************************************
 * 				            OSAL相关定义                                       *
 *******************************************************************************/
/*******************************************************************************
* 描述： OSAL任务优先级定义
*/
#define	OS_TASK_PRO_DISP                OS_TASK_PRIO_HIGHEST - 0
#define	OS_TASK_PRO_TMR                 OS_TASK_PRIO_HIGHEST - 1
#define	OS_TASK_PRO_CALC                OS_TASK_PRIO_HIGHEST - 2
#define	OS_TASK_PRO_SPEED               OS_TASK_PRIO_HIGHEST - 3
#define	OS_TASK_PRO_STORE               OS_TASK_PRIO_HIGHEST - 4
#define	OS_TASK_PRO_TIMER               OS_TASK_PRIO_HIGHEST - 5
#define	OS_TASK_PRO_LED                 OS_TASK_PRIO_HIGHEST - 6


/***************************************************
* 描述： OSAL任务ID定义
*/
#define OS_TASK_ID_DISP                 0x00
#define OS_TASK_ID_TMR                  0x01
#define OS_TASK_ID_CALC		            0x02
#define OS_TASK_ID_SPEED	            0x03
#define OS_TASK_ID_STORE		        0x04
#define OS_TASK_ID_TIMRE		        0x05
#define OS_TASK_ID_LED		            0x06


/*******************************************************************************
* 描述： OSAL事件定义
*/
/***************************************************
* 描述： OSAL事件定义：TMR任务事件
*/
#define OS_EVT_TMR_TICKS                0X00000001
#define OS_EVT_TMR_MSEC                 0X00000002
#define OS_EVT_TMR_SEC                  0X00000004
#define OS_EVT_TMR_MIN                  0X00000008
#define OS_EVT_TMR_DEAL                 0X00000010
#define OS_EVT_TMR_MTR                  0X00000020
#define OS_EVT_TMR_DTU                  0X00000040
#define OS_EVT_TMR_OTR                  0X00000080
/***************************************************
* 描述： OSAL事件定义：LED任务事件
*/
#define OS_EVT_LED_TICKS                0X00000001
#define OS_EVT_LED_1                    0X00000002
#define OS_EVT_LED_2                    0X00000004
#define OS_EVT_LED_3                    0X00000008
#define OS_EVT_LED_4                    0X00000010
#define OS_EVT_LED_5                    0X00000020
#define OS_EVT_LED_6                    0X00000040
#define OS_EVT_LED_7                    0X00000080

/***************************************************
* 描述： OSAL事件定义：DISP任务事件
*/
#define OS_EVT_DISP_TICKS                0X00000001
#define OS_EVT_DISP_INIT                 0X00000002
#define OS_EVT_DISP_2                    0X00000004
#define OS_EVT_DISP_3                    0X00000008
#define OS_EVT_DISP_4                    0X00000010
#define OS_EVT_DISP_5                    0X00000020
#define OS_EVT_DISP_6                    0X00000040
#define OS_EVT_DISP_7                    0X00000080


/***************************************************
* 描述： OSAL事件定义：STORE 任务事件
*/
#define OS_EVT_STORE_TICKS              0X00000001
#define OS_EVT_STORE_INIT               0X00000002
#define OS_EVT_STORE_2                  0X00000004
#define OS_EVT_STORE_3                  0X00000008
#define OS_EVT_STORE_4                  0X00000010
#define OS_EVT_STORE_5                  0X00000020
#define OS_EVT_STORE_6                  0X00000040
#define OS_EVT_STORE_7                  0X00000080

/***************************************************
* 描述： OSAL事件定义：Calc 任务事件
*/
#define OS_EVT_CALC_TICKS              0X00000001
#define OS_EVT_CALC_INIT               0X00000002
#define OS_EVT_CALC_2                  0X00000004
#define OS_EVT_CALC_3                  0X00000008
#define OS_EVT_CALC_4                  0X00000010
#define OS_EVT_CALC_5                  0X00000020
#define OS_EVT_CALC_6                  0X00000040
#define OS_EVT_CALC_7                  0X00000080

/***************************************************
* 描述： OSAL事件定义：speed 任务事件
*/
#define OS_EVT_SPEED_TICKS              0X00000001
#define OS_EVT_SPEED_INIT               0X00000002
#define OS_EVT_SPEED_2                  0X00000004
#define OS_EVT_SPEED_3                  0X00000008
#define OS_EVT_SPEED_4                  0X00000010
#define OS_EVT_SPEED_5                  0X00000020
#define OS_EVT_SPEED_6                  0X00000040
#define OS_EVT_SPEED_7                  0X00000080

/*******************************************************************************
 * 				            其他定义                                           *
 *******************************************************************************/
/***************************************************
* 描述： 时间定义
*/
#define FILTER_LIFE_SAVE_TIME           30          // 分钟
#define FILTER_LIFE_TIME               (3*30*24*60/1)// 分钟(3个月)
//#define FILTER_LIFE_SAVE_TIME           1          // 分钟
//#define FILTER_LIFE_TIME               (60/FILTER_LIFE_SAVE_TIME)        // 分钟(24小时，测试用)

/*******************************************************************************
 * TYPEDEFS
 */
#define PARA_TYPE_INT08S                0
#define PARA_TYPE_INT08U                1
#define PARA_TYPE_INT16S                2
#define PARA_TYPE_INT16U                3

#define PARA_TYPE   PARA_TYPE_INT16S
__packed
typedef struct {
    
#if     ( PARA_TYPE == PARA_TYPE_INT08S )
    INT08S      Min;
    INT08U      Max;
#elif   ( PARA_TYPE == PARA_TYPE_INT08U )
    INT08U      Min;
    INT08U      Max;
#elif   ( PARA_TYPE == PARA_TYPE_INT16S )
    INT16S      Min;
    INT16S      Max;
#elif   ( PARA_TYPE == PARA_TYPE_INT16U )
    INT16U      Min;
    INT16U      Max;
#else
    INT08S      Min;
    INT08U      Max;
#endif
} StrParaRange;

//
//
//typedef struct _stcFlshRec   	//定义油尺记录数据结构
//{
//	uint32	StoreCnt;         	//存储流水号    		4   追踪记录用
//
//	uint8	RecTypCod;       	//记录类型          	1   数据类型（开机、上班卡、下班卡、数据记录）
//	uint8	RelSpeed[3];      	//实速              	3   D9~D0：实速，D19~D10：预留
//
//	uint8	Time[4];          	//月/日/时/分/秒      	4   D5~D0:秒,D11~D6:分,D16~D12:时D21~D17：日，
//	                               	//                      	D25~D22：月 ;D26~D31:年
//	uint8	LocoTyp;          	//机车型号          	1   
//	uint8	E_LocoTyp;        	//机车型号扩展字节  	1   机车型号扩充字节,定义与LKJ2000格式相同
//	uint8 	LocoNum[2];       	//机车号            	2	
//
//	uint8 	TrainTyp[4];     	//车次种类          	4   车次种类,车次编码新方案(铁运[2005]72号文件)
//	                             	//                      	的车次种类标识符,定义与LKJ2000格式相同
//	                             	
//	uint8	TrainNum[3];     	//车次数字部分      	3   车次编码新方案(铁运[2005]72号文)的车次数据部分
//	uint8 	Car_Truck;       	//本/补、客/货       	1   D0＝0/1 → 货/客   D1＝0/1 → 本/补   
//	 
//	uint8 	VoitureCnt;       	//辆数              	1	
//	uint8 	KmMark[3];   		//公里标            	3   单位：米
//
//	uint8 	Weight[2];       	//总重              	2
//	uint8 	PlanLen[2];       	//计长              	2   单位：0.1   
//
//	uint8 	MstDriverNum[2];  	//司机号            	2	
//	uint8 	E_MstDriverNum; 	//司机号扩展字节    	1   
//	uint8 	E_SlvDriverNum;   	//副司机号扩展字节  	1   
//
//	uint8 	SlvDriverNum[2];  	//副司机号          	2   
//	uint8	DriverUnitNum[2];  	//司机单位编号      	2   //用来唯一确定司机
//
//	uint8 	RoadNum;          	//区段号(交路号)     	1	
//	uint8  	RelRoadNum;     	//实际交路号         	1   定义格式与LKJ2000相同   
//	uint8  	StationNum;     	//车站号            	1
//	uint8  	E_StationNum;   	//车站号扩展字节    	1   车站号扩展字节
//
//	uint8 	SignalTyp;        	//信号机类型        	1   B2-B0 :2-出站,3-进站,4-通过,5-预告,6-容许
//	uint8 	LocoSign;        	//机车信号          	1   B4 = 0/1  单灯/多灯;   B3~B0 0-无灯,1-绿,2-黄, 
//	                            	//                      	3-双黄,4-红黄,5-红,6-白,7-绿黄,8-黄
//	uint8 	LocoWorkState;    	//机车工况          	1   b0-零位,B1-向后,B2-向前,B3-制动,B4-牵引
//	uint8 	LocoState;      	//装置状态          	1   B0 = 1/0 - 降级/监控; B2 = 1/0 - 调车/非调车
//
////	uint8 	SignalNum[2];      	//信号机编号        	2   
////	uint8 	LocoPipePress[2]; 	//列车管压          	2   B9-B0:管压(单位:1kPa)
//
//	uint16	Myspeed1;			//自测速度1				2
//	uint16	Myspeed2;			//自测速度2				2	
//	  
//	uint32 	MstDip;           	//主机油量          	4   主油尺油量(主从机级联时为主机油量,否则为本机车油量)
//
////	uint32 	SlvDip;           	//从机油量          	4   主油尺油量(主从机级联时为从机油量,否则此油量值为空)
//	uint32	MyKileMeter;		//总里程				4
//		
//	int32 	MyMstLocoPower;     //主机机车功率       	4   主机外接设备测量
//
////	int32 	SlvLocoPower;     	//从机机车功率       	4   从机外接设备测量
//	uint32	MyPower;			//累计电量				4	
//	//090901
// 	//uint16	MstDip1Prs;        	//压强            		2   主机油尺1压强值
//	//uint16	MstDip2Prs;        	//压强            		2   主机油尺2压强值
//	int16		Myhig1;				//液位高度1				2	
//	int16		Myhig2;				//液位高度2				2							
//
//	uint16	SlvDip1Prs;       	//压强            		2   从机油尺1压强值
//	uint16 	SlvDip2Prs;       	//压强            		2   从机油尺2压强值
//	
//	uint8  	MyYear;           	//年      				1   装置时间的年
//	uint8 	MyMonth;         	//月      				1   装置时间的月
//	uint8 	MyData;          	//日      				1   装置时间的日
//	uint8 	MyHour;           	//时      				1   装置时间的时
//	
//	uint8 	MyMinute;     		//分      				1   装置时间的分
//	uint8 	MySecond;       	//秒       				1   装置时间的秒
//	uint16	MstDensity;			//密度							2	主机燃油密度
//	
////	uint16	SlvDensity;			//密度						2	从机燃油密度
////	uint16	GpsHeight;			//高度						2	地理高度
//	int16		MyAddspeed;			//自测加速度					2
//	uint16	MyDip;				//自测油量							2
//		
//	int32	GpsLongitude;		//经度					4	地理经度
//	int32	GpsLatitude;		//纬度					4	地理纬度
//		
//	int16	MstEngRotSpd;		//主机柴油机转速		2	主机柴油机转速
////	int16	SlvEngRotSpd;		//从机柴油机转速		2	从机柴油机转速
//	int16	MyEngRotSpd;		//从机柴油机转速		2	从机柴油机转速
//
//	//补充内容 
//	//与自加时钟芯片配套的数据有
//	uint16 	JcRealType;			//机车型号				2   外部设置的机车型号
//	uint16 	JcRealNo;			//机车编号				2   外部设置的机车编号
//	
////	uint16 	MstPotPress;		//主机缸压				2	预留接监控	
////	uint16 	SlvPotPress;		//从机缸压				2	预留接监控	
//	uint16	MyDip1Tmp;	  		//油尺1温度
//	uint16	MyDip2Tmp;			//油尺2温度
//		
////	uint16 	MstPot1;			//主机均缸1				2	预留接监控	
////	uint16 	MstPot2;			//主机均缸2				2	预留接监控
//	uint16	MyDip1Den;
//	uint16	MyDip2Den;	
//
////	uint16 	SlvPot1;			//从机均缸1				2	预留接监控		
////	uint16 	SlvPot2;			//从机均缸2				2	预留接监控
//	uint16	MyDip1Err;
//	uint16	MyDip2Err;
//	
//	uint16 	MaxSpeed;			//限速					2	预留接监控	
////	uint16 	MstOcurrent;		//主机原边电流   		2	预留接监控
//	uint16	ErrNum;				//现有故障条数			2	
//	
//	
//	uint8	ErrorCode;			//故障代码				1   0~255
////	uint8 	DipValChgState;  	//油量变化状态   		1   油量变化情况分：两油尺增加/减小、变化平稳、轻微波动、剧烈波动
//	uint8	MyLocoWorkState;	//自测机车状态			1
//	//090901
//	uint8	MstDipTemp;			//温度					1	主机温度
//	uint8	OilBoxModel;		//油箱模型				1	机车油箱代码
//	
//	uint8	SoftVes;        	//软件版本          	1   
//	uint8	Reserve1;        	//预留          		1  	
////	uint16	Reserve0;        	//预留          		2   
//	uint16 	CrcCheck;         	//存储的校验和    		2   Flash记录存储CRC校验
//}stcFlshRec;
//
//


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//与测量装置通讯结构
//
//设备单帧允许有效数据长度
#define	MTR_COMM_RECV_BUF_LEN	160
//设备Mtr
/***************************************************
* 描述：统计模块-- 测量装置，长沙南睿轨道交通车辆装置异步串行通信通用通信协议
*/
// 发送数据结构：主机-->油尺
__packed
typedef struct {
    INT32U                  Oil;                                // 油量	        4	数值范围0-9999L  
    INT16S                  Accelerations;                      // 加速度	    2	-9.000-9.000（m/s2）(1000倍)
    INT16U                  Speed;                              // 速度	        2	数值0-600（Km/H）
    struct __recv_work__ {                                     	// 机车工况	    1
    INT08U                  Null     	: 1;   	//              0x01:零位
    INT08U                  Tow   	: 1;       	 //              0x02:牵引
    INT08U                  Brake   	: 1;       	 //              0x04:制动
    INT08U                  Rsv        	: 5;  	// 其他：未定义
    } Work;
    INT16U                  Density;                            	// 高度计算密度	2	0.xxxx克/立方厘米（10000倍）
    INT08U                  Rsv[1+10];                         	 // 备用	        3
} StrMtrCommSendData;


//发送联合体，可定义多类型结构互用
typedef union {
	StrMtrCommSendData	StrSend;
	uint8				Buf[MTR_COMM_RECV_BUF_LEN];
}UniMtrCommSendData;


/***************************************************
* 描述： 统计模块-- 测量装置，长沙南睿轨道交通车辆装置异步串行通信通用通信协议
*/
// 应答数据结构：油尺-->主机
__packed
typedef struct {
    INT16U                  Hight;                              	// 液位高度	    2	xxx.x毫米（10倍）
    INT16U                  Density;                            	// 高度计算密度	2	0.xxxx克/立方厘米（10000倍）
//    union __usend_status {
//    StrSenErr               Udat;
//    INT08U                  Dat;
//    } Sts;
    INT08U                  RefLiqTemp;            		// 液体参考温度	1	0.25~63.50度（分别用1-254表示）或者-49.5~77.0度（分别用1-254表示，100表示0度）0：过低；255：过高
    INT16U                  AdcValue[2];                        // Adc值
    INT16U                  HightDiff;                          	// 高度差	    2	0~1000x0.1mm(两传感器高度差)
    INT16U                  Volume;                             	// 体积  	    2	0~10000L(油量)
    INT16U                  Weight;                             	// 重量	        2	0~10000kg(重量)
    INT16S                  Pressure[2];                        // 压强	        2	0~10000pa
    INT16S                  CalcDensity;                        // 测量密度     x10000
    INT16S                  Gravity;                            	// 计算加速度   x10000
    INT16S                  CalcGravity;                        // 测量加速度   x10000
    //INT08U                  Rsv[2+10];                        // 备用	        2	
} StrMtrCommRecvData;

//应答联合体，可定义多类型结构互用
typedef union {
	StrMtrCommRecvData		sRecv;
	uint8				Buf[MTR_COMM_RECV_BUF_LEN];
}UniMtrCommRecvData;

//接收区控制字
//8 Bytes
//#define     COMM_REC_END_FLG        0x01
//#define     COMM_REC_STRAT_FLG      0x02
//

typedef struct {     
    INT08U          DestAddr;       //接收地址      slave  =0xA1\A2	   
    INT08U          SourceAddr;     //源地址       master   =0x80	   
    INT08U          FramNum;        //帧序号
    INT08U			Len;			//接收有效数据长度
    INT32U			Code;			//接收控制字
} StrDevMtrRecvCtrl;

//连接控制字
// 4 Bytes
typedef struct {     
    uint8          ConnFlg;        //连接控制,1，允许连接，0，不允许连接
    uint8			RecvEndFlg;		//接收标示，1，数据接收完成，0，无数据接收。
    uint8			TimeOut;		//超时时间，单位1s
    uint8			ErrFlg;		    //错误标示，连接正常，0；连接错误，1
    uint8          SlaveAddr;       //接收地址         slave  =0xCA	   
    uint8          MasterAddr;     //源地址           master   =0x80	   
    uint8          SendFramNum;    //帧序号   
    uint8          SendFlg;        //发送标示， 有数据发送，1；无数据发送，0
} StrDevMtrConnCtrl;		

//统计某块通讯控制结构
//128 +128 + 8 + 8 + 4  = 276
#define	COMM_DEV_MTR_CONN_NUM	2	// 设备连接数A1\A2	
__packed
typedef struct {  
	UniMtrCommRecvData	Rd;				// 160 接收数据区
	UniMtrCommSendData	Wr;				// 160 发送数据区

    /***************************************************
    * 描述： 串口控制组
    */
	StrDevMtrRecvCtrl    	RxCtrl;     // 8		接收控制，包含当前接收到的控制信息
	StrDevMtrConnCtrl		ConnCtrl[COMM_DEV_MTR_CONN_NUM];
                                        // 4*2	连接控制，对每个地址作为单独的数据连接。
	
    MODBUS_CH            	*pch;       // 4 	MODBUS句柄
    
    INT08U                  ConnectTimeOut  : 7;             // 连接超时计数器，秒为单位
    INT08U                  ConnectFlag     : 1;                // 连接标志
} StrDevMtr;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//与无线发送装置通讯结构
//设备DTU
//
/***************************************************
* 描述： 统计模块--无线发送模块，长沙南睿轨道交通车辆装置异步串行通信通用通信协议
*/
// 发送数据结构  		统计-->无线
__packed
typedef union {
	stcFlshRec	sRec;						//数据记录     128 	
	uint8		Buf[160];					//
} UniDtuCommSendData;

typedef struct {     
 	uint16	Type;				//机车类型	2	参见机车类型代码表
 	uint16	Num;				//机车号		2	
} stcLocoId;
/***************************************************
* 描述： 统计模块--无线发送模块，长沙南睿轨道交通车辆装置异步串行通信通用通信协议
*/
// 发送数据结构		无线-->统计		
__packed
typedef union {
	int16		Density;			//DENSITY_CARD   设置密度值   		2     0.xxxx克/立方厘米（10000倍）
	stcTime		sTime;			    //TIME_CARD     设置时间            8     second,minute,hour,data,month,year,crccheck
	int16		Hig;				//HIGHT_CARD    设置高度值          2     0.1mm  (10倍)
	stcLocoId   sLocoId;		    //FIX_CARD      设置机车信息	    4     type,num;
	uint32      Slope;			    //CALC_PARA	    设置计算的斜率	    4	(slope/10000);     后续补充其他
    uint8       ModelNum;           //              模型编号            1       
	uint8		Buf[160];		    //	
} UniDtuCommRecvData;

//接收控制字
typedef struct {     
    INT08U          DestAddr;       //接收地址      slave  =
    INT08U          SourceAddr;     //源地址       master   =0x80	   
    INT08U          FramNum;        //帧序号
    INT08U			Len;			//接收有效数据长度
    INT32U			Code;			//接收控制字
} StrDevDtuRecvCtrl;

//连接控制字，
typedef struct {     
    INT08U          ConnFlg;        //连接控制,1，允许连接，0，不允许连接
    INT08U			RecvEndFlg;		//接收标示，1，数据接收完成，0，无数据接收。
    INT08U			TimeOut;		//超时时间，单位1s
    INT08U			ErrFlg;		    //错误标示，连接正常，0；连接错误，1

    INT08U          SlaveAddr;       //接收地址      slave  =0xCA	   
    INT08U          MasterAddr;     //源地址           master   =0x80	   
    INT08U          SendFramNum;    //帧序号   
    INT08U          SendFlg;        //发送标示， 有数据发送，1；无数据发送，0
} StrDevDtuConnCtrl;		


#define	COMM_DEV_DTU_CONN_NUM	2	// 设备连接数CA/C1	
__packed
typedef struct {  
	UniDtuCommRecvData	Rd;			//接收数据区
	UniDtuCommSendData	Wr;			//发送数据区

    /***************************************************
    * 描述： 串口控制组
    */
	StrDevDtuRecvCtrl    	RxCtrl;				//接收控制，包含当前接收到的控制信息
	StrDevDtuConnCtrl		ConnCtrl[COMM_DEV_DTU_CONN_NUM];//连接控制，对每个地址作为单独的数据连接。
	
    MODBUS_CH            	*pch;                   		// MODBUS句柄
    
    INT08U                  ConnectTimeOut  : 7;     	// 连接超时计数器，秒为单位
    INT08U                  ConnectFlag     : 1;         	// 连接标志
} StrDevDtu;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//与邋IC卡模块通讯结构
//设备OTR
//
/***************************************************
* 描述： 统计模块--卡发送模块，长沙南睿轨道交通车辆装置异步串行通信通用通信协议
*/
typedef struct _stcTinyRec{  
    uint8   Addr;
    uint8   ModelNum;
    uint16  Oil;
    uint32  CurRecNum;
    uint16  Speed1;
    uint16  Speed2;
    uint16  Speedcs;
    int16   Hig1;
    int16   Hig2;
    uint8   LocoWorkSta;
}stcTinyRec;


// 发送数据结构  		统计--> 卡
__packed
typedef union {
	stcFlshRec	sRec;						//数据记录     128 	
    stcTinyRec  sTinyRec;                   //简单数据记录
	uint8		Buf[160];					//
} UniOtrCommRecvData;


/***************************************************
* 描述： 统计模块--卡发送模块，长沙南睿轨道交通车辆装置异步串行通信通用通信协议
*/
// 发送数据结构	卡-->统计		
typedef struct _stcReqRec_{
	uint32	StartNum;
	uint32	EndNum;
}stcReqRec;

__packed
typedef union {
	int16		Density;		//DENSITY_CARD   设置密度值   		2     0.xxxx克/立方厘米（10000倍）
	stcTime		sTime;			//TIME_CARD        设置时间        		8     second,minute,hour,data,month,year,crccheck
	int16		Hig;			//HIGHT_CARD	     设置高度值   		2     0.1mm  (10倍)
	stcLocoId	sLocoId;		//FIX_CARD          设置机车信息	4     type,num;
	uint32      Slope;			//CALC_PARA	     设置计算的斜率	4	(slope/10000);     后续补充其他
	uint8       ModelNum;    	
    uint8		Buf[160];		//	
    stcReqRec	sReqRec;		//请求数据记录
    uint32      CardRecNum;     //卡已读记录
} UniOtrCommSendData;

//接收控制字
typedef struct {     
    INT08U          DestAddr;       //接收地址      slave  = 
    INT08U          SourceAddr;     //源地址       master   =0x80	   
    INT08U          FramNum;        //帧序号
    INT08U			Len;			//接收有效数据长度
    INT32U			Code;			//接收控制字
} StrDevOtrRecvCtrl;

//连接控制字，
typedef struct {     
    INT08U          ConnFlg;        //连接控制,1，允许连接，0，不允许连接
    INT08U			RecvEndFlg;		//接收标示，1，数据接收完成，0，无数据接收。
    INT08U			TimeOut;		//超时时间，单位1s
    INT08U			ErrFlg;		    //错误标示，连接正常，0；连接错误，1

    INT08U          SlaveAddr;       //接收地址      slave  =0xCA	   
    INT08U          MasterAddr;     //源地址           master   =0x80	   
    INT08U          SendFramNum;    //帧序号   
    INT08U          SendFlg;        //发送标示， 有数据发送，1；无数据发送，0
} StrDevOtrConnCtrl;		


typedef	struct _stcOtrCommSendData{
	uint32				Code;		//信息类型
	UniOtrCommSendData	Info;		//信息区
}stcOtrCommSendData;

#define	COMM_DEV_OTR_CONN_NUM	2	// 设备连接数CA/C1	
__packed
typedef struct {  
	UniOtrCommRecvData	Rd;			//接收数据区
	stcOtrCommSendData	Wr;			//发送数据区

    /***************************************************
    * 描述： 串口控制组
    */
	StrDevOtrRecvCtrl    	RxCtrl;				//接收控制，包含当前接收到的控制信息
	StrDevOtrConnCtrl		ConnCtrl[COMM_DEV_OTR_CONN_NUM];//连接控制，对每个地址作为单独的数据连接。
	
    MODBUS_CH            	*pch;                   		// MODBUS句柄
    
    INT08U                  ConnectTimeOut  : 7;     	// 连接超时计数器，秒为单位
    INT08U                  ConnectFlag     : 1;         	// 连接标志
} StrDevOtr;





//    Ctrl.Os.CommEvtFlag = M_EVT_FLAG_HEART       	// ?????
//                        + COMM_EVT_FLAG_RESET       	// COMM??
//                        + COMM_EVT_FLAG_CONNECT    	 // COMM??
//                        + COMM_EVT_FLAG_RECV        	 // ????
//                        + COMM_EVT_FLAG_REPORT     	 // ????
//                        + COMM_EVT_FLAG_CLOSE      	 // ??
//                        + COMM_EVT_FLAG_TIMEOUT    	 // ??
//                        + COMM_EVT_FLAG_CONFIG     	 // ??
//                        + COMM_EVT_FLAG_IAP_START  	 // IAP??
//                        + COMM_EVT_FLAG_IAP_END;    	// IAP??



//记录号管理:当前记录号，卡已读记录号，无线发送模块记录号
//16 bytes
typedef struct _StrRecNumMgr {
    uint32		Current;			//当前记录号	0~0xffffffff
    uint32		IcRead;				//IC卡模块已读取记录号,统计模块
    uint32		GrsRead;			//无线已读取记录号
    uint32		CardRead;			// 
}StrRecNumMgr;

//产品信息:  型号+ 编号
//12 bytes
typedef struct _StrProductInfo {
	uint32		Type;			//产品类别，0 :NDP02  ,1:NDP02-B
	uint32		ID;				//产品编号	16110002
	stcLocoId		sLocoId;		//机车信息	104(DF4B) - 1000
}StrProductInfo;

//油量计算参数: 模型编号，高度，密度，斜率
//12 bytes
typedef struct _StrOilPara {
    uint8			ModelNum;			//模型编号	0~64
    uint8			Rec8;				//备用
    int16			Hig;				//高度  -500~500
    int16			Density;			//密度  800~900,     0.xxxx克/立方厘米（10000倍）
    int16			Rec16;				//备用
    uint32			Slope;				//修正系数 :计算值再乘(slope/10000)  
}StrOilPara;

//运行参数，装置运行相关，数据存储周期，显示参数，恢复出厂设置
//8 bytes
typedef struct _stcRunPara_					
{
	uint8		StoreTime;					// 1 		数据记录存储周期
	uint8		plugcard;					// 1		插卡
	uint8		StartFlg;					// 1    	首次运行
	uint8		SysSta;						// 1   	    系统运行状态
	uint32		CardType;					// 4        卡参数
    uint8       CardErrData;                // 1        卡错误数据
    uint8       tmpbuf[3];                  // 3        卡缓冲区
    uint32      tmp32;                      // 4        数据变量
}stcRunPara;

//油箱模型点，高度，油量
// 4 bytes
//typedef struct _stcModPot_					//
//{
//	uint16	Hig;								// 2 		高度
//	uint16	Oil;								// 2 		油量
//}stcModPot;
//运算用油箱模型
//408 bytes
typedef struct _stcCalcModel_					
{
	stcModPot	sModPot[100];					// 4*100 	内容
	uint16		PotNum;							// 2 		模型有效点数 
	uint16		StartOil;                       // 2    	模型初始油量   	
	uint8		ModelNum;						// 1		模型编号
	uint8		Res0;							// 1        备用
	uint16		CrcCheck;						// 2 		CrcCheck;
}stcCalcModel;

//OS 系统运行参数。
//事件标示组。
__packed
typedef struct {
    OS_FLAG_GRP             WifiEvtFlagGrp;
    OS_FLAGS                WifiEvtFlag;
    OS_FLAG_GRP             CommEvtFlagGrp;		// 串口通讯标示组
    OS_FLAGS                CommEvtFlag;        // 串口通讯标示
    OS_FLAG_GRP             MaterEvtFlagGrp;
    OS_FLAGS                MaterEvtFlag;
    OS_FLAG_GRP             DtuEvtFlagGrp;
    OS_FLAGS                DtuEvtFlag;
} StrCtrlOS;

//系统参数
__packed
typedef struct _StrSysCtrlPara {
    /***************************************************
    * 描述： 系统参数：起始地址 = 000   通讯密码，软件版本，记录号，产品信息
    */    
    INT16U      Password;           	// 	2  	用于MODEBUS通讯确认密钥，默认为6237，防止非法改数据
    INT16U		SoftWareID;				//	2  	软件版本
    //
    /***************************************************
    * 描述：记录号管理地址：起始地址 = 004
    */
    StrRecNumMgr   sRecNumMgr;			//	16	记录号管理: 当前记录号，卡已读记录号，无线已读记录号
       //
    /***************************************************
    * 描述：产品信息：起始地址 = 020
    */
    StrProductInfo	sProductInfo;		//	12	产品信息: 产品信号及产品编号；
	   
    //
    /***************************************************
    * 描述：数据记录：起始地址 = 032
    */
	stcFlshRec	sRec;					//  128  数据记录    接收到的数据记录

    //
    /***************************************************
    * 描述：油量计算参数：起始地址 = 160
    */
	StrOilPara	SOilPara;				// 12  高度、密度，斜率，模型编号；

    //
    /***************************************************
    * 描述：系统运行参数：起始地址 = 172
    */
	stcRunPara	sRunPara;					// 8		//数据存储周期，数据清零，参数清零

    /***************************************************
    * 描述：通讯设备
    */
    StrDevOtr	Otr;

//定义全局的Os操作变量?   
	StrCtrlOS    	Os;                                	 // OS系统结构体变量    
	
//    //显示结构体	
//    	StrDisp           Disp;                            	//显示结构体变量 	
	
} stcSysCtrl;

extern  stcSysCtrl       sCtrl;


/*******************************************************************************
 * GLOBAL FUNCTIONS
 */

/*******************************************************************************
 * EXTERN VARIABLES
 */

/*******************************************************************************
 * EXTERN FUNCTIONS
 */
void        Beep                        (INT32U flag);
//void        BeepExt                     (u8 cnt, u16 freq, u16 dly);
void        Power                       (INT08U onoff);
/***************************************************
* 描述： 应用层函数体声明
*/
BOOL        App_PraInit                 (void);
BOOL        App_GetParaFromEeprom       (void);
BOOL        App_SetParaToEeprom         (void);
void        App_Error                   (void);
void        App_FanHandCtrl             (INT08U step);
void        App_FanAutoCtrl             (INT08U step);
void        App_ParaFilter              (void);

/***************************************************
* 描述： OSAL任务初始化函数体声明
*/
void        TaskInitBeep                ( void );
void        TaskInitKey                 ( void );
void        TaskInitCtrl                ( void );
void        TaskInitTmr                 ( void );
void        TaskInitDisp                ( void );
/***************************************************
* 描述： OSAL任务事件处理函数体声明
*/
osalEvt     TaskBeepEvtProcess          (INT8U task_id, osalEvt task_event);
osalEvt     TaskKeyEvtProcess           (INT8U task_id, osalEvt task_event);
osalEvt     TaskCtrlEvtProcess          (INT8U task_id, osalEvt task_event);
osalEvt     TaskTmrEvtProcess           (INT8U task_id, osalEvt task_event);
osalEvt     TaskDispEvtProcess          (INT8U task_id, osalEvt task_event);

/***************************************************
* 描述： AES加减密函数体声明
*/
/***************************************************
* 描述： 看门狗函数体声明
*/
void        App_MsgWriteCmd             ( INT16U cmd );
/*******************************************************************************
 * 				end of file                                                    *
 *******************************************************************************/
#endif	/* GLOBLES_H */
