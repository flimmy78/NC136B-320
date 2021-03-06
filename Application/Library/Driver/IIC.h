#ifndef  _IIC_h_
#define  _IIC_h_

//------------------------------------------------------------------------
//  名  称 ：void void InitI2CIO(void)
//  功  能 ：设置 SDA SCL 为普通IO口 进行模拟总线操作
// 入口参数：无
// 出口参数：无
//------------------------------------------------------------------------
extern	void InitI2CIO(void);

//------------------------------------------------------------
//函数名称:         StartI2C()
//功    能:         I2C总线开始条件
//入口参数:         无
//出口参数:         无
//说明：            I2C总线的开始条件：
//                  SCL高电平时SDA的下降沿
//------------------------------------------------------------
extern	void StartI2C(void);

//---------------------------------------------------------------
//函数名称:         StopI2C()
//功    能:         I2C总线结束条件
//入口参数:         无
//出口参数:         无
//说明：            I2C总线结束条件：SCL为高电平时SDA的上升沿
//--------------------------------------------------------------------
extern	void StopI2C(void);

//-------------------------------------------------------------------------------
//函数名称:         MasterAckI2C()
//功    能:         主机接收应答，在接收到1字节数据后，返回应答，告诉从机接收完成
//入口参数:         无
//出口参数:         无
//说明：            I2C总线应答：读完一个字节后将SDA线拉低，即应答0
//---------------------------------------------------------------------------------
extern	void MasterAckI2C(void);

//-----------------------------------------------------------------------------------
//函数名称:         MasterNoAckI2C()
//功    能:         主机接收应答，告诉通信从机接收完成
//入口参数:         无
//出口参数:         无
//说明：            I2C总线应答：
//                  读完所要求的字节数据后将SDA线拉高，为结束IIC总线操作做准备
//-------------------------------------------------------------------------------------
extern	void MasterNoAckI2C(void);

//------------------------------------------------------------------------------------
//函数名称:         CheckSlaveAckI2C()
//功    能:         读I2C从机应答
//入口参数:         无
//出口参数:         ackflag
//                  如果从机产生应答，则返回0，
//                  从机没有产生应答,否则返回1
//说明：            检查I2C器件(从机)应答情况,应答返回1,否则为零
//these waw a bug befor 2008/05/17.
//------------------------------------------------------------------------------------
extern	uint8 CheckSlaveAckI2C(void);

//------------------------------------------------------------------------------------
//函数名称:         WriteByteWithI2C()
//功    能:         主机对I2C总线写1操作
//入口参数:         Data:要写出的数据
//出口参数:         无
//说明：            写一字节数据Data
//------------------------------------------------------------------------------------
extern	void WriteByteWithI2C(uint8 Data)  ;

//-------------------------------------------------------------------------------------------------------
//函数名称:         ReadByteWithI2C()
//功    能:         主机对I2C总线写1操作
//入口参数:         无
//出口参数:         无
//说明：            读一字节数据：返回值即读的数据
//--------------------------------------------------------------------------------------------------------
extern	uint8 ReadByteWithI2C(void)      ;

//------------------------------------------------------------------------
//  名  称 ：void void InitI2CIO(void)
//  功  能 ：设置 SDA SCL 为普通IO口 进行模拟总线操作
// 入口参数：无
// 出口参数：无
//------------------------------------------------------------------------
extern	void InitI2CIO(void);

#endif
