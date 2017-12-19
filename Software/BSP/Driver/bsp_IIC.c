/*******************************************************************************
 *   Filename:      bsp_IIC.c
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
#include <includes.h>



#define     SCL     	206		//P0.15
#define     SDA     207		//P0.18

//#define FALSE              1
//#define TRUE               0
#define Delay_Factor       5                                        		//��ʱ����

#define SCL_SET            	GPIO_SetBits(GPIO_I2C1_PORT, SCL)				//SCL=1
#define SCL_RESET          	GPIO_ResetBits(GPIO_I2C1_PORT, SCL)			    //SCL=0
   
#define SDA_SET            	GPIO_SetBits(GPIO_I2C1_PORT, SDA)				//SDA=1
#define SDA_RESET          	GPIO_ResetBits(GPIO_I2C1_PORT, SDA)			    //SDA=0
                            
#define SDA_read()         	GPIO_ReadInputDataBit(GPIO_I2C1_PORT, SDA)	    //��ȡSDA״̬

/*******************************************************************************
* Function Name  : I2C_delay
* Description    : ��ʱ����i������ʱʱ��
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_delay(void)
{
	uint8_t i = Delay_Factor; 
	while(i) 
	{ 
		i--; 
	}  
}

//------------------------------------------------------------------------
//  ��  �� ��		GPIO_Fram_Init
//  ��  �� :    		��ʼ������� IO�ڣ�Ϊͨ��IO ��
// ��ڲ�����	��
// ���ڲ�����	��
//------------------------------------------------------------------------
void	GPIO_Fram_Init(void)
{
	GPIO_PinselConfig(SCL,0);
	GPIO_PinselConfig(SCL,0);
	GPIO_PinselConfig(SDA,0);
	GPIO_PinselConfig(SDA,0);
}

//------------------------------------------------------------------------
//  ��  �� ��void WriteSCL( uint8 temp )
//  ��  �� ������ SCL
// ��ڲ�������
// ���ڲ�������
//------------------------------------------------------------------------
void WriteSCL(uint8 temp)
{	
	if(temp)		
	{
		GPIO_SetOrClearValue(SCL,1);
	}
	else
	{
		GPIO_SetOrClearValue(SCL,0);		
	}	

	I2C_delay();
}

//------------------------------------------------------------------------
//  ��  �� ��uint8 ReadSCL(void)
//  ��  �� ����ȡ SCL ��ֵ
// ��ڲ�������
// ���ڲ�������
//------------------------------------------------------------------------
uint8 ReadSCL(void)
{
   uint8    temp;
    temp = 	GPIO_ReadIoVal(SCL);
    I2C_delay();
    return temp;
}

//------------------------------------------------------------------------
//  ��  �� ��void WriteSDA( uint8 temp )
//  ��  �� ������ SDA
// ��ڲ�������
// ���ڲ�������
//------------------------------------------------------------------------
void WriteSDA(uint8 temp)
{
	if(temp)		
	{
		GPIO_SetOrClearValue(SDA,1);
	}
	else
	{
		GPIO_SetOrClearValue(SDA,0);		
	}	
	I2C_delay();
}

//------------------------------------------------------------------------
//  ��  �� ��uint8 ReadSDA(void)
//  ��  �� ����ȡ SDA ��ֵ
// ��ڲ�������
// ���ڲ�������
//------------------------------------------------------------------------
uint8 ReadSDA(void)
{
    uint8 temp;
    
    temp = 	GPIO_ReadIoVal(SDA);
    I2C_delay();

    return temp;
}

//------------------------------------------------------------
//��������:         StartI2C()
//��    ��:         I2C���߿�ʼ����
//��ڲ���:         ��
//���ڲ���:         ��
//˵����            I2C���ߵĿ�ʼ������
//                  SCL�ߵ�ƽʱSDA���½���
//------------------------------------------------------------
void StartI2C(void)
{
    WriteSCL(0);                                // SCL=0;
    WriteSDA(1);                                //SDA=1;
    WriteSCL(1);                                //SCL=1;
    WriteSDA(0);                                //SDA=0;
    WriteSCL(0);                                // SCL=0;
}

//---------------------------------------------------------------
//��������:         StopI2C()
//��    ��:         I2C���߽�������
//��ڲ���:         ��
//���ڲ���:         ��
//˵����            I2C���߽���������SCLΪ�ߵ�ƽʱSDA��������
//--------------------------------------------------------------------
void StopI2C(void)
{
    WriteSCL(0);                                // SCL=0;
    WriteSDA(0);                                //SDA=0;
    WriteSCL(1);                                //SCL=1;
    WriteSDA(1);                                //SDA=1;
    WriteSCL(0);                                //SCL=0;
}

//-------------------------------------------------------------------------------
//��������:         MasterAckI2C()
//��    ��:         ��������Ӧ���ڽ��յ�1�ֽ����ݺ󣬷���Ӧ�𣬸��ߴӻ��������
//��ڲ���:         ��
//���ڲ���:         ��
//˵����            I2C����Ӧ�𣺶���һ���ֽں�SDA�����ͣ���Ӧ��0
//---------------------------------------------------------------------------------
void MasterAckI2C(void)
{
    WriteSCL(0);                                // SCL=0;
    WriteSDA(0);                               //SDA=0;
    WriteSCL(1);                                //SCL=1;
    WriteSCL(0);                                 //SCL=0;
    WriteSDA(0);                                //SDA=0;
}

//-----------------------------------------------------------------------------------
//��������:         MasterNoAckI2C()
//��    ��:         ��������Ӧ�𣬸���ͨ�Ŵӻ��������
//��ڲ���:         ��
//���ڲ���:         ��
//˵����            I2C����Ӧ��
//                  ������Ҫ����ֽ����ݺ�SDA�����ߣ�Ϊ����IIC���߲�����׼��
//-------------------------------------------------------------------------------------
void MasterNoAckI2C(void)
{
    WriteSCL(0);                                    // SCL=0;
    WriteSDA(1);                                    //SDA=1;
    WriteSCL(1);                                    //SCL=1;
    WriteSCL(0);                                    //SCL=0;
    WriteSDA(0);                                    //SDA=0;
}

//------------------------------------------------------------------------------------
//��������:         CheckSlaveAckI2C()
//��    ��:         ��I2C�ӻ�Ӧ��
//��ڲ���:         ��
//���ڲ���:         ackflag
//                  ����ӻ�����Ӧ���򷵻�0��
//                  �ӻ�û�в���Ӧ��,���򷵻�1
//˵����            ���I2C����(�ӻ�)Ӧ�����,Ӧ�𷵻�1,����Ϊ��
//these waw a bug befor 2008/05/17.
//------------------------------------------------------------------------------------
uint8 CheckSlaveAckI2C(void)
{
    uint8 ackflag;                                  //���Ӧ���־;
    
    ackflag = ReadSDA();
    WriteSCL(1);                                    //SCL=1;
    WriteSCL(0);                                    //SCL=0;
    
    return ackflag;
}

//------------------------------------------------------------------------------------
//��������:         WriteByteWithI2C()
//��    ��:         	������I2C����д1����
//��ڲ���:         Data:Ҫд��������
//���ڲ���:         ��
//˵����            	дһ�ֽ�����Data
//------------------------------------------------------------------------------------
void WriteByteWithI2C(uint8 Data)  
{
    uint8 i;
    for(i=0;i<8;i++)
    {
        if(Data & 0x80)                             //����λ�Ӹ�λ����λ,�ȴ����λ 
        {
            WriteSDA(1);                            //SDA=1;
            WriteSCL(1);                            //SCL=1;
        }
        else
        {  
            WriteSDA(0);                            //SDA=0;
            WriteSCL(1);                            //SCL=1;
        }   
        WriteSCL(0);                                //SCL=0;
        WriteSDA(0);                                //SDA=0;
        Data <<= 1;                                 //��������             
    }
}

//-------------------------------------------------------------------------------------------------------
//��������:         ReadByteWithI2C()
//��    ��:         ������I2C����д1����
//��ڲ���:         ��
//���ڲ���:         ��
//˵����            ��һ�ֽ����ݣ�����ֵ����������
//--------------------------------------------------------------------------------------------------------
uint8 ReadByteWithI2C(void)            
{
    uint8 readdata=0;                           // Read_DataΪ��������ݣ�0
    uint8 j;
    
    for (j=0;j<8;j++)
    {
        readdata <<=1;
								  //���ݶ���������0����λ������1����0x01��
        if(ReadSDA())
        {
            readdata =(readdata | 0x01);               
        }
        WriteSCL(1);                              //SCL=1;
        WriteSCL(0);                              //SCL=0;
    }
    
    return   readdata ;                         //���ض������ֽ�
}

