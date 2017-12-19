/*******************************************************************************
 *   Filename:       app_comm_protocol.h
 *   Revised:        $Date: 2015-12-07$
 *   Revision:       $
 *	 Writer:		 Wuming Shen.
 *
 *   Description:
 *   Notes:
 *					QQ:276193028
 *     				E-mail:shenchangwei945@163.com
 *
 *   All copyrights reserved to Wuming Shen.
 *
 *******************************************************************************/
#ifndef	APP_COMM_PROTOCOL_H
#define	APP_COMM_PROTOCOL_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * INCLUDES
 */
#include <includes.h>

/*******************************************************************************
 * CONSTANTS
 */

/*******************************************************************************
 * MACROS
 */
#ifdef   APP_GLOBALS
#define  APP_EXT
#else
#define  APP_EXT  extern
#endif
    
/*******************************************************************************
 * TYPEDEFS
 */
/*******************************************************************************
 * LOCAL VARIABLES
 */

/*******************************************************************************
 * GLOBAL VARIABLES
 */
 
/*******************************************************************************
 * LOCAL FUNCTIONS
 */

/*******************************************************************************
 * GLOBAL FUNCTIONS
 *
 * 返回类型         函数名称                入口参数
 */
uint8	            GetRecvFrameNbr      (void);
void	            SetRecvFrameNbr      (void);
uint8	            GetSendFrameNbr      (void);
void	            SetSendFrameNbr      (void);
     
unsigned char      CSNC_GetData        ( unsigned char *RecBuf,
                                          unsigned short  RecLen,
                                          unsigned char  *DataBuf,
                                          unsigned short *InfoLen);

extern  void	CSNC_SendData(	MODBUS_CH *pch,
                                INT08U  SourceAddr,
                                INT08U  DistAddr,
                                INT08U  SendFrameNum,
                                INT08U  SendCode,                       
                                INT08U *DataBuf,
                                INT16U  DataLen);
                                          

char	            CSNC_WrDataRsp      ( MODBUS_CH *pch,
                                          unsigned char   SourceAddr,
                                          unsigned char   DistAddr,
                                          unsigned char  *DataBuf,
                                          unsigned short  DataLen);
/*******************************************************************************
 * EXTERN VARIABLES
 */

/*******************************************************************************
 * EXTERN FUNCTIONS
 */

/*******************************************************************************
 *              end of file                                                    *
 *******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif