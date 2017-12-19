/*******************************************************************************
 *   Filename:      bsp_gpio.c
 *   Revised:       $Date: 2017-05-15$
 *   Revision:      $
 *   Writer:	    redmorningcn.
 *
 *   Description:
 *   Notes:
 *     			E-mail:redmorningcn@qq.com
 *
 *   All copyrights reserved to redmorningcn.
 *
 *******************************************************************************/
#include 		<includes.h>  
#include 		"string.h"
#include    	"stdio.h" 
#include   	    "stdarg.h"

//-------------------------------------------------------------------------------------------------------
//函数名称:         GPIO_SetOrClearValue()
//功    能:         	对指定的端口写1或0
//入口参数:         GpioNum:   引脚编号，由端口*100+ 引脚编号 组成。
//                  		Val:   设定值，0 或
//作		者:		redmorningcn
//日		期:		2017-05-15
//出口参数:         无
//说明：            
//--------------------------------------------------------------------------------------------------------
void	GPIO_SetOrClearValue(uint32_t GpioNum,uint8_t Val)
{
	uint32 		PortBuf[] = {PINSEL_PORT_0,PINSEL_PORT_1,PINSEL_PORT_2,PINSEL_PORT_3,PINSEL_PORT_4};
	uint8		PortNum;
	uint8		IoNum;

	PortNum 	= GpioNum/100;
	IoNum	= GpioNum%100;

	GPIO_SetDir(PortBuf[PortNum], 0x01ul<<IoNum,1);
	Val>0?GPIO_SetValue(PortBuf[PortNum], 0x01ul<<IoNum):
			  GPIO_ClearValue(PortBuf[PortNum], 0x01ul<<IoNum);
}

//-------------------------------------------------------------------------------------------------------
//函数名称:         GPIO_PinselConfig()
//功    能:         	配置引脚功能
//入口参数:         GpioNum:   引脚编号，由端口*100+ 引脚编号 组成。
//                  		FuncNum:    引脚功能号，一般功能0~4
//作		者:		redmorningcn
//日		期:		2017-05-15
//出口参数:         无
//说明：            
//--------------------------------------------------------------------------------------------------------
void	GPIO_PinselConfig(uint16 GpioNum,uint8_t FuncNum)
{
	PINSEL_CFG_Type	PinCfg_Test;

	uint32 		PortBuf[] = {PINSEL_PORT_0,PINSEL_PORT_1,PINSEL_PORT_2,PINSEL_PORT_3,PINSEL_PORT_4};
	uint8		PortNum;
	uint8		IoNum;

	PortNum 	= GpioNum/100;
	IoNum	    = GpioNum%100;

	PinCfg_Test.Portnum = PortBuf[PortNum];
	PinCfg_Test.Pinnum  = IoNum;
	PinCfg_Test.Funcnum = FuncNum;
    
	PINSEL_ConfigPin((PINSEL_CFG_Type *)&PinCfg_Test);
}

//-------------------------------------------------------------------------------------------------------
//函数名称:        GPIO_ReadIoVal()
//功    能:         	读取引脚上值
//入口参数:         GpioNum:   引脚编号，由端口*100+ 引脚编号 组成。
//出口参数:		0 或1                 		
//作		者:		redmorningcn
//日		期:		2017-05-15
//出口参数:         无
//说明：            
//--------------------------------------------------------------------------------------------------------
uint8	GPIO_ReadIoVal(uint32_t GpioNum)
{
	uint32 		PortBuf[] = {PINSEL_PORT_0,PINSEL_PORT_1,PINSEL_PORT_2,PINSEL_PORT_3,PINSEL_PORT_4};
	uint8		PortNum;
	uint8		IoNum;
	uint32 		PortVal;	

	PortNum = GpioNum/100;
	IoNum	= GpioNum%100;

	GPIO_SetDir(PortBuf[PortNum], 0x01ul<<IoNum,0);
	PortVal = GPIO_ReadValue(PortBuf[PortNum]);	
	
	if(PortVal & 1ul<<IoNum)
	{
		return	1;
	}
	return	0;
}
