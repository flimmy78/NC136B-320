/*******************************************************************************
*   Filename:       app_comm_otr.c
*   Revised:        All copyrights reserved to redmorningcn.
*   Revision:       v1.0
*   Writer:	     redmorningcn.
*
*   Description:    ˫��ѡ�� otr �� Ctrl + H, ��ѡ Match the case, Replace with
*                   ������Ҫ�����֣���� Replace All
*                   ˫��ѡ�� Otr �� Ctrl + H, ��ѡ Match the case, Replace with
*                   ������Ҫ�����֣���� Replace All
*                   ˫��ѡ�� OTR �� Ctrl + H, ��ѡ Match the case, Replace with
*                   ������Ҫ�����֣���� Replace All
*                   �� app_cfg.h ��ָ��������� ���ȼ�  �� APP_TASK_COMM_PRIO     ��
*                                            �� �����ջ�� APP_TASK_COMM_STK_SIZE ����С
*
*   Notes:
*     				E-mail: redmorningcn@qq.com
*
*******************************************************************************/

/*******************************************************************************
* INCLUDES
*/
#define  SNL_APP_SOURCE
#include <includes.h>
#include <app_comm_protocol.h>
#include <app_ctrl.h>
#include <app.h>
#include <os_cfg_app.h>
#include <RecDataTypeDef.h>


#ifdef VSC_INCLUDE_SOURCE_FILE_NAMES
const  CPU_CHAR  *app_comm_otr__c = "$Id: $";
#endif

#define APP_TASK_COMM_EN     DEF_ENABLED
#if APP_TASK_COMM_EN == DEF_ENABLED
/*******************************************************************************
* CONSTANTS
*/

/*******************************************************************************
* MACROS
*/
#define CYCLE_TIME_TICKS            (OS_TICKS_PER_SEC * 1)

/*******************************************************************************
* TYPEDEFS
*/
#define		DATA_CARD           0x00000001			//���ݿ�------��ʱδ��
#define		CALC_MODEL_CARD     0x00000002			//����ģ�Ϳ�--��ʱδ��
#define		IAP_CARD            0x00000004			//IAP����-----��ʱδ��

#define		DENSITY_CARD		0x00000020			//�ܶȿ�
#define		MODEL_CARD			0x00000040			//ģ�Ϳ�-----��ʱδ��
#define		TIME_CARD           0x00000100			//ʱ�俨-----
#define		COM_MODEL_CARD	    0x00000100		    //����ģ�Ϳ�-��ʱδ��
#define		HIGHT_CARD			0x00000200			//�߶ȿ�-----
#define		COPY_CARD           0x00000400			//���ƿ�-----��ʱδ��
#define		MODEL_SELECT_CARD	0x00001000			//ģ��ѡ��-
#define		FIX_CARD            0x00002000			//װ����-----
#define		COPY_FRAM_CARD	    0x00004000			//���縴�ƿ�-��ʱδ��
#define		SET_DATA_CARD		0x00000080			//������-----��ʱδ��
#define		DEBUG_CARD			0x00000800			//���Կ�-----��ʱδ��

#define		EXPAND_CARD			0x00008000			//��չ��-----��ʱδ��

#define		DATA_CARD_ERR		0x00008000			//IC������

#define		DATA_CARD_DIS		0x00010000			//���ݿ�ָʾ
#define		DATA_CARD_FIN		0x00020000			//���ݿ�����
#define		SYS_RUN_PARA		0x00040000			//ϵͳ������--��δʹ��
#define		CLEAR_ALL			0x00080000			//���㿨�����ݼ�¼����

#define		RST_SYS				0x00100000		    //��λָ��
#define		CALC_PARA           0x00200000		    //����б��

/*******************************************************************************
* LOCAL VARIABLES
*/

/*******************************************************************************
* GLOBAL VARIABLES
*/

/***********************************************
* ������ ��ʱ������
*/

/***********************************************
* ������ ��ʱ���ص�����
*/


/*******************************************************************************
* LOCAL FUNCTIONS
*/


/*******************************************************************************
* GLOBAL FUNCTIONS
*/

/*******************************************************************************
* EXTERN VARIABLES
*/

/*******************************************************************************
* EXTERN FUNCTIONS
*/
uint8   FRAM_StoreRecNumMgr(StrRecNumMgr  *SRecNumMgr);
uint8   FRAM_StoreOilPara(StrOilPara  *SOilPara);
void    BSP_DispClrAll(void);
uint8   FRAM_StoreProductInfo(StrProductInfo  *sProductInfo);
void    comm_para_flow(StrDevOtr * sDtu,uint8 addrnum);
uint8 	ReadFM24CL64(uint16  Addr, uint8 *DataBuf, uint32 DataBufLen);            
uint32  GetRecNumAddr(uint32 FlshRecNum);
INT8U	WriteFlsh(INT32U Dst, uint8 * sFlshRec, INT32U NByte);


/*******************************************************************************/
extern  stcSysCtrl      sCtrl;
stcModelCard            m_sModelCard;

/*******************************************************************************
* ��    �ƣ� DataComReadAsk
* ��    �ܣ� IC���������ݼ�¼��
			 ��ʼ���ݼ�¼�źͽ������ݼ�¼�š�
* ��ڲ����� 
* ���ڲ����� ��
* ����  �ߣ� redmorningcn.
* �������ڣ� 2017-05-31
* ��    �ģ�
* �޸����ڣ�
* ��    ע�� ���񴴽�������Ҫ��app.h�ļ�������
*******************************************************************************/
void DataComReadAsk(unsigned int startnum,unsigned int endnum)
{
	sCtrl.Otr.Wr.Code = DATA_CARD;						//��������
	sCtrl.Otr.Wr.Info.sReqRec.StartNum 	= startnum;		//��ʼ��¼
	sCtrl.Otr.Wr.Info.sReqRec.EndNum	= endnum;		//������¼
	
	CSNC_SendData(	sCtrl.Otr.pch,						//ͨѶ���ƿ�
                  SLAVE_ADDR_OTR,						//Դ��ַ
                  MASTE_ADDR_HOST,					    //Ŀ���ַ
                  sCtrl.Otr.ConnCtrl[0].SendFramNum,	//����֡��
                  0,									//֡���ͣ�Ĭ��Ϊ0��������������ȷ		
                  (uint8 *)&sCtrl.Otr.Wr,				//���ͻ�����
                  //��������������
                  sizeof(sCtrl.Otr.Wr.Code)+sizeof(sCtrl.Otr.Wr.Info.sReqRec));
}


/*******************************************************************************
* ��    �ƣ� ȡ������Ϣ
* ��    �ܣ� 
* ��ڲ����� 
* ���ڲ����� ��
* ����  �ߣ� redmorningcn.
* �������ڣ� 2017-12-06
* ��    �ģ�
* �޸����ڣ�
* ��    ע�� ���񴴽�������Ҫ��app.h�ļ�������
*******************************************************************************/
uint8  	JudegLocoInfo(uint8   buf[])
{
    uint16  crc16;
    stcFlshRec	*psRec;						    //���ݼ�¼     128 	
    psRec       = (stcFlshRec	*)buf;
    
    crc16 = GetCrc16Check(buf,sizeof(stcFlshRec)-2);
    
    if(crc16 == psRec->CrcCheck){               //У����ȷ
        //������Ϣ����
        if(     psRec->JcRealType   != 0xffff 
           &&   psRec->JcRealNo     != 0xffff
               &&    psRec->JcRealType  != 0 
                   &&   psRec->JcRealNo     != 0    ){
                       
                       if(psRec->JcRealType   == sCtrl.sProductInfo.sLocoId.Type
                           &&psRec->JcRealNo     == sCtrl.sProductInfo.sLocoId.Num){
                               return 1;                       //������Ϣ��ȷ
                           }
                       
                       sCtrl.sProductInfo.sLocoId.Type = psRec->JcRealType ; 
                       sCtrl.sProductInfo.sLocoId.Num  = psRec->JcRealNo   ;
                       
                       FRAM_StoreProductInfo((StrProductInfo  *)&sCtrl.sProductInfo);//�������
                       return 0;
                   }
    }
    
    return 1;
}

//stcFlshRec  sFlshRecTmp;

/*******************************************************************************
* ��    �ƣ� comm_rec_read
* ��    �ܣ� ��ȡ���ݼ�¼
* ��ڲ����� 
* ���ڲ����� ��
* ����  �ߣ� redmorningcn.
* �������ڣ� 2017-05-26
* ��    �ģ�
* �޸����ڣ�
* ��    ע�� ���񴴽�������Ҫ��app.h�ļ�������
*******************************************************************************/
void	comm_rec_read(StrDevOtr * sDtu,uint8 addrnum)
{
    /***********************************************
    * ������ �������ݳ��ȣ��ж����յ��Ƿ�Ϊ���ݼ�¼��
    * 		 ���ճ���Ϊ128�����ݼ�¼������С��128,����
    * 		 ��ͨ���ݡ�
    */ 
	static	    uint8	    errtimes = 0;
    static     uint8       recsmalltimes = 0;//���յ��ļ�¼��С
    static     uint8       recbigtimes = 0; //���յ��ļ�¼�Ŵ�
    static     uint8       rectimes = 0;
	
	//���ݼ�¼����128�ֽ�
	if(sCtrl.Otr.RxCtrl.Len == sizeof(sCtrl.Otr.Rd.sRec)){		
		//�����ݼ�¼
		if(sCtrl.Otr.Rd.sRec.StoreCnt == sCtrl.sRecNumMgr.IcRead){//������յ������ݼ�¼����Ҫ���յ���ͬ���򱣴����ݡ�		
			//�������ݼ�¼
            recsmalltimes = 0;
            recbigtimes = 0;    
                           
			sCtrl.sRecNumMgr.IcRead++;
            
//            if(++rectimes == 1 )        //������256�κ��ж�һ��
//            {
//                if(JudegLocoInfo((uint8 *)&sCtrl.Otr.Rd.sRec) == 0)//������Ϣ�仯����¼������(bug�����;����˳��ţ����������޷���ȡ��)
//                    sCtrl.sRecNumMgr.IcRead = 0;
//            }
            
			//�����������δȡ�꣬�����������ݡ�
			if(sCtrl.sRecNumMgr.IcRead < sCtrl.sRecNumMgr.Current){	
				errtimes = 0;
				DataComReadAsk(sCtrl.sRecNumMgr.IcRead,sCtrl.sRecNumMgr.IcRead +1);
				
			}else if(sCtrl.sRecNumMgr.IcRead > sCtrl.sRecNumMgr.Current){
				errtimes++;
				if(errtimes > 2)									//��ˮ���쳣��
					sCtrl.sRecNumMgr.IcRead = sCtrl.sRecNumMgr.Current;
			}
            
		  	uint32	FlshAddr = GetRecNumAddr(sCtrl.Otr.Rd.sRec.StoreCnt);       //��Ӧ���ٴ洢
            
			WriteFlsh(FlshAddr, (uint8 *)&sCtrl.Otr.Rd.sRec, sizeof(stcFlshRec));
            
            ReadFlsh(FlshAddr, (uint8 *)&sCtrl.sRec, sizeof(stcFlshRec));       //�洢���ж�
            
			FRAM_StoreRecNumMgr((StrRecNumMgr *)&sCtrl.sRecNumMgr);	//����ˮ��
            
		}
        else  if(sCtrl.Otr.Rd.sRec.StoreCnt +1 == sCtrl.sRecNumMgr.Current) //�����¼�¼����ǰ�����ݣ�������
        {
        }
        else
        //if(sCtrl.Otr.Rd.sRec.StoreCnt < sCtrl.sRecNumMgr.IcRead ) //��ˮ�ű��Ѵ��С����Ϊ���»����壬���¸�����ˮ��
        {
            recsmalltimes++;
            if(recsmalltimes > 3){                                  //�����쳣����
                recsmalltimes = 0;
                
                if(sCtrl.Otr.Rd.sRec.StoreCnt != 0xffffffff && sCtrl.Otr.Rd.sRec.StoreCnt != 0)
                {     //��������     
                    if(sCtrl.Otr.Rd.sRec.StoreCnt < sCtrl.sRecNumMgr.Current) //���ݼ�¼���Ѵ�ļ�¼��Χ��
                    {
                        sCtrl.sRecNumMgr.IcRead     = sCtrl.Otr.Rd.sRec.StoreCnt;               //
                        sCtrl.sRecNumMgr.CardRead   = sCtrl.sRecNumMgr.IcRead ;                 //�����x��
                    }else{
                        sCtrl.sRecNumMgr.IcRead++;                              //�����Ч�����¼��++������ִ��
                    }
                }             
                else
                {
                    sCtrl.sRecNumMgr.IcRead++;                              //�����Ч�����¼��++������ִ��
                }
                
                uint32	FlshAddr = GetRecNumAddr(sCtrl.sRecNumMgr.IcRead);       //��Ӧ���ٴ洢
                
                WriteFlsh(FlshAddr, (uint8 *)&sCtrl.Otr.Rd.sRec, sizeof(stcFlshRec));    
                
                DataComReadAsk(sCtrl.sRecNumMgr.IcRead,sCtrl.sRecNumMgr.IcRead +1);
            }
        }
//        else
//        {
//            recbigtimes++;
//            if(sCtrl.sRecNumMgr.IcRead  > 5){       //�����ݼ�¼�쳣�������ü�¼   
//                recbigtimes = 0;
//                sCtrl.sRecNumMgr.IcRead++;
//
//            }
//        }
        
	}
	else if(sCtrl.Otr.RxCtrl.Len == sizeof(sCtrl.Otr.Rd.sTinyRec))	//ͳ��ģ�鷢���ѯ
	{
		if(sCtrl.Otr.Rd.sTinyRec.CurRecNum < sCtrl.sRecNumMgr.IcRead)
		{
			errtimes++;
			if( errtimes > 2 ){										//��ˮ���쳣��
				sCtrl.sRecNumMgr.Current = sCtrl.Otr.Rd.sTinyRec.CurRecNum;
				sCtrl.sRecNumMgr.IcRead = sCtrl.sRecNumMgr.Current;
                
				FRAM_StoreRecNumMgr((StrRecNumMgr *)&sCtrl.sRecNumMgr);	//����ˮ��
			}
		}
		else
		{
			sCtrl.sRecNumMgr.Current = sCtrl.Otr.Rd.sTinyRec.CurRecNum;
			DataComReadAsk(sCtrl.sRecNumMgr.IcRead,sCtrl.sRecNumMgr.IcRead +1);
            
			FRAM_StoreRecNumMgr((StrRecNumMgr *)&sCtrl.sRecNumMgr);	//����ˮ��
		}
		
	}else{			
        //����������ݣ�Ӧ���������
		DataComReadAsk(sCtrl.sRecNumMgr.IcRead,sCtrl.sRecNumMgr.IcRead +1);
	}
    
}


static     	uint32      modelsendnum = 0;	//ģ�ͷ�����š�
/*******************************************************************************
* ��    �ƣ� ClearModelSendNum
* ��    �ܣ� ��ģ�ͷ�����š�
* ��ڲ����� 
* ���ڲ����� ��
* ����  �ߣ� redmorningcn.
* �������ڣ� 2017-05-31
* ��    �ģ�
* �޸����ڣ�
* ��    ע�� ���񴴽�������Ҫ��app.h�ļ�������
*******************************************************************************/
void	ClearModelSendNum(void)
{
	modelsendnum = 0;
}

/*******************************************************************************
* ��    �ƣ� comm_para_flow
* ��    �ܣ� ������������IC�������ͽ��в���
* ��ڲ����� 
* ���ڲ����� ��
* ����  �ߣ� redmorningcn.
* �������ڣ� 2017-05-26
* ��    �ģ�
* �޸����ڣ�
* ��    ע�� ���񴴽�������Ҫ��app.h�ļ�������
*******************************************************************************/
void    comm_para_flow(StrDevOtr * sDtu,uint8 addrnum)
{
//    stcTime     sTime;
//    uint8       i = 0;
//    uint32      modelrecvnum =0;
//    uint32      tmp32;
	uint8		Infolen = 0;
	uint8		*p = (uint8 *)&m_sModelCard;
    
    /***********************************************
    * ������ ����IC�����ͽ�����Ӧ�������������ڲ��뿨�󣬸�ֵ
    * 	     ���δ�忨������Ϊ����ͨ���ݿ���
    */ 
    sDtu->Wr.Code	=	sCtrl.sRunPara.CardType;
	
    switch (sCtrl.sRunPara.CardType)  	
    {
        //�����ܶ�
    case    DENSITY_CARD: 
        sDtu->Wr.Info.Density	=	sCtrl.SOilPara.Density;
        Infolen	=	sizeof(sCtrl.SOilPara.Density);
        break;
        
    case    HIGHT_CARD:  
        sDtu->Wr.Info.Hig	=	sCtrl.SOilPara.Hig;
        Infolen	=	sizeof(sCtrl.SOilPara.Hig);            
        break;
        
        //ģ��ѡ��
    case    MODEL_SELECT_CARD:   
        sDtu->Wr.Info.ModelNum	=	sCtrl.SOilPara.ModelNum;
        Infolen	=	sizeof(sCtrl.SOilPara.ModelNum);
        break;
        
        //���ͳ���
    case    FIX_CARD: 
        sDtu->Wr.Info.sLocoId.Num	=	sCtrl.sProductInfo.sLocoId.Num;
        sDtu->Wr.Info.sLocoId.Type	=	sCtrl.sProductInfo.sLocoId.Type;
        
        Infolen	=	sizeof(	sCtrl.sProductInfo.sLocoId.Num) +
            sizeof( sCtrl.sProductInfo.sLocoId.Type	);
        break;
        
        //������ָʾ
    case    DATA_CARD_DIS: 
        
        sDtu->Wr.Info.CardRecNum	=	sCtrl.sRec.StoreCnt;
        
        Infolen	=	sizeof(sCtrl.sRec.StoreCnt);
        
        sDtu->ConnCtrl[0].SendFramNum++;
        
        break;
        
    case    DATA_CARD_ERR: 
        
        sDtu->Wr.Info.Buf[0]	=	sCtrl.sRunPara.CardErrData;
        
        Infolen	=	sizeof(sCtrl.sRunPara.CardErrData);
        
        sDtu->ConnCtrl[0].SendFramNum++;
        
        break;
                
        //������ָʾ����
    case    DATA_CARD_FIN: 
        
        Infolen = 0;
        break; 
        
        /***********************************************
        * ������ ��������ģ�͡�
        *   IC ��ģ�����IC����IC������Ӧ��ģ�͡�  
        *   ����ÿ200ms�������ݣ���Ŵ�0��ʼ�ۼӡ�
        ���ݷ�����ɺ����Ϊ0xffffffff��
        */    
    case    MODEL_CARD: 
//        if(sizeof(stcModelCard ) > 128 * modelsendnum )
//        {
//            if((sizeof(stcModelCard ) - 128 * modelsendnum) > 128 ){
//                if(modelsendnum == 0)
//                    p = (uint8 *)&m_sModelCard;		
//                //�������
//                memcpy((uint8 *)&sDtu->Wr.Info.Buf[0],(uint8 *)&modelsendnum,sizeof(modelsendnum));	
//                //��������
//                memcpy((uint8 *)&sDtu->Wr.Info.Buf[4],&p[modelsendnum*128],128);
//                modelsendnum++;
//                
//                Infolen = sizeof(modelsendnum)+128;
//            } else {		
//                //���ͽ����ź�
//                modelsendnum = 0xffffffff;
//                //�������
//                memcpy((uint8 *)&sDtu->Wr.Info.Buf[0],(uint8 *)&modelsendnum,sizeof(modelsendnum));	
//                //��������
//                memcpy((uint8 *)&sDtu->Wr.Info.Buf[4],&p[modelsendnum*128],(sizeof(stcModelCard ) - 128 * modelsendnum));
//                
//                Infolen = sizeof(modelsendnum) + (sizeof(stcModelCard ) - 128 * modelsendnum);
//                
//                //���Ŀ����͡��˳�ģ�Ϳ�
//                modelsendnum = 0;
//                sCtrl.sRunPara.CardType = DATA_CARD;
//            }
//        }else{
//            sCtrl.sRunPara.CardType = 	DATA_CARD; //�˳�ģ�Ϳ�
//            sDtu->Wr.Code			=	sCtrl.sRunPara.CardType;
//            Infolen = 0;
//            //����������
//            return;			
//        }
//        
        
        modelsendnum         = 0;
        p = (uint8 *)&m_sModelCard;		
        int     sendmodellen  = 0;  //�ѷ������ݳ���
        while(sizeof(stcModelCard ) > 128 * modelsendnum )
        {
            if((sizeof(stcModelCard ) - 128 * modelsendnum) > 128 ){
                if(modelsendnum == 0)
                    p = (uint8 *)&m_sModelCard;		
                //�������
                memcpy((uint8 *)&sDtu->Wr.Info.Buf[0],(uint8 *)&modelsendnum,sizeof(modelsendnum));	
                //��������
                memcpy((uint8 *)&sDtu->Wr.Info.Buf[4],&p[sendmodellen],128);
                modelsendnum++;
                sendmodellen += 128;
                
                Infolen = sizeof(modelsendnum)+128;
            } else {		
                //���ͽ����ź�
                modelsendnum = 0xffffffff;
                //�������
                memcpy((uint8 *)&sDtu->Wr.Info.Buf[0],(uint8 *)&modelsendnum,sizeof(modelsendnum));	
                //��������
                memcpy((uint8 *)&sDtu->Wr.Info.Buf[4],&p[sendmodellen],(sizeof(stcModelCard ) - sendmodellen));
                
                Infolen = sizeof(modelsendnum) + (sizeof(stcModelCard ) - sendmodellen);
                
                sendmodellen = sizeof(stcModelCard ) ;
                //���Ŀ����͡��˳�ģ�Ϳ�
                //modelsendnum = 0;
                sCtrl.sRunPara.CardType = DATA_CARD;
            }
            
            
            CSNC_SendData(	    sDtu->pch,						//ͨѶ���ƿ�
                                SLAVE_ADDR_OTR,					//Դ��ַ
                                MASTE_ADDR_HOST,				//Ŀ���ַ
                                sDtu->ConnCtrl[0].SendFramNum,	//����֡��
                                0,								//֡���ͣ�Ĭ��Ϊ0��������������ȷ		
                                (uint8 *)&sDtu->Wr,				//���ͻ�����
                                Infolen+sizeof(sDtu->Wr.Code)	//��������������
                                    );
            
//            int senddelay = 10000000;
//            while(senddelay--);
            OS_ERR  os_err;
            OSTimeDlyHMSM(0, 0, 0, 200,
                          OS_OPT_TIME_HMSM_STRICT, &os_err);
            
        }
        
        return;
        
        break;
        /***********************************************
        * ������ ���ݿ�(Ĭ��״̬)��
        *   ��
        *   ����ÿ200ms�������ݣ���Ŵ�0��ʼ�ۼӡ�
        ���ݷ�����ɺ����Ϊ0xffffffff��
        */    		
        //���ݿ�
    case    DATA_CARD: 
        return;
        break;
        
        //�����������ݣ�ֱ���˳�
    default:
        return;					   
        
    }
    
	CSNC_SendData(	sDtu->pch,						//ͨѶ���ƿ�
                  SLAVE_ADDR_OTR,					//Դ��ַ
                  MASTE_ADDR_HOST,				//Ŀ���ַ
                  sDtu->ConnCtrl[0].SendFramNum,	//����֡��
                  0,								//֡���ͣ�Ĭ��Ϊ0��������������ȷ		
                  (uint8 *)&sDtu->Wr,				//���ͻ�����
                  Infolen+sizeof(sDtu->Wr.Code)	//��������������
                      );
}


/*******************************************************************************
* ��    �ƣ� 		GetRecNumAddr
* ��    �ܣ�      ȡ���ݼ�¼��ַ��
flash��ַ;���¼�¼�ŵ���Ϣ��
* ��ڲ����� 	��
* ���ڲ����� 	��
* ���� ���ߣ� 	redmornigcn
* �������ڣ� 	2017-05-15
* ��    �ģ�
* �޸����ڣ�
*******************************************************************************/
uint32  GetRecNumAddr(uint32 FlshRecNum)
{				
    return  (uint32)(((FlshRecNum * sizeof(stcFlshRec)) % FLSH_MAX_SIZE)); 			
}
/*******************************************************************************
* 				end of file
*******************************************************************************/
#endif


